
/*$Header: /usr16/software/web/src/RCS/bar.c,v 1.5 2018/12/07 17:03:30 leith Exp $*/

/*
C***********************************************************************
 C
 C  WID_BAR.FOR  -- CREATED DEC 89
 C                  REWRITTEN MAR 91 al
 C  bar.c           NOV 92                                                               
 C *********************************************************************
 C * AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
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
 C    bar(Widget, void *, void *)
 C
 C    PURPOSE:      Draws vertical or horizontal bar at cursor
 C
 C    PARAMETERS:   None 
 C
 C    CALLED BY:    web_com
 C
 C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "bar.h"

 /* internal subroutines */
 static void bar_pop(Widget, XEvent *, String *, Cardinal *);

 /* External common variables */
 
/**************************  bar  *******************************/

 void bar(Widget iw_caller, void * data, void * call_data)

 {
 /* Put up menu */
 barmen(NULL, NULL, NULL);
 
 /* Open a message window with the following strings */
 showbutx("Set bar location.", 
          "Display menu.", 
          "Stop this routine.", FALSE); 

 /* Set actions for right, left, and center buttons */
 actions(iw_win, "bar_pop",bar_pop,"M123");
 }

 /********************** bar_pop  (draw bars)  *******************/

 void bar_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {
 int           ix1, iy1, ix2, iy2;

 /* Get mouse position ix1,iy1  */
 getloc(event,'M',&ix1,&iy1);

 ix2 = ix1 + idx;     iy2 = iy1 + idy;       /* Bar length */

 if (!(strcmp(*params,"M")))                 /* Cursor moved */
    {
    /* Copy backing store to image window & draw new bar on screen */
    wirubberline(iwin,icontx,TRUE,FALSE,ix1,iy1,ix2,iy2);
    }

 else if (!(strcmp(*params,"1")))            /* Button 1 pushed */ 
    {
    /* Draw permanent bar to backing store and copy to screen */
    wirubberline(iwin,icontx,TRUE,TRUE,ix1,iy1,ix2,iy2);
    }

 else if (!(strcmp(*params,"2")))           /* Button 2 pushed */ 
    {
    /* Erase current bar from screen */
    wirubberline(iwin,icontx,FALSE,FALSE,ix1,iy1,ix2,iy2);

    /* Put up menu */
    barmen(NULL, NULL, NULL);
    } 

 else if (!(strcmp(*params,"3")))           /* Button 3 pushed */ 
    {
    /* Erase current bar from screen */
    wirubberline(iwin,icontx,FALSE,FALSE,ix1,iy1,ix2,iy2);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);
 
    /* Unmanage information box */
    showbutx("","","",TRUE);

    XtUnmanageChild(iw_barmen);
    } 
 }

