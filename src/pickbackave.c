
/*$Header: /usr8/web/src/RCS/pickbackave.c,v 1.9 2015/09/01 18:24:15 leith Exp $*/

/*
 C++********************************************************************
 C
 C  pickbackave.c
 C
 C**********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C**********************************************************************
 C
 C  pickbackave
 C
 C  PURPOSE:    Add average to background averge docfile (dfil)
 C
 C  CALLED BY:  pickbackmen.c 
 C
 C*********************************************************************
*/

#include "common.h"
#include "routines.h"

 /********************   pickbackave   ****************************/

 int pickbackave(char dfil[])

 { 
 int      ncount, k,maxback;
 float *  ptr;
 float    sum, avg, dlist[7];
 float *  dbuf     = NULL;
 FILE  *  fp       = NULL;
 char     strwin[] = " Overall-window-avg     #windows"; 
 int      maxregp1 = 6+1;     /* Size for array pointed to by dbuf */
 int      maxkeys  = 1001;    /* Size for array pointed to by dbuf */
 int      openit   = TRUE;

 /* Retrieve window averages from dfil */
 
 if (getdoc((FILE *) NULL, dfil, datexc, maxkeys, maxregp1, &dbuf, &maxback) > 0) 
    {   /* Problem retrieving doc file, assume it does not exist */
    if (dbuf) free(dbuf); dbuf = NULL;
    spouts("*** Unable to read doc file: "); spout(dfil); 
    return -1;
    }

 ptr    = dbuf;
 sum    = 0.0;
 ncount = 0;

 /* Sum window averages from dfil */
 for (k = 0; k < maxback; k++)
    {
    if (*ptr != 0.0) 
       { ncount++; sum += *(ptr+1); }
    ptr += maxregp1; 
    }

 if (dbuf) free(dbuf);  dbuf = NULL;

 if (ncount < 1)
   {
   spouts("*** No windows for averaging in doc file: "); spout(dfil);
   XBell(idispl,50);
   return 0;
   }

 /* Find average */
 avg      = sum / ncount;
 dlist[0] = 999; 
 dlist[1] = avg;
 dlist[2] = ncount; 
 dlist[3] = 0.0; 
 dlist[4] = 0.0; 
 dlist[5] = 0.0;
 dlist[6] = 0.0;
 
 /* Save average in doc file  */

 if (!(fp = savdnc(dfil, datexc, &fp, dlist, 7, &openit, TRUE, FALSE, strwin))) 
    { /* Unable to open the doc file!! */
    XBell(idispl,50); XBell(idispl,50); return 0;
    }

 fclose(fp); fp = NULL;
    
 return ncount;
 }

