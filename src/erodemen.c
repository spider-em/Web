
/*$Header: /usr8/web/src/RCS/erodemen.c,v 1.2 2005/10/18 16:59:43 leith Exp $*/

/*
C++*********************************************************************
C
C erodemen
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C **********************************************************************
C
C    threshmen
C
C    PURPOSE:         display erodemen menu
C
C    CALLED BY:       imagemen
C
C    CALL TREE:

                  
   imagemen       
      |          
      v           
  imagemen_cb     
      |           
      v           
   imagemen1      
      |            
      v           
   filtermen --> erodemen --> erodemen_buta --> filter_but --> erode
                                                           --> dilate

C***********************************************************************
*/

#include <Xm/Text.h>
#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"

 /* internal function prototypes */
 void          erodemen_buta(Widget, XtPointer, XtPointer);

 /* external global variables used here */
 extern float    fminold, fmaxold;
 extern float    erodethresh;

 /* common variables used here */
 static Widget   iw_erodemen = NULL, iw_lower = NULL;

 /* file scope variables */
 char filtertype[2];

/*********************************** erodemen **********************/

 void erodemen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval11[12];
 Widget   iw_rowcol;
 Widget   iw_pushc, iw_pusha, iw_dums;

 filtertype[0] = * ((char * ) data);
 filtertype[1] = '\0';

 if (iw_erodemen <= (Widget)0)
    {   /* create erodemen menu widget first */

    iw_erodemen = wid_dialog(iw_win,iw_erodemen,
                           "Threshold values",20,20);
    iw_rowcol    = wid_rowcol(iw_erodemen,'v',-1,-1);

    /* create text box for erodethresh ----------------  erodethresh */
    erodethresh = fmaxold;
    sprintf(cval11,"%f",erodethresh);
    iw_lower = wid_textboxb(iw_rowcol,iw_lower,
                              "Erode/Dilate Threshold:",cval11,10);

    /* create accept boxes  ---------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    wid_stdbut(iw_rowcol, iw_erodemen, 
               &iw_dums, &iw_pushc, &iw_pusha, 
               "CA",fin_cb,fin_cb,erodemen_buta, NULL);
    }

 else
    {   /* erodemen menu widget already exists, update it */

    /* create text box for erodethresh ----------------  erodethresh */
    sprintf(cval11,"%f",erodethresh);
    iw_lower = wid_textboxb(iw_rowcol,iw_lower,
                              "Erode/Dilate Threshold:",cval11,10);
    }

 XtManageChild(iw_erodemen);
 }



 /*********************  accept button callback *****************/

 void erodemen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * string;

 /* find erodethresh */
 string = XmTextGetString(iw_lower);
 sscanf(string,"%f",&erodethresh);
 if (string) free(string);

 XtUnmanageChild(iw_erodemen);
 filter_but(iw_temp, (XtPointer) filtertype, call_data);
 }
