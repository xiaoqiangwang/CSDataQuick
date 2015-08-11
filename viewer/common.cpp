#define ALLOCATE_STORAGE
#include "common.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <cstdio>
#include <string>
#include <map>
#include <vector>

static std::map<std::string, std::string> nameValueTable;

/* A lexical analyzer (as a state machine), based upon ideas from
  *   "Advanced Unix Programming" by Marc J. Rochkind, with
  *   extensions.  Understands macros of the form $(xyz), and
  *   substitutes the value in displayInfo's nameValueTable..name with
  *   nameValueTable..value */
TOKEN getToken(std::istream &fstream, char *word)
{
    enum {NEUTRAL,INQUOTE,INWORD,INMACRO} state = NEUTRAL, savedState = NEUTRAL;
    char c;
    char *w;
    char *m, macro[MAX_TOKEN_LENGTH];
    int j;

    w = word;
    m = macro;

    while( (c=fstream.get()) != EOF) {
        switch (state) {
        case NEUTRAL:
            switch(c) {
            case '=' : return(T_EQUAL);
            case '{' : return(T_LEFT_BRACE);
            case '}' : return(T_RIGHT_BRACE);
            case '"' : state = INQUOTE;
                break;
            case '$' : c=fstream.get();
                /* only do macro substitution if in execute mode or
           * parsing a composite file */
                if( c == '(' ) {
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
            case '$' : c=fstream.get();
                /* do macro substitution */
                if( c == '(' ) {
                    savedState = INQUOTE;
                    state = INMACRO;
                } else {
                    *w++ = '$';
                    fstream.unget();
                }
                break;
            default  : *w++ = c;
                break;
            }
            break;
        case INMACRO:
            switch(c) {
            case ')' : *m = '\0';
                if (nameValueTable.find(macro) != nameValueTable.end()) {
                    std::string v = nameValueTable[macro];
                    strncpy(w,  v.c_str(), v.size());
                    w += v.size();
                }
                else {
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
            case '"' : fstream.unget(); *w = '\0'; return(T_WORD);
            default  : *w++ = c;
                break;
            }
            break;
        }
    }
    return(T_EOF);
}

void parseAndSkip(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

  /* Just read and look for braces until we return to the same level */
    do {
        switch( (tokenType=getToken(fstream,token)) ) {
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

