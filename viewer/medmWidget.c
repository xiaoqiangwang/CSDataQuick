#include "medm.h"

DlElement *createDlArc(DlElement *p)
{
    DlArc *dlArc;
    DlElement *dlElement;

    dlArc = (DlArc*)malloc(sizeof(DlArc));
    if (!dlArc) return 0;
    if (p) {
        *dlArc = *p->structure.arc;
    } else {
        objectAttributeInit(&(dlArc->object));
        basicAttributeInit(&(dlArc->attr));
        dynamicAttributeInit(&(dlArc->dynAttr));
        dlArc->begin = 0;
        dlArc->path = 90*64;
    }

    if (!(dlElement = createDlElement(DL_Arc,
      (void *)      dlArc,
      &arcDlDispatchTable))) {
        free(dlArc);
    }

    return(dlElement);
}

DlElement *parseArc(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlArc *dlArc;
    DlElement *dlElement = createDlArc(NULL);
    if (!dlElement) return 0;
    dlArc = dlElement->structure.arc;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"object"))
              parseObject(displayInfo,&(dlArc->object));
            if (!strcmp(token,"basic attribute"))
              parseBasicAttribute(displayInfo,&(dlArc->attr));
            else
              if (!strcmp(token,"dynamic attribute"))
                parseDynamicAttribute(displayInfo,&(dlArc->dynAttr));
              else
                if (!strcmp(token,"begin")) {
                    getToken(displayInfo,token);
                    getToken(displayInfo,token);
                    dlArc->begin = atoi(token);
                } else
                  if (!strcmp(token,"path")) {
                      getToken(displayInfo,token);
                      getToken(displayInfo,token);
                      dlArc->path = atoi(token);
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

    return dlElement;
}

DlElement *createDlBar(DlElement *p)
{
    DlBar *dlBar;
    DlElement *dlElement;

    dlBar = (DlBar *)malloc(sizeof(DlBar));
    if(!dlBar) return 0;
    if(p) {
        *dlBar = *p->structure.bar;
    } else {
        objectAttributeInit(&(dlBar->object));
        monitorAttributeInit(&(dlBar->monitor));
        limitsAttributeInit(&(dlBar->limits));
        dlBar->label = LABEL_NONE;
        dlBar->clrmod = STATIC;
        dlBar->direction = RIGHT;
        dlBar->fillmod = FROM_EDGE;
    }

    if(!(dlElement = createDlElement(DL_Bar,
      (XtPointer)      dlBar,
      &barDlDispatchTable))) {
        free(dlBar);
    }
    return(dlElement);
}


DlElement *parseBar(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlBar *dlBar;
    DlElement *dlElement = createDlBar(NULL);

    if(!dlElement) return 0;
    dlBar = dlElement->structure.bar;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlBar->object));
            } else if(!strcmp(token,"monitor")) {
                parseMonitor(displayInfo,&(dlBar->monitor));
            } else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"none"))
                  dlBar->label = LABEL_NONE;
                else if(!strcmp(token,"no decorations"))
                  dlBar->label = NO_DECORATIONS;
                else if(!strcmp(token,"outline"))
                  dlBar->label = OUTLINE;
                else if(!strcmp(token,"limits"))
                  dlBar->label = LIMITS;
                else
                  if(!strcmp(token,"channel"))
                    dlBar->label = CHANNEL;
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))
                  dlBar->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))
                  dlBar->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))
                  dlBar->clrmod = DISCRETE;
            } else if(!strcmp(token,"direction")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"up"))
                  dlBar->direction = UP;
                else if(!strcmp(token,"right"))
                  dlBar->direction = RIGHT;
                else if(!strcmp(token,"down"))
                  dlBar->direction = DOWN;
                else if(!strcmp(token,"left"))
                  dlBar->direction = LEFT;
            } else if(!strcmp(token,"fillmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"from edge"))
                  dlBar->fillmod = FROM_EDGE;
                else if(!strcmp(token,"from center"))
                  dlBar->fillmod = FROM_CENTER;
            } else if(!strcmp(token,"limits")) {
              parseLimits(displayInfo,&(dlBar->limits));
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

    return dlElement;
}

DlElement *createDlByte(DlElement *p) {
    DlByte *dlByte;
    DlElement *dlElement;

    dlByte = (DlByte *)malloc(sizeof(DlByte));
    if(!dlByte) return 0;
    if(p) {
        *dlByte = *p->structure.byte;
    } else {
        objectAttributeInit(&(dlByte->object));
        monitorAttributeInit(&(dlByte->monitor));
        dlByte->clrmod = STATIC;
        dlByte->direction = RIGHT;
        dlByte->sbit = 15;
        dlByte->ebit = 0;
    }

    if(!(dlElement = createDlElement(DL_Byte,
      (XtPointer)      dlByte,
      &byteDlDispatchTable))) {
        free(dlByte);
    }

    return(dlElement);
}



DlElement *parseByte( DisplayInfo *displayInfo) {
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlByte *dlByte;
    DlElement *dlElement = createDlByte(NULL);

    if(!dlElement) return 0;
    dlByte = dlElement->structure.byte;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlByte->object));
            } else if(!strcmp(token,"monitor")) {
                parseMonitor(displayInfo,&(dlByte->monitor));
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))       dlByte->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))   dlByte->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))dlByte->clrmod = DISCRETE;
            } else if(!strcmp(token,"direction")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"up")) dlByte->direction = UP;
                else if(!strcmp(token,"right")) dlByte->direction = RIGHT;
                else if(!strcmp(token,"down")) dlByte->direction = DOWN;
                else if(!strcmp(token,"left")) dlByte->direction = LEFT;
            } else if(!strcmp(token,"sbit")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlByte->sbit = atoi(token);
            } else if(!strcmp(token,"ebit")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlByte->ebit = atoi(token);
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

    return dlElement;
}

DlElement *createDlCartesianPlot(DlElement *p)
{
    DlCartesianPlot *dlCartesianPlot;
    DlElement *dlElement;
    int traceNumber;

    dlCartesianPlot = (DlCartesianPlot *)malloc(sizeof(DlCartesianPlot));
    if(!dlCartesianPlot) return 0;
    if(p) {
        *dlCartesianPlot = *p->structure.cartesianPlot;
    } else {
        objectAttributeInit(&(dlCartesianPlot->object));
        plotcomAttributeInit(&(dlCartesianPlot->plotcom));
        dlCartesianPlot->count = 1;
        dlCartesianPlot->style = POINT_PLOT;
        dlCartesianPlot->erase_oldest = ERASE_OLDEST_OFF;
        for(traceNumber = 0; traceNumber < MAX_TRACES; traceNumber++){
          traceAttributeInit(&(dlCartesianPlot->trace[traceNumber]));
          if(traceNumber == 0) dlCartesianPlot->trace[traceNumber].yaxis=0;
        }
        plotAxisDefinitionInit(&(dlCartesianPlot->axis[X_AXIS_ELEMENT]));
        plotAxisDefinitionInit(&(dlCartesianPlot->axis[Y1_AXIS_ELEMENT]));
        plotAxisDefinitionInit(&(dlCartesianPlot->axis[Y2_AXIS_ELEMENT]));
        dlCartesianPlot->trigger[0] = '\0';
        dlCartesianPlot->erase[0] = '\0';
        dlCartesianPlot->countPvName[0] = '\0';
        dlCartesianPlot->eraseMode = ERASE_IF_NOT_ZERO;
    }

    if(!(dlElement = createDlElement(DL_CartesianPlot,
      (XtPointer)      dlCartesianPlot,
      &cartesianPlotDlDispatchTable))) {
        free(dlCartesianPlot);
    }

    return(dlElement);
}

DlElement *parseCartesianPlot(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlCartesianPlot *dlCartesianPlot;
    DlElement *dlElement = createDlCartesianPlot(NULL);
    int traceNumber;

    if(!dlElement) return 0;
    dlCartesianPlot = dlElement->structure.cartesianPlot;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlCartesianPlot->object));
            else if(!strcmp(token,"plotcom"))
              parsePlotcom(displayInfo,&(dlCartesianPlot->plotcom));
            else if(!strcmp(token,"count")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlCartesianPlot->count= atoi(token);
                strcpy(dlCartesianPlot->countPvName,token);
            } else if(!strcmp(token,"style")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"point plot"))
                  dlCartesianPlot->style = POINT_PLOT;
                else if(!strcmp(token,"point"))
                  dlCartesianPlot->style = POINT_PLOT;
                else if(!strcmp(token,"line plot"))
                  dlCartesianPlot->style = LINE_PLOT;
                else if(!strcmp(token,"line"))
                  dlCartesianPlot->style = LINE_PLOT;
                else if(!strcmp(token,"fill under"))
                  dlCartesianPlot->style = FILL_UNDER_PLOT;
                else if(!strcmp(token,"fill-under"))
                  dlCartesianPlot->style = FILL_UNDER_PLOT;
            } else if(!strcmp(token,"erase_oldest")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"on"))
                  dlCartesianPlot->erase_oldest = ERASE_OLDEST_ON;
                else if(!strcmp(token,"off"))
                  dlCartesianPlot->erase_oldest = ERASE_OLDEST_OFF;
                else if(!strcmp(token,"plot last n pts"))
                  dlCartesianPlot->erase_oldest = ERASE_OLDEST_ON;
                else if(!strcmp(token,"plot n pts & stop"))
                  dlCartesianPlot->erase_oldest = ERASE_OLDEST_OFF;
            } else if(!strncmp(token,"trace",5)) {
                traceNumber = MIN(token[6] - '0', MAX_TRACES - 1);
                if(traceNumber == 0) dlCartesianPlot->trace[traceNumber].yaxis=0;
                parseTrace(displayInfo,
                  &(dlCartesianPlot->trace[traceNumber]));
            } else if(!strcmp(token,"x_axis")) {
                parsePlotAxisDefinition(displayInfo,
                  &(dlCartesianPlot->axis[X_AXIS_ELEMENT]));
            } else if(!strcmp(token,"y1_axis")) {
                parsePlotAxisDefinition(displayInfo,
                  &(dlCartesianPlot->axis[Y1_AXIS_ELEMENT]));
            } else if(!strcmp(token,"y2_axis")) {
                parsePlotAxisDefinition(displayInfo,
                  &(dlCartesianPlot->axis[Y2_AXIS_ELEMENT]));
            } else if(!strcmp(token,"trigger")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlCartesianPlot->trigger,token);
            } else if(!strcmp(token,"erase")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlCartesianPlot->erase,token);
            } else if(!strcmp(token,"countPvName")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlCartesianPlot->count= atoi(token);
                strcpy(dlCartesianPlot->countPvName,token);
            } else if(!strcmp(token,"eraseMode")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"if not zero"))
                  dlCartesianPlot->eraseMode = ERASE_IF_NOT_ZERO;
                else if(!strcmp(token,"if zero"))
                  dlCartesianPlot->eraseMode = ERASE_IF_ZERO;
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

    return dlElement;

}

DlElement *createDlChoiceButton(DlElement *p)
{
    DlChoiceButton *dlChoiceButton;
    DlElement *dlElement;

    dlChoiceButton = (DlChoiceButton *)malloc(sizeof(DlChoiceButton));
    if(!dlChoiceButton) return 0;
    if(p) {
        *dlChoiceButton = *(p->structure.choiceButton);
    } else {
        objectAttributeInit(&(dlChoiceButton->object));
        controlAttributeInit(&(dlChoiceButton->control));
        dlChoiceButton->clrmod = STATIC;
        dlChoiceButton->stacking = ROW;
    }

    if(!(dlElement = createDlElement(DL_ChoiceButton,
      (XtPointer)      dlChoiceButton,
      &choiceButtonDlDispatchTable))) {
        free(dlChoiceButton);
    }

    return(dlElement);
}

DlElement *parseChoiceButton(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlChoiceButton *dlChoiceButton;
    DlElement *dlElement = createDlChoiceButton(NULL);

    if(!dlElement) return 0;
    dlChoiceButton = dlElement->structure.choiceButton;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlChoiceButton->object));
            else if(!strcmp(token,"control"))
              parseControl(displayInfo,&(dlChoiceButton->control));
            else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))
                  dlChoiceButton->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))
                  dlChoiceButton->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))
                  dlChoiceButton->clrmod = DISCRETE;
            } else if(!strcmp(token,"stacking")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"row"))
                  dlChoiceButton->stacking = ROW;
                else if(!strcmp(token,"column"))
                  dlChoiceButton->stacking = COLUMN;
                else if(!strcmp(token,"row column"))
                  dlChoiceButton->stacking = ROW_COLUMN;
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

    return dlElement;
}

DlElement *createDlComposite(DlElement *p) {
    DlComposite *dlComposite;
    DlElement *dlElement;

#if DEBUG_COMPOSITE
    print("createDlComposite:\n");
#endif
    dlComposite = (DlComposite *)malloc(sizeof(DlComposite));
    if(!dlComposite) return 0;
    if(p) {
        DlElement *child;
        *dlComposite = *p->structure.composite;

      /* Create the first node */
        dlComposite->dlElementList = createDlList();
        if(!dlComposite->dlElementList) {
            free(dlComposite);
            return NULL;
        }
      /* Copy all children to the composite element list */
        child = FirstDlElement(p->structure.composite->dlElementList);
        while(child) {
            DlElement *copy = child->run->create(child);
            if(copy)
              appendDlElement(dlComposite->dlElementList,copy);
            child = child->next;
        }
    } else {
        objectAttributeInit(&(dlComposite->object));
        dynamicAttributeInit(&(dlComposite->dynAttr));
        dlComposite->compositeName[0] = '\0';
        dlComposite->compositeFile[0] = '\0';
        dlComposite->dlElementList = createDlList();
        if(!dlComposite->dlElementList) {
            free(dlComposite);
            return NULL;
        }
    }

    if(!(dlElement = createDlElement(DL_Composite,
      (XtPointer) dlComposite, &compositeDlDispatchTable))) {
        free(dlComposite->dlElementList);
        free(dlComposite);
        return NULL;
    }
    return dlElement;
}


DlElement *parseComposite(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlComposite *newDlComposite;
    DlElement *dlElement = createDlComposite(NULL);

    if(!dlElement) return 0;
    newDlComposite = dlElement->structure.composite;

    do {
        switch(tokenType=getToken(displayInfo,token)) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(newDlComposite->object));
            } else if(!strcmp(token,"dynamic attribute")) {
                parseDynamicAttribute(displayInfo,&(newDlComposite->dynAttr));
            } else if(!strcmp(token,"composite name")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(newDlComposite->compositeName,token);
            } else if(!strcmp(token,"composite file")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(newDlComposite->compositeFile,token);
                compositeFileParse(displayInfo,dlElement);
              /* Handle composite file here */
            } else if(!strcmp(token,"children")) {
                tokenType=getToken(displayInfo,token);
                parseAndAppendDisplayList(displayInfo,
                  newDlComposite->dlElementList,token,tokenType);
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

    return dlElement;
}

void controlAttributeInit(DlControl *control) {
    control->ctrl[0] = '\0';
    control->clr = 14;
    control->bclr = 0;
}

void parseControl(
  DisplayInfo *displayInfo,
  DlControl *control)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"ctrl") ||
              !strcmp(token,"chan")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(control->ctrl,token);
            } else if (!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                control->clr = atoi(token) % DL_MAX_COLORS;
            } else if (!strcmp(token,"bclr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                control->bclr = atoi(token) % DL_MAX_COLORS;
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
    } while ( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

#define DISPLAY_DEFAULT_X 10
#define DISPLAY_DEFAULT_Y 10

DlElement *createDlDisplay(DlElement *p)
{
    DlDisplay *dlDisplay;
    DlElement *dlElement;


    dlDisplay = (DlDisplay *)malloc(sizeof(DlDisplay));
    if(!dlDisplay) return 0;
    if(p) {
        *dlDisplay = *p->structure.display;
    } else {
        objectAttributeInit(&(dlDisplay->object));
        dlDisplay->object.x = DISPLAY_DEFAULT_X;
        dlDisplay->object.y = DISPLAY_DEFAULT_Y;
        dlDisplay->clr = 0;
        dlDisplay->bclr = 1;
        dlDisplay->cmap[0] = '\0';
        dlDisplay->grid.gridSpacing = DEFAULT_GRID_SPACING;
        dlDisplay->grid.gridOn = DEFAULT_GRID_ON;
        dlDisplay->grid.snapToGrid = DEFAULT_GRID_SNAP;
    }

    if(!(dlElement = createDlElement(DL_Display,(XtPointer)dlDisplay,
      &displayDlDispatchTable))) {
        free(dlDisplay);
    }

    return(dlElement);
}


DlElement *parseDisplay(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlDisplay *dlDisplay;
    DlElement *dlElement = createDlDisplay(NULL);

    if(!dlElement) return 0;
    dlDisplay = dlElement->structure.display;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlDisplay->object));
            } else if(!strcmp(token,"grid")) {
                parseGrid(displayInfo,&(dlDisplay->grid));
            } else if(!strcmp(token,"cmap")) {
              /* Parse separate display list to get and use that colormap */
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(strlen(token) > (size_t) 0) {
                    strcpy(dlDisplay->cmap,token);
                }
            } else if(!strcmp(token,"bclr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlDisplay->bclr = atoi(token) % DL_MAX_COLORS;
                displayInfo->drawingAreaBackgroundColor =
                  dlDisplay->bclr;
            } else if(!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlDisplay->clr = atoi(token) % DL_MAX_COLORS;
                displayInfo->drawingAreaForegroundColor =
                  dlDisplay->clr;
            } else if(!strcmp(token,"gridSpacing")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlDisplay->grid.gridSpacing = atoi(token);
            } else if(!strcmp(token,"gridOn")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlDisplay->grid.gridOn = atoi(token);
            } else if(!strcmp(token,"snapToGrid")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlDisplay->grid.snapToGrid = atoi(token);
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

    appendDlElement(displayInfo->dlElementList,dlElement);
  /* fix up x,y so that 0,0 (old defaults) are replaced */
    if(dlDisplay->object.x <= 0) dlDisplay->object.x = DISPLAY_DEFAULT_X;
    if(dlDisplay->object.y <= 0) dlDisplay->object.y = DISPLAY_DEFAULT_Y;

    return dlElement;
}

DlElement *createDlImage(DlElement *pE)
{
    DlImage *dlImage;
    DlElement *dlElement;

    dlImage = (DlImage *)malloc(sizeof(DlImage));
    if(!dlImage) return 0;
    if(pE) {
        *dlImage = *pE->structure.image;
      /* Make copies of the data pointed to by privateData */
        copyGIF(dlImage,dlImage);
    } else {
        objectAttributeInit(&(dlImage->object));
        dynamicAttributeInit(&(dlImage->dynAttr));
        dlImage->calc[0] = '\0';
        dlImage->calc[0] = '\0';
        dlImage->imageType = NO_IMAGE;
        dlImage->imageName[0] = '\0';
        dlImage->privateData = NULL;
    }

    if(!(dlElement = createDlElement(DL_Image, (XtPointer)dlImage,
      &imageDlDispatchTable))) {
        free(dlImage);
    }

    return(dlElement);
}

DlElement *parseImage(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlImage *dlImage = 0;
    DlElement *dlElement = createDlImage(NULL);

    if(!dlElement) return 0;
    dlImage = dlElement->structure.image;
    do {
        switch( (tokenType = getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlImage->object));
            } else if(!strcmp(token,"dynamic attribute")) {
                parseDynamicAttribute(displayInfo,&(dlImage->dynAttr));
            } else if(!strcmp(token,"type")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"none"))
                  dlImage->imageType = NO_IMAGE;
                else if(!strcmp(token,"gif"))
                  dlImage->imageType = GIF_IMAGE;
                else if(!strcmp(token,"tiff"))
                /* KE: There is no TIFF capability */
                  dlImage->imageType = TIFF_IMAGE;
            } else if(!strcmp(token,"image name")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlImage->imageName,token);
            } else if(!strcmp(token,"calc")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlImage->calc,token);
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

  /* Just to be safe, initialize the privateData member separately */
    dlImage->privateData = NULL;

    return dlElement;
}

DlElement *createDlIndicator(DlElement *p)
{
    DlIndicator *dlIndicator;
    DlElement *dlElement;

    dlIndicator = (DlIndicator *)malloc(sizeof(DlIndicator));
    if(!dlIndicator) return 0;
    if(p) {
        *dlIndicator = *p->structure.indicator;
    } else {
        objectAttributeInit(&(dlIndicator->object));
        monitorAttributeInit(&(dlIndicator->monitor));
        limitsAttributeInit(&(dlIndicator->limits));
        dlIndicator->label = LABEL_NONE;
        dlIndicator->clrmod = STATIC;
        dlIndicator->direction = RIGHT;
    }

    if(!(dlElement = createDlElement(DL_Indicator,
      (XtPointer)      dlIndicator,
      &indicatorDlDispatchTable))) {
        free(dlIndicator);
    }

    return(dlElement);
}


DlElement *parseIndicator(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlIndicator *dlIndicator;
    DlElement *dlElement = createDlIndicator(NULL);

    if(!dlElement) return 0;
    dlIndicator = dlElement->structure.indicator;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlIndicator->object));
            } else if(!strcmp(token,"monitor")) {
                parseMonitor(displayInfo,&(dlIndicator->monitor));
            } else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"none"))
                  dlIndicator->label = LABEL_NONE;
                else if(!strcmp(token,"no decorations"))
                  dlIndicator->label = NO_DECORATIONS;
                else if(!strcmp(token,"outline"))
                  dlIndicator->label = OUTLINE;
                else if(!strcmp(token,"limits"))
                  dlIndicator->label = LIMITS;
                else if(!strcmp(token,"channel"))
                  dlIndicator->label = CHANNEL;
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))
                  dlIndicator->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))
                  dlIndicator->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))
                  dlIndicator->clrmod = DISCRETE;
            } else if(!strcmp(token,"direction")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"up")) dlIndicator->direction = UP;
                else if(!strcmp(token,"right")) dlIndicator->direction = RIGHT;
                else if(!strcmp(token,"down")) dlIndicator->direction = DOWN;
                else if(!strcmp(token,"left")) dlIndicator->direction = LEFT;
            } else if(!strcmp(token,"limits")) {
              parseLimits(displayInfo,&(dlIndicator->limits));
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

    return dlElement;
}

DlElement *createDlMenu(DlElement *p)
{
    DlMenu *dlMenu;
    DlElement *dlElement;

    dlMenu = (DlMenu *)malloc(sizeof(DlMenu));
    if(!dlMenu) return 0;
    if(p) {
        *dlMenu = *(p->structure.menu);
    } else {
        objectAttributeInit(&(dlMenu->object));
        controlAttributeInit(&(dlMenu->control));
        dlMenu->clrmod = STATIC;
    }
    if(!(dlElement = createDlElement(DL_Menu,
      (XtPointer)      dlMenu,
      &menuDlDispatchTable))) {
        free(dlMenu);
    }

    return(dlElement);
}

DlElement *parseMenu(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlMenu *dlMenu;
    DlElement *dlElement = createDlMenu(NULL);

    if(!dlElement) return 0;
    dlMenu = dlElement->structure.menu;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlMenu->object));
            else if(!strcmp(token,"control"))
              parseControl(displayInfo,&(dlMenu->control));
            else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))
                  dlMenu->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))
                  dlMenu->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))
                  dlMenu->clrmod = DISCRETE;
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

    return dlElement;
}

/***
 *** Message Button
 ***/

DlElement *createDlMessageButton(DlElement *p)
{
    DlMessageButton *dlMessageButton;
    DlElement *dlElement;

    dlMessageButton = (DlMessageButton *)malloc(sizeof(DlMessageButton));
    if(p) {
        *dlMessageButton = *(p->structure.messageButton);
    } else {
        objectAttributeInit(&(dlMessageButton->object));
        controlAttributeInit(&(dlMessageButton->control));
        dlMessageButton->label[0] = '\0';
        dlMessageButton->press_msg[0] = '\0';
        dlMessageButton->release_msg[0] = '\0';
        dlMessageButton->clrmod = STATIC;
    }

    if(!(dlElement = createDlElement(DL_MessageButton,
      (XtPointer) dlMessageButton,
      &messageButtonDlDispatchTable))) {
        free(dlMessageButton);
    }

    return(dlElement);
}

DlElement *parseMessageButton(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlMessageButton *dlMessageButton;
    DlElement *dlElement = createDlMessageButton(NULL);

    if(!dlElement) return 0;
    dlMessageButton = dlElement->structure.messageButton;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlMessageButton->object));
            else if(!strcmp(token,"control"))
              parseControl(displayInfo,&(dlMessageButton->control));
            else if(!strcmp(token,"press_msg")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlMessageButton->press_msg,token);
            } else if(!strcmp(token,"release_msg")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlMessageButton->release_msg,token);
            } else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlMessageButton->label,token);
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))
                  dlMessageButton->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))
                  dlMessageButton->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))
                  dlMessageButton->clrmod = DISCRETE;
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

    return dlElement;
}

DlElement *createDlMeter(DlElement *p)
{
    DlMeter *dlMeter;
    DlElement *dlElement;

    dlMeter = (DlMeter *)malloc(sizeof(DlMeter));
    if(!dlMeter) return 0;
    if(p) {
        *dlMeter = *p->structure.meter;
    } else {
        objectAttributeInit(&(dlMeter->object));
        monitorAttributeInit(&(dlMeter->monitor));
        limitsAttributeInit(&(dlMeter->limits));
        dlMeter->label = LABEL_NONE;
        dlMeter->clrmod = STATIC;
    }

    if(!(dlElement = createDlElement(DL_Meter, (XtPointer)dlMeter,
      &meterDlDispatchTable))) {
        free(dlMeter);
    }

    return(dlElement);
}

DlElement *parseMeter(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlMeter *dlMeter;
    DlElement *dlElement = createDlMeter(NULL);
    int i = 0;

    if(!dlElement) return 0;
    dlMeter = dlElement->structure.meter;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlMeter->object));
            else if(!strcmp(token,"monitor"))
              parseMonitor(displayInfo,&(dlMeter->monitor));
            else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_LABEL_TYPE;i<FIRST_LABEL_TYPE+NUM_LABEL_TYPES;i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlMeter->label = i;
                        break;
                    }
                }
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlMeter->clrmod = i;
                        break;
                    }
                }
            } else if(!strcmp(token,"limits")) {
              parseLimits(displayInfo,&(dlMeter->limits));
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

    return dlElement;
}

void monitorAttributeInit(DlMonitor *monitor)
{
    monitor->rdbk[0] = '\0';
    monitor->clr = DEFAULT_CLR;
    monitor->bclr = DEFAULT_BCLR;
}

void plotcomAttributeInit(DlPlotcom *plotcom)
{
    plotcom->title[0] = '\0';
    plotcom->xlabel[0] = '\0';
    plotcom->ylabel[0] = '\0';
    plotcom->clr = DEFAULT_CLR;
    plotcom->bclr = DEFAULT_BCLR;
}

void plotAxisDefinitionInit(DlPlotAxisDefinition *axisDefinition)
{
    axisDefinition->axisStyle = LINEAR_AXIS;
    axisDefinition->rangeStyle = CHANNEL_RANGE;
    axisDefinition->minRange = 0.0;
    axisDefinition->maxRange = 1.0;
    axisDefinition->timeFormat = HHMMSS;
}

void penAttributeInit(DlPen *pen)
{
    pen->chan[0] = '\0';
    pen->clr = DEFAULT_CLR;
    limitsAttributeInit(&(pen->limits));
}

void traceAttributeInit(DlTrace *trace)
{
    trace->xdata[0] = '\0';
    trace->ydata[0] = '\0';
    trace->data_clr = DEFAULT_CLR;
    trace->yaxis = 1;
}

void parseMonitor(DisplayInfo *displayInfo, DlMonitor *monitor)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"rdbk") ||
              !strcmp(token,"chan")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(monitor->rdbk,token);
            } else if (!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                monitor->clr = atoi(token) % DL_MAX_COLORS;
            } else if (!strcmp(token,"bclr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                monitor->bclr = atoi(token) % DL_MAX_COLORS;
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
    } while ( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void parsePlotcom(DisplayInfo *displayInfo, DlPlotcom *plotcom)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"title")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(plotcom->title,token);
            } else if (!strcmp(token,"xlabel")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(plotcom->xlabel,token);
            } else if (!strcmp(token,"ylabel")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(plotcom->ylabel,token);
            } else if (!strcmp(token,"package")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(plotcom->package,token);
            } else if (!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                plotcom->clr = atoi(token) % DL_MAX_COLORS;
            } else if (!strcmp(token,"bclr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                plotcom->bclr = atoi(token) % DL_MAX_COLORS;
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
    } while ( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}


/***
 *** PlotAxisDefinition element in each plot type object
 ***/


void parsePlotAxisDefinition(DisplayInfo *displayInfo, DlPlotAxisDefinition *dlPlotAxisDefinition)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"axisStyle")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if (!strcmp(token,"linear"))
                  dlPlotAxisDefinition->axisStyle = LINEAR_AXIS;
                else if (!strcmp(token,"log10"))
                  dlPlotAxisDefinition->axisStyle = LOG10_AXIS;
                else  if (!strcmp(token,"time"))
                  dlPlotAxisDefinition->axisStyle = TIME_AXIS;
            } else if (!strcmp(token,"rangeStyle")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if (!strcmp(token,"from channel"))
                    dlPlotAxisDefinition->rangeStyle = CHANNEL_RANGE;
                else if (!strcmp(token,"user-specified"))
                  dlPlotAxisDefinition->rangeStyle = USER_SPECIFIED_RANGE;
                else if (!strcmp(token,"auto-scale"))
                  dlPlotAxisDefinition->rangeStyle = AUTO_SCALE_RANGE;
            } else if (!strcmp(token,"minRange")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlPlotAxisDefinition->minRange = (float)atof(token);
            } else if (!strcmp(token,"maxRange")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlPlotAxisDefinition->maxRange = (float)atof(token);
            } else if (!strcmp(token,"timeFormat")) {
                int i;
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for (i=FIRST_CP_TIME_FORMAT;
                     i<FIRST_CP_TIME_FORMAT+NUM_CP_TIME_FORMAT;i++) {
                    if (!strcmp(token,stringValueTable[i])) {
                        dlPlotAxisDefinition->timeFormat = i;
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
    } while ( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}



/***
 *** pen element in each strip chart
 ***/


void parsePen(DisplayInfo *displayInfo, DlPen *pen)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"chan")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(pen->chan,token);
            } else if (!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                pen->clr = atoi(token) % DL_MAX_COLORS;
            } else if (!strcmp(token,"limits")) {
                parseLimits(displayInfo,&pen->limits);
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
    } while ( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

/***
 *** trace element in each cartesian plot
 ***/

void parseTrace(DisplayInfo *displayInfo, DlTrace *trace)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if (!strcmp(token,"xdata")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(trace->xdata,token);
            } else if (!strcmp(token,"ydata")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(trace->ydata,token);
            } else if (!strcmp(token,"data_clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                trace->data_clr = atoi(token) % DL_MAX_COLORS;
#if defined(XRTGRAPH)
            } else if (!strcmp(token,"yaxis")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                trace->yaxis = atoi(token);
#endif
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
    } while ( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

DlElement *createDlOval(DlElement *p)
{
    DlOval *dlOval;
    DlElement *dlElement;

    dlOval = (DlOval *)malloc(sizeof(DlOval));
    if(!dlOval) return 0;
    if(p) {
        *dlOval = *p->structure.oval;
    } else {
        objectAttributeInit(&(dlOval->object));
        basicAttributeInit(&(dlOval->attr));
        dynamicAttributeInit(&(dlOval->dynAttr));
    }

    if(!(dlElement = createDlElement(DL_Oval, (XtPointer)dlOval,
      &ovalDlDispatchTable))) {
        free(dlOval);
    }

    return(dlElement);
}

DlElement *parseOval(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlOval *dlOval;
    DlElement *dlElement = createDlOval(NULL);

    if(!dlElement) return 0;
    dlOval = dlElement->structure.oval;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlOval->object));
            else if(!strcmp(token,"basic attribute"))
              parseBasicAttribute(displayInfo,&(dlOval->attr));
            else if(!strcmp(token,"dynamic attribute"))
              parseDynamicAttribute(displayInfo,&(dlOval->dynAttr));
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

    return dlElement;

}

DlElement *createDlPolygon(DlElement *p)
{
    DlPolygon *dlPolygon;
    DlElement *dlElement;

    dlPolygon = (DlPolygon *)malloc(sizeof(DlPolygon));
    if(!dlPolygon) return 0;
    if(p) {
        int i;
        *dlPolygon = *p->structure.polygon;
        dlPolygon->points = (XPoint *)malloc(dlPolygon->nPoints*sizeof(XPoint));
        for(i = 0; i < dlPolygon->nPoints; i++) {
            dlPolygon->points[i] = p->structure.polygon->points[i];
        }
    } else {
        objectAttributeInit(&(dlPolygon->object));
        basicAttributeInit(&(dlPolygon->attr));
        dynamicAttributeInit(&(dlPolygon->dynAttr));
        dlPolygon->points = NULL;
        dlPolygon->nPoints = 0;
    }

    if(!(dlElement = createDlElement(DL_Polygon,
      (XtPointer)      dlPolygon,
      &polygonDlDispatchTable))) {
        free(dlPolygon);
    }

    return(dlElement);
}

void parsePolygonPoints(DisplayInfo *displayInfo, DlPolygon *dlPolygon)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel;
#define INITIAL_NUM_POINTS 16
    int pointsArraySize = INITIAL_NUM_POINTS;

/* initialize some data in structure */
    dlPolygon->nPoints = 0;
    dlPolygon->points = (XPoint *)malloc(pointsArraySize*sizeof(XPoint));

    nestingLevel = 0;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"(")) {
                if(dlPolygon->nPoints >= pointsArraySize) {
                  /* reallocate the points array: enlarge by 4X, etc */
                    pointsArraySize *= 4;
                    dlPolygon->points = (XPoint *)realloc(
                      dlPolygon->points,
                      (pointsArraySize+1)*sizeof(XPoint));
                }
                getToken(displayInfo,token);
                dlPolygon->points[dlPolygon->nPoints].x = atoi(token);
                getToken(displayInfo,token);    /* separator    */
                getToken(displayInfo,token);
                dlPolygon->points[dlPolygon->nPoints].y = atoi(token);
                getToken(displayInfo,token);    /*   ")"        */
                dlPolygon->nPoints++;
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

  /* ensure closure of the polygon... */
    if(dlPolygon->points[0].x != dlPolygon->points[dlPolygon->nPoints-1].x &&
      dlPolygon->points[0].y != dlPolygon->points[dlPolygon->nPoints-1].y) {
        if(dlPolygon->nPoints >= pointsArraySize) {
            dlPolygon->points = (XPoint *)realloc(dlPolygon->points,
              (dlPolygon->nPoints+2)*sizeof(XPoint));
        }
        dlPolygon->points[dlPolygon->nPoints].x = dlPolygon->points[0].x;
        dlPolygon->points[dlPolygon->nPoints].y = dlPolygon->points[0].y;
        dlPolygon->nPoints++;
    }
}

DlElement *parsePolygon(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlPolygon *dlPolygon;
    DlElement *dlElement = createDlPolygon(NULL);
    if(!dlElement) return 0;
    dlPolygon = dlElement->structure.polygon;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlPolygon->object));
            else
              if(!strcmp(token,"basic attribute"))
                parseBasicAttribute(displayInfo,&(dlPolygon->attr));
              else
                if(!strcmp(token,"dynamic attribute"))
                  parseDynamicAttribute(displayInfo,&(dlPolygon->dynAttr));
                else
                  if(!strcmp(token,"points"))
                    parsePolygonPoints(displayInfo,dlPolygon);
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

    return dlElement;
}

DlElement *createDlPolyline(DlElement *p)
{
    DlPolyline *dlPolyline;
    DlElement *dlElement;


    dlPolyline = (DlPolyline *)malloc(sizeof(DlPolyline));
    if(!dlPolyline) return 0;
    if(p) {
        int i;
        *dlPolyline = *p->structure.polyline;
        dlPolyline->points = (XPoint *)malloc(dlPolyline->nPoints*sizeof(XPoint));
        for(i = 0; i < dlPolyline->nPoints; i++) {
            dlPolyline->points[i] = p->structure.polyline->points[i];
        }
    } else {
        objectAttributeInit(&(dlPolyline->object));
        basicAttributeInit(&(dlPolyline->attr));
        dynamicAttributeInit(&(dlPolyline->dynAttr));
        dlPolyline->points = NULL;
        dlPolyline->nPoints = 0;
        dlPolyline->isFallingOrRisingLine = False;
    }

    if(!(dlElement = createDlElement(DL_Polyline,
      (XtPointer)      dlPolyline,
      &polylineDlDispatchTable))) {
        free(dlPolyline);
    }

    return(dlElement);
}

void parsePolylinePoints(DisplayInfo *displayInfo, DlPolyline *dlPolyline)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel;
    int pointsArraySize = INITIAL_NUM_POINTS;

/* initialize some data in structure */
    dlPolyline->nPoints = 0;
    dlPolyline->points = (XPoint *)malloc(pointsArraySize*sizeof(XPoint));

    nestingLevel = 0;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"(")) {
                if(dlPolyline->nPoints >= pointsArraySize) {
                  /* reallocate the points array: enlarge by 4X, etc */
                    pointsArraySize *= 4;
                    dlPolyline->points = (XPoint *)realloc(
                      dlPolyline->points,
                      (pointsArraySize+1)*sizeof(XPoint));
                }
                getToken(displayInfo,token);
                dlPolyline->points[dlPolyline->nPoints].x = atoi(token);
                getToken(displayInfo,token);    /* separator    */
                getToken(displayInfo,token);
                dlPolyline->points[dlPolyline->nPoints].y = atoi(token);
                getToken(displayInfo,token);    /*   ")"        */
                dlPolyline->nPoints++;
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
}


DlElement *parsePolyline(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlPolyline *dlPolyline;
    DlElement *dlElement = createDlPolyline(NULL);
    if(!dlElement) return 0;
    dlPolyline = dlElement->structure.polyline;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlPolyline->object));
            else if(!strcmp(token,"basic attribute"))
              parseBasicAttribute(displayInfo,&(dlPolyline->attr));
            else if(!strcmp(token,"dynamic attribute"))
              parseDynamicAttribute(displayInfo,&(dlPolyline->dynAttr));
            else if(!strcmp(token,"points"))
              parsePolylinePoints(displayInfo,dlPolyline);
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

    return dlElement;
}

DlElement *createDlRectangle(DlElement *p)
{
    DlRectangle *dlRectangle;
    DlElement *dlElement;

    dlRectangle = (DlRectangle *)malloc(sizeof(DlRectangle));
    if(!dlRectangle) return 0;
    if(p) {
        *dlRectangle = *p->structure.rectangle;
    } else {
        objectAttributeInit(&(dlRectangle->object));
        basicAttributeInit(&(dlRectangle->attr));
        dynamicAttributeInit(&(dlRectangle->dynAttr));
    }

    if(!(dlElement = createDlElement(DL_Rectangle,
      (XtPointer)dlRectangle,
      &rectangleDlDispatchTable))) {
        free(dlRectangle);
    }

    return(dlElement);
}

DlElement *parseRectangle(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlRectangle *dlRectangle;
    DlElement *dlElement = createDlRectangle(NULL);

    if(!dlElement) return 0;
    dlRectangle = dlElement->structure.rectangle;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlRectangle->object));
            else if(!strcmp(token,"basic attribute"))
              parseBasicAttribute(displayInfo,&(dlRectangle->attr));
            else if(!strcmp(token,"dynamic attribute"))
              parseDynamicAttribute(displayInfo,&(dlRectangle->dynAttr));
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

    return dlElement;
}

DlElement *createDlRelatedDisplay(DlElement *p)
{
    DlRelatedDisplay *dlRelatedDisplay;
    DlElement *dlElement;
    int displayNumber;

    dlRelatedDisplay = (DlRelatedDisplay *)malloc(sizeof(DlRelatedDisplay));
    if(!dlRelatedDisplay) return 0;
    if(p) {
        *dlRelatedDisplay = *p->structure.relatedDisplay;
    } else {
        objectAttributeInit(&(dlRelatedDisplay->object));
        for(displayNumber = 0; displayNumber < MAX_RELATED_DISPLAYS;
             displayNumber++)
          createDlRelatedDisplayEntry(
            &(dlRelatedDisplay->display[displayNumber]),
            displayNumber );

        dlRelatedDisplay->clr = globalResourceBundle.clr;
        dlRelatedDisplay->bclr = globalResourceBundle.bclr;
        dlRelatedDisplay->label[0] = '\0';
        dlRelatedDisplay->visual = RD_MENU;
    }

    if(!(dlElement = createDlElement(DL_RelatedDisplay,
      (XtPointer)dlRelatedDisplay,
      &relatedDisplayDlDispatchTable))) {
        free(dlRelatedDisplay);
    }

    return(dlElement);
}

void parseRelatedDisplayEntry(DisplayInfo *displayInfo,
  DlRelatedDisplayEntry *relatedDisplay)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch(tokenType=getToken(displayInfo,token)) {
        case T_WORD:
            if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(relatedDisplay->label,token);
            } else if(!strcmp(token,"name")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(relatedDisplay->name,token);
            } else if(!strcmp(token,"args")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(relatedDisplay->args,token);
            } else if(!strcmp(token,"policy")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,stringValueTable[REPLACE_DISPLAY]))
                  relatedDisplay->mode = REPLACE_DISPLAY;
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
    } while((tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF));
}

DlElement *parseRelatedDisplay(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlRelatedDisplay *dlRelatedDisplay = 0;
    DlElement *dlElement = createDlRelatedDisplay(NULL);
    int displayNumber;
    int rc;

    if(!dlElement) return 0;
    dlRelatedDisplay = dlElement->structure.relatedDisplay;

    do {
        switch(tokenType=getToken(displayInfo,token)) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlRelatedDisplay->object));
            } else if(!strncmp(token,"display",7)) {
              /* Get the display number */
                displayNumber=MAX_RELATED_DISPLAYS-1;
                rc=sscanf(token,"display[%d]",&displayNumber);
                if(rc == 0 || rc == EOF || displayNumber < 0 ||
                  displayNumber > MAX_RELATED_DISPLAYS-1) {
                    displayNumber=MAX_RELATED_DISPLAYS-1;
                }
                parseRelatedDisplayEntry(displayInfo,
                  &(dlRelatedDisplay->display[displayNumber]) );
            } else if(!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlRelatedDisplay->clr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"bclr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlRelatedDisplay->bclr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlRelatedDisplay->label,token);
            } else if(!strcmp(token,"visual")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,stringValueTable[FIRST_RD_VISUAL+1])) {
                    dlRelatedDisplay->visual = RD_ROW_OF_BTN;
                } else
                  if(!strcmp(token,stringValueTable[FIRST_RD_VISUAL+2])) {
                      dlRelatedDisplay->visual = RD_COL_OF_BTN;
                  } else
                    if(!strcmp(token,stringValueTable[FIRST_RD_VISUAL+3])) {
                        dlRelatedDisplay->visual = RD_HIDDEN_BTN;
                    }
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
    } while((tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF));

    return dlElement;

}

DlElement *createDlShellCommand(DlElement *p)
{
    DlShellCommand *dlShellCommand;
    DlElement *dlElement;
    int cmdNumber;

    dlShellCommand = (DlShellCommand *)malloc(sizeof(DlShellCommand));
    if(!dlShellCommand) return 0;
    if(p) {
        *dlShellCommand = *p->structure.shellCommand;
    } else {
        objectAttributeInit(&(dlShellCommand->object));
        for(cmdNumber = 0; cmdNumber < MAX_SHELL_COMMANDS; cmdNumber++)
          createDlShellCommandEntry(
            &(dlShellCommand->command[cmdNumber]),
            cmdNumber );
        dlShellCommand->clr = globalResourceBundle.clr;
        dlShellCommand->bclr = globalResourceBundle.bclr;
        dlShellCommand->label[0] = '\0';
    }

    if(!(dlElement = createDlElement(DL_ShellCommand,
      (XtPointer)      dlShellCommand,
      &shellCommandDlDispatchTable))) {
        free(dlShellCommand);
    }

    return(dlElement);
}

void parseShellCommandEntry(DisplayInfo *displayInfo,
  DlShellCommandEntry *shellCommand)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(shellCommand->label,token);
            } else if(!strcmp(token,"name")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(shellCommand->command,token);
            } else if(!strcmp(token,"args")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(shellCommand->args,token);
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

DlElement *parseShellCommand(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlShellCommand *dlShellCommand = 0;
    DlElement *dlElement = createDlShellCommand(NULL);
    int cmdNumber;
    int rc;

    if(!dlElement) return 0;
    dlShellCommand = dlElement->structure.shellCommand;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlShellCommand->object));
            } else if(!strncmp(token,"command",7)) {
              /* Get the command number */
                cmdNumber=MAX_SHELL_COMMANDS-1;
                rc=sscanf(token,"command[%d]",&cmdNumber);
                if(rc == 0 || rc == EOF || cmdNumber < 0 ||
                  cmdNumber > MAX_SHELL_COMMANDS-1) {
                    cmdNumber=MAX_SHELL_COMMANDS-1;
                }
                parseShellCommandEntry(displayInfo,
                  &(dlShellCommand->command[cmdNumber]) );
            } else if(!strcmp(token,"clr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlShellCommand->clr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"bclr")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlShellCommand->bclr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlShellCommand->label,token);
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

    return dlElement;
}

DlElement *createDlStripChart(DlElement *p)
{
    DlStripChart *dlStripChart;
    DlElement *dlElement;
    int penNumber;


    dlStripChart = (DlStripChart *)malloc(sizeof(DlStripChart));
    if(!dlStripChart) return 0;
    if(p) {
        *dlStripChart = *p->structure.stripChart;
    } else {
        objectAttributeInit(&(dlStripChart->object));
        plotcomAttributeInit(&(dlStripChart->plotcom));
        dlStripChart->period = SC_DEFAULT_PERIOD;
        dlStripChart->units = SC_DEFAULT_UNITS;
#if 1
      /* For backward compatibility */
        dlStripChart->delay = -1.0;
        dlStripChart->oldUnits = SC_DEFAULT_UNITS;
#endif
        for(penNumber = 0; penNumber < MAX_PENS; penNumber++) {
            penAttributeInit(&(dlStripChart->pen[penNumber]));
        }
    }

    if(!(dlElement = createDlElement(DL_StripChart,
      (XtPointer)dlStripChart,
      &stripChartDlDispatchTable))) {
        free(dlStripChart);
    }

    return(dlElement);
}

DlElement *parseStripChart(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlStripChart *dlStripChart;
    DlElement *dlElement = createDlStripChart(NULL);
    int penNumber;
    int isVersion2_1_x = False;

    if(!dlElement) return 0;
    dlStripChart = dlElement->structure.stripChart;

    do {
        switch( (tokenType=getToken(displayInfo, token)) ) {
        case T_WORD:
            if(!strcmp(token, "object"))
              parseObject(displayInfo, &(dlStripChart->object));
            else if(!strcmp(token, "plotcom"))
              parsePlotcom(displayInfo, &(dlStripChart->plotcom));
            else if(!strcmp(token, "period")) {
                getToken(displayInfo, token);
                getToken(displayInfo, token);
                dlStripChart->period = atof(token);
                isVersion2_1_x = True;
            } else if(!strcmp(token, "delay")) {
                getToken(displayInfo, token);
                getToken(displayInfo, token);
                dlStripChart->delay = atoi(token);
            } else if(!strcmp(token, "units")) {
                getToken(displayInfo, token);
                getToken(displayInfo, token);
                if(!strcmp(token, "minute"))
                  dlStripChart->units = MINUTES;
                else if(!strcmp(token, "second"))
                  dlStripChart->units = SECONDS;
                else if(!strcmp(token, "milli second"))
                  dlStripChart->units = MILLISECONDS;
                else if(!strcmp(token, "milli-second"))
                  dlStripChart->units = MILLISECONDS;
                else
                  medmPostMsg(1, "parseStripChart: Illegal units %s\n"
                    "  Using: %s\n", token, stringValueTable[SC_DEFAULT_UNITS]);
            } else if(!strncmp(token, "pen", 3)) {
                penNumber = MIN(token[4] - '0', MAX_PENS-1);
                parsePen(displayInfo, &(dlStripChart->pen[penNumber]));
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

    if(isVersion2_1_x) {
        dlStripChart->delay = -1.0;  /* -1.0 is used as a indicator to save
                                        as new format */
    } else
      if(dlStripChart->delay > 0) {
          double val, dummy1, dummy2;
          switch (dlStripChart->units) {
          case MILLISECONDS:
              dummy1 = -0.060 * (double) dlStripChart->delay;
              break;
          case SECONDS:
              dummy1 = -60 * (double) dlStripChart->delay;
              break;
          case MINUTES:
              dummy1 = -3600.0 * (double) dlStripChart->delay;
              break;
          default:
              dummy1 = -60 * (double) dlStripChart->delay;
              break;
          }

          linear_scale(dummy1, 0.0, 2, &val, &dummy1, &dummy2);
          dlStripChart->period = -val;
          dlStripChart->oldUnits = dlStripChart->units;
          dlStripChart->units = SC_DEFAULT_UNITS;
      }

    return dlElement;

}

DlElement *createDlText(DlElement *p)
{
    DlText *dlText;
    DlElement *dlElement;

    dlText = (DlText *)malloc(sizeof(DlText));
    if(!dlText) return 0;
    if(p) {
        *dlText = *p->structure.text;
    } else {
        objectAttributeInit(&(dlText->object));
        basicAttributeInit(&(dlText->attr));
        dynamicAttributeInit(&(dlText->dynAttr));
        dlText->textix[0] = '\0';
        dlText->align = HORIZ_LEFT;
    }

    if(!(dlElement = createDlElement(DL_Text,
      (XtPointer)      dlText,
      &textDlDispatchTable))) {
        free(dlText);
    }

    return(dlElement);
}

DlElement *parseText(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlText *dlText;
    DlElement *dlElement = createDlText(NULL);
    int i = 0;

    if(!dlElement) return 0;
    dlText = dlElement->structure.text;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlText->object));
            } else if(!strcmp(token,"basic attribute"))
              parseBasicAttribute(displayInfo,&(dlText->attr));
            else if(!strcmp(token,"dynamic attribute"))
              parseDynamicAttribute(displayInfo,&(dlText->dynAttr));
            else if(!strcmp(token,"textix")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlText->textix,token);
            } else if(!strcmp(token,"align")) {
                int found=0;

                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_TEXT_ALIGN;i<FIRST_TEXT_ALIGN+NUM_TEXT_ALIGNS; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlText->align = i;
                        found=1;
                        break;
                    }
                }
              /* Backward compatibility */
                if(!found) {
                    if(!strcmp(token,"vert. top")) {
                        dlText->align = HORIZ_LEFT;
                    } else if(!strcmp(token,"vert. centered")) {
                        dlText->align = HORIZ_CENTER;
                    } else if(!strcmp(token,"vert. bottom")) {
                        dlText->align = HORIZ_RIGHT;
                    }
                }
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

    return dlElement;
}

DlElement *createDlTextEntry(DlElement *p)
{
    DlTextEntry *dlTextEntry;
    DlElement *dlElement;

    dlTextEntry = (DlTextEntry *)malloc(sizeof(DlTextEntry));
    if(!dlTextEntry) return 0;
    if(p) {
        *dlTextEntry = *(p->structure.textEntry);
    } else {
        objectAttributeInit(&(dlTextEntry->object));
        controlAttributeInit(&(dlTextEntry->control));
        limitsAttributeInit(&(dlTextEntry->limits));
        dlTextEntry->limits.loprSrc0 = PV_LIMITS_UNUSED;
        dlTextEntry->limits.loprSrc = PV_LIMITS_UNUSED;
        dlTextEntry->limits.hoprSrc0 = PV_LIMITS_UNUSED;
        dlTextEntry->limits.hoprSrc = PV_LIMITS_UNUSED;
        dlTextEntry->clrmod = STATIC;
        dlTextEntry->format = MEDM_DECIMAL;
    }

    if(!(dlElement = createDlElement(DL_TextEntry,
      (XtPointer)      dlTextEntry,
      &textEntryDlDispatchTable))) {
        free(dlTextEntry);
    }

    return dlElement;
}

DlElement *parseTextEntry(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlTextEntry *dlTextEntry;
    DlElement *dlElement = createDlTextEntry(NULL);
    int i = 0;

    if(!dlElement) return 0;
    dlTextEntry = dlElement->structure.textEntry;
    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlTextEntry->object));
            else if(!strcmp(token,"control"))
              parseControl(displayInfo,&(dlTextEntry->control));
            else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlTextEntry->clrmod = i;
                        break;
                    }
                }
            } else if(!strcmp(token,"format")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_TEXT_FORMAT;i<FIRST_TEXT_FORMAT+NUM_TEXT_FORMATS; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlTextEntry->format = i;
                        break;
                    }
                }
            } else if(!strcmp(token,"limits")) {
                parseLimits(displayInfo,&(dlTextEntry->limits));
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

    return dlElement;

}

DlElement *createDlTextUpdate(DlElement *p)
{
    DlTextUpdate *dlTextUpdate;
    DlElement *dlElement;

    dlTextUpdate = (DlTextUpdate *)malloc(sizeof(DlTextUpdate));
    if(!dlTextUpdate) return 0;
    if(p) {
        *dlTextUpdate = *p->structure.textUpdate;
    } else {
        objectAttributeInit(&(dlTextUpdate->object));
        monitorAttributeInit(&(dlTextUpdate->monitor));
        limitsAttributeInit(&(dlTextUpdate->limits));
        dlTextUpdate->limits.loprSrc0 = PV_LIMITS_UNUSED;
        dlTextUpdate->limits.loprSrc = PV_LIMITS_UNUSED;
        dlTextUpdate->limits.hoprSrc0 = PV_LIMITS_UNUSED;
        dlTextUpdate->limits.hoprSrc = PV_LIMITS_UNUSED;
        dlTextUpdate->clrmod = STATIC;
        dlTextUpdate->align = HORIZ_LEFT;
        dlTextUpdate->format = MEDM_DECIMAL;
    }

    if(!(dlElement = createDlElement(DL_TextUpdate,
      (XtPointer)      dlTextUpdate,
      &textUpdateDlDispatchTable))) {
        free(dlTextUpdate);
    }

    return(dlElement);
}

DlElement *parseTextUpdate(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlTextUpdate *dlTextUpdate;
    DlElement *dlElement = createDlTextUpdate(NULL);
    int i= 0;

    if(!dlElement) return 0;
    dlTextUpdate = dlElement->structure.textUpdate;


    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
                parseObject(displayInfo,&(dlTextUpdate->object));
            } else if(!strcmp(token,"monitor")) {
                parseMonitor(displayInfo,&(dlTextUpdate->monitor));
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlTextUpdate->clrmod = i;
                        break;
                    }
                }
            } else if(!strcmp(token,"format")) {
                int found = 0;

                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_TEXT_FORMAT;i<FIRST_TEXT_FORMAT+NUM_TEXT_FORMATS; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlTextUpdate->format = i;
                        found = 1;
                        break;
                    }
                }
              /* Backward compatibility */
                if(!found) {
                    if(!strcmp(token,"decimal")) {
                        dlTextUpdate->format = MEDM_DECIMAL;
                    } else if(!strcmp(token,
                      "decimal- exponential notation")) {
                        dlTextUpdate->format = EXPONENTIAL;
                    } else if(!strcmp(token,"engr. notation")) {
                        dlTextUpdate->format = ENGR_NOTATION;
                    } else if(!strcmp(token,"decimal- compact")) {
                        dlTextUpdate->format = COMPACT;
                    } else if(!strcmp(token,"decimal- truncated")) {
                        dlTextUpdate->format = TRUNCATED;
                      /* (MDA) allow for LANL spelling errors {like
                         above, but with trailing space} */
                    } else if(!strcmp(token,"decimal- truncated ")) {
                        dlTextUpdate->format = TRUNCATED;
                      /* (MDA) allow for LANL spelling errors
                         {hexidecimal vs. hexadecimal} */
                    } else if(!strcmp(token,"hexidecimal")) {
                        dlTextUpdate->format = HEXADECIMAL;
                    }
                }
            } else if(!strcmp(token,"align")) {
                int found=0;

                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_TEXT_ALIGN;i<FIRST_TEXT_ALIGN+NUM_TEXT_ALIGNS; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlTextUpdate->align = i;
                        found=1;
                        break;
                    }
                }
              /* Backward compatibility */
                if(!found) {
                    if(!strcmp(token,"vert. top")) {
                        dlTextUpdate->align = HORIZ_LEFT;
                    } else if(!strcmp(token,"vert. centered")) {
                        dlTextUpdate->align = HORIZ_CENTER;
                    } else if(!strcmp(token,"vert. bottom")) {
                        dlTextUpdate->align = HORIZ_RIGHT;
                    }
                }
            } else if(!strcmp(token,"limits")) {
                parseLimits(displayInfo,&(dlTextUpdate->limits));
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

    return dlElement;
}

DlElement *createDlValuator(DlElement *p)
{
    DlValuator *dlValuator;
    DlElement *dlElement;

    dlValuator = (DlValuator *)malloc(sizeof(DlValuator));
    if(!dlValuator) return 0;
    if(p) {
        *dlValuator = *(p->structure.valuator);
    } else {
        objectAttributeInit(&(dlValuator->object));
        controlAttributeInit(&(dlValuator->control));
        limitsAttributeInit(&(dlValuator->limits));
        dlValuator->label = LABEL_NONE;
        dlValuator->clrmod = STATIC;
        dlValuator->direction = RIGHT;
        dlValuator->dPrecision = 1.;
    }

  /* Private run-time valuator field */
    dlValuator->enableUpdates = True;
    dlValuator->dragging = False;

    if(!(dlElement = createDlElement(DL_Valuator,
      (XtPointer)dlValuator,
      &valuatorDlDispatchTable))) {
        free(dlValuator);
    }

    return(dlElement);
}

DlElement *parseValuator(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlValuator *dlValuator;
    DlElement *dlElement = createDlValuator(NULL);
    if(!dlElement) return 0;
    dlValuator = dlElement->structure.valuator;

    do {
        switch((tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object"))
              parseObject(displayInfo,&(dlValuator->object));
            else if(!strcmp(token,"control"))
              parseControl(displayInfo,&(dlValuator->control));
            else if(!strcmp(token,"label")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"none"))
                  dlValuator->label = LABEL_NONE;
                else if(!strcmp(token,"no decorations"))
                  dlValuator->label = NO_DECORATIONS;
                else if(!strcmp(token,"outline"))
                  dlValuator->label = OUTLINE;
                else if(!strcmp(token,"limits"))
                  dlValuator->label = LIMITS;
                else if(!strcmp(token,"channel"))
                  dlValuator->label = CHANNEL;
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"static"))
                  dlValuator->clrmod = STATIC;
                else if(!strcmp(token,"alarm"))
                  dlValuator->clrmod = ALARM;
                else if(!strcmp(token,"discrete"))
                  dlValuator->clrmod = DISCRETE;
            } else  if(!strcmp(token,"direction")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                if(!strcmp(token,"up")) dlValuator->direction = UP;
                else if(!strcmp(token,"right")) dlValuator->direction = RIGHT;
                else if(!strcmp(token,"down")) dlValuator->direction = DOWN;
                else if(!strcmp(token,"left")) dlValuator->direction = LEFT;
            } else if(!strcmp(token,"dPrecision")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                dlValuator->dPrecision = atof(token);
            } else if(!strcmp(token,"limits")) {
                parseLimits(displayInfo,&(dlValuator->limits));
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
    } while((tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );

    return dlElement;
}

DlElement *createDlWheelSwitch(DlElement *p)
{
    DlWheelSwitch *dlWheelSwitch;
    DlElement *dlElement;

    dlWheelSwitch = (DlWheelSwitch *)malloc(sizeof(DlWheelSwitch));
    if(!dlWheelSwitch) return 0;
    if(p) {
        *dlWheelSwitch = *p->structure.wheelSwitch;
    } else {
        objectAttributeInit(&(dlWheelSwitch->object));
        controlAttributeInit(&(dlWheelSwitch->control));
        limitsAttributeInit(&(dlWheelSwitch->limits));
        dlWheelSwitch->clrmod = STATIC;
        *dlWheelSwitch->format = '\0';
    }

    if(!(dlElement = createDlElement(DL_WheelSwitch, (XtPointer)dlWheelSwitch,
      &wheelSwitchDlDispatchTable))) {
        free(dlWheelSwitch);
    }

    return(dlElement);
}

DlElement *parseWheelSwitch(DisplayInfo *displayInfo)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    DlWheelSwitch *dlWheelSwitch;
    DlElement *dlElement = createDlWheelSwitch(NULL);
    int i = 0;

    if(!dlElement) return 0;
    dlWheelSwitch = dlElement->structure.wheelSwitch;

    do {
        switch( (tokenType=getToken(displayInfo,token)) ) {
        case T_WORD:
            if(!strcmp(token,"object")) {
              parseObject(displayInfo,&(dlWheelSwitch->object));
            } else if(!strcmp(token,"control")) {
              parseControl(displayInfo,&(dlWheelSwitch->control));
            } else if(!strcmp(token,"clrmod")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                for(i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        dlWheelSwitch->clrmod = i;
                        break;
                    }
                }
            } else if(!strcmp(token,"limits")) {
                parseLimits(displayInfo,&(dlWheelSwitch->limits));
            } else if(!strcmp(token,"format")) {
                getToken(displayInfo,token);
                getToken(displayInfo,token);
                strcpy(dlWheelSwitch->format,token);
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

    return dlElement;
}


