
/*$Header: /usr8/web/src/RCS/pickbackave.c,v 1.7 2005/10/18 17:00:04 leith Exp $*/

/*
C++*************************************************************************
C
C    pickbackave
C
C **********************************************************************
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
C **********************************************************************
C
C    pickbackave
C
C    PURPOSE:    add average to background averge docfile (dfil)
C
C    CALLED BY:  pickbackmen.c 
C
C **********************************************************************
*/

#include "common.h"
#include "routines.h"

 /********************   pickbackave   ****************************/

 void pickbackave(char dfil[])

 { 
 int      ncount, k, maxregp1, maxkeys, openit, maxback;
 float  * dbuf = NULL, * ptr;
 float    sum, dlist[6];
 FILE   * fp = NULL;

 /* set size for array pointed to by dbuf */
 maxregp1 = 5+1;
 maxkeys  = 1001;

 /* retrieve data from dfil1 for untilted points */
 
 if (getdoc((FILE *) NULL, dfil, datexc, maxkeys, maxregp1, 
           &dbuf, &maxback) > 0) 
    {   /* problem retrieving doc file, assume it does not exist */
    if (dbuf) free(dbuf); dbuf = NULL;
    spouts("*** Unable to read doc. file: "); spout(dfil); 
    return;
    }

 ptr    = dbuf;
 sum    = 0.0;
 ncount = 0;

 for (k = 0; k < maxback; k++)
    {
    if (*ptr != 0.0) 
        { ncount++; sum += *(ptr+1);  }
    ptr  += maxregp1; 
    }

 if (ncount < 1)
   {
   spouts("*** No windows for averaging from doc. file: ");spout(dfil);
   XBell(idispl,50);
   return;
   }

 /* find average */
 sum = sum / ncount;
 dlist[0] = 999; dlist[1] = sum;
 dlist[2] = 0.0; dlist[3] = 0.0; dlist[4] = 0.0; dlist[5] = 0.0;
 
 /* save info in doc file  */
 openit = TRUE;  
 if (!(fp = savdn1(dfil, datexc, &fp, dlist, 6, &openit, TRUE, TRUE))) 
    { /* unable to open the doc file!! */
    XBell(idispl,50); XBell(idispl,50); return;
    }

 fclose(fp);     
 return;

}

