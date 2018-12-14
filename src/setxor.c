/*$Header: /usr16/software/web/src/RCS/setxor.c,v 1.4 2018/12/07 17:03:34 leith Exp $*/
/*
C***************************************************************************
C
C SETXOR  -- CREATED NOV 89
C           -- converted to C Aug 92 al                                                     
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
C SETXOR(ICONTX1)
C
C PURPOSE:      CREATES NEW GRAPHICS CONTEXT SET TO EXCLUSIVE OR
C
C PARAMETERS:   icontx1    SOURCE  CONTEXT
C               
C RETURNS:      icontx2    new x-or context
C
C CALLED BY:    winsiz 
C
C--*********************************************************************
*/

#include "x.h"
#include "std.h"
#include "common.h"
#include "routines.h"


 /************************************************************/

 GC setxor(GC icontx1)
 {

 GC             icontx2;
 XGCValues      gcval;
 unsigned long  gcmask;

 /* create new graphics context */
 icontx2 = XCreateGC(idispl, iwin, 0, &gcval);

 /* COPY SOME CHARACTARISTICS OF THE OLD GC TO THE XOR GC */
 gcmask = GCForeground | GCBackground | GCLineWidth | GCFont;

 XCopyGC(idispl, icontx1, gcmask, icontx2);

 XSetFunction(idispl,icontx2,GXxor);

 XFlush(idispl);

 return icontx2;
 }
