/*$Header: /usr16/software/web/src/RCS/winbox.c,v 1.7 2018/12/07 17:06:44 leith Exp $*/
/*
C***********************************************************************
C
C WINBOX  -- Created                             FEB   92 ArDean Leith
C            Converted to C                      SEP   92 ArDean Leith
C            For TRUE                            SEP   11 ArDean Leith *                                                               
C                                                                 
C **********************************************************************
C *  AUTHOR: ArDean Leith                                              *
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
 C winbox(iwintyp)
 C
 C PURPOSE:      Draw box on screen in current color
 C
 C PARAMETERS:   none
 C
 C CALLED BY:    winsel     pickback
 C
 C CALL TREE:  
 C                    / winsiz_pop --> winsiz_pop2 --> winsel --> winbox
 C winsiz -> winsiz1 -- wid_posiz  --> winmov       /           
 C                    \ wid_posiz  --> winsiz3     /
 C
 C--*********************************************************************
*/


#include "common.h"
#include "routines.h"

 /*************************  winbox  ***************************/

 void winbox(int iwintyp)
 {
#ifdef WEB_TRUE

 // Draw box in backing store and copy to screen with cmap  
 xorboxt(imagsav, icontx, TRUE,TRUE,TRUE, ixulw,iyulw, nsamw,nroww);

#else

 // Draw box on screen   
 xorbox(iwin,    icontx, TRUE, ixulw,iyulw, nsamw,nroww);

 // Draw box in backing store   
 xorbox(imagsav, icontx, TRUE, ixulw,iyulw, nsamw,nroww);

#endif

 if (iwintyp != 3) 
    {            /* Fixed size or interactive window   */
    winmov();    // Move next window around screen
    }

 return;
 }

