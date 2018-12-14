/*$Header: /usr16/software/web/src/RCS/xorbox.c,v 1.7 2011/09/23 12:47:48 leith Exp $*/
/*
C***********************************************************************
C                                                                      *
C xorbox  -- Created                             JAN   90 ArDean Leith *
C            Converted to C                      SEP   92 ArDean Leith *
C            For TRUE                            SEP   11 ArDean Leith *                                                               
C                                                                      *
C **********************************************************************
C *  AUTHOR: ArDean Leith                                              *
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
C xorbox(iwin,icontx,perm,ix2,iy2,nsam,nrow)
C
C PURPOSE:      Draw a xored box
C
C PARAMETERS:   iwin        Drawable
C               icontx      Graphics context
C               perm        Logical flag for permanent drawing
C               ix2,iy2     Upper left corner of box
C               nsam,nrow   Box dimensions
C
C CALLED BY:    
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

#ifdef WEB_TRUE
// WEB_TRUE -----------------------------------------------------------

 /**********************  xorboxt  *****************************/

 void xorboxt(Window iwint, GC icontxt, 
             Boolean perm,  Boolean draw, Boolean copy,
             int ix2,   int iy2,
             int nsamt, int nrowt)
 {
 if (perm)
     {
    // Draw rectangle on iwint (usually backing store)  
    if (draw) XDrawRectangle(idispl,iwint,icontxt,ix2,iy2,nsamt,nrowt);

    // Copy whole backing store to display with colormapping  
    if (copy) copyarea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);
    }

 else
    {
    // Copy backing store to image window with colormapping 
    if (copy) copyarea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);

    // Draw line on screen (but not in backing store) 
    if (draw) XDrawRectangle(idispl,iwint,icontxt,ix2,iy2,nsamt,nrowt);
    }

 XFlush(idispl);

 neednewmap = TRUE;

 return;
 }

 /**********************  xorbox  *****************************/

 static int drawingt = FALSE;

 void xorbox(Window iwint, GC icontxt, 
             int perm,  
             int ix2,   int iy2,
             int nsamt, int nrowt)
 {
 if (perm)
     {
    // Draw rectangle on backing store  
    XDrawRectangle(idispl,iwint,icontxt,ix2,iy2,nsamt,nrowt);

    // Copy whole backing store to display with colormapping  
    copyarea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);
    }

 else
    {
    // Copy backing store to image window with colormapping 
    copyarea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);

    // Draw line on screen  
    XDrawRectangle(idispl,iwint,icontxt,ix2,iy2,nsamt,nrowt);
    }

 XFlush(idispl);

 neednewmap = TRUE;

 return;
 }


#else
// WEB_24 -----------------------------------------------------------


 static int drawingt = FALSE;
 static int ixo,iyo,nsamo,nrowo;

 /**********************  xorboxt  *****************************/

 void xorboxt(Window iwint, GC icontxt, 
             Boolean perm,  Boolean draw, Boolean copy,
             int ix2,   int iy2,
             int nsamt, int nrowt)

  {  
  // Just redirect to old xorbox
  xorbox(iwint, icontxt, perm,ix2,iy2, nsamt,nrowt);
  return;
  }

 /**********************  xorbox  *****************************/

 void xorbox(Window iwint, GC icontxt, int perm, 
             int ix2,   int iy2,
             int nsamt, int nrowt)
 {

 if (perm) drawingt = FALSE;

 if (drawingt)
    {     /* In rubber-band loop so erase old box and draw new one */
    XDrawRectangle(idispl,iwint,icontxt,ixo,iyo,nsamo,nrowo);
    XDrawRectangle(idispl,iwint,icontxt,ix2,iy2,nsamt,nrowt);
    }
 else
    {     /* Starting new rubber band loop , so draw new box */
    XDrawRectangle(idispl,iwint,icontxt,ix2,iy2,nsamt,nrowt);
    if (!perm) drawingt = TRUE;
    }
 XFlush(idispl);

 ixo        = ix2;
 iyo        = iy2;
 nsamo      = nsamt;
 nrowo      = nrowt;

 neednewmap = TRUE;

 return;
 }

#endif
