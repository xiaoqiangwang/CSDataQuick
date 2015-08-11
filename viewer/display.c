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

#define DEBUG_RELATED_DISPLAY 0
#define DEBUG_POSITION 0
#define DEBUG_REPOSITION 0
#define DEBUG_OPEN 0
#define DEBUG_PARSE 0

#ifdef DEBUG_OPEN
#include <errno.h>
#endif

#include "medm.h"
#include <stdbool.h>
#include <stdlib.h>

/* KE: Used to move the display if its x and y are zero
 * The following account for the borders and title bar with the TED WM
 *   Were formerly both 10 (not very aesthetic) */
#define DISPLAY_DEFAULT_X 5
#define DISPLAY_DEFAULT_Y 24

#define MEDM_EXEC_LIST_MAX 1024

typedef DlElement *(*medmParseFunc)(DisplayInfo *);
typedef struct {
    char *name;
    medmParseFunc func;
} ParseFuncEntry;

typedef struct _parseFuncEntryNode {
    ParseFuncEntry *entry;
    struct _parseFuncEntryNode *next;
    struct _parseFuncEntryNode *prev;
} ParseFuncEntryNode;


/* Global variables */

ParseFuncEntry parseFuncTable[] = {
    {"rectangle",            parseRectangle},
    {"oval",                 parseOval},
    {"arc",                  parseArc},
    {"text",                 parseText},
/*     {"falling line",         parseFallingLine}, */
/*     {"rising line",          parseRisingLine}, */
    {"falling line",         parsePolyline},
    {"rising line",          parsePolyline},
    {"related display",      parseRelatedDisplay},
    {"shell command",        parseShellCommand},
    {"bar",                  parseBar},
    {"indicator",            parseIndicator},
    {"meter",                parseMeter},
    {"byte",                 parseByte},
    {"strip chart",          parseStripChart},
#ifdef CARTESIAN_PLOT
    {"cartesian plot",       parseCartesianPlot},
#endif     /* #ifdef CARTESIAN_PLOT */
    {"text update",          parseTextUpdate},
    {"choice button",        parseChoiceButton},
    {"button",               parseChoiceButton},
    {"message button",       parseMessageButton},
    {"menu",                 parseMenu},
    {"text entry",           parseTextEntry},
    {"valuator",             parseValuator},
    {"image",                parseImage},
    {"composite",            parseComposite},
    {"polyline",             parsePolyline},
    {"polygon",              parsePolygon},
    {"wheel switch",         parseWheelSwitch},
};

static int parseFuncTableSize = sizeof(parseFuncTable)/sizeof(ParseFuncEntry);

/***  displayInfo routines ***/

/*
 * create and return a DisplayInfo structure pointer on tail of displayInfoList
 *  this includes a shell (with it's dialogs and event handlers)
 */
DisplayInfo *allocateDisplayInfo()
{
    DisplayInfo *displayInfo;

/* Allocate a DisplayInfo structure and shell for this display file/list */
    displayInfo = (DisplayInfo *)malloc(sizeof(DisplayInfo));
    if(!displayInfo) return NULL;

    displayInfo->dlElementList = createDlList();
    if(!displayInfo->dlElementList) {
    free(displayInfo);
        return NULL;
    }

    displayInfo->filePtr = NULL;
    displayInfo->versionNumber = 0;


    displayInfo->nameValueTable = NULL;
    displayInfo->numNameValues = 0;

    displayInfo->dlFile = NULL;
    displayInfo->dlColormap = NULL;


  /* Append to end of the list */
    displayInfo->next = NULL;
    displayInfo->prev = displayInfoListTail;
    displayInfoListTail->next = displayInfo;
    displayInfoListTail = displayInfo;

    return(displayInfo);
}

/*
 * function which cleans up a given displayInfo in the displayInfoList
 * (including the displayInfo's display list if specified)
 */
void dmCleanupDisplayInfo(DisplayInfo *displayInfo, Boolean_t cleanupDisplayList)
{
    int i;

        //free( (char *) displayInfo->colormap);
        //displayInfo->colormap = NULL;
        displayInfo->dlColormapCounter = 0;
        displayInfo->dlColormapSize = 0;
}


void dmRemoveDisplayInfo(DisplayInfo *displayInfo)
{
    displayInfo->prev->next = displayInfo->next;
    if(displayInfo->next != NULL)
      displayInfo->next->prev = displayInfo->prev;
    if(displayInfoListTail == displayInfo)
      displayInfoListTail = displayInfoListTail->prev;
    if(displayInfoListTail == displayInfoListHead )
      displayInfoListHead->next = NULL;

  /* Cleanup resources and free display list */
    dmCleanupDisplayInfo(displayInfo,true);
    freeNameValueTable(displayInfo->nameValueTable,displayInfo->numNameValues);
    if(displayInfo->dlElementList) {
        clearDlDisplayList(displayInfo, displayInfo->dlElementList);
        free ( (char *) displayInfo->dlElementList);
    }
    if(displayInfo->dlFile) {
        free((char *)displayInfo->dlFile);
        displayInfo->dlFile = NULL;
    }
    if(displayInfo->dlColormap) {
        free((char *)displayInfo->dlColormap);
        displayInfo->dlColormap = NULL;
    }
    if(displayInfo) {
        free((char *)displayInfo);
        displayInfo = NULL;
    }

    if(displayInfoListHead == displayInfoListTail) {
        currentDisplayInfo = NULL;
    }
}

/*
 * function to remove ALL displayInfo's
 *   this includes a full cleanup of associated resources and displayList
 */
void dmRemoveAllDisplayInfo()
{
    DisplayInfo *nextDisplay, *displayInfo;

    displayInfo = displayInfoListHead->next;
    while(displayInfo != NULL) {
        nextDisplay = displayInfo->next;
        dmRemoveDisplayInfo(displayInfo);
        displayInfo = nextDisplay;
    }
    displayInfoListHead->next = NULL;
    displayInfoListTail = displayInfoListHead;

    currentDisplayInfo = NULL;
}

/***  Parsing routines ***/

DlElement *getNextElement(DisplayInfo *pDI, char *token) {
    int i;
    for(i=0; i < parseFuncTableSize; i++) {
        if(!strcmp(token,parseFuncTable[i].name)) {
            return parseFuncTable[i].func(pDI);
        }
    }
    return 0;
}

TOKEN parseAndAppendDisplayList(DisplayInfo *displayInfo, DlList *dlList,
  char *firstToken, TOKEN firstTokenType)
{
    TOKEN tokenType;
    char token[MAX_TOKEN_LENGTH];
    int nestingLevel = 0;
    static DlBasicAttribute attr;
    static DlDynamicAttribute dynAttr;
    static Boolean_t init = true;
    int first = 1;

  /* Initialize attributes to defaults for old format */
    if(init && displayInfo->versionNumber < 20200) {
        basicAttributeInit(&attr);
        dynamicAttributeInit(&dynAttr);
        init = false;
    }

  /* Loop over tokens until T_EOF */
    do {
        if(first) {
            tokenType=firstTokenType;
            strcpy(token,firstToken);
            first = 0;
        } else {
            tokenType=getToken(displayInfo,token);
        }
        switch(tokenType) {
        case T_WORD : {
            DlElement *pe = getNextElement(displayInfo,token);
            if(pe) {
              /* Found an element via the parseFuncTable */
                if(displayInfo->versionNumber < 20200) {
                    switch (pe->type) {
                    case DL_Rectangle :
                    case DL_Oval      :
                    case DL_Arc       :
                    case DL_Text      :
                    case DL_Polyline  :
                    case DL_Polygon   :
                      /* Use the last found attributes */
                        pe->structure.rectangle->attr = attr;
#if 0
                        pe->structure.rectangle->dynAttr = dynAttr;
                      /* KE: Don't want to do this.  The old format
                       *  relies on them not being reset */
                      /* Reset the attributes to defaults */
                        basicAttributeInit(&attr);
                        dynamicAttributeInit(&dynAttr);
#else
                      /* KE: This was what was done in MEDM 2.2.9 */
                        if(dynAttr.chan[0][0] != '\0') {
                            pe->structure.rectangle->dynAttr = dynAttr;
                            dynAttr.chan[0][0] = '\0';
                        }
#endif
                        break;
                    default:
                        break;
                    }
                }
            } else if(displayInfo->versionNumber < 20200) {
              /* Did not find an element and old file version
               *   Must be an attribute, which appear before the object does */
                if(!strcmp(token,"<<basic atribute>>") ||
                  !strcmp(token,"basic attribute") ||
                  !strcmp(token,"<<basic attribute>>")) {
                    parseOldBasicAttribute(displayInfo,&attr);
                } else if(!strcmp(token,"dynamic attribute") ||
                  !strcmp(token,"<<dynamic attribute>>")) {
                    parseOldDynamicAttribute(displayInfo,&dynAttr);
                }
            }
            if(pe) {
#if DEBUG_PARSE
                {
                    static int i=0;

                    print("%3d Element: %s x=%d y=%d width=%d height=%d\n",
                      ++i,
                      elementType(pe->type),
                      (int)pe->structure.display->object.x,
                      (int)pe->structure.display->object.y,
                      (int)pe->structure.display->object.width,
                      (int)pe->structure.display->object.height);
                }
#endif
                appendDlElement(dlList,pe);
            }
            break;
        }
        case T_EQUAL:
            break;
        case T_LEFT_BRACE:
            nestingLevel++; break;
        case T_RIGHT_BRACE:
            nestingLevel--; break;
        default :
            break;
        }
    } while((tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF));

  /* Reset the init flag */
    if(tokenType == T_EOF) init = true;
    return tokenType;
}
