
/*$Header: /usr16/software/web/src/RCS/wid_toggleg.c,v 1.6 2018/12/07 17:06:44 leith Exp $*/
/*
 * wid_toggleg.c
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
 * wid_toggleg(iw_parent, iw_toggleg, label,value,callback, data, ix, iy)
 * 
 * PURPOSE:    creates a toggle button gadget with callback 
 *
 * RETURNS:    iw_toggleg      toggle widget 
 * 
 * PARAMETERS:     
 *
 ***********************************************************************
*/

#include <Xm/ToggleBG.h>
#include "x.h"
#include "std.h"
#include "routines.h"

/******************  wid_toggleg  ******************************/

 Widget wid_toggleg(Widget iw_parent, Widget iw_toggleg, 
               char *label, int value,
               XtCallbackProc cb, void *data, int ix, int iy)
 { 
 Arg           args[6];   /* arg list */
 register      int  n;           /* arg count */
 XmString      str_temp;

 n = 0;

 if (ix > 0 || iy > 0)
    {
    XtSetArg(args[n], XmNx, (Position) ix); n++;
    XtSetArg(args[n], XmNy, (Position) iy); n++;
    }

 /* set the color to red */
 XtSetArg(args[n], XmNselectColor, 147); n++;
	
 str_temp =  XmStringCreate(label, XmSTRING_DEFAULT_CHARSET); 
 XtSetArg(args[n], XmNlabelString,  str_temp); n++;

 if ( iw_toggleg == (Widget) 0)
    {   
    /* create widget first time */
    iw_toggleg = XmCreateToggleButtonGadget(iw_parent,"iw_tog",args,n);
    }

 else 
    {   /* update callbacks in case they are changed */
    XtRemoveAllCallbacks(iw_toggleg, XmNvalueChangedCallback);

    /* set label & position */ 
    XtSetValues(iw_toggleg,args,n);
    }

 /* can free the string now */
 XmStringFree(str_temp);

 /* add call back */
 XtAddCallback(iw_toggleg,XmNvalueChangedCallback,cb,data);

 /* set the toggle, do not notify other toggles */
 XmToggleButtonGadgetSetState(iw_toggleg,value,FALSE);

 XtManageChild(iw_toggleg);

 return iw_toggleg;
 }
 
