
/*$Header: /usr16/software/web/src/RCS/wid_labelg.c,v 1.8 2018/12/07 17:03:35 leith Exp $*/
/*
 * wid_labelg.c
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
 * wid_labelg(Widget iw_parent, Widget iw_labelg, 
 *              char *label, int ix, int iy)
 * 
 * PURPOSE:    Creates a label widget  
 *
 * RETURNS:    iw_button      widget for the pushbutton gadget
 * 
 * PARAMETERS:     
 *
 * RETURNS:    widget number
 *
 ***********************************************************************
*/


#include <Xm/LabelG.h>
#include "common.h"
#include "routines.h"

/********************** wid_labelg ***************************/

 Widget wid_labelg(Widget iw_parent, Widget iw_labelg, 
               char *label, int ix, int iy)
 { 
 Arg           args[4];          /* arg list */
 int           n;                /* arg count */
 XmString      str_label;        /* String label */

 n = 0;
#ifdef WEB_BGFGO
 XtSetArg(args[n], XmNbackground,  icolorxmb);    n++;
#endif

 /* Note: XmStringCreateLtoR handles newline char oK) if this
    changed to XmStringCreate you will have to solve newline problem*/

 str_label = XmStringCreateLtoR(label,XmSTRING_DEFAULT_CHARSET);
 XtSetArg(args[n], XmNlabelString, str_label); n++;

 
 if (iw_labelg <= (Widget)0)
    { /* Create new label gadget */
    if (ix > 0 && iy > 0)
       { /* set label position */
       XtSetArg(args[n], XmNx, (Position) ix); n++;
       XtSetArg(args[n], XmNy, (Position) iy); n++;
       }
    else
       {  /* Use default alignment */
       XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING);  n++;
       }

    iw_labelg = XmCreateLabelGadget(iw_parent,"iw_labelg",args,n);
    }
 else 
    {
    XtSetValues(iw_labelg,args,n);
    }

 /* Free the String (not doing this may result in a memory leak) */
 XmStringFree(str_label);

 XtManageChild(iw_labelg);
 return iw_labelg;

 }
 
