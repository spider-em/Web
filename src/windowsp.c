
/*$Header: /usr8/web/new/RCS/windowsp.c,v 1.2 2005/10/18 16:59:52 leith Exp $*/
/*
C++*******************************************************************
C
C WINDOW
C        CONVERTED TO C -- SEPT 2 1992 ArDean Leith
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
C    WINDOW(BUF,NSAM,NROW,NSLICE,NSAMW1,NROWW1,NSLIW1,
C           NSAM2,NROW2,NSLIC2,BACK)
C
C    PURPOSE:  THIS SUBROUTINE CUTS OUT A RECTANGULAR IMAGE SECTION FROM
C              IMAGE 1 WITH A SPECIFIED SIZE: NSAM2,NROW2 FROM A 
C              SPECIFIED LOCATION: IXULW, IYULW AND WRITES OUT 
C              IMAGE 2.
C
C    PARAMETERS:
C         NSAM,NROW,NSLICE 	DIMENSIONS OF INPUT VOLUME/PICTURE
C         NSAMW1,NROWW1,NSLIW1  COORDINATES, WITH RESPECT TO INPUT VOLUME, 
C                        	OF TOP LEFT CORNER OF WINDOW
C         NSAM2,NROW2,NSLIC2    DIMENSIONS OF WINDOW = DIMENSIONS OF OUTPUT 
C                        	VOLUME
C         BACK                  BACKGROUND FILL VALUE
C
C--*******************************************************************
*/

#include "files.h"

 /* external functions */
 extern   int    wrtlin     (FILEDATA *, float *, int, int);
 extern   int    redlin     (FILEDATA *, float *, int, int);


 /************************  window  *****************************/

 void windowsp(FILEDATA *filedata1, FILEDATA *filedata2, 
             int nsam ,  int nrow,   int nslice,
             int ixulw, int  iyulw, int izulw,
             int nsam2,  int nrow2,  int nslice2, float back)

 {
 float *buf1, *buf2;
 int   nsamd, nroww2, nsls, nsle, ns, l, i1, i2, new, ne, nrowp1;
 int   k, i, icon, ixulwm1;

 buf1   = filedata1 -> buf;
 buf2   = filedata2 -> buf;
 
 nsamd  = nsam + nsam2;
 nroww2 = iyulw + nrow2 - 1;
 nsls   = 1;
 nsle   = 1;
 ns     = iyulw;

 if (nslice != 1)
    {
    nsls = izulw;
    nsle = izulw - nslice2 - 1;
    }
 
 for (l=nsls; l<=nsle; l++)
    {
    i1 = (l - nsls) * nrow2;
    i2 = (l - 1)    * nrow;

    if (ns < 1)
       {
       new = 1 - ns;
       for (k=0; k<nsam2; k++)
          buf2[k] = back;
       for (i=1; i<=new; i++)
          wrtlin(filedata2, buf2, nsam2, i+i1);
       ns = 1;
       }

    ne   = nroww2;
    icon = -ns + 1 + i1;

    if (ne > nrow) 
       {
       ne    = nrow;
       nrowp1 = nrow + 1;
       for (k=0; k<nsamd; k++)
           buf2[k] = back;
       for (i=nrowp1; i<=nroww2; i++)
          wrtlin(filedata2, buf2, nsam2, i+icon);
       }

    ixulwm1 = ixulw - 1;

    for (i=ns; i<=ne; i++)
       {
       redlin(filedata1, buf1, nsam, i+i2);
       wrtlin(filedata2, &buf1[ixulwm1], nsam2, i+icon);
       }
    }   

 return;
 }
     
