
/*$Header: /usr16/software/web/src/RCS/wid_togmen.c,v 1.8 2018/12/07 17:06:44 leith Exp $*/
/*
 * wid_togmen.c
 *
 ***********************************************************************
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
 ***********************************************************************
 *
 * wid_togmen(iw_parent, iw_pushg, label, value, ix, iy)
 * 
 * PURPOSE:    creates a toggle button gadget  
 *
 * RETURNS:    iw_button      widget for the pushbutton gadget
 * 
 * PARAMETERS:     
 *
 ***********************************************************************
*/

#include <Xm/ToggleBG.h>
#include "std.h"
#include "x.h"
#include "routines.h"

#define MAX_ARGS 8

/*************************  wid_togmen  ***************************/

 Widget wid_togmen(Widget iw_parent, Widget iw_toggleg, 
               char *label,int value, int ix, int iy)
 { 
 Arg           args[MAX_ARGS];   /* arg list */
 register      int  n;           /* arg count */

 n = 0;

 XtSetArg(args[n], XmNset,TRUE);     n++;
 XtSetArg(args[n], XmNselectColor, 147); n++;	/* red */
 XtSetArg(args[n], XmNlabelString, XmStringCreate(label,
                              XmSTRING_DEFAULT_CHARSET)); n++;

 if ((long)iw_toggleg <= 0)
    {   /* create widget first */

    if (ix >= 0 && iy >= 0)
       { /* set label position */
       XtSetArg(args[n], XmNx, ix); n++;
       XtSetArg(args[n], XmNy, iy); n++;
       }
    else
       {
       /* use default alignment */
       XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING);  n++;
       }

    iw_toggleg = XmCreateToggleButtonGadget(iw_parent,"iw_tog",args,n);
    }
 else 
    {
    XtSetValues(iw_toggleg,args,n);
    }

 XmToggleButtonGadgetSetState(iw_toggleg,value,FALSE);

 XtManageChild(iw_toggleg);

 return iw_toggleg;
 }
 
