
/*$Header: /usr16/software/web/src/RCS/wimakecur.c,v 1.5 2018/12/07 17:06:44 leith Exp $*/
/*
 C++********************************************************************
 C
 C  WIMAKECUR.FOR  -- CREATED MAR 91
 C                                                                     
 C *********************************************************************
 C * AUTHOR: A. LEITH                                                  *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 C    WIMAKECUR(ICUR,LFORE,LBACK,COLORF,COLORB,IXHOT,IYHOT)
 C
 C    PARAMETERS:   ICUR        CURSOR NUMBER             WRITES
 C                  LFORE       CURSOR FORGROUND DATA     READS
 C                  LBACK       CURSOR BACKGROUND DATA    READS
 C                  COLORF      CURSOR FOREGROUND COLOR   READS
 C                  COLORB      CURSOR BACKGROUND DATA    READS
 C                  IXHOT,IYHOT CURSOR HOT SPOT COOR.     READS
 C
 C    PURPOSE:      SETS UP A CURSOR FROM BITMAP DATA
 C
 C    CALLED BY:    CURSOR_BUT
 C
 C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* external function prototypes */

 /* internal function prototypes */

 /* external common variables */

 /* internal common variables */

 /********************   wimakecur   ****************************/

 int  wimakecur(Cursor *icur, char * lfore, char * lback,
                XColor * colorft, XColor * colorb,
                int ixhot, int iyhot)
 {

 static int iwidec=16, ihighc=16;
 Pixmap     ifore, iback;
 
 if (*icur == 0) 
    {   /* cursor does not exist, need to create it */

    /* create cursor foreground pixmap */
    ifore = XCreatePixmapFromBitmapData(idispl,iwin,
                   lfore,iwidec,ihighc,1,0,1);

    /* create cursor background pixmap */
    iback = XCreatePixmapFromBitmapData(idispl,iwin,
                    lback,iwidec,ihighc,1,0,1);

    /* create the cursor */
    *icur  = XCreatePixmapCursor(idispl,ifore,iback,
                   colorft,colorb,ixhot,iyhot);

    /* free pixmap storage */
    XFreePixmap(idispl,ifore);
    XFreePixmap(idispl,iback);
    }
 return TRUE;
 }

