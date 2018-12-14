
/*$Header: /usr16/software/web/src/RCS/wid_textboxb.c,v 1.7 2018/12/07 17:06:44 leith Exp $*/
/*
 C **********************************************************************
 *
 *  TEXTBOXB
 *
 C **********************************************************************
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
**************************************************************************
*
*  TEXTBOXB(IW_PARENT,IW_TEXTIN,STRING,PROMPT,IX,IY)       
*
*  PURPOSE:        CREATES A TEXT INPUT WINDOW USING ROW COLUMN WIDGET
* 
*  PARAMETERS      IW_PARENT       PARENT WIDGET                READS
*                  IW_TEXT         TEXT WIDGET                  WRITES
*                  PROMPT          BOX LABEL                    READS
*                  STRING          OUTPUT STRING                READS
*                  ICOL            LENGTH OF TEXT BOX           READS
*
* CALLED BY:       many routines
*
**************************************************************************
*/

#include <Xm/Text.h>
#include <Xm/RowColumn.h>
#include "common.h"
#include "routines.h"

/************************  wid_textboxb   ***********************/

 Widget wid_textboxb(Widget iw_parent, Widget iw_text,  
               char *prompt, char *string,  int icol)

 {  
 Arg           args[3];    /* Arg list  */
 register      int n;      /* Arg count */
 Widget        iw_rowcol;

 n = 0;
 
 if (iw_text == (Widget)0)
    {
    if (strlen(prompt) > (size_t) 0)
       {    
       /* Create a new RowColumn widget as a parent for text input box  */
#ifdef WEB_BGFGO
       XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
#endif

       XtSetArg(args[n], XmNorientation, XmHORIZONTAL);    n++;
       iw_rowcol = XmCreateRowColumn(iw_parent, "iw_rowcol", args, n);
       XtManageChild(iw_rowcol);

       /* Create label widget for the prompt */
       wid_labelg(iw_rowcol, 0, prompt, -1,-1);
       }
    else
       {iw_rowcol = iw_parent;}

    n = 0;
#ifdef WEB_BGFGO
       XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
#endif
    XtSetArg(args[n], XmNvalue, string); n++;                  
    XtSetArg(args[n], XmNcolumns, (short) icol); n++; 
    XtSetArg(args[n], XmNeditMode, XmSINGLE_LINE_EDIT); n++; 
 
    /* XmTextField is not used because it results in a memory leak */ 
    iw_text = XmCreateText(iw_rowcol, "iw_text", args, n);
    }

 else 
    {    /* Alter existing textbox string value */
    XtSetArg(args[n], XmNvalue, string); n++;                  
    XtSetArg(args[n], XmNcolumns, (short) icol); n++; 
    XtSetValues(iw_text, args, n);

    /* Note that iw_text is child of iw_rowcol which may be unmanaged */
    XtManageChild(XtParent(iw_text));
    }

 XtManageChild(iw_text);
 return (iw_text);
 }


