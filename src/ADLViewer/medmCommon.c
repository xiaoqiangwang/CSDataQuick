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

#include "medm.h"
#include <ctype.h>
#include <stdbool.h>

void parseAttr(DisplayInfo *displayInfo, DlBasicAttribute *attr);
void parseDynamicAttr(DisplayInfo *displayInfo, DlDynamicAttribute *dynAttr);
void parseDynAttrMod(DisplayInfo *displayInfo, DlDynamicAttribute *dynAttr);
void parseDynAttrParam(DisplayInfo *displayInfo, DlDynamicAttribute *dynAttr);
void parseOldDlColor( DisplayInfo *, FILE *, DlColormapEntry *);

static DlList *dlElementFreeList = 0;

int initMedmCommon()
{
    if(dlElementFreeList) return 0;
    dlElementFreeList = createDlList();
    if(dlElementFreeList) {
	return 0;
    } else {
	return -1;
    }
}

void parseAndSkip(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

  /* Just read and look for braces until we return to the same level */
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
        }
    } while( (tokenType != T_RIGHT_BRACE || nestingLevel > 0) &&
      (nestingLevel > 0) && (tokenType != T_EOF) );
}

DlFile *parseFile(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlFile *dlFile = createDlFile(displayInfo);

    if(!dlFile) return 0;
    dlFile->versionNumber = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"name")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		strcpy(dlFile->name,token);
	    }
	    if(!strcmp(token,"version")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		dlFile->versionNumber = atoi(token);
	    }
	    break;
	case T_EQUAL:
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );

    return dlFile;
}

DlColormap *createDlColormap(DisplayInfo *displayInfo)
{
    DlColormap *dlColormap;

    dlColormap = (DlColormap *)malloc(sizeof(DlColormap));
    if(!dlColormap) return 0;
  /* structure copy */
    *dlColormap = defaultDlColormap;

    return(dlColormap);
}


void parseDlColor(DisplayInfo *displayInfo, FILE *filePtr,
  DlColormapEntry *dlColor)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    int counter = 0;

    FILE *savedFilePtr;


 /* (MDA) have to be sneaky for these colormap parsing routines: *
  * since possibly external colormap, save and restore * external file
  * ptr in displayInfo so that getToken() * works with displayInfo and
  * not the filePtr directly */
    savedFilePtr = displayInfo->filePtr;
    displayInfo->filePtr = filePtr;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD: {
	    char *end;
	    unsigned long color = strtoul(token,&end,16);
	    if(counter < DL_MAX_COLORS) {
		dlColor[counter].r = (color & 0x00ff0000) >> 16;
		dlColor[counter].g = (color & 0x0000ff00) >> 8;
		dlColor[counter].b = color & 0x000000ff;
		counter++;
	    }
	    getToken(displayInfo,token);
	    break;
	}
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );

  /* Restore displayInfo->filePtr to previous value */
    displayInfo->filePtr = savedFilePtr;
}

void parseOldDlColor(DisplayInfo *displayInfo, FILE *filePtr,
  DlColormapEntry *dlColor)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    FILE *savedFilePtr;

  /*
   * (MDA) have to be sneaky for these colormap parsing routines:
   *      since possibly external colormap, save and restore
   *      external file ptr in  displayInfo so that getToken()
   *      works with displayInfo and not the filePtr directly
   */
    savedFilePtr = displayInfo->filePtr;
    displayInfo->filePtr = filePtr;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"r")) {
		getToken(displayInfo,token);getToken(displayInfo,token);
		dlColor->r = atoi(token);
	    } else if(!strcmp(token,"g")) {
		getToken(displayInfo,token);getToken(displayInfo,token);
		dlColor->g = atoi(token);
	    } else if(!strcmp(token,"b")) {
		getToken(displayInfo,token);getToken(displayInfo,token);
		dlColor->b = atoi(token);
	    } else if(!strcmp(token,"inten")) {
		getToken(displayInfo,token);getToken(displayInfo,token);
		dlColor->inten = atoi(token);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );

  /* and restore displayInfo->filePtr to previous value */
    displayInfo->filePtr = savedFilePtr;
}

/* parseColormap and parseDlColor have two arguments, since could in fact
 *   be parsing an external colormap file, hence need to pass in the
 *   explicit file ptr for the current colormap file
 */
DlColormap *parseColormap(DisplayInfo *displayInfo, FILE *filePtr)
{
    char token[MAX_TOKEN_LENGTH];
    char msg[2*MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlColormap *dlColormap;
    DlColormapEntry dummyColormapEntry;
    int counter;
    FILE *savedFilePtr;

  /* Free any existing colormap and create a new onep */
    if(!displayInfo) return NULL;
    if(displayInfo->dlColormap) {
	free((char *)displayInfo->dlColormap);
	displayInfo->dlColormap = NULL;
    }
    dlColormap = createDlColormap(displayInfo);
    if(!dlColormap) return NULL;

  /*
   * (MDA) have to be sneaky for these colormap parsing routines:
   *	since possibly external colormap, save and restore
   *	external file ptr in  displayInfo so that getToken()
   *	works with displayInfo and not the filePtr directly
   */
    savedFilePtr = displayInfo->filePtr;
    displayInfo->filePtr = filePtr;

  /* initialize some data in structure */
    dlColormap->ncolors = 0;

  /* new colormap, get values (pixel values are being stored) */
    counter = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"ncolors")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		dlColormap->ncolors = atoi(token);
		if(dlColormap->ncolors > DL_MAX_COLORS) {
		    medmPrintf(1,"\nMaximum # of colors in colormap exceeded\n"
		      "  Will continue with truncated color space\n"
		      "(You may want to change the colors of some objects)\n");
		    dmSetAndPopupWarningDialog(displayInfo, msg,"OK",NULL,NULL);
		}
	    } else if(!strcmp(token,"dl_color")) {
	      /* continue parsing but throw away "excess" colormap entries */
		if(counter < DL_MAX_COLORS) {
		    parseOldDlColor(displayInfo,filePtr,&(dlColormap->dl_color[counter]));
		    counter++;
		} else {
		    parseOldDlColor(displayInfo,filePtr,&dummyColormapEntry);
		    counter++;
		}
	    } else if(!strcmp(token,"colors")) {
		parseDlColor(displayInfo,filePtr,dlColormap->dl_color);
	    }
	    break;
	case T_EQUAL:
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while((tokenType != T_RIGHT_BRACE) && (nestingLevel > 0) &&
      (tokenType != T_EOF));

  /* restore the previous filePtr */
    displayInfo->filePtr = savedFilePtr;
    return (dlColormap);
}

void parseBasicAttribute(DisplayInfo *displayInfo, DlBasicAttribute *attr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"clr")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		attr->clr = atoi(token) % DL_MAX_COLORS;
	    } else if(!strcmp(token,"style")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"solid")) {
		    attr->style = SOLID;
		} else if(!strcmp(token,"dash")) {
		    attr->style = DASH;
		}
	    } else if(!strcmp(token,"fill")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"solid")) {
		    attr->fill = F_SOLID;
		} else if(!strcmp(token,"outline")) {
		    attr->fill = F_OUTLINE;
		}
	    } else if(!strcmp(token,"width")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		attr->width = atoi(token);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parseOldBasicAttribute(DisplayInfo *displayInfo, DlBasicAttribute *attr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    basicAttributeInit(attr);
    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"attr"))
	      parseAttr(displayInfo,attr);
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parseLimits(DisplayInfo *displayInfo, DlLimits *limits)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int i, nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"loprSrc")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		for(i=FIRST_PV_LIMITS_SRC;
		     i < FIRST_PV_LIMITS_SRC+NUM_PV_LIMITS_SRC; i++) {
		    if(!strcmp(token,stringValueTable[i])) {
			limits->loprSrc0 = limits->loprSrc = i;
			break;
		    }
		}
	    } else if(!strcmp(token,"loprDefault")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		limits->loprDefault = atof(token);
	    } else if(!strcmp(token,"hoprSrc")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		for(i=FIRST_PV_LIMITS_SRC;
		     i < FIRST_PV_LIMITS_SRC+NUM_PV_LIMITS_SRC; i++) {
		    if(!strcmp(token,stringValueTable[i])) {
			limits->hoprSrc0 = limits->hoprSrc = i;
			break;
		    }
		}
	    } else if(!strcmp(token,"hoprDefault")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		limits->hoprDefault = atof(token);
	    } else if(!strcmp(token,"precSrc")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		for(i=FIRST_PV_LIMITS_SRC;
		     i < FIRST_PV_LIMITS_SRC+NUM_PV_LIMITS_SRC; i++) {
		    if(!strcmp(token,stringValueTable[i])) {
			limits->precSrc0 = limits->precSrc = i;
			break;
		    }
		}
	    } else if(!strcmp(token,"precDefault")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		limits->precDefault = atoi(token);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void limitsAttributeInit(DlLimits *limits)
{
    limits->loprSrc = limits->loprSrc0 = PV_LIMITS_CHANNEL;
    limits->lopr = limits->loprChannel = limits->loprDefault =
      limits->loprUser = LOPR_DEFAULT;
    limits->hoprSrc = limits->hoprSrc0 = PV_LIMITS_CHANNEL;
    limits->hopr = limits->hoprChannel = limits->hoprDefault =
      limits->hoprUser = HOPR_DEFAULT;
    limits->precSrc = limits->precSrc0 = PV_LIMITS_CHANNEL;
    limits->prec = limits->precChannel = limits->precDefault =
      limits->precUser = PREC_DEFAULT;
}

void objectAttributeInit(DlObject *object)
{
    object->x = 0;
    object->y = 0;
    object->width = 10;
    object->height = 10;
}

void objectAttributeSet(DlObject *object, int x, int y,
  unsigned int width, unsigned int height)
{
    object->x = x;
    object->y = y;
    object->width = width;
    object->height = height;
}

void basicAttributeInit(DlBasicAttribute *attr)
{
    attr->clr = 0;
    attr->style = SOLID;
    attr->fill = F_SOLID;
    attr->width = 0;
}

void dynamicAttributeInit(DlDynamicAttribute *dynAttr)
{
    int i;

    dynAttr->clr = STATIC;
    dynAttr->vis = V_STATIC;
    *(dynAttr->calc) = '\0';
    *(dynAttr->post) = '\0';
    dynAttr->validCalc = false;
    for(i=0; i < MAX_CALC_RECORDS; i++) {
	*(dynAttr->chan[i]) = '\0';
    }
}

DlElement* createDlElement(DlElementType type, void *structure,
  DlDispatchTable *dlDispatchTable)
{
    DlElement *dlElement;

  /* Obtain dlElement */
    if(dlElementFreeList->count > 0) {
      /* Get it from the free list */
	dlElement = dlElementFreeList->tail;
	removeDlElement(dlElementFreeList,dlElement);
#if 0
	printf("createDlElement, %d\n",dlElementFreeList->count);
#endif
    } else {
      /* Allocate it */
	dlElement = (DlElement *)malloc(sizeof(DlElement));
    }
  /* If unsuccessful, return */
    if(!dlElement) return dlElement;
  /* Define the elements of the struct */
    dlElement->type = type;
  /* Note: structure is a union of pointers
   *   Just need to define the (one) pointer
   *   Use composite (doesn't matter which member of the union you use) */
    dlElement->structure.composite = (DlComposite *)structure;
  /* Use the supplied dispatch table or a default if the supplied one is NULL
    * Note: createDlElement for DL_Element passes a NULL dispatch table */
    if(dlDispatchTable) {
	dlElement->run = dlDispatchTable;
    } else {
#if 0
	printf("createDlElement\n");
#endif
	//dlElement->run = &elementDlDispatchTable;
    }
    dlElement->hidden = false;
    dlElement->data = 0;
    dlElement->next = 0;
    dlElement->prev = 0;
    return dlElement;
}

static DlObject defaultObject = {0,0,5,5};


void parseDynamicAttribute(DisplayInfo *displayInfo,
  DlDynamicAttribute *dynAttr) {
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"clr")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		  if(!strcmp(token,"discrete"))
		    dynAttr->clr = DISCRETE;
		  else if(!strcmp(token,"static"))
		    dynAttr->clr = STATIC;
		  else if(!strcmp(token,"alarm"))
		    dynAttr->clr = ALARM;
	    } else if(!strcmp(token,"vis")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"static"))
		  dynAttr->vis = V_STATIC;
		else if(!strcmp(token,"if not zero"))
		  dynAttr->vis = IF_NOT_ZERO;
		else if(!strcmp(token,"if zero"))
		  dynAttr->vis = IF_ZERO;
		else if(!strcmp(token,"calc"))
		  dynAttr->vis = V_CALC;
	    } else if(!strcmp(token,"calc")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if((strlen(token) > (size_t)0)) {
		    strcpy(dynAttr->calc,token);
		}
	    } else {
	      /* Channel names */
		int i;
		char chanName[6];

		for(i=0; i < MAX_CALC_RECORDS; i++) {
		  /* Names are chan, chanB, chanC, etc. */
		    sprintf(chanName,"chan%c",i?'A'+i:'\0');
		    if(!strcmp(token,chanName)) {
			getToken(displayInfo,token);
			getToken(displayInfo,token);
			if((strlen(token) > (size_t)0)) {
			    strcpy(dynAttr->chan[i],token);
			}
		    }
		}
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parseOldDynamicAttribute(DisplayInfo *displayInfo,
  DlDynamicAttribute *dynAttr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    dynamicAttributeInit(dynAttr);
    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"attr"))
	      parseDynamicAttr(displayInfo,dynAttr);
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}


DlElement *parseFallingLine(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlPolyline *dlPolyline;
    DlElement *dlElement;

  /* Rising Line is replaced by PolyLine */
    dlPolyline = (DlPolyline *) calloc(1,sizeof(DlPolyline));
  /* initialize some data in structure */
    dlPolyline->object = defaultObject;
    dlPolyline->nPoints = 0;
    dlPolyline->points = (XPoint *)NULL;
    dlPolyline->isFallingOrRisingLine = true;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"object"))
	      parseObject(displayInfo,&(dlPolyline->object));
	    break;
	case T_EQUAL:
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
        }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );

#define INITIAL_NUM_POINTS 16
    dlPolyline->points = (XPoint *)malloc(16*sizeof(XPoint));
    dlPolyline->nPoints = 2;
    dlPolyline->points[0].x = dlPolyline->object.x;
    dlPolyline->points[0].y = dlPolyline->object.y;
    dlPolyline->points[1].x = dlPolyline->object.x + dlPolyline->object.width;
    dlPolyline->points[1].y = dlPolyline->object.y + dlPolyline->object.height;

    dlElement = (DlElement *)malloc(sizeof(DlElement));
    dlElement->type = DL_Polyline;
    dlElement->structure.polyline = dlPolyline;
    dlElement->next = NULL;

    return dlElement;
}

DlElement *parseRisingLine(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlPolyline *dlPolyline;
    DlElement *dlElement;

  /* Rising Line is replaced by PolyLine */
    dlPolyline = (DlPolyline *) calloc(1,sizeof(DlPolyline));
  /* initialize some data in structure */
    dlPolyline->object = defaultObject;
    dlPolyline->nPoints = 0;
    dlPolyline->points = (XPoint *)NULL;
    dlPolyline->isFallingOrRisingLine = true;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"object"))
	      parseObject(displayInfo,&(dlPolyline->object));
	    break;
	case T_EQUAL:
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );

#define INITIAL_NUM_POINTS 16
    dlPolyline->points = (XPoint *)malloc(16*sizeof(XPoint));
    dlPolyline->nPoints = 2;
    dlPolyline->points[0].x = dlPolyline->object.x;
    dlPolyline->points[0].y = dlPolyline->object.y + dlPolyline->object.height;
    dlPolyline->points[1].x = dlPolyline->object.x + dlPolyline->object.width;
    dlPolyline->points[1].y = dlPolyline->object.y;

    dlElement = (DlElement *)malloc(sizeof(DlElement));
    dlElement->type = DL_Polyline;
    dlElement->structure.polyline = dlPolyline;
    dlElement->next = NULL;

    return dlElement;
}

/**********************************************************************
 *********    nested objects (not to be put in display list   *********
 **********************************************************************/

void parseObject(DisplayInfo *displayInfo, DlObject *object)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"x")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		object->x = atoi(token);
	    } else if(!strcmp(token,"y")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		object->y = atoi(token);
	    } else if(!strcmp(token,"width")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		object->width = atoi(token);
	    } else if(!strcmp(token,"height")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		object->height = atoi(token);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parseGrid(DisplayInfo *displayInfo, DlGrid *grid)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"gridSpacing")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		grid->gridSpacing = atoi(token);
	    } else if(!strcmp(token,"gridOn")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		grid->gridOn = atoi(token);
	    } else if(!strcmp(token,"snapToGrid")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		grid->snapToGrid = atoi(token);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}


void parseAttr(DisplayInfo *displayInfo, DlBasicAttribute *attr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"clr")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		attr->clr = atoi(token) % DL_MAX_COLORS;
	    } else if(!strcmp(token,"style")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"solid")) {
		    attr->style = SOLID;
		} else if(!strcmp(token,"dash")) {
		    attr->style = DASH;
		}
	    } else if(!strcmp(token,"fill")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"solid")) {
		    attr->fill = F_SOLID;
		} else if(!strcmp(token,"outline")) {
		    attr->fill = F_OUTLINE;
		}
	    } else if(!strcmp(token,"width")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		attr->width = atoi(token);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) ) ;     /* Do nothing */
}

void parseDynamicAttr(DisplayInfo *displayInfo, DlDynamicAttribute *dynAttr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"mod")) {
		parseDynAttrMod(displayInfo,dynAttr);
	    } else if(!strcmp(token,"param")) {
		parseDynAttrParam(displayInfo,dynAttr);
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parseDynAttrMod(DisplayInfo *displayInfo, DlDynamicAttribute *dynAttr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    if(!strcmp(token,"clr")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"discrete"))
		  dynAttr->clr = DISCRETE;
		else if(!strcmp(token,"static"))
		  dynAttr->clr = STATIC;
		else if(!strcmp(token,"alarm"))
		  dynAttr->clr = ALARM;
	    } else if(!strcmp(token,"vis")) {
		getToken(displayInfo,token);
		getToken(displayInfo,token);
		if(!strcmp(token,"static"))
		  dynAttr->vis = V_STATIC;
		else if(!strcmp(token,"if not zero"))
		  dynAttr->vis = IF_NOT_ZERO;
		else if(!strcmp(token,"if zero"))
		  dynAttr->vis = IF_ZERO;
		else if(!strcmp(token,"calc"))
		  dynAttr->vis = V_CALC;
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parseDynAttrParam(DisplayInfo *displayInfo, DlDynamicAttribute *dynAttr)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    int i;
    char chanName[6];

    do {
	switch( (tokenType=getToken(displayInfo,token)) ) {
	case T_WORD:
	    for(i=0; i < MAX_CALC_RECORDS; i++) {
	      /* Names are chan, chanB, chanC, etc. */
		sprintf(chanName,"chan%c",i?'A'+i:'\0');
		if(!strcmp(token,chanName)) {
		    getToken(displayInfo,token);
		    getToken(displayInfo,token);
		    if(token[0]) {
			strcpy(dynAttr->chan[i],token);
			break;
		    }
		}
		if(!strcmp(token,"calc")) {
		    getToken(displayInfo,token);
		    getToken(displayInfo,token);
		    if(token[0]) {
			strcpy(dynAttr->calc,token);
			break;
		    }
		}
	    }
	    break;
	case T_LEFT_BRACE:
	    nestingLevel++;
	    break;
	case T_RIGHT_BRACE:
	    nestingLevel--;
	    break;
	default:
	    break;
	}
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

/* Reads a colormap from an external file */
DlColormap *parseAndExtractExternalColormap(DisplayInfo *displayInfo,
  char *filename)
{
    DlColormap *dlColormap = NULL;
    FILE *externalFilePtr, *savedFilePtr;
    char token[MAX_TOKEN_LENGTH];
    char msg[2*MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0, done = 0;

    externalFilePtr = dmOpenUsableFile(filename, NULL);
    if(externalFilePtr == NULL) {
      /* Error opening file */
	sprintf(msg,
	  "Can't open external colormap file\n"
	  "Name=\"%s\"\n"
	  "Check cmap specification",
	  filename);
	dmSetAndPopupWarningDialog(displayInfo,msg,"OK",NULL,NULL);
	medmPrintf(1,"\nparseAndExtractExternalColormap: Cannot open file\n"
	  "  filename: %s\n",filename);
    } else {
      /* File opened */
	savedFilePtr = displayInfo->filePtr;
	displayInfo->filePtr = externalFilePtr;

	do {
	    switch( (tokenType=getToken(displayInfo,token)) ) {
	    case T_WORD:
		if(!strcmp(token,"color map") ||
		  !strcmp(token,"<<color map>>")) {
		    dlColormap = parseColormap(displayInfo,externalFilePtr);
		  /* Don't need to parse further, so we'll return here */
		    done = 1;
		}
		break;
	    case T_EQUAL:
		break;
	    case T_LEFT_BRACE:
		nestingLevel++;
		break;
	    case T_RIGHT_BRACE:
		nestingLevel--;
		break;
	    default:
		break;
	    }
	} while(!done && tokenType != T_EOF);

	fclose(externalFilePtr);

      /* restore old filePtr */
	displayInfo->filePtr = savedFilePtr;
    }
  /* KE: There was no return for this branch */
    return(dlColormap);
}


 /* A lexical analyzer (as a state machine), based upon ideas from
  *   "Advanced Unix Programming" by Marc J. Rochkind, with
  *   extensions.  Understands macros of the form $(xyz), and
  *   substitutes the value in displayInfo's nameValueTable..name with
  *   nameValueTable..value */
TOKEN getToken(DisplayInfo *displayInfo, char *word)
{
    FILE *filePtr;
    enum {NEUTRAL,INQUOTE,INWORD,INMACRO} state = NEUTRAL, savedState = NEUTRAL;
    int c;
    char *w, *value;
    char *m, macro[MAX_TOKEN_LENGTH];
    int j;

    filePtr = displayInfo->filePtr;

    w = word;
    m = macro;

    while( (c=getc(filePtr)) != EOF) {
	switch (state) {
	case NEUTRAL:
	    switch(c) {
	    case '=' : return(T_EQUAL);
	    case '{' : return(T_LEFT_BRACE);
	    case '}' : return(T_RIGHT_BRACE);
	    case '"' : state = INQUOTE;
		break;
	    case '$' : c=getc(filePtr);
	      /* only do macro substitution if in execute mode or
	       * parsing a composite file */
		if(parsingCompositeFile && c == '(' ) {
		    state = INMACRO;
		} else {
		    *w++ = '$';
		    *w++ = c;
		}
		break;
	    case ' ' :
	    case '\t':
            case '\r':
	    case '\n': break;

	      /* for constructs of the form (a,b) */
	    case '(' :
	    case ',' :
	    case ')' : *w++ = c; *w = '\0'; return(T_WORD);

	    default  : state = INWORD;
		*w++ = c;
		break;
	    }
	    break;
	case INQUOTE:
	    switch(c) {
	    case '"' : *w = '\0'; return(T_WORD);
	    case '$' : c=getc(filePtr);
	      /* only do macro substitution if in execute mode */
		if((
		     parsingCompositeFile) && c == '(' ) {
		    savedState = INQUOTE;
		    state = INMACRO;
		} else {
		    *w++ = '$';
	            ungetc(c,filePtr);
		}
		break;
	    default  : *w++ = c;
		break;
	    }
	    break;
	case INMACRO:
	    switch(c) {
	    case ')' : *m = '\0';
		value = lookupNameValue(displayInfo->nameValueTable,
		  displayInfo->numNameValues,macro);
		if(value != NULL) {
		    for(j = 0; j < (int) strlen(value); j++) {
			*w++ = value[j];
		    }
		} else {
		    *w++ = '$';
		    *w++ = '(';
		    for(j = 0; j < (int) strlen(macro); j++) {
			*w++ = macro[j];
		    }
		    *w++ = ')';
		}
		state = savedState;
		m = macro;
		break;

	    default  : *m++ = c;
		break;
	    }
	    break;
	case INWORD:
	    switch(c) {
	    case ' ' :
            case '\r':
	    case '\n':
	    case '\t':
	    case '=' :
	    case '(' :
	    case ',' :
	    case ')' :
	    case '"' : ungetc(c,filePtr); *w = '\0'; return(T_WORD);
	    default  : *w++ = c;
		break;
	    }
	    break;
	}
    }
    return(T_EOF);
}


