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

#ifndef __PROTO_H__
#define __PROTO_H__

/* This file uses timeval, but it will be defined before this file is
   included. */
#include "parse.h"

/* Create methods  */
DisplayInfo *createDisplay(void);
DlColormap *createDlColormap(DisplayInfo *displayInfo);
DlElement *createDlArc(DlElement *);
DlElement *createDlBar(DlElement *);
DlElement *createDlByte(DlElement *);
DlElement *createDlCartesianPlot(DlElement *);
DlElement *createDlChoiceButton(DlElement *);
DlElement *createDlComposite(DlElement *);
DlElement *createDlDisplay(DlElement *);
DlElement *createDlImage(DlElement *);
DlElement *createDlIndicator(DlElement *);
DlElement *createDlMenu(DlElement *);
DlElement *createDlMessageButton(DlElement *);
DlElement *createDlMeter(DlElement *);
DlElement *createDlOval(DlElement *);
DlElement *createDlPolygon(DlElement *);
DlElement *createDlPolyline(DlElement *);
DlElement *createDlRectangle(DlElement *);
DlElement *createDlRelatedDisplay(DlElement *);
DlElement *createDlShellCommand(DlElement *);
DlElement *createDlStripChart(DlElement *);
DlElement *createDlText(DlElement *);
DlElement *createDlTextEntry(DlElement *);
DlElement *createDlTextUpdate(DlElement *);
DlElement *createDlValuator(DlElement *);
DlElement *createDlWheelSwitch(DlElement *);
DlElement *handleImageCreate();
DlElement *handlePolygonCreate(int x0, int y0);
DlElement *handlePolylineCreate(int x0, int y0, Boolean_t simpleLine);
DlElement *handleTextCreate(int x0, int y0);
DlElement* createDlElement(DlElementType, void*, DlDispatchTable *);
DlFile *createDlFile(DisplayInfo *displayInfo);
void createDlObject(DisplayInfo *displayInfo, DlObject *object);

/* Parse methods */
DlElement *parseChoiceButton(DisplayInfo *);
DlElement *parseMessageButton(DisplayInfo *);
DlElement *parseValuator(DisplayInfo *);
DlElement *parseWheelSwitch(DisplayInfo *);
DlElement *parseTextEntry(DisplayInfo *);
DlElement *parseMenu(DisplayInfo *);
void parseControl(DisplayInfo *, DlControl *control);
DlElement *parseImage(DisplayInfo *);
DlElement *parseComposite(DisplayInfo *);
DlElement *parsePolyline(DisplayInfo *);
DlElement *parsePolygon(DisplayInfo *);
DlElement *parseMeter(DisplayInfo *);
DlElement *parseBar(DisplayInfo *);
DlElement *parseByte(DisplayInfo *);
DlElement *parseIndicator(DisplayInfo *);
DlElement *parseTextUpdate(DisplayInfo *);
DlElement *parseStripChart(DisplayInfo *);
DlElement *parseCartesianPlot(DisplayInfo *);
void parseMonitor(DisplayInfo *displayInfo, DlMonitor *monitor);
void parsePlotcom(DisplayInfo *displayInfo, DlPlotcom *plotcom);
void parsePen(DisplayInfo *displayInfo, DlPen *pen);
void parseTrace(DisplayInfo *displayInfo, DlTrace *trace);
void parsePlotAxisDefinition(DisplayInfo *displayInfo,
  DlPlotAxisDefinition *axisDefinition);

/* display.c */
DisplayInfo *allocateDisplayInfo(void);
TOKEN parseAndAppendDisplayList(DisplayInfo *displayInfo, DlList *dlList,
  char *firstToken, TOKEN firstTokenType);

/* medmCommon.c */
void objectAttributeInit(DlObject *object);
void basicAttributeInit(DlBasicAttribute *attr);
void dynamicAttributeInit(DlDynamicAttribute *dynAttr);
void limitsAttributeInit(DlLimits *limits);
DlFile *parseFile(DisplayInfo *displayInfo);
DlColormap *parseColormap(DisplayInfo *displayInfo, FILE *filePtr);
void parseBasicAttribute(DisplayInfo *, DlBasicAttribute *);
void parseDynamicAttribute(DisplayInfo *, DlDynamicAttribute *);
void parseOldBasicAttribute(DisplayInfo *, DlBasicAttribute *);
void parseOldDynamicAttribute(DisplayInfo *, DlDynamicAttribute *);
void parseLimits(DisplayInfo *displayInfo, DlLimits *limits);
DlElement *parseRectangle(DisplayInfo *);
DlElement *parseOval(DisplayInfo *);
DlElement *parseArc(DisplayInfo *);
DlElement *parseText(DisplayInfo *);
DlElement *parseRisingLine(DisplayInfo *);
DlElement *parseFallingLine(DisplayInfo *);
DlElement *parseRelatedDisplay(DisplayInfo *);
DlElement * parseShellCommand(DisplayInfo *);
void parseDlColor(DisplayInfo *displayInfo, FILE *filePtr,
  DlColormapEntry *dlColor);
void parseObject(DisplayInfo *displayInfo, DlObject *object);
void parseGrid(DisplayInfo *displayInfo, DlGrid *grid);
void parseRelatedDisplayEntry(DisplayInfo *displayInfo,
  DlRelatedDisplayEntry *relatedDisplay);
void parseShellCommandEntry(DisplayInfo *displayInfo,
  DlShellCommandEntry *shellCommand);
DlColormap *parseAndExtractExternalColormap(DisplayInfo *displayInfo,
  char *filename);
void parseAndSkip(DisplayInfo *displayInfo);
TOKEN getToken(DisplayInfo *displayInfo, char *word);

/* utils.c */
DlList *createDlList();
void emptyDlList(DlList *);
void appendDlList(DlList *, DlList *);
void insertDlElement(DlList *,DlElement *);
void insertAfter(DlList *l, DlElement *p1, DlElement *p2);
void insertDlListAfter(DlList *l1, DlElement *p, DlList *l2);
void removeDlElement(DlList *,DlElement *);
void dumpDlElementList(DlList *l);

#endif  /* __PROTO_H__ */
