
/*$Header: /usr8/web/src/RCS/wid_text.c,v 1.8 2011/09/14 15:37:50 leith Exp $*/
/*
 * wid_text.c
 *
 ***********************************************************************
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
 ***********************************************************************
 *
 * wid_text(iw_parent, iw_text, prompt, text, cb(), cbdata, ix, iy)
 * 
 * PURPOSE:	Create a text input pop up dialog box
 *
 * RETURNS:	iw_text
 * 
 * PARAMETERS:  Widget	iw_parent	Parent widget
 *		Widget  iw_text		Text input dialog box
 *		char	*prompt		Prompt string
 *		int	(*cb)()		OK callback
 *		int	ix		X coord of ulh corner
 *		int 	iy		Y coord of ulh corner
 *
 ***********************************************************************
*/

#include "common.h"
#include <Xm/SelectioB.h>

#define  MAX_ARGS 9

 /* Function prototypes */
 extern void fin_cb  (Widget, caddr_t, caddr_t);
 extern void widpos  (int *, int *);

 /**********************************************************************/

 Widget wid_text(Widget iw_parent, Widget iw_text, char *prompt, 
                 char * text,
                void (*cb)(), caddr_t cbdata, int ix, int iy)
 {
   Arg           args[MAX_ARGS];              /* Arg list      */
   Arg           arg1[MAX_ARGS] ;             /* Arg list      */
   int           n,n1;                        /* Arg count     */
   char          label[80];
   XmString      str_prompt, str_text;        /* String prompt */

   n  = 0;
   n1 = 0;

   /* Create a String prompt */
   str_prompt = XmStringCreate(prompt, XmSTRING_DEFAULT_CHARSET);

   XtSetArg(args[n], XmNselectionLabelString, str_prompt); n++;

   if (text != (char *) NULL)
      {
      str_text = XmStringCreate(text, XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[n], XmNtextString, str_text);      n++;
      }

   // For all widgets that are subclass of XmBulletinBoard, must set
   // this argument = false, otherwise, positioning does not work the
   // first time! 
   XtSetArg(args[n], XmNdefaultPosition, False);    n++;


   if (iw_text <= (Widget)0) 
      {              /* Must create new widget */
      Setbgfg(arg1, MAX_ARGS, &n1);  // Handles BGFG Args
      iw_text = XmCreatePromptDialog(iw_parent, "iw_text", arg1, n1);

      widpos(&ix,&iy);
      XtSetArg(args[n], XmNx, (Position) ix);    n++;
      XtSetArg(args[n], XmNy, (Position) iy);    n++;
      XtSetValues(iw_text, args, n);

      /* OK callback should be sent the text the user inserted */
      XtAddCallback(iw_text,XmNokCallback,    cb,    cbdata);
      XtAddCallback(iw_text,XmNcancelCallback, (XtCallbackProc) fin_cb,
                              (XtPointer) iw_text);
      }

   else 
      {             /* Can use existing widget */
      if (ix > 0 || iy > 0)
         {
         widpos(&ix,&iy);
         XtSetArg(args[n], XmNx, (Position) ix);     n++;
         XtSetArg(args[n], XmNy, (Position) iy);     n++;
         }
      XtSetValues(iw_text, args, n);

      XtRemoveAllCallbacks(iw_text,XmNokCallback); 
      /* OK callback should be sent the text the user inserted */
      XtAddCallback(iw_text,XmNokCallback, cb, cbdata);
      }

   XtManageChild(iw_text);

   XtUnmanageChild(XmSelectionBoxGetChild
                   (iw_text,XmDIALOG_HELP_BUTTON));

   /* Free String storage */
   XmStringFree(str_prompt);
   if (text != (char *) NULL)
       XmStringFree(str_text);

   return (iw_text);
 }
