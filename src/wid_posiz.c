
/*$Header: /usr8/web/new/RCS/wid_posiz.c,v 1.13 2011/05/18 18:24:48 leith Exp $*/
/*
 * wid_posiz.c
 *
 **********************************************************************
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
 **********************************************************************
 *
 * PURPOSE:	Creates a position and size widget
 *
 * RETURNS:	Widget number
 * 
 * PARAMETERS:  
 *
 * NOTE:	A row column widget is used for the vertical alignment
 *		of the children and a separate row column child is
 *		used whenever horizontal orientation is needed
 *
 **********************************************************************
*/

#include "std.h"
#include "x.h"
#include "routines.h"

 /* Internal functions */
 void          wid_posiz_buta(Widget, XtPointer, XtPointer);
 void          junk          ();
 void          subit         (void (*)());
 void          doit	     (void);
 
 /* File variables */
 static Widget        iw_ix = (Widget)0,  iw_iy = (Widget)0,
                      iw_icols = (Widget)0, iw_irows = (Widget)0;
 static Widget        iw_pushc,   iw_pusha;   
 static Widget        iw_rowcol1, iw_rowcol2; 
 static Widget        iw_title1,  iw_title2;
 static Widget        iw_posizt= (Widget)0;

 /* Common variables */
 int           ix_posiz;
 int           iy_posiz;
 int           icols_posiz  = 64 ;  // Default size
 int           irows_posiz  = 64 ;

 /******************************************************************/
 
 Widget wid_posiz(Widget iw_parent, Widget iw_posiz, int ixt, int iyt, 
                  int icolst, int irowst, char ctype, void (* call)())
 {
 Widget        iw_rowcol, iw_dum; 
 char          cval[5];

 ix_posiz    = ixt;    iy_posiz    = iyt;
 icols_posiz = icolst; irows_posiz = irowst;

 if (iw_posiz == (Widget)0) 
    {
    /* Create a  new dialog widget for iw_posiz */
    iw_posiz  = wid_dialog(iw_parent,0, "position",-1,-1);

    /* Create a RowColumn widget within iw_posiz */
    iw_rowcol = wid_rowcol(iw_posiz, 'v', -1, -1);

    /*--------------- Position portion of widget ----------------*/

    /* Create a label widget with default alignment */
    iw_title1 = wid_labelg(iw_rowcol, 0, 
                "Enter X & Y position (in pixels)", -1,-1);

    /* Create a parent for the prompts and labels */
    iw_rowcol1  = wid_rowcol(iw_rowcol, 'h', -1, -1);

    /* Create a label and text box for ix */
    sprintf(cval,"%4d",ix_posiz);
    iw_ix = wid_textboxb(iw_rowcol1,0,"X:   ",cval,4);
    sprintf(cval,"%4d",iy_posiz);
    iw_iy = wid_textboxb(iw_rowcol1,0,"Y:   ",cval,4);

    if (ctype == 's'  || ctype == 'S' )
       {
       XtUnmanageChild(iw_title1);
       XtUnmanageChild(iw_rowcol1);
       }
 
    /*------------------- Size portion of widget ----------------*/

    /* Create a label widget with default alignment */
    iw_title2 = wid_labelg(iw_rowcol, 0, 
                        "Enter cols. & rows (in pixels)", -1,-1);

    /* Create a rowcol parent for the prompts and labels */
    iw_rowcol2  = wid_rowcol(iw_rowcol, 'h', -1, -1);

    /* create a label and text box for width & height */
    sprintf(cval,"%4d",icols_posiz);
    iw_icols = wid_textboxb(iw_rowcol2,0,"COLS:",cval,4);
    sprintf(cval,"%4d",irows_posiz);
    iw_irows = wid_textboxb(iw_rowcol2,0,"ROWS:",cval,4);

    if (ctype == 'P'  || ctype == 'p' )
       {
       XtUnmanageChild(iw_title2);
       XtUnmanageChild(iw_rowcol2);
       }
   
   /* Create a separator for consistancy with other pop up dialogs */
   wid_sep(iw_rowcol);

   iw_dum = wid_stdbut(iw_rowcol, iw_posiz, &iw_dum, &iw_pushc, &iw_pusha, 
                       "CA",fin_cb,fin_cb, wid_posiz_buta, (char *) call);
   }

 else 
    {
    /* Update widget for iw_posiz */
    iw_posiz  = wid_dialog(iw_parent,iw_posiz, "position",-1,-1);

    XtRemoveAllCallbacks(iw_pusha,XmNactivateCallback); 
    XtAddCallback(iw_pusha,XmNactivateCallback,
                   (XtCallbackProc) wid_posiz_buta, (XtPointer) call);

    if (ctype == 'p' || ctype == 'b' || ctype == 'P' || ctype == 'B' )
       {
       /* Update label and text box for ix */
       sprintf(cval,"%4d",ix_posiz);
       iw_ix = wid_textboxb(iw_rowcol1,iw_ix,"X:",cval,4);
       sprintf(cval,"%4d",iy_posiz);
       iw_iy = wid_textboxb(iw_rowcol1,iw_iy,"Y:",cval,4);

       XtManageChild(iw_title1);
       XtManageChild(iw_rowcol1);
       }
    if (ctype == 'P'  || ctype == 'p' )
       {
       XtUnmanageChild(iw_title2);
       XtUnmanageChild(iw_rowcol2);
       }
  
    if (ctype == 's' || ctype == 'b' || ctype == 'S' || ctype == 'B' )
       {
       /* Update text box for cols & rows */
       sprintf(cval,"%4d",icols_posiz);
       iw_icols = wid_textboxb(iw_rowcol2,iw_icols,"COLS:",cval,4);
       sprintf(cval,"%4d",irows_posiz);
       iw_irows = wid_textboxb(iw_rowcol2,iw_irows,"ROWS:",cval,4);
       XtManageChild(iw_title2);
       XtManageChild(iw_rowcol2); 
       }
    if (ctype == 'P'  || ctype == 'p' )
       {
       XtUnmanageChild(iw_title2);
       XtUnmanageChild(iw_rowcol2);
       }
    if (ctype == 's'  || ctype == 'S' )
       {
       XtUnmanageChild(iw_title1);
       XtUnmanageChild(iw_rowcol1);
       }
    }

 XtManageChild(iw_posiz);

 iw_posizt = iw_posiz;
 return iw_posiz;
 }


 /****************** Accept button callback   *********************/

 void wid_posiz_buta(Widget iw_temp, XtPointer data, XtPointer call_data)

 { 
 /* Recover values from the text boxes */
 if (iw_ix    > (Widget)0 && strtoint(iw_ix,   "x position",&ix_posiz)    
             == FALSE) return;
 if (iw_iy    > (Widget)0 && strtoint(iw_iy,   "y position",&iy_posiz)    
             == FALSE) return;
 if (iw_icols > (Widget)0 && strtoint(iw_icols,"columns",   &icols_posiz)
             == FALSE) return;
 if (iw_irows > (Widget)0 && strtoint(iw_irows,"rows",      &irows_posiz)
             == FALSE) return;

 /* Unmanage wid_posiz widget */
 XtUnmanageChild(iw_posizt);

 /* Kludge to get it to call routine passed in data */
 subit((void (*)()) data); 
 }


 /****************** Kludge routine to activate data *****************/


 void subit(void (*doit)())
 {
 doit();
 }



 /****************** Sample kludge routine    ************************/


 void junk()
 {
 printf(" in junk, SUCCESS \n");
 printf(" ix,iy: %d %d \n",ix_posiz,iy_posiz);
 }
