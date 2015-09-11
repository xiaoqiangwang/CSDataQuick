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

/*
 * include file for widget-based display manager
 */
#ifndef __MEDMWIDGET_H__
#define __MEDMWIDGET_H__

#ifdef ALLOCATE_STORAGE
#define EXTERN
#else
#define EXTERN extern
#endif

#include <limits.h>
#include <sys/types.h>
#if 0
#include <sys/times.h>
#else
#include <time.h>
#endif

#ifndef MAX
#define MAX(a,b)  ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b)  ((a)<(b)?(a):(b))
#endif

#ifndef STRDUP
#define STRDUP(a) (strcpy( (char *)malloc(strlen(a)+1),a))
#endif

/************************************************************************
 * special types
 ************************************************************************/

/* Name-value table (for macro substitutions in display files) */
typedef struct {
    char *name;
    char *value;
} NameValueTable;

/* DisplayInfo: EPICS Display specific information, one per display
   file */
typedef struct _DisplayInfo {
    FILE *filePtr;
    int versionNumber;
  /* Widgets and main pixmap */
  /* Colormap and attribute data (one exists at a time for each display)        */
    int dlColormapCounter;
    int dlColormapSize;
  /* Display list pointers */
    DlList *dlElementList;
  /* For macro-substitution in display lists */
    NameValueTable *nameValueTable;
    int numNameValues;
    DlFile *dlFile;
    DlColormap  *dlColormap;
  /* Linked list of displayInfo's    */
    struct _DisplayInfo *next;
    struct _DisplayInfo *prev;
} DisplayInfo;

/***
 *** ELEMENT_STRING_TABLE for definition of strings for display elements
 ***	any changes of types or ordering of {DL_Valuator,...,DL_Meter,...} etc
 ***	in displayList.h must have corresponding changes in this table!!
 ***/
#ifndef ALLOCATE_STORAGE
extern char *elementStringTable[NUM_DL_ELEMENT_TYPES];
#else
char *elementStringTable[NUM_DL_ELEMENT_TYPES] = {
    "Element", "Composite", "Display",
    "Choice Button", "Menu", "Message Button", "Related Display",
    "Shell Command", "Text Entry", "Slider", "Wheel Switch",
    "Bar Monitor", "Byte Monitor", "Cartesian Plot", "Scale Monitor", "Meter",
    "Strip Chart", "Text Monitor",
    "Arc", "Image", "Line", "Oval", "Polygon", "Polyline", "Rectangle", "Text"
};
#endif

#define elementType(x) (elementStringTable[(x)-DL_Element])

/* Global variables */

/* Initial (default - not related to any displays) colormap */
EXTERN DisplayInfo *displayInfoListHead, *displayInfoListTail;
EXTERN DisplayInfo *displayInfoSaveListHead, *displayInfoSaveListTail;
EXTERN DisplayInfo *currentDisplayInfo;

/* (MDA) */
EXTERN DisplayInfo *pointerInDisplayInfo;

/* Flag which says we are parsing a composite file and to replace macros */
EXTERN Boolean_t parsingCompositeFile;

/* For object palette selections and single object selection */
EXTERN DlElementType currentElementType;

/* For private or shared/default colormap utilization */
EXTERN Boolean_t privateCmap;

#endif  /* __MEDMWIDGET_H__ */
