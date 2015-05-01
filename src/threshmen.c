
/*$Header: /usr8/web/src/RCS/threshmen.c,v 1.3 2011/09/26 12:14:23 leith Exp $*/

/*
C++*********************************************************************
C
C threshmen
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
C    PURPOSE:         Display threshmen menu
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
   filtermen --> threshmen --> threshmen_buta --> filter_but --> thresh

C***********************************************************************
*/

#include <Xm/Text.h>
#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"

 /* Internal function prototypes */
 void          threshmen_buta(Widget, XtPointer, XtPointer);
 void          threshmen     (Widget, XtPointer, XtPointer);

 /* External global variables used here */
 extern float    fminold, fmaxold;
 extern float    lower, lowerval, upper,  upperval;

 /* Common variables used here */
 static Widget   iw_threshmen, iw_lower,    iw_lowerval,  
                 iw_upper,     iw_upperval, iw_kgo, iw_kend;

 static int      kgo=0, kend=100;

/*********************************** threshmen **********************/

 void threshmen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval11[12];
 Widget   iw_rowcol, iw_rowcolh;
 Widget   iw_pushc, iw_pusha, iw_dums;

 if (iw_threshmen <= (Widget)0)
    {   /* Create thresh menu widget first */

    iw_threshmen = wid_dialog(iw_win,iw_threshmen,
                           "Threshold values",20,20);
    iw_rowcol    = wid_rowcol(iw_threshmen,'v',-1,-1);

    /* Create a horizontal row-col widget for scales */
    iw_rowcolh = wid_rowcol(iw_rowcol, 'h', -1,-1);

    /* create scale widget for lower --------------------------- kgo */
    kgo     = 0;
    iw_kgo  = wid_scale(iw_rowcolh, 0, "", 0,100,  kgo, 
                        180,50, -1,-1);

    /* Create scale widget for upper -------------------------- kend */
    kend    = 100;
    iw_kend = wid_scale(iw_rowcolh, 0, "", 0,100,  kend, 
                         180,50, -1,-1);

    XtAddCallback(iw_kgo,XmNvalueChangedCallback,
                           (XtCallbackProc) threshmen,"0");
    XtAddCallback(iw_kend,XmNvalueChangedCallback,
                           (XtCallbackProc) threshmen,"0");

    /* Create text box for lower ----------------------------  lower */
    lower = fminold + kgo * (fmaxold - fminold) / 100.0;
    sprintf(cval11,"%f",lower);
    iw_lower = wid_textboxb(iw_rowcol,iw_lower,
                              "Lower Threshold:",cval11,10);

    /* Create text box for lowerval ----------------------  lowerval */
    lowerval = fminold;
    sprintf(cval11,"%f",lowerval);
    iw_lowerval = wid_textboxb(iw_rowcol,iw_lowerval,
                               "Lower Value:    ",cval11,10);

    /* Create text box for upper ----------------------------  upper */
    upper = fminold + kend * (fmaxold - fminold)/ 100.0;
    sprintf(cval11,"%f",upper);
    iw_upper = wid_textboxb(iw_rowcol,iw_upper,
                               "Upper Threshold:",cval11,10);

    /* Create text box for upperval ----------------------  upperval */
    upperval = fmaxold;
    sprintf(cval11,"%f",upperval);
    iw_upperval = wid_textboxb(iw_rowcol,iw_upperval,
                              "Upper Value:    ",cval11,10);

    /* Create accept boxes  ---------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    wid_stdbut(iw_rowcol, iw_threshmen, 
               &iw_dums, &iw_pushc, &iw_pusha, 
               "CA",fin_cb,fin_cb,threshmen_buta, NULL);
    }

 else
    {   /* Thresh menu widget already exists, update it */

    /* Get kgo & kend */
    XmScaleGetValue(iw_kgo, &kgo);
    XmScaleGetValue(iw_kend,&kend);

    /* Alter text box for lower -----------------------------  lower */
    lower = fminold + kgo * (fmaxold - fminold)/ 100.0;
    sprintf(cval11,"%f",lower);
    iw_lower = wid_textboxb(iw_rowcol,iw_lower,
                            "Lower Threshold:",cval11,10);

    /* Create text box for lowerval ----------------------  lowerval */
    sprintf(cval11,"%f",lowerval);
    iw_lowerval = wid_textboxb(iw_rowcol,iw_lowerval,
                               "Lower Value:    ",cval11,10);

    /* Alter text box for upper -----------------------------  upper */
    upper = fminold + kend * (fmaxold - fminold)/ 100.0;
    sprintf(cval11,"%f",upper);
    iw_upper = wid_textboxb(iw_rowcol,iw_upper,
                            "Upper Threshold:",cval11,10);

    /* alter text box for upperval ------------------------  upperval */
    sprintf(cval11,"%f",upperval);
    iw_upperval = wid_textboxb(iw_rowcol,iw_upperval,
                               "Upper Value:    ",cval11,10);
   }

 XtManageChild(iw_threshmen);
 }



 /********************* thresh accept button callback *****************/

 void threshmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *         string;
 char           filtertype[2];

 /* Find thresholds */
 string = XmTextGetString(iw_lower);
 sscanf(string,"%f",&lower);
 if (string) free(string);

 string = XmTextGetString(iw_lowerval);
 sscanf(string,"%f",&lowerval);
 if (string) free(string);

 string = XmTextGetString(iw_upper);
 sscanf(string,"%f",&upper);
 if (string) free(string);

 string = XmTextGetString(iw_upperval);
 sscanf(string,"%f",&upperval);
 if (string) free(string);

 filtertype[0] = 'u';
 filtertype[1] = '\0';
 filter_but(iw_temp, (XtPointer) filtertype, call_data);
 }
