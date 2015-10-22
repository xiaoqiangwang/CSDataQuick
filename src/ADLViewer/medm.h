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

#ifndef __MEDM_H__
#define __MEDM_H__
#undef __MONITOR_CA_PEND_EVENT__
#define SUPPORT_0201XX_FILE_FORMAT


#include <limits.h>
#include <float.h>	/* XPG4 limits.h doesn't include float.h */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* PATH_MAX */
#ifdef WIN32
/* Is in stdlib.h for WIN32 */
# define PATH_MAX _MAX_PATH
#else
/* May be in limits.h.  Kludge it if not */
# ifndef PATH_MAX
# define PATH_MAX 1024
# endif
#endif

/* For dumb SUNOS and GNU... */
#ifndef FLT_MAX		/* FLT_MAX is supposed to be in limits.h/float.h */
#  define FLT_MAX ((float)1.+30)
#endif
#ifndef M_PI		/* Similarly M_PI should be in math.h */
#  define M_PI    3.14159265358979323846
#endif

#ifdef ALLOCATE_STORAGE
#define EXTERN
#else
#define EXTERN extern
#endif

#if defined(XRTGRAPH) || defined(SCIPLOT) || defined(JPT)
#define CARTESIAN_PLOT
#endif

#ifdef __cplusplus
#define UNREFERENCED(x) (x)
#else
#define UNREFERENCED(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /* WIN32 differences */
#ifdef WIN32
  /* Path delimiter is different */
# define MEDM_PATH_DELIMITER ';'
# define MEDM_DIR_DELIMITER_CHAR '\\'
# define MEDM_DIR_DELIMITER_STRING "\\"
  /* The following is done in Exceed 6 but not in Exceed 5
   *   Need it to define printf as lprintf for Windows
   *   (as opposed to Console) apps */
# ifdef _WINDOWS
#  ifndef printf
#   define printf lprintf
#  endif
# endif
#else /* #ifdef WIN32 */
  /* Path delimiter is different */
# define MEDM_PATH_DELIMITER ':'
# define MEDM_DIR_DELIMITER_CHAR '/'
# define MEDM_DIR_DELIMITER_STRING "/"
  /* WIN32 does not have unistd.h */
# include <unistd.h>
#endif /* #ifdef WIN32 */

#include "siteSpecific.h"
#include "medmWidget.h"
#include "parse.h"

#define MAIN_NAME "Medm"
#define OBJECT_PALETTE_NAME "Object "
  /* The following works on both WIN32 and Solaris */
#define STRFTIME_FORMAT "%a %b %d %H:%M:%S %Z %Y"

#include "proto.h"

  /* Global variables */

#ifdef __cplusplus
	   }  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif  /* __MEDM_H__ */
