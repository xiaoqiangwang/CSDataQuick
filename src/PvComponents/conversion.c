/*************************************************************************\
 * This source file includes function to convert numeric value to various
 * formats string. It is composed from source file updateMonitors.c of MEDM.
 * The copyright belongs to the original author.
\*************************************************************************/
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

#include <epicsTypes.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int localCvtLongToHexString (epicsInt32 source, char *pdest);
void localCvtDoubleToString (double flt_value, char  *pstr_value, unsigned short precision);
void localCvtDoubleToExpNotationString (double value, char *textField, unsigned short precision);
void medmLocalCvtDoubleToSexaStr (double value,  char *string, unsigned short prec,
                                 double hopr, double lopr, int *status);

/* Convert hex digits to ascii */
static char hex_digit_to_ascii[16]={'0','1','2','3','4','5','6','7','8','9',
                    'a','b','c','d','e','f'};

int localCvtLongToHexString(epicsInt32 source, char *pdest)
{
    epicsInt32 val,temp;
    char digit[10];
    int i,j;
    char *startAddr = pdest;

    *pdest++ = '0';
    *pdest++ = 'x';
    if(source==0) {
    *pdest++ = '0';
    } else {
    val = source;
    temp = 0xf & val;
    val = val >> 4;
    digit[0] = hex_digit_to_ascii[temp];
    if(val < 0) val = val & 0xfffffff;
    for(i=1; val!=0; i++) {
        temp = 0xf & val;
        val = val >> 4;
        digit[i] = hex_digit_to_ascii[temp];
    }
    for(j=i-1; j>=0; j--) {
        *pdest++ = digit[j];
    }

    }
    *pdest = 0;
    return((int)(pdest-startAddr));
}


void localCvtDoubleToString(double flt_value, char  *pstr_value,
  unsigned short precision)
{
  /* The PREC field is a short, which can be negative, which will be a
   *   large number when converted to unsigned short.  Handle this by
   *   not letting precision be larger than 17 (as for libCom:
   *   cvtDoubleToString()).  Letting it be zero here would be OK, but
   *   not in general, since precision is the number of significant
   *   digits for g and e formats. */
    if(precision > 17) precision=17;
    sprintf(pstr_value,"%.*f",(int)precision,flt_value);
}

void localCvtDoubleToExpNotationString(double value, char *textField,
  unsigned short precision)
{
    double absVal, newVal;
    int minus;
    int exp, k, l;
    char TF[128];

  /* The PREC field is a short, which can be negative, which will be a
   *   large number when converted to unsigned short.  This is handled
   *   in localCvtDoubleToString. */

    absVal = fabs(value);
    minus = (value < 0.0 ? 1 : 0);
    newVal = absVal;

    if(absVal < 1.) {
      /* absVal < 1. */
    exp = 0;
    if(absVal != 0.) {     /* really ought to test against some epsilon */
        do {
        newVal *= 1000.0;
        exp += 3;
        } while(newVal < 1.);
    }
    localCvtDoubleToString(newVal,TF,precision);
    k = 0; l = 0;
    if(minus) textField[k++] = '-';
    while(TF[l] != '\0') textField[k++] = TF[l++];
    textField[k++] = 'e';
    if(exp == 0) {
        textField[k++] = '+';     /* want e+00 for consistency with norms */
    } else {
        textField[k++] = '-';
    }
    textField[k++] = '0' + exp/10;
    textField[k++] = '0' + exp%10;
    textField[k++] = '\0';

    } else {

      /* absVal >= 1. */
    exp = 0;
    while(newVal >= 1000.) {
        newVal *= 0.001; /* since multiplying is usually faster than dividing */
        exp += 3;
    }
    localCvtDoubleToString(newVal,TF,precision);
    k = 0; l = 0;
    if(minus) textField[k++] = '-';
    while(TF[l] != '\0') textField[k++] = TF[l++];
    textField[k++] = 'e';
    textField[k++] = '+';
    textField[k++] = '0' + exp/10;
    textField[k++] = '0' + exp%10;
    textField[k++] = '\0';
    }
}

/* Sexagesimal Numbers */

/* These routines are taken from those developed by Nick Rees (JAC) Oct-94 */

/* medmLocalCvtDoubleToSexaString:
   Converts a double precision number to a sexagesimal string.

   The prec field indicates the number of significant digits
   after the hours (or degrees) field in the returned string. For
   example, the value 12.0 returns the following strings dependent
   on the prec field:

   12         => if prec = 0
   12:0       => if prec = 1
   12:00      => if prec = 2
   12:00:0    => if prec = 3
   12:00:00   => if prec = 4
   12:00:00.0 => if prec = 5

   The hopr and lopr parameters are only used to scale the value into
   an open interval before conversion. If (hopr-lopr) is equal to 24
   or 360, the value is renormalised to the semi-open interval [lopr,hopr).

   The value is rounded to the nearest number displayable with the indicated
   precision.

   Call:
   stostr(value, prec, hopr, lopr, string, &status);

   Parameters:   (">" input, "!" modified, "W" workspace, "<" output)

   (>) value     (double)    The number to be encoded as a sexagesimal string
   (>) prec      (int)       The required precision
   (>) hopr      (double)    The high operating range
   (>) lopr      (double)    The low operating range
   (<) string    (char *)    The string to interpret
   (<) status    (integer *) The conversion status value
       0=> Status OK
       1 => prec parameter less than zero
*/

#define MAXPREC 8

void medmLocalCvtDoubleToSexaStr(double value,  char *string,
  unsigned short prec, double hopr, double lopr, int *status)
{
    static const double prec_tab[MAXPREC+1] ={1.0, 1.0/6.0, 1.0/60.0, 1.0/360.0,
                                     1.0/3.6E3, 1.0/3.6E4, 1.0/3.6E5, 1.0/3.6E6,
                                     1.0/3.6E7 };
    double prec_frac, range, hrs, frac;
    char *ptr;
    int i, min, sec;

  /* Check the precision field is less than zero */
  /* KE: Because it is an unsigned short, this test is unnecessary */
#if 0
    if(prec < 0) {
    *status = 1;
    prec = 0;
    } else {
    *status = 0;
    }
#else
    *status = 0;
#endif

  /* Round the multiplier required to represent the value as an integer,
     retaining the required precision */
    if(prec <= MAXPREC) {
    prec_frac = prec_tab[prec];
    } else {
    prec_frac=prec_tab[MAXPREC];
    for(i=prec; i>MAXPREC; i--, prec_frac *= 0.1);
    }

  /* Add half the maximum displayed precision to aid with rounding */
    value = value + 0.5 * prec_frac;

  /* Normalise the value to within the range [hopr,lopr), if required. */
    range = (hopr-lopr);
    if(range == 360.0 || range == 24.0) {
    value = value - floor((value-lopr)/range)*range;
    }

  /* Insert a leading negative sign, if required */
    if(value < 0.0) {
    string[0] = '-';
    ptr       = &string[1];
    value     = -value + prec_frac;
    } else {
    ptr = string;
    }

  /* Now format the numbers */
    hrs = floor(value);
    value = (value - hrs)*60.0;
    min = (int)value;
    value = (value - min)*60.0;
    sec = (int)value;

    if(prec == 0) sprintf(ptr, "%.0f", hrs);
    else if(prec == 1) sprintf(ptr, "%.0f:%d", hrs, (min/10));
    else if(prec == 2) sprintf(ptr, "%.0f:%02d", hrs, min);
    else if(prec == 3) sprintf(ptr, "%.0f:%02d:%d", hrs, min, sec/10);
    else if(prec == 4) sprintf(ptr, "%.0f:%02d:%02d", hrs, min, sec);
    else  {
    frac = floor((value-sec)/(prec_frac*3600.0));
    sprintf(ptr, "%.0f:%02d:%02d.%0*.0f", hrs, min, sec, (prec-4), frac);
    }
}


/* s t r t o s d:
   Converts the leading portion of a string to a double with sign
   This is a routine used for intermediate decoding of sexagesimal strings.

   The routine assumes that the input string points to a string that is
   only separated from a number by leading spaces or tabs. It skips over
   this white space and if it finds an explicit leading sign it makes a note
   of it and skips over that two. If it finds a second leading sign it flags
   an error.

   If the next character is the delimiter is assumes a value of zero for the
   number, otherwise it tries to interpret the next characters as a number
   (it uses sscanf).  If this fails, it returns an error.

   If the conversion is successful it searches past remaining blank space,
   and past a single occurance of the delimiter, leaving rptr pointing at
   the next number.

   Note that this routine is very similar in function to SLA_DFLTIN except
   that:

   - It is in C.
   - You can specifiy the delimiter (SLA_DFLTIN uses comma, which is not
   very useful).
   - It uses sscanf to decode the number.

   The returned value is always positive.

   Call:
   value = strtosd(string, delim, &sign, &rptr, &status)

   Parameters:   (">" input, "!" modified, "W" workspace, "<" output)

   (>) string    (char *)    The string to interpret.
   (>) delim     (char)      The non blank delimiter.
   (<) sign      (int *)     The sign of the number.
                                -1 => Explicit minus sign found
                                0  => no explicit sign found
                                1  => Explicit plus sign found.
   (<) rptr      (char **)   If not NULL, a pointer to the next non-blank
                             character after the next delimiter (if it exists).
   (<) status    (integer *) The status value
                              0 => Successful conversion
                              1 => Conversion failed (function returns 0.0)
*/
static double strtosd(char *string, char delim, int *sign, char **rptr,
  int *status)
{

    int    n, i;
    char   *ptr=string;
    double retval=0.0;

/*  Initialize return parameters */
    *status = 0;
    *sign   = 0;

/*  Skip over leading blank space */
    while((*ptr == ' ') || (*ptr == '\t')) ptr++;

/*  Check to see whether we have found a sign */
    if((*ptr == '+') || (*ptr == '-')) {
        *sign = (*ptr == '+') ? 1 : -1;
        ptr++;
        while((*ptr == ' ') || (*ptr == '\t')) ptr++;
    }

/*  If we have found another sign, return an error */
    if((*ptr == '+') || (*ptr == '-')) {
        *status = 1;
    } else if(*ptr != delim) {
      /* Try and decode the number */
        n = sscanf(ptr, "%lf%n", &retval, &i);
        if(n != 1) {
      /* Return an error if the decode failed */
            *status = 1;
        } else {
      /*  Search past trailing white space */
            ptr += i;
            while((*ptr == ' ') || (*ptr == '\t')) ptr++;
        }
    }

  /*  Search past one delimiter and whitespace */
    if(*ptr == delim) {
        ptr++;
        while((*ptr == ' ') || (*ptr == '\t')) ptr++;
    }

  /*  Return values */
    if(rptr != NULL) *rptr = ptr;
    return retval;
}

/* strtos: Converts a sexagesimal string to a double precision number.
   The sexagesimal number is a character string with up to three fields
   (hours - or degrees, minutes and seconds) delimited by spaces or colons.
   The sign of the number can be before any leading zero field, but not after a
   any non-zero field.

   A blank field, or a field with just a sign, delimited by colons is
   interpreted as zero, with the sign propagated as required.

   Any of the fields can be a floating point number, but if it is not an
   integer then it is accepted as a fractional representation of the
   appropriate field and the parsing terminates with no error.

   The hopr and lopr parameters are only used to scale the value into
   an open interval after conversion. If (hopr-lopr) is equal to 24
   or 360, the value is renormalised to the semi-open interval [lopr,hopr).

   Note that return status's less than 3 are probably just informative.
   It is probably OK to leave out the less significant fields.

   Some examples follow.

   {12.0}    => 12 hours, return status of 2.
   {12.5}    => 12 hours 30 minutes.
   -0:0:0.1  => -0.1 second
   0 0 -0.1  => -0.1 second
   -0 0 +0.1 => Error - status = 4
   10:3      => 10 hours 3 minutes, return status of 1
   10        => 10 hours, return status of 2

   Call:
   value = strtos(string, hopr, lopr, &rptr, &status);

   Parameters:   (">" input, "!" modified, "W" workspace, "<" output)

   (>) string    (char *)    The string to interpret.
   (<) rptr      (char **)   If not NULL, a pointer to the next non-blank
                             character after the sexagesimal number.
   (<) status    (integer *) The status value
                              0 => Successful conversion
                              1 => No seconds found when expected
                              2 => No minutes found when expected
                              3 => No number found
                              4 => Conflicting signs in the fields
                              5 => Minutes or seconds greater than 60
*/
double strtos(char *string, char **rptr, int *status)
{
    int sign, new_sign;
    char *ptr=string;
    char delim=':';
    double retval = 0.0, minutes = 0.0, seconds = 0.0;

    *status = 0;

    retval = strtosd(ptr, delim, &sign, &ptr, status);

    if(*status != 0) {
        *status = 3;
    } else if(retval == floor(retval)) {
      /* Hours are integral - look for minutes */
        minutes = strtosd(ptr, delim, &new_sign, &ptr, status);
        if(*status != 0) {
            *status = 2;
        } else {
            if((new_sign != 0) && ((sign != 0) || (retval != 0.0)) ) {
          /* Have found a sign when there is already a real or
                 implied one */
                *status = 4;
            } else {
                if(sign == 0) sign = new_sign;
                if(minutes >= 60.0) *status = 5;
                retval = retval + minutes/60.0;
            }
        }
        if(*status == 0 && minutes == floor(minutes)) {
      /* Minutes are integral - look for seconds */
            seconds = strtosd(ptr, ' ', &new_sign, &ptr, status);
            if(*status != 0) {
                *status = 1;
            } else {
                if((new_sign != 0) && ((sign != 0) || (retval != 0.0)) ) {
          /* Have found a sign when there is already a real or
                     implied one */
                    *status = 4;
                } else {
                    if(sign == 0) sign = new_sign;
                    if(seconds >= 60.0) *status = 5;
                    retval = retval + seconds/3600.0;
                }
            }
        }
    }

    if(sign < 0) retval = -retval;

    if(rptr != NULL) *rptr = ptr;
    return retval;
}


