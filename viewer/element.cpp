#include "element.h"

BasicAttribute::BasicAttribute ()
{
    clr = 0;
    style = SOLID;
    fill = F_SOLID;
    width = 0;
}

void BasicAttribute::parse(istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(fstream,token)) ) {
        case T_WORD:
            if(!strcmp(token,"clr")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->clr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"style")) {
                getToken(fstream,token);
                getToken(fstream,token);
                if(!strcmp(token,"solid")) {
                    this->style = SOLID;
                } else if(!strcmp(token,"dash")) {
                    this->style = DASH;
                }
            } else if(!strcmp(token,"fill")) {
                getToken(fstream,token);
                getToken(fstream,token);
                if(!strcmp(token,"solid")) {
                    this->fill = F_SOLID;
                } else if(!strcmp(token,"outline")) {
                    this->fill = F_OUTLINE;
                }
           } else if(!strcmp(token,"width")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->width = atoi(token);
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

DynamicAttribute::DynamicAttribute ()
{
    clr = STATIC;
    vis = V_STATIC;
    validCalc = false;
}

void DynamicAttribute::parse(istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"mod")) {
            this->parseMode(fstream);
        } else if(!strcmp(token,"param")) {
            this->parseParam(fstream);
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

void DynamicAttribute::parseMode(istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"clr")) {
            getToken(fstream,token);
            getToken(fstream,token);
        if(!strcmp(token,"discrete"))
            this->clr = DISCRETE;
        else if(!strcmp(token,"static"))
            this->clr = STATIC;
        else if(!strcmp(token,"alarm"))
            this->clr = ALARM;
        } else if(!strcmp(token,"vis")) {
            getToken(fstream,token);
            getToken(fstream,token);
            if(!strcmp(token,"static"))
                this->vis = V_STATIC;
            else if(!strcmp(token,"if not zero"))
                this->vis = IF_NOT_ZERO;
            else if(!strcmp(token,"if zero"))
                this->vis = IF_ZERO;
            else if(!strcmp(token,"calc"))
                this->vis = V_CALC;
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

void DynamicAttribute::parseParam(istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    int i;
    char chanName[6];

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        for(i=0; i < MAX_CALC_RECORDS; i++) {
            /* Names are chan, chanB, chanC, etc. */
            sprintf(chanName,"chan%c",i?'A'+i:'\0');
            if(!strcmp(token,chanName)) {
                getToken(fstream,token);
                getToken(fstream,token);
                if(token[0]) {
                    this->chan[i] = token;
                    break;
                }
            }
            if(!strcmp(token,"calc")) {
                getToken(fstream,token);
                getToken(fstream,token);
                if(token[0]) {
                    this->calc = token;
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

Object::Object()
{
    x = 0;
    y = 0;
    width = 10;
    height = 10;
}

void Object::parse(istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"x")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->x = atoi(token);
        } else if(!strcmp(token,"y")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->y = atoi(token);
        } else if(!strcmp(token,"width")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->width = atoi(token);
        } else if(!strcmp(token,"height")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->height = atoi(token);
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
