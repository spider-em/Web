/*$Header: /usr16/software/web/src/RCS/xorc.c,v 1.7 2018/12/07 17:06:44 leith Exp $*/
/*
C***************************************************************************
C
C XORC  -- CREATED JAN 90
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
C xorc(iwint,icontxt,perm,ix2,iy2,irad)
C
C PURPOSE:      draw an a-xored circle
C
C PARAMETERS:   iwin        X window
c               icontxt     Graphics context
c               iperm       Logical falg for permanent drawing
c               ix2,iy2     Point on circle
c               irad        Radius of circle
C
C CALLED BY:    pixel
C
C--***************************************************************

*/

#include "common.h"
#include "routines.h"

 static int drawingt = FALSE;
 static int ixo,iyo,iwido;

 /**********************   xorc   ******************************/

 void xorc(Window iwint, GC icontxt, int perm, 
           int ix2, int iy2, int irad)
 {
 int iwid, ix, iy;

 if (perm) drawingt = FALSE;

 ix   = ix2 - irad;
 iy   = iy2 - irad;
 iwid = 2 * irad;

 if (drawingt)
    {            /* In rubber-band loop so erase old O and draw new */
    XDrawArc(idispl,iwint,icontxt,ixo,iyo,iwido,iwido,0,23040);
    XDrawArc(idispl,iwint,icontxt,ix, iy, iwid, iwid, 0,23040);
    }
 else
    {            /* Starting new rubber band loop, so draw new O */
    XDrawArc(idispl,iwint,icontxt,ix, iy, iwid, iwid, 0,23040);
    if (!perm) drawingt = TRUE;
    }

 XFlush(idispl);
 ixo   = ixo;
 iyo   = iyo;
 iwido = iwid;

 neednewmap = TRUE;

 return;
 }

