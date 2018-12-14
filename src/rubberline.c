
/*$Header: /usr16/software/web/src/RCS/rubberline.c,v 1.5 2018/12/07 17:03:34 leith Exp $*/
/*
C***************************************************************************
C
C RUBBERLINE  -- CREATED DEC 89 as WID_RUBR.FOR
C                ALTERED FOR WIDGETS MAR 91 al
C                CONVERTED TO C OCT 92 al
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
C RUBBERLINE
C
C PURPOSE:      Draws rubber band lines 
C
C PARAMETERS:   none
C
C CALLED BY:    web_com
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
 
 /* internal subroutines */
 static void rubber_pop(Widget, XEvent *, String *, Cardinal *);
 
 /* global variables */
 static int firstpt;
 static int ix1, iy1, ix2, iy2;


/*********************  rubberline  *******************************/

 void rubberline(Widget w, XtPointer c, XtPointer u)
 {
 
 /* open a message window with the following strings */
 showbutx("Starts / sets line segment.", 
          NULL, 
          "Stop this routine.", FALSE);


 /* set the actions for right, left, and center buttons */
 actions(iw_win, "rubber_pop",rubber_pop,"M13");

 /* set first point flag */
 firstpt = TRUE;
 }


 /**************** rubber_pop  (draw rubber lines)  ***************/


 void rubber_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {
 if (!(strcmp(*params,"M")))                 /* cursor moved */
    {
    /* get mouse position ix2,iy2  */
    getloc(event,'M',&ix2,&iy2);

    if (!firstpt)
       {
       /* already have line, mouse moved, so erase and draw new line */
       /* Copy backing store to image window & draw new line on screen */
       wirubberline(iwin,icontx,TRUE,FALSE,ix1,iy1,ix2,iy2);
       }
    }

 else if (!(strcmp(*params,"1")))            /* button 1 pushed */ 
    {
    /* get mouse position ix2,iy2  */
    getloc(event,'M',&ix2,&iy2);

    if (firstpt)
       firstpt = FALSE;
    else
       {   /* draw permanent line to this location */
       /* Draw permanent line to backing store and copy to screen */
       wirubberline(iwin,icontx,TRUE,TRUE,ix1,iy1,ix2,iy2);
       }
    ix1 = ix2;
    iy1 = iy2;
    }
 
 else if (!(strcmp(*params,"3")))          /* button 3 pushed */        
    {                                       

    /* Erase current bar from screen */
    wirubberline(iwin,icontx,FALSE,FALSE,ix1,iy1,ix2,iy2);

    /* uninstall translations */
    XtUninstallTranslations(iw_t);
 
    /* unmanage information box */
    showbutx("","","",TRUE);

    /* reset first point jflag */
    firstpt = TRUE;

    return;
    }
 }
