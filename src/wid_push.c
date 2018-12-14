
/*$Header: /usr16/software/web/src/RCS/wid_push.c,v 1.6 2018/12/07 17:03:35 leith Exp $*/
/*
 * wid_push.c
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
 * wid_push(iw_parent, iw_pushg, *data, mapt, ix, iy)
 * 
 * PURPOSE:    creates a push button widget  
 *
 * RETURNS:    iw_button      widget for the pushbutton gadget
 * 
 * PARAMETERS: iw_parent      parent widget
 *             iw_pushg       this widget identifier
 *             data           callback client data
 *             ix,iy          position    
 *
 ***********************************************************************
*/

#include <Xm/PushB.h>
#include "x.h"
#include "std.h"
#include "routines.h"

#define MAX_ARGS 4

/***********************  wid_push  *************************/

 Widget wid_push(Widget iw_parent, Widget iw_push, 
               char *label, XtCallbackProc cb,
               char *data, Colormap mapt, int ix, int iy)
 { 

 Arg           args[4];   /* arg list */
 int           n;         /* arg count */
 XmString      str_temp;

 n = 0;

 if (ix > 0 || iy > 0)
    {
    XtSetArg(args[n], XmNx, (Position) ix); n++;
    XtSetArg(args[n], XmNy, (Position) iy); n++;
    }

 str_temp =  XmStringCreate(label, XmSTRING_DEFAULT_CHARSET); 
 XtSetArg(args[n], XmNlabelString,  str_temp); n++;


 if (iw_push <= (Widget)0)
    {   /* create widget first */

    if ((int) mapt != 0) XtSetArg(args[n], XmNcolormap, mapt);  n++;
    iw_push = XmCreatePushButton(iw_parent,"iw_pb",args,n);
    }

 else 
    {
    /* update callbacks in case they are changed */
    XtRemoveAllCallbacks(iw_push,XmNactivateCallback);

    /* set label / position */ 
    XtSetValues(iw_push,args,n);
    }

 /* add call back */
 XtAddCallback(iw_push,XmNactivateCallback,cb,data);

 /* can free the string now */
 XmStringFree(str_temp);

 XtManageChild(iw_push);

 return iw_push;
 }
 
