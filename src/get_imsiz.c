
/*$Header: /usr8/web/src/RCS/get_imsiz.c,v 1.6 2005/10/18 16:59:59 leith Exp $*/
/*
 **********************************************************************
 *
 * get_imsiz.c
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
 * 
 * 
 * PURPOSE:	creates a position and size widget
 *
 * RETURNS:	unused integer
 * 
 * PARAMETERS:  
 *
 **********************************************************************
*/

#include "files.h"
#include "common.h"

/* external function prototypes */
 extern Widget wid_dialog    (Widget, Widget, char *, int, int);
 extern Widget wid_rowcol    (Widget, char , int, int);
 extern Widget wid_textboxb  (Widget, Widget, char *, char *,  int);
 extern Widget wid_labelg    (Widget, Widget, char *, int, int);
 extern Widget wid_sep       (Widget);
 extern Widget wid_stdbut    (Widget, Widget, Widget*, Widget*, Widget*,
                             char *, void (*)(), void (*)(), void (*)(),
                             void (*)());
 extern void   fin_cb        (Widget, caddr_t, caddr_t);
 extern int    strtoint      (Widget, char *, int *);

 /* internal functions */
 void          get_imsiz_buta(Widget, XtPointer, XtPointer);

 /* file variables */
 static Widget        iw_cols = 0, iw_rows = 0;
 static Widget        iw_pushc, iw_pusha, iw_rowcolv;
 static Widget        iw_imsiz = (Widget) 0, iw_slices;

 /* common variables */
 extern int nsam8, nrow8, nslice8;

 /*********************  get_imsiz  *********************************/
 
 void get_imsiz(void)
 {
 Widget        iw_dum; 
 char          cval[5];

 if (iw_imsiz <= (Widget)0) 
    {
    /* create a  new dialog widget for iw_imsiz */
    iw_imsiz  = wid_dialog(iw_win,0, "position",-1,-1);

    /* create a vertical RowColumn widget within iw_imsiz */
    iw_rowcolv = wid_rowcol(iw_imsiz, 'v', -1, -1);

    /* create a label widget with default alignment */
    wid_labelg(iw_rowcolv, 0, 
             "Enter columns, rows & slices (in pixels)", -1,-1);
    
    /* create a label and text box for width & height */
    sprintf(cval,"%4d",nsam8);
    iw_cols = wid_textboxb(iw_rowcolv,0,"Cols:",cval,4);

    sprintf(cval,"%4d",nrow8);
    iw_rows = wid_textboxb(iw_rowcolv,0,"Rows:",cval,4);

    sprintf(cval,"%4d",nslice8);
    iw_slices = wid_textboxb(iw_rowcolv,0,"Slices:",cval,4);
 
   /* create a separator for consistancy with other pop up dialogs */
   wid_sep(iw_rowcolv);

   wid_stdbut(iw_rowcolv, iw_imsiz, &iw_dum, &iw_pushc, &iw_pusha, 
                       "CA",fin_cb,fin_cb, get_imsiz_buta, NULL);
   }

 XtManageChild(iw_imsiz);
 return;
 }


 /****************** accept button callback   *********************/


 void get_imsiz_buta(Widget iw_temp, XtPointer data, XtPointer call_data)

 { 

 /* recover values from the text boxes */
 if (iw_cols > (Widget)0 && strtoint(iw_cols,"columns",   &nsam8)
             == FALSE) return;
 if (iw_rows > (Widget)0 && strtoint(iw_rows,"rows",      &nrow8)
             == FALSE) return;
 if (iw_slices > (Widget)0 && strtoint(iw_slices,"slices",&nslice8)
             == FALSE) return;

 /* unmanage get_imsiz widget */
 XtUnmanageChild(iw_imsiz);

 }




