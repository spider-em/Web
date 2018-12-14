/*$Header: /usr16/software/web/src/RCS/filget.c,v 1.13 2018/12/07 17:03:31 leith Exp $*/

/*
C++*************************************************************************
C
C FILGET  -- CREATED JAN 89
C            CONVERTED TO C -- JULY 92 al
C
C *********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
 C=*                                                                   *
 C=* HEALTH RESEARCH INCORPORATED (HRI),                               *   
 C=* ONE UNIVERSITY PLACE, RENSSELAER, NY 12144-3455.                  *
 C=*                                                                   *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 C=* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU *
 C=* General Public License for more details.                          *
 C=*                                                                   *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program; if not, write to the                     *
 C=* Free Software Foundation, Inc.,                                   *
 C=* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.     *
 C=*                                                                   *
C *********************************************************************
C
C    FILGET
C
C    PURPOSE:        USED TO CREATE FILENAME FROM 
C                    FILE-NAME TEMPLATE AND ARRAY OF FILE NUMBERS.
C
C    PARAMETERS:     FILPAT    CHAR. FILE NAME PATTERN (NEEDED)
C                    FILNAM    CHAR. FILE NAME         (RETURNED)
C                    MAXLEN    MAX LENGTH FOR FILNAM   (NEEDED)
C                    INUM      FILE NUMBER             (NEEDED)
C
C    RETURNS:        IRTFLG    ERROR FLAG (0 IS ERROR, 2 IS ALL OK)
C
C--*************************************************************************
*/

#include "std.h"
#include "common.h"
#include "routines.h"

  
/************************  filget  *********************************/

 int filget(char * filpat, char * filnam, int maxlen, int inum)

 { 
 char   *dot, dataext[5]=".";
 long   igo, numast, nlet, locast, numi;
 int    ierflg;

 /* find if there is an extension */
 strcat(dataext,datexc);
 dot = strstr(filpat,dataext);
 
 /* normal return is 2 */
 ierflg = 2;

 nlet   = strlen(filpat);

 /*  find location of first * in filpat */
 if ((locast = strcspn(filpat,"*")) == nlet)
    { /* no * in template, find run of digits at end */

    /* skip extension at end of filename (if any) */
    if (dot) nlet = nlet - strlen(dot);

    for (locast=nlet-1; locast >=  0; locast--)
       if (! isdigit(*(filpat+locast)))
       {
       locast++;
       break;
       }

    numast = 1;
    }
 else
    {  /* has *(s)  in filpat (template)  */

    /* skip extension at end of filpat (if any) */
    if (dot) nlet = nlet - strlen(dot);

    numast = nlet - locast;
    }

 /* find number of digits in file number. (must be >= numast) */
 if (inum > 0)
    numi = MYMAX(((int) log10((float) inum) + 1.0),(numast));
 else
    numi = numast;

  
 /* find 1st char in filpat that will be altered */
 igo = nlet - numi;

 if (igo < 0)
    {
    spouts("*** FILE NUMBER: ");            spouti(inum);
    spouts("*** DESTROYS FILE TEMPLATE: "); spout(filpat);
    return 0;
    }

 if (igo < locast)
    {
    /*    printf("igo: %d, locast: %d, numi: %d, inum: %d \n",
                igo,     locast,      numi,   inum); */
    spouts("WARNING, FILE NUMBER: ");    spouti(inum);
    spouts("MAY ALTER FILE TEMPLATE: "); spout(filpat);
    ierflg = 1;
    }

 if (strlen(filpat) >= (size_t) maxlen)
    {
    spouts("*** Filename too long in filget: "); spout(filpat);
    return 0;
    }
 strcpy(filnam,filpat);

 if (sprintf(&filnam[igo],"%*.*d",(int)numi,(int)numi,inum) < 0) 
    return 0;
 if (dot) strcat(filnam,dataext);

 return ierflg;
 }

