/*
 * dist.c
 *		---- Converted from wid_dist.for
 *		     by Yanhong Li
 *
 ***********************************************************************
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
 ***********************************************************************
 * 
 * PURPOSE:     Find the distance between pixels      
 * 
 * PARAMETERS:  None 
 *
 * Called by:   distmen
 *
 ***********************************************************************
 */

#include "common.h"
#include "routines.h"

#define  MAXREG	7

 /* external  functions */
 extern void distmen_buts(Widget , XtPointer, XtPointer);

 /* internal functions */
 void    dist_pop (Widget, XEvent *, String *, Cardinal *);

 /* external global variables */
 extern FILE    *fpdocpix;

 /* global variables used elsewhere (distmen.c) */
 int     opendistdoc = TRUE;
 int     singledist  = TRUE;
 int     dist_docit  = TRUE;
 int     dist_inimag = TRUE;
 int     dist_leavit = TRUE;

 /* file scope variables */
 static GC     icontxx;
 static int    ix0, iy0, ix1, iy1;     /* starting and ending point */
 static float  dlist[MAXREG];
 static int    ndist;			/* The starting point flag */
 static float  distance;		/* the distance between two points */

/***************************  dist  *********************************/

 void dist()
 {
 ndist = -1;
  
 /* turn on xor writing in icontxx */
 icontxx = setxor(icontx);

 /* open a message window with the following strings  */
 if (singledist)
    {
    showbutx("Set the starting point/ending point.", 
             "Menu .", 
             "Set new starting point.", FALSE);
    }
 else
    {
    showbutx("Set the starting point/ending point.", 
             "Menu.", 
             "Stop this starting point.", FALSE);
    }
 /* set the actions for right, left, and center buttons */
 actions(iw_win, "dist_pop", dist_pop,"M123");
 }

 /************************* dist_pop ********************************/

 void dist_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 int   dx, dy;	       /* the distance for x-y axis */
 int   ixi, iyi;
 char  outstr[80];     /* print out string */

 if (!(strcmp(*params, "M")))
    {                           /* List location ---- mouse moved */
    getloc(event,'m',&ix1,&iy1);
    if (dist_inimag) 
       {     /* find location relative to last image */
       ixi = ix1 - ixul + 1;     iyi = iy1 - iyul + 1;
       }

    if (ndist < 0)  	/* the starting point flag */
       /* haven't got the starting point yet */
       return;

    else if (dist_inimag && (ixi < 1 || ixi > nsam ||
                             iyi < 1 || iyi > nrow))
       {    /* cursor is outside of image, only want inside */
       sprintf(outstr,"*** Outside: (%d,%d)$", ixi,iyi);
       spout(outstr);

       /* draw line */
       xorline(iwin, icontxx, FALSE, ix0, iy0, ix1, iy1);
       }

    else 
       {      /* report the distance, don't save it */     
       dx = ix1 - ix0;    dy = iy1 - iy0;
       distance = sqrt((float)(dx*dx + dy*dy));
       sprintf(outstr,"*** Distance: %f ****$", distance);
       spout(outstr);

       xorline(iwin, icontxx, FALSE, ix0, iy0, ix1, iy1);
       }
    }

 else if (!(strcmp(*params, "1")))
    {                     /*  List location -------- button 1 pushed */
    getloc(event,'B',&ix1,&iy1);

    if (dist_inimag) 
       {     /* find location relative to last image */
       ixi = ix1 - ixul + 1;     iyi = iy1 - iyul + 1;
       }

    if (ndist < 0)	  
       {   /* set starting point for distance listing */
       ndist = 0;
       ix0   = ix1;     iy0   = iy1;
       return;
       }

    else if (dist_inimag && ( ixi < 1 || ixi > nsam ||
                              iyi < 1 || iyi > nrow))
       {    /* cursor is outside of image, only want inside */
       sprintf(outstr,"*** Outside image: (%d,%d)", ixi,iyi);
       spout(outstr);
       }

    else {
       /* record the distance */

       if (dist_leavit) 
	  { 	/* draw a permanent line along the distance */
	  xorline(iwin,    icontxx, TRUE, ix0, iy0, ix1, iy1);
	  xorline(iwin,    icontxx, TRUE, ix0, iy0, ix1, iy1);
	  xorline(imagsav, icontxx, TRUE, ix0, iy0, ix1, iy1);
	  }
 
       ndist++;

       if (dist_docit)
	  {	/* want to save this distance for doc file */

	  if (ndist > MAXREG )
	     {	/* too many distances from this starting point for doc file */
       	     spout("*** Document key is full, maximum line # is 7");
	     ndist = MAXREG;
	     }

	  else 
	     {	/* save the location for entry in doc file */
	     dlist[ndist] = distance;
       	     sprintf(outstr,"*** Selected: #%d: %d  %f", ikey, 
                            ndist - 1, distance);
       	     spout(outstr);
             if (singledist) 
                {    /* only want one distance from this start */
                if (dist_docit > 0)
                   {      /* save the current distance line */
                   dlist[0] = ikey++;
                   dlist[2] = ix0;
                   dlist[3] = iy0;
                   dlist[4] = ix1;
                   dlist[5] = iy1;
                   fpdocpix = savdn1(docnam, datexc, &fpdocpix, 
                            dlist, 6, &opendistdoc, 
                            TRUE, TRUE);
                   }
                ndist    = 0;
                ix0      = ix1;     iy0   = iy1;
                distance = 0;
                }
	     }
	  }
       }
    }

 else if (!(strcmp(*params, "2")))
    {                     /*  display the dist_menu */
    /* remove current actions */
    XtUninstallTranslations(iw_win);
    xorline(iwin, icontxx, TRUE, ix0, iy0, ix1, iy1);

    /* show menu */
    showbutx(NULL, NULL, NULL, TRUE);
    distmen(NULL, NULL, NULL);
    }

 else if (!(strcmp(*params, "3")))
    {                     /*  set a new starting point */
    if (ndist > 0) 
       /* remove the current line */
       xorline(iwin, icontxx, TRUE, ix0, iy0, ix1, iy1);

    if ( !singledist) 
       {     
       if (dist_docit > 0 && ndist > 0)
          {      /* first save the current distance line */
          dlist[0] = ikey;
          fpdocpix    = savdn1(docnam, datexc, &fpdocpix, dlist, ndist+1, 
                            &opendistdoc, TRUE, TRUE);
          ikey++;
          }
       }
    ndist = -1;
    }
 }
 

       
