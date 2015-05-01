
/*$Header: /usr8/web/src/RCS/wirubberline.c,v 1.5 2011/09/23 11:54:11 leith Exp $*/

/*
C***************************************************************************
C                                                                      *                                                             
C  wid_bar.for     Created                         DEC 89 ArDean Leith *
C                  Rewritten                       MAR 91 ArDean Leith *
C  wirubberline    Ported to C                     NOV 92 ArDean Leith *                                                             
C                  For Directcolor                 NOV 10 ArDean Leith *                                                             
C                                                                      *                                                             
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
C    wirubberline(win, icontxt, draw, perm,ixs,   iys,ixe,   iye)
C
C    PURPOSE:      Draws rubber line
C
C    PARAMETERS:    
C
C    CALLED BY:    
C
C--*********************************************************************
*/

#include "common.h"

 /**********************  wirubberline  *****************************/

 void wirubberline(Drawable win, GC icontxt, 
                   Boolean draw, Boolean perm,
                   int ixs,   int iys,
                   int ixe,   int iye)
 {
 if (perm) 
    {
    /* Draw line on backing store */
    if (draw) XDrawLine(idispl,imagsav,icontxt,ixs,iys, ixe,iye);

    /* Copy backing store to image window */
     copyarea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);
    }

 else
    {
    /* Copy backing store to image window */
    copyarea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);

    /* Draw line on screen */
    if (draw) XDrawLine(idispl,win,icontxt,ixs,iys, ixe,iye);
    }

 neednewmap = TRUE;

 }

