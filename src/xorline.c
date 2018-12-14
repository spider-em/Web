 
/*$Header: /usr16/software/web/src/RCS/xorline.c,v 1.5 2018/12/07 17:06:44 leith Exp $*/
/*
C***************************************************************************
C
C XORLINE  -- CREATED NOV 89 as WIXOR.FOR
C          CONVERTED TO C -- 1 OCT 92 al
C                                                                 
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C XORLINE(IWINT,ICONTXT,PERM,IX2,IY2,IX3,IY3)
C
C PURPOSE:      DRAW AN A-XORED LINE
C
C PARAMETERS:   IWIN        X WINDOW
C               ICONTXT     GRAPHICS CONTEXT
C               IPERM       LOGICAL FALG FOR PERMANENT DRAWING
C               IX2,IY2     NEW START POINT
C               IX3,IY3     NEW END POINT
C
C CALLED BY:    PIXEL
C
C--***************************************************************
*/

#include "common.h"
#include "routines.h"

 static int drawingt = FALSE;
 static int ix0,iy0,ix1, iy1;

 /**********************   xorc   ******************************/

 void xorline(Window iwint, GC icontxt, int perm, 
           int ix2, int iy2, int  ix3, int iy3)
 {
 if (perm) drawingt = FALSE;

 if (drawingt)
    {            /* In rubber-band loop so erase old and draw new */
    XDrawLine(idispl,iwint,icontxt,ix0,iy0,ix1,iy1);
    XDrawLine(idispl,iwint,icontxt,ix2,iy2,ix3,iy3);
    }
 else
    {            /* Starting new rubber band loop, so draw new  */
    XDrawLine(idispl,iwint,icontxt,ix2,iy2,ix3,iy3);
    if (!perm) drawingt = TRUE;
    }

 XFlush(idispl);
 ix0   = ix2;
 iy0   = iy2;
 ix1   = ix3;
 iy1   = iy3;

 neednewmap = TRUE;

 return;
 }

