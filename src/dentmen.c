

/*
C++************************************************************************
C
C DENTMEN  -- 
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
C    DENTMEN
C
C    PURPOSE:         DISPLAY DENTIST MENU
C
C    CALLED BY:       IMAGEMEN
C
C        
C    imagemen --> wid_files --> imagemen_cb 
C                                 v
C                               imagemen1 
C                                 v
C                               dentmen  
C                                 v
C                               dentmen_buta
C                                 v 
C                               dent |--> distoref
C                                    |--> dentmen2               
C                                    |--> dentaction   
C
C***********************************************************************
*/

#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#include "common.h"
#include "routines.h"

 /* external function prototypes */
 extern void  dent(void);

 /* internal function prototypes */
 static void dentmen_buta(Widget iw_temp, caddr_t, caddr_t);
 static void dentmen_butc(Widget iw_temp, caddr_t, caddr_t);
 
 /* global variables used elsewhere (in dent.c)  */
 float    dentrefl = 0.5, dentfct = 0.5, dentthlev;

 /* internal file scope variables */
 static Widget   iw_scales[6];
 static Widget   iw_thlev=(Widget) 0, iw_refl=(Widget) 0, iw_fct=(Widget) 0;
 static Widget   iw_pushc=(Widget) 0, iw_pusha=(Widget) 0;
 static Widget   iw_dentmen = (Widget) 0;
 static Widget   iw_lab     = (Widget)0,  iw_rowcolh2 =(Widget) 0, 
                 iw_rowcolv = (Widget)0;

 /***************************   dentmen   ****************************/

 void dentmen()

 { 
 int    i, ifct, irefl;
 Widget iw_dum;
 char   cval[50];

 if (iw_dentmen <= (Widget)0)
    {   /* create dentist menu widget first */

    iw_dentmen  = wid_dialog(iw_win, 0, "Drill-n-fill options", -1, -1);
    iw_rowcolv  = wid_rowcol(iw_dentmen, 'v', -1, -1);

    /* create label box for view dir.  ---------------------  label  */
    sprintf(cval," Viewed along rows of volume (nrow x nslice plane)");
    wid_labelg(iw_rowcolv, 0, cval, -1, -1);

    /* create label box for image range ---------------------  range  */
    sprintf(cval,"Range:     %f ...%f",filedata->fmin_spi,filedata->fmax_spi);
    iw_lab  = wid_labelg(iw_rowcolv, 0, cval, -1, -1);

    /* create text box  for threshold --------------------  threshold */
    /* SET DEFAULT THRESHOLD TO 50% OF RANGE */
    dentthlev = filedata -> fmin_spi + 
                ((filedata->fmax_spi - filedata->fmin_spi) / 2.0);

    sprintf(cval,"%f",dentthlev);
    iw_thlev = wid_textboxb(iw_rowcolv,0,"Threshold:",cval,10);

    /* create horizontal rowcol for refl and fct ------------------   */
    iw_rowcolh2  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* create scale widget for % reflection ---------------- dentrefl */
    irefl = 100 * dentrefl;   
    iw_refl = wid_scale(iw_rowcolh2, 0, "% Reflection ", 0,100,  irefl, 
                       180,50, -1,-1);

    /* create scale widget for fct  ------------------------- dentfct */
    ifct = 100.0 * dentfct;
    iw_fct  = wid_scale(iw_rowcolh2, 0, "% Attenuation", 0,100,  ifct, 
                       180,50, -1,-1);

    /* create scale widgets for windowing volume ------------- scales */
    for (i=0; i < 6; iw_scales[i++]  = (Widget) 0);
    wid_win(iw_rowcolv, &iw_scales[0], nsam, 0, 0);

    /* create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcolv, iw_dentmen, 
                        &iw_dum, &iw_pushc, &iw_pusha,  "CA",
                        fin_cb, dentmen_butc, dentmen_buta, NULL);
    }   /* (iw_dentmen <= (Widget)0)  */



 else
    {   /* dentist menu widget already exists, update it */

    /* create label box for image range ---------------------  range  */
    sprintf(cval,"Range:     %f ...%f",filedata->fmin_spi,filedata->fmax_spi);
    iw_lab  = wid_labelg(iw_rowcolv, iw_lab, cval, -1, -1);

    /* create text box for threshold ---------------------  threshold */
    /* must reset default threshold */
    dentthlev = filedata->fmin_spi + 
                ((filedata->fmax_spi - filedata->fmin_spi) / 2.0);

    sprintf(cval,"%f",dentthlev);
    iw_thlev = wid_textboxb(iw_rowcolv,iw_thlev,"Threshold:",cval,10);

    /* create scale widget for % reflection ----------------- dentrefl */
    irefl   = 100.0 * dentrefl;
    iw_refl = wid_scale(iw_rowcolh2, iw_refl, "% Reflection ", 0,100, 
                        irefl, 180, 50, -1, -1);

    /* create scale widget for fct  -------------------------- dentfct */
    ifct   = 100.0 * dentfct;
    iw_fct = wid_scale(iw_rowcolh2, iw_fct,  "% Attenuation", 0,100, 
                        ifct, 225, 50, -1, -1);

    /* create scale widgets for windowing volume -------------- scales */
    wid_win(iw_rowcolv, &iw_scales[0], nsam, 0, 0);
    }

 XtManageChild(iw_dentmen);
 }

 /*********** cancel button callback **********************************/

 void dentmen_butc(Widget iw_temp, caddr_t data, caddr_t call_data)
   {
   if (filedata && filedata->fp) closefile(filedata);
   XtUnmanageChild(iw_dentmen);
   }

 /*********** accept button callback **********************************/

 void dentmen_buta(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
 char *string;
 int  irefl, ifct;

 /* set default values for size range */
 nslice1 = 1; nslice2 = nslice;
 nrow1   = 1; nrow2   = nrow;

 /* get nsam1 & nsam2 */
 XmScaleGetValue(iw_scales[0],&nsam1);
 XmScaleGetValue(iw_scales[1],&nsam2);
 if (nsam2 < nsam1)
    { spout("*** ENDING COL MUST BE > STARTING COL"); return; }

 /* get reflection percentage */
 XmScaleGetValue(iw_refl,&irefl);
 dentrefl = (float) irefl / 100.0;

 /* get dentfct percentage */
 XmScaleGetValue(iw_fct,&ifct);
 dentfct = (float) ifct / 100.0; 

 /* find threshold */
 string = XmTextGetString(iw_thlev);
 sscanf(string,"%f",&dentthlev);
 if (dentthlev > filedata -> fmax_spi) dentthlev = filedata -> fmax_spi;
 if (dentthlev < filedata -> fmin_spi) dentthlev = filedata -> fmin_spi;
 if (string) free(string);

 XtUnmanageChild(iw_dentmen);

 /* refresh screen ( this does not take effect, will need event loop */
 expos_cb(NULL,NULL,NULL); 
 XFlush(idispl);
 XmUpdateDisplay(iw_win);

 /* display surfaces */
 dent();

 }


	
