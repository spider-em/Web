
/*$Header: /usr16/software/web/src/RCS/pickdisp.c,v 1.5 2018/12/07 17:03:33 leith Exp $*/

/*C++*************************************************************************
C
C pickdisp.c             Mar. 94
C                                                                  
C **********************************************************************
C *  AUTHOR: A. LEITH                                                      *
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
C    PARAMETERS:   
C
C    PURPOSE:      redisplays particle image pair
C
C    CALLED BY:    pickmen  backmen
C
C--********************************************************************
*/

#include "common.h"
#include "routines.h"
 
 /* external commmon variables */

 /***************************** pickdisp *****************************/

 void pickdisp(int leftside, int rightside)

 {
 if (leftside)
    {
    /* shift left image to shifted location */
    XPutImage(idispl,iwin,   icontx,imagel, nsam1l, nrow1l,
              ixull,iyull, nsam2l - nsam1l + 1, nrow2l - nrow1l + 1);
    XPutImage(idispl,imagsav,icontx,imagel, nsam1l, nrow1l,
              ixull,iyull, nsam2l - nsam1l + 1,nrow2l - nrow1l + 1);
    neednewmap = TRUE;
    }

 if (rightside)
    {
    /* shift right image */      
    /* display, window, gc, image, srcx, srcy destx, desty width height */
    XPutImage(idispl,iwin,   icontx,imager, nsam1r, nrow1r,
              ixulr,iyulr, nsam2r - nsam1r + 1, nrow2r - nrow1r + 1);
    XPutImage(idispl,imagsav,icontx,imager, nsam1r, nrow1r,
              ixulr,iyulr, nsam2r - nsam1r + 1, nrow2r - nrow1r + 1);
    neednewmap = TRUE;
    }

 return; 
 }



   
  
