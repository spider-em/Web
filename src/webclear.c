
/*$Header: /usr16/software/web/src/RCS/webclear.c,v 1.9 2018/12/07 17:03:34 leith Exp $*/
/*
 C++*************************************************************************
 C
 C WEBCLEAR.FOR -- MARCH 91 al
 C                 CONVERTED TO C -- AUG 91
 C
 C *********************************************************************
 C=* AUTHOR:  ArDean Leith
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 C *********************************************************************
 C
 C    webclear(iw_dum, data, call_data)
 C
 C    PURPOSE:          Clears image window
 C
 C    PARAMETERS:       
 C
 C    CALLED BY:        web_com
 C
 C--*********************************************************************
*/

/*****************  webclear  ******************************/

#include "common.h"
#include "routines.h"

 void webclear(Widget iw_dum, XtPointer data, XtPointer call_data)
 {

 XGCValues    gcval;

 // Preserve current foreground
 XGetGCValues(idispl, icontx, GCForeground, &gcval);
 
 // Set clearing color
 if (icolorxb < 1) icolorxb = 0;
 XSetForeground(idispl,icontx,icolorxb);

 // Clear imagsav backing store 
 XFillRectangle(idispl,imagsav,icontx,0,0,iwidex,ihighx);

 // Clear displayed window
 XClearWindow(idispl,iwin);

 expos_cb(NULL,NULL,NULL);

 // Restore current color in icontx
 XSetForeground(idispl, icontx, gcval.foreground);

 // Should start next image at upper left corner 
 nbot       = ihighx;
 ntop       = ihighx;

 neednewmap = TRUE;

 return;
 }

