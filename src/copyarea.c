
/*$Header: /usr8/web/src/RCS/copyarea.c,v 1.2 2011/09/28 12:07:31 leith Exp leith $*/

/*
C***********************************************************************
C                                                                      *                                                              
C  copyarea.c      Created                       Sep 2011 ArDean Leith *
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
C    
C
C    PURPOSE:      Copys image from backing store to backing store
C
C    PARAMETERS:    
C
C    CALLED BY:    
C
C--*********************************************************************
*/

#include "common.h"

#ifdef WEB_TRUE                                                                    
// WEB_TRUE ---------------------------------------------------------

 extern Pixmap        imapsav;        // Virtual cmpa backing store
 extern Boolean       use3colors;     // lut colormap in use
 extern int           mapgo,mapend;   // Image start & end in colormap 

 void copyarea(Display * idis, Drawable src, Drawable dest,
               GC gc,  int srcx_ulc, int srcy_ulc, 
               unsigned int width, unsigned int height,
               int destx_ulc, int desty_ulc)

 {
 if (!use3colors &&  (mapgo == 0) && (mapend == 255) )
    {  // Virtual colormap NOT in use  
    XCopyArea(idis,src,dest,gc,srcx_ulc,srcy_ulc,
              width,height,destx_ulc,desty_ulc);
    }
 else
    {  // Virtual colormap in use update imapsav & copy to display

    expos_cb(NULL, NULL, NULL);

    //XCopyArea(idis,imapsav,dest,gc,srcx_ulc,srcy_ulc,
    //          width,height,destx_ulc,desty_ulc);
    }
 }

#else
// WEB_24 ----------------------------------------------------------- 
           
 void copyarea(Display * idis, Drawable src, Drawable dest,
               GC gc, int srcx_ulc, int srcy_ulc, 
               unsigned int width, unsigned int height,
               int destx_ulc, int desty_ulc)
 {
 XCopyArea(idis,imagsav,dest,gc,srcx_ulc,srcy_ulc,
              width,height,destx_ulc,desty_ulc);
 }
#endif
