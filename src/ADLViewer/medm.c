/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution.
\*************************************************************************/
/*****************************************************************************
 *
 *     Original Author : Mark Anderson
 *     Second Author   : Frederick Vong
 *     Third Author    : Kenneth Evans, Jr.
 *
 *****************************************************************************
*/

#ifdef WIN32
/* WIN32 does not have unistd.h and does not define the following constants */
#define F_OK 00
#define W_OK 02
#define R_OK 04
#include <direct.h>     /* for getcwd (usually in sys/parm.h or unistd.h) */
#include <io.h>         /* for access, chmod  (usually in unistd.h) */
#endif

#include <signal.h>
#ifdef WIN32
/* Define signals that Windows does not define */
#define SIGQUIT SIGTERM
#define SIGBUS SIGILL
#endif

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>



#define ALLOCATE_STORAGE
#include "medm.h"

typedef enum {EDIT,EXECUTE,HELP,VERSION} opMode_t;
typedef enum {ATTACH,CLEANUP,LOCAL} medmMode_t;
typedef enum {FIXED_FONT,SCALABLE_FONT} fontStyle_t;

typedef struct {
    opMode_t opMode;
    medmMode_t medmMode;
    fontStyle_t fontStyle;
    Boolean privateCmap;
    char *macroString;
    char displayFont[256];          /* !!!! warning : fixed array size */
    char *displayName;
    char *displayGeometry;
    int  fileCnt;
    char **fileList;
} request_t;

request_t * parseCommandLine(int argc, char *argv[]) {
    int i;
    int argsUsed = 0;
    int fileEntryTableSize = 0;
    request_t *request = NULL;
    char fullPathName[PATH_MAX];

    request = (request_t *)malloc(sizeof(request_t));
    if(request == NULL) return request;
    request->opMode = EDIT;
    request->medmMode = LOCAL;
    request->fontStyle = MEDM_DEFAULT_FONT_STYLE;
    request->privateCmap = False;
    request->macroString = NULL;
    strcpy(request->displayFont,MEDM_DEFAULT_DISPLAY_FONT);
    request->displayName = NULL;
    request->displayGeometry = NULL;
    request->fileCnt = 0;
    request->fileList = NULL;

  /* Parse the switches */
    for(i = 1; i < argc; i++) {
        if(!strcmp(argv[i],"-x")) {
            request->opMode = EXECUTE;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-help") || !strcmp(argv[i],"-h") ||
          !strcmp(argv[i],"-?")) {
            request->opMode = HELP;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-version")) {
            request->opMode = VERSION;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-local")) {
            request->medmMode = LOCAL;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-attach")) {
            request->medmMode = ATTACH;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-cleanup")) {
            request->medmMode = CLEANUP;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-cmap")) {
            request->privateCmap = True;
            argsUsed = i;
        } else if(!strcmp(argv[i],"-macro")) {
            char *tmp;
            argsUsed = i;
            tmp = (((i+1) < argc) ? argv[++i] : NULL);
            if(tmp) {
                argsUsed = i;
                request->macroString = STRDUP(tmp);
              /* since parameter of form   -macro "a=b,c=d,..."  replace '"' with ' ' */
                if(request->macroString != NULL) {
                    size_t len;
                    if(request->macroString[0] == '"') request->macroString[0] = ' ';
                    len = strlen(request->macroString) - 1;
                    if(request->macroString[len] == '"') request->macroString[len] = ' ';
                }
            }
        } else if(!strcmp(argv[i],"-displayFont")) {
            char *tmp;
            argsUsed = i;
            tmp = (((i+1) < argc) ? argv[++i] : NULL);
            if(tmp) {
                argsUsed = i;
                strcpy(request->displayFont,tmp);
#if 0
              /* KE: The following code is useless.  We could change
                 == to !=, however, request->fontStyle eventually is
                 only used to set the windowPropertyAtom.  Currently,
                 it will be set to whatever the default is for
                 request->fontStyle.  Moreover, if the displayFont is
                 an X font specification, this method will not be able
                 to determine if it is scalable or not. Further, it
                 probably doesn't make a difference.  */
                if(request->displayFont[0] == '\0') {
                    if(!strcmp(request->displayFont,FONT_ALIASES_STRING))
                      request->fontStyle = FIXED_FONT;
                    else if(!strcmp(request->displayFont,DEFAULT_SCALABLE_STRING))
                      request->fontStyle = SCALABLE_FONT;
                }
#endif
            }
        } else if(!strcmp(argv[i],"-display")) {
          /* (Not trapped by X because this routine is called first) */
            char *tmp;
            argsUsed = i;
            tmp = (((i+1) < argc) ? argv[++i] : NULL);
            if(tmp) {
                argsUsed = i;
                request->displayName = STRDUP(tmp);
            }
        } else if((!strcmp(argv[i],"-displayGeometry")) || (!strcmp(argv[i],"-dg"))) {
            char *tmp;
            argsUsed = i;
            tmp = (((i+1) < argc) ? argv[++i] : NULL);
            if(tmp) {
                argsUsed = i;
                request->displayGeometry = STRDUP(tmp);
            }
        } else if(!strcmp(argv[i],"-bigMousePointer")) {
            medmUseBigCursor = 1;

            argsUsed = i;
        } else if(!strcmp(argv[i],"-noMsg")) {
            medmRaiseMessageWindow = 0;

            argsUsed = i;
        } else if(argv[i][0] == '-') {
            medmPrintf(1,"\nInvalid option: %s\n",argv[i]);
            request->opMode = HELP;
            argsUsed = i;
        }

    }

  /* Parse the display name */
    for(i = argsUsed+1; i < argc; i++) {
        Boolean canAccess;
        char    *fileStr;

        canAccess = False;

      /* Check the next argument, if doesn't match the suffix, continue */
      /* KE: May not be a suffix (junk.adlebrained.txt fits) */
        fileStr = argv[i];
        if(strstr(fileStr,DISPLAY_FILE_ASCII_SUFFIX) == NULL) {
            medmPrintf(1,"\nFile has wrong suffix: %s\n",fileStr);
            continue;
        }
        if(strlen(fileStr) > (size_t)(PATH_MAX-1)) {
            medmPrintf(1,"\nFile name too long: %s\n",fileStr);
            continue;
        }

      /* Mark the fullPathName as an empty string */
        fullPathName[0] = '\0';

      /* Found string with right suffix - presume it's a valid display name */
        canAccess = !access(fileStr,R_OK|F_OK);
        if(canAccess) {
            int status;

          /* Found the file.  Convert to a full path. */
            status = convertNameToFullPath(fileStr, fullPathName, PATH_MAX);
            if(!status) canAccess = False;
        } else {
          /* Not found, try with directory specified in the environment */
            char *dir = NULL;
            char name[PATH_MAX];
            int startPos;

            dir = getenv("EPICS_DISPLAY_PATH");
            if(dir != NULL) {
                startPos = 0;
                while(extractStringBetweenColons(dir,name,startPos,&startPos)) {
                    if(strlen(name)+strlen(fileStr) <
                      (size_t)(PATH_MAX - 1)) {
                        strcpy(fullPathName,name);
#ifndef VMS
                        strcat(fullPathName,MEDM_DIR_DELIMITER_STRING);
#endif
                        strcat(fullPathName,fileStr);
                        canAccess = !access(fullPathName,R_OK|F_OK);
                        if(canAccess) break;
                    }
                }
            }
        }
        if(canAccess) {
          /* build the request */
            if(fileEntryTableSize == 0) {
                fileEntryTableSize =  10;
                request->fileList =
                  (char **)malloc(fileEntryTableSize*sizeof(char *));
            }
            if(fileEntryTableSize > request->fileCnt) {
                fileEntryTableSize *= 2;
#if defined(__cplusplus) && !defined(__GNUG__)
                request->fileList =
                  (char **)realloc((malloc_t)request->fileList,
                    fileEntryTableSize);
#else
                request->fileList =
                  (char **)realloc(request->fileList,fileEntryTableSize);
#endif
            }
            if(request->fileList) {
                request->fileList[request->fileCnt] = STRDUP(fullPathName);
                request->fileCnt++;
            }
        } else {
            medmPrintf(1,"\nCannot access file: %s\n",fileStr);
        }
    }
    return request;
}

/* medm allowed a .template used as a suffix for compatibility. This exception is
   caused by a bug in the save routine at checking the ".adl" suffix.
*/

const char *templateSuffix = ".template";


/* This routines is used for file conversions only. */
/* KE: The error handling could be improved.  See dmDisplayListParse.  */
char token[MAX_TOKEN_LENGTH];
DisplayInfo* parseDisplayFile(char *filename) {
    DisplayInfo *displayInfo = NULL;
    FILE *filePtr;
    TOKEN tokenType;
    filePtr = fopen(filename,"r");
    if(filePtr) {
        displayInfo = (DisplayInfo *)malloc(sizeof(DisplayInfo));
        displayInfo->dlElementList = createDlList();
        currentDisplayInfo = displayInfo;
        displayInfo->filePtr = filePtr;
      /* if first token isn't "file" then bail out! */
        tokenType=getToken(displayInfo,token);
        if(tokenType == T_WORD && !strcmp(token,"file")) {
            displayInfo->dlFile = parseFile(displayInfo);
            if(displayInfo->dlFile) {
                displayInfo->versionNumber = displayInfo->dlFile->versionNumber;
                strcpy(displayInfo->dlFile->name,filename);
            }
        } else {
            fclose(filePtr);
            return NULL;
        }
        tokenType=getToken(displayInfo,token);
        if(tokenType == T_WORD && !strcmp(token,"display")) {
            parseDisplay(displayInfo);
        }
        tokenType=getToken(displayInfo,token);
        if(tokenType == T_WORD && (!strcmp(token,"color map") ||
          !strcmp(token,"<<color map>>"))) {
            displayInfo->dlColormap=parseColormap(displayInfo,displayInfo->filePtr);
            tokenType=getToken(displayInfo,token);
        } else {
            fclose(filePtr);
            return NULL;
        }

      /* Proceed with parsing */
        while(parseAndAppendDisplayList(displayInfo,displayInfo->dlElementList,
          token,tokenType) != T_EOF) {
            tokenType=getToken(displayInfo,token);
        }
        displayInfo->filePtr=NULL;
        fclose(filePtr);
    }

    return displayInfo;
}

/**************************************************************************/
/**************************** main ****************************************/
/**************************************************************************/
int main(int argc, char *argv[])
{

  /* Initialize global variables */
    parsingCompositeFile = False;
    maxLabelWidth = 0;
    maxLabelHeight = 0;
    dashes = "******";
    cpMatrix = cpForm = NULL;
    medmInitializeUpdateTasks();
    updateTaskExposedRegion =  XCreateRegion();
    windowPropertyAtom = (Atom)NULL;
    medmWorkProcId = 0;
    medmUpdateRequestCount = 0;
    medmCAEventCount = 0;
    medmScreenUpdateCount = 0;
    medmUpdateMissedCount = 0;
    MedmUseNewFileFormat = True;
    medmRaiseMessageWindow = 1;
    popupExistingDisplay = POPUP_EXISTING_DISPLAY;
    setTimeValues();

  /* Parse command line */
    request_t *request = parseCommandLine(argc,argv);

    if(request->macroString != NULL && request->opMode != EXECUTE) {
    medmPrintf(0,"\nIgnored -macro command line option\n"
          "  (Only valid for Execute (-x) mode operation)\n");
        free(request->macroString);
        request->macroString = NULL;
    }

  /* Usage and error exit */
    if(request->opMode == HELP) {
        print("\n%s\n",MEDM_VERSION_STRING);
        print("Usage:\n"
          "  medm [X options]\n"
          "  [-help | -h | -?]\n"
          "  [-version]\n"
          "  [-x | -e]\n"
          "  [-local | -attach | -cleanup]\n"
          "  [-cmap]\n"
          "  [-bigMousePointer]\n"
          "  [-noMsg]\n"
          "  [-displayFont font-spec]\n"
          "  [-macro \"xxx=aaa,yyy=bbb, ...\"]\n"
          "  [-dg [xpos[xypos]][+xoffset[+yoffset]]\n"
          "  [display-files]\n"
          "  [&]\n"
          "\n");
        exit(0);
    } else if(request->opMode == VERSION) {
        print("\n%s\n",MEDM_VERSION_STRING);
        exit(0);
    }
}
