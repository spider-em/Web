
/*$Header: /usr8/web/src/RCS/hist.c,v 1.4 2011/09/14 15:29:04 leith Exp $*/
/*
C++*********************************************************************
C
C WID_HIST.FOR  -- CREATED APRIL 91
C hist.c                   NOV   92
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C    HIST
C
C    PURPOSE:     DISPLAY A HISTOGRAM OVERLAY FOR IMAGE
C
C    PARAMETERS:  FILEDATA    FILE INFO
C
C    VARIABLES:   JHIST       ARRAY WITH HISTOGRAM DATA
C
C    CALLS:       
C
C    CALLED BY:   
C 
C--********************************************************************
*/

#include "files.h"
#include "common.h"
#include "routines.h"

#define NHIST 128

 /**************************** hist  **********************************/

 int hist(FILEDATA *filedata)
 {

 int    i, maxhis, ixgo, iygo, irep, ix, iy2, j, ixt, iyt,  iyb;
 float  scale, hscale,  xinc;
 int *  jhist;
 char   cval[5];

 /* Allocate space for historam array */
 if ((jhist  = (int *) calloc(NHIST,sizeof(int))) == NULL) 
    {
    spout("*** Unable to calloc memory in hist.c!");
    return FALSE;
    }

 /* Find histogram for this image */
 if (!his(filedata, jhist)) 
    {
    if (jhist) free(jhist) ;
    return FALSE;
    }

 /* Find max y value in the histogram */
 maxhis = 0;
 for (i = 0; i < NHIST; i++)
    { 
    if (jhist[i] > maxhis) maxhis = jhist[i];
    }

 if (maxhis <= 0) 
    {
    spout("*** Empty histogram!");
    if (jhist) free(jhist);
    return FALSE;
    }

 /* Find y-scaling factor for  height */
 hscale = (float) (ihighx - 150) / maxhis;

 /* Each bar will be irep pixels wide */
 ixgo = NHIST;
 iygo = ihighx - 100;

 irep = 7;
 if ((ixgo + NHIST * irep) > iwidex) irep = 6;
 if ((ixgo + NHIST * irep) > iwidex) irep = 5;
 if ((ixgo + NHIST * irep) > iwidex) irep = 4;
 if ((ixgo + NHIST * irep) > iwidex) irep = 3;
 if ((ixgo + NHIST * irep) > iwidex) irep = 2;
 if ((ixgo + NHIST * irep) > iwidex) irep = 1;

 /* Make box for histogram */
 xorbox(iwin,   icontx,TRUE,ixgo, iygo-(hscale*maxhis), 
                            NHIST*irep, ihighx - 150);
 xorbox(imagsav,icontx,TRUE,ixgo, iygo-(hscale*maxhis), 
                            NHIST*irep, ihighx - 150);

 /* Write scale on histogram box */
 iyt = iygo + 25;

 /* xinc is 1/10 of x scale */
 xinc = (float) ((irep * NHIST)) / 10.0;

 /* Set tick mark length for y axis */
 iyb = iygo+8;

 /* Set start label for axis */
 strcpy(cval,"0%");
 ixt = ixgo;
 witext(icontx,cval, ixt,iyt, 1, 0, 9, 2, FALSE);
 xorline(iwin,   icontx,TRUE, ixt,iygo, ixt,iyb);
 xorline(imagsav,icontx,TRUE, ixt,iygo, ixt,iyb);

 for (i=1; i < 10; i++)
    {
    sprintf(cval,"%2d",i*10);
    ixt = (int) (ixgo + xinc * i);
    witext(icontx,cval, ixt,iyt, 1, 0, 9, 2, FALSE);

    xorline(iwin,   icontx,TRUE, ixt,iygo, ixt,iyb);
    xorline(imagsav,icontx,TRUE, ixt,iygo, ixt,iyb);
    }

 /* set end label for axis */
 ixt = ixgo + (xinc * 10.0);
 strcpy(cval,"100%");
 witext(icontx,cval, ixt,iyt, 1, 0, 9, 2, FALSE);
 xorline(iwin,   icontx,TRUE, ixt,iygo, ixt,iyb);
 xorline(imagsav,icontx,TRUE, ixt,iygo, ixt,iyb);


 /* plot histogram */
 for (i=0; i < NHIST; i++)
    {
    /* histogram has up to NHIST bins in it */
    ix  = ixgo + i * irep;
    iy2 = iygo - jhist[i] * hscale;

    for (j=0; j < irep; j++)
       {
       /*     each bin is irep pixels wide */
       xorline(iwin,   icontx,TRUE, ix+j,iygo, ix+j,iy2);
       xorline(imagsav,icontx,TRUE, ix+j,iygo, ix+j,iy2);
       }
    }

 if (jhist) free(jhist);
 return TRUE;
 }
