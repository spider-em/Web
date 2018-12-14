
/*$Header: /usr16/software/web/src/RCS/wid_label.c,v 1.8 2018/12/07 17:03:35 leith Exp $*/
/*
 * wid_label.c
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
 * wid_label(Widget iw_parent, Widget iw_label, 
 *              char *label, int ix, int iy)
 * 
 * PURPOSE:    creates a label widget  
 *
 * RETURNS:    iw_button      widget for the pushbutton gadget
 * 
 * PARAMETERS:     
 *
 * RETURNS:    widget number
 *
 ***********************************************************************
*/

#include <Xm/Label.h>
#include "std.h"
#include "x.h"
#include "routines.h"

/***************************  wid_label  *****************************/

 Widget wid_label(Widget iw_parent, Widget iw_label, 
               char *label, Colormap mapt, int ix, int iy)
 { 
 Arg           args[5];          /* arg list */
 int           n;                /* arg count */
 XmString      str_label;        /* String label */

 n = 0;

 /* note: XmStringCreateLtoR handles newline char oK) if this
    changed to XmStringCreate you will have to solve newline problem*/

 str_label = XmStringCreateLtoR(label,XmSTRING_DEFAULT_CHARSET);
 XtSetArg(args[n], XmNlabelString, str_label); n++;
 
 if (  iw_label ==  (Widget) 0)
    {      /* create new label gadget */
    if (ix > 0 && iy > 0)
       {     /* set label position */
       XtSetArg(args[n], XmNx, (Position) ix);   n++;
       XtSetArg(args[n], XmNy, (Position) iy);   n++;
       }
    else
       {
       /* use default alignment */
       XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING);  n++;
       }

     if (mapt) 
        {
        XtSetArg(args[n], XmNcolormap, mapt);  n++;
        }

    iw_label = XmCreateLabel(iw_parent,"iw_label",args,n);
    }
 else 
    {
    XtSetValues(iw_label,args,n);
    }

 /* free the String (not doing this may result in a memory leak) */
 XmStringFree(str_label);

 XtManageChild(iw_label);
 return iw_label;
 }
 
