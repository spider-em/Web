 
/*$Header: /usr16/software/web/src/RCS/xorlin.c,v 1.5 2018/12/07 17:06:44 leith Exp $*/
/*
C***************************************************************************
C
C XORLIN  -- CREATED NOV 93
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
C xorlin(iwint,icontxt,erase,draw,ix0,iy0,ix1,iy1,ix2,iy2,ix3,iy3)
C
C PURPOSE:      Draw an a-xored line
C
C PARAMETERS:   iwin        X window
c               icontxt     Graphics context
c               erase,draw  Logical flags for erase then draw
c               iperm       Logical falg for permanent drawing
c               ix0,iy0     Old start point
c               ix1,iy1     Old end point
c               ix2,iy2     New start point
c               ix3,iy3     New end point
C
C CALLED BY:    
C
C--***************************************************************
*/

#include "common.h"
#include "routines.h"

 /**********************   xorlin   ******************************/

 void xorlin(Window iwint, GC icontxt, int erase, int draw, 
           int * ix0, int * iy0, int * ix1, int * iy1,
           int ix2, int iy2, int ix3, int iy3)
 {

 if (erase)
    {            
    XDrawLine(idispl,iwint,icontxt,*ix0,*iy0, *ix1,*iy1);
    }

 if (draw)
    {            
    XDrawLine(idispl,iwint,icontxt,ix2,iy2, ix3,iy3);

    /* Set ix0,iy0,ix1,iy1 to new line coordinates */
    *ix0 = ix2;  *iy0 = iy2;
    *ix1 = ix3;  *iy1 = iy3;
    }

 XFlush(idispl);
 neednewmap = TRUE;


 return;
 }

