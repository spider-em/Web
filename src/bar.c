
/*$Header: /usr8/web/src/RCS/bar.c,v 1.4 2011/05/05 11:57:30 leith Exp $*/

/*
C***************************************************************************
C
C  WID_BAR.FOR  -- CREATED DEC 89
C                  REWRITTEN MAR 91 al
C  BAR.C           NOV 92                                                               
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
C    BAR
C
C    PURPOSE:      DRAWS VERTICAL OR HORIZONTAL BAR AT CURSOR
C
C    PARAMETERS:   NONE 
C
C    CALLED BY:    WEB_COM
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* internal subroutines */
 void bar_pop(Widget, XEvent *, String *, Cardinal *);

 /* External common variables */
 extern Widget  iw_barmen;
 extern int     idx, idy;
 
/**************************  bar  *******************************/

 void bar(Widget iw_caller, caddr_t data, caddr_t call_data)

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

