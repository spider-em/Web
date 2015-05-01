/*$Header: /usr8/web/src/RCS/pickdraw.c,v 1.12 2011/06/22 19:10:53 leith Exp $*/

/*
C++*********************************************************************
C
C pickdraw
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C    pickdraw
C
C    PURPOSE:         Display points from recovered arrays
C
C    CALLED BY:       pickp, fitmen
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* External common variables */
 extern int    ixull, iyull, ixulr, iyulr;
 extern float  * xu0, * yu0;
 extern float  * xs,  * ys;
 extern float  * xs2, * ys2;
 extern int    iradi;              /* Marker radius, set in pickp */

 /************************  pickdraw *********************************/

 void pickdraw(int leftside,  int ritside, 
               int locations, int numbers,  int predicted, int iend)
 {
 char *  string = NULL;
 int     ixi, iyi, i;

 /* Set color to green */
 wicolor(icontx,colorgo + 2);

 /* Draw each of the points */
 for (i= 0; i < iend; i++)
    {
    if (leftside)
       {
       if( xu0[i] > nsam1l && xu0[i] < nsam2l && yu0[i] > nrow1l 
              && yu0[i] < nrow2l)
          {  
          ixi = xu0[i] + ixulli;
          iyi = yu0[i] + iyulli;
          if (locations)
             {
             /*   Draw a circle at this location */
             xorc(iwin,   icontx,TRUE,ixi,iyi,iradi);
             xorc(imagsav,icontx,TRUE,ixi,iyi,iradi);
             }

          if (numbers)
             {
             /*   Write marker number at this location */
             string = itoa(i + 1);
             witext(icontx, string, ixi, iyi, 1, 0, -1, 2, FALSE);
             if(string) {free(string); string = (char *) NULL;}
             }
          }
       }

    if (ritside)
       {
       if( xs[i] > nsam1r && xs[i] < nsam2r && ys[i] > nrow1r 
              && ys[i] < nrow2r)
          {  
          ixi = xs[i] + ixulri;
          iyi = ys[i] + iyulri;
          if ( locations )
             {
             /*   Draw a circle at this location */
             xorc(iwin,icontx,   TRUE,ixi,iyi,iradi);
             xorc(imagsav,icontx,TRUE,ixi,iyi,iradi);
             }

          if (numbers)
             {
             /* Write marker number at this location */
             string = itoa(i + 1);
             witext(icontx, string, ixi, iyi, 1, 0, -1, 2, FALSE);
             if(string) {free(string); string = (char *) NULL;}
             }
          }
       }

    if (predicted)
       {
       if( xs2[i] > nsam1r && xs2[i] < nsam2r && ys2[i] > nrow1r 
              && ys2[i] < nrow2r)
          {  
          ixi = xs2[i] + ixulri;
          iyi = ys2[i] + iyulri;
          if (locations)
              {
              /* Draw a box at this location */
              xorbox(iwin,   icontx,TRUE,ixi-2,iyi-2, 5,5);
              xorbox(imagsav,icontx,TRUE,ixi-2,iyi-2, 5,5);
              }
          if (numbers)
              {
              /* Write marker number at this location */
              string = itoa(i + 1);
              witext(icontx, string, ixi, iyi, 1, 0, -1, 2, FALSE);
              if(string) {free(string); string = (char *) NULL;}
              }
           }
       }
    }

 /*     Refresh screen  */
 expos_cb(NULL,NULL,NULL);

 }
