/*
C***************************************************************************
C  corr_mask.c                   8/93
c
C  WID_MASK.FOR (VAX) transported to UNIX
C  made into mask.c, & corr_mask.c
C                                                              
C **********************************************************************
 C=* AUTHOR: Mahieddine Ladjadj                                       *
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
C    CORR_MASK
C
C    PURPOSE:      Creates mask by drawing around areas with mouse
C
C    PARAMETERS:   
C
C    CALLED BY:    MASKMEN
C
C    COLORS:       black,  blue,    green,  blue_green, 
c                  red,    magenta, yellow, white
C 
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "comap.h"

 static void   corr_mask_pop  (Widget, XEvent *, String *, Cardinal *);

 /* Externally defined common variables */

 /* Internal common variables */
 GC             icontmx;
 static int     masks, gotpt, ipts=0;

/************************ corr_mask   *******************************/

void corr_mask(int itag)
  {
  /* Set number of masked polygons made so far to zero */
  if (itag == 0 || itag == 1)
      masks  = 0;
  /* else if (itag==2) continue showing all masks */

  /* Set number of points on current polygon of this mask */
  gotpt = FALSE;

  /* Interactive correspondance map masking */
  showbutx("Starts / sets line segment.",
           "Close Polygon / Call Menu.",
           "Stop routine.",FALSE);

  actions(iw_win, "corr_mask_pop", corr_mask_pop, "M123");

  /* Update the screen */
  XFlush(idispl);
  expos_cb(NULL,NULL,NULL); 

  /* Set up a xor graphics context */
  icontmx = setxor(icontx);
  }


/****************** corr_mask_pop  ************************************/

void corr_mask_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
  {
  static XPoint *points;
  static int    icoloro, ix2, iy2, ix1, iy1, ix0, iy0, nsize = 10;

  /* Save and reset color */
  icoloro = icolor;
  wicolor(icontx,colorgo+icolorov);

  if (!(strcmp(*params,"M")))                 /* cursor moved */
      {  /* Get mouse position ix2,iy2  */
      getloc(event,'M',&ix2,&iy2);

      if (gotpt)
         /* Already have line, mouse moved, so erase and draw new line */
         xorline(iwin,icontmx,FALSE,ix1,iy1,ix2,iy2);
      }
  else if (!(strcmp(*params,"1")))            /* button 1 pushed */ 
      { 
      /* Get mouse position ix2,iy2  */
      getloc(event,'M',&ix2,&iy2);

      /* Left button, end a line segment of the polygon */
      if (!gotpt)
          {
          gotpt = TRUE;
          ix0     = ix2;
          iy0     = iy2;
          ipts    = 0;
          }
      else
          {   /* Draw permanent line to this location */

          /* Draw line and save line in backing store */
          xorline(iwin,   icontx,TRUE,ix1,iy1,ix2,iy2);
          xorline(imagsav,icontx,TRUE,ix1,iy1,ix2,iy2);
          }
      if (ipts == 0)
          {
          if ((points = (XPoint *) calloc(nsize, sizeof(XPoint)))
                      ==(XPoint *) NULL )
              { spout("***  Can't allocate: points"); return; }
          }
      else if (ipts >= nsize)
          {
          nsize += 10;
          if((points = (XPoint *) realloc(points, nsize * sizeof(XPoint)))
                     ==(XPoint *) NULL )                                  {
              spout("*** Unable to realloc points array in corr_mask.c.");
              ipts = nsize - 2;                                           }
          }
      ix1              = ix2;
      iy1              = iy2;
      points[ipts].x   = ix2;
      points[ipts++].y = iy2;
      }
 
  else if (!(strcmp(*params,"2")))          /* button 2 pushed */ 
      { /*   Finished with this mask polygon */

      /* Clear any stuff in overlay planes (any previous masks) */
      if (ipts > 3)
          {
          /* Close the mask */
          points[ipts].x   = ix0;
          points[ipts++].y = iy0;

          /* Save orig color & Set fill color to white */
          icoloro = icolor;
          wicolor(icontx,colorgo+icolorov);
          //printf(" colorgo:: %d , icolorov: %d\n",colorgo,icolorov);
 
          /* Draw filled mask on the image and save in backing store */
          XFillPolygon(idispl,iwin,icontx,points,ipts,
                       Complex,CoordModeOrigin);
          XFillPolygon(idispl,imagsav,icontx,points,ipts,
                       Complex,CoordModeOrigin);
 
          neednewmap = TRUE;

          /* May have multiple masks on the screen, but only
             interested in last one */

          /* Create a pixmap for the new mask*/
          msksav = XCreatePixmap(idispl,iwin,iwidex,ihighx,idepth);
          if (msksav <= 0)  
             { spout(" *** WARNING: Can not create a pixmap for new mask.");
               spout(" The average image will be wrong !!!!!");
             } 

	  /* Clear mask buffer with black--0 */
#ifdef WEB_TRUE
          wicolor(icontx,0);
#else
          wicolor(icontx,-1);
#endif
	  XFillRectangle(idispl,msksav,icontx, 0,0,iwidex,ihighx);

	  /* Draw white polygon */
          wicolor(icontx,colorgo+icolorov);    // White
          XFillPolygon(idispl,msksav,icontx,points,ipts,
                       Complex,CoordModeOrigin);
 
          /* Reset original color in icontx*/
          wicolor(icontx,icoloro);

          masks++;
          if (points) free(points);
          ipts   = 0;
          points =  (XPoint *) NULL;
          gotpt  = FALSE;
          nsize  = 10;
          }
      else
          { spout(" Not enough points for a mask!"); }

      /* Reset original color in icontx*/
      wicolor(icontx,icoloro);

      /* Cancel button assignments */  
      XtUninstallTranslations(iw_t);

      /* Remove instructional message */
      showbutx("","","",TRUE); 
 
      XFlush(idispl);
   
      // Halt correspondence map masking, get 2nd map menu
      // for storing data or averaging images from map /
      comapmen2();          
      }
  else if (!(strcmp(*params,"3")))          /* Button 3 pushed */        
      {   /* Stop the correspondance map */

      /* Reset original color in icontx*/
      wicolor(icontx,icoloro);

      /* Cancel button assignments */  
      XtUninstallTranslations(iw_t);

      /* Remove instructional message */
      showbutx("","","",TRUE);
      }
  return;
  }
