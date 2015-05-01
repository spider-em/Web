
/*$Header: /usr8/web/src/RCS/lahemen.c,v 1.3 2005/10/18 16:59:56 leith Exp $*/

/*
C++*********************************************************************
C
C lahemen
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
C    lahemen
C
C    PURPOSE:         display lahe menu
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
   filtermen -->  lahemen --> lahe -->
                   

C***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* function prototypes */
 void         lahemen       (Widget, XtPointer, XtPointer);
 void         lahemen_buta  (Widget, XtPointer, XtPointer);
 
 /* common global set elsewhere */
 extern int      nsamold, nrowold;
 extern int      bins;
 extern int      border;

 /* file level variables used here */
 static Widget   iw_lahemen, iw_border, iw_bins;

/************************************  lahemen **********************/

 void lahemen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval[5];
 Widget   iw_rowcol;
 Widget   iw_pushc, iw_pusha, iw_dums;

 if (iw_lahemen <= (Widget)0)
    {   /* create lahe menu widget first */

    iw_lahemen = wid_dialog(iw_win,iw_lahemen,"lahe options",20,20);
    iw_rowcol   = wid_rowcol(iw_lahemen,'v',-1,-1);

    /* create text box for bordersize input ----------------- border */
    sprintf(cval,"%4d",border);
    iw_border = wid_textboxb(iw_rowcol,0, 
                  "Border ((box -1)/2):",cval,4);

    /* create text box for bins input ------------------------- bins */
    sprintf(cval,"%4d",bins);
    iw_bins = wid_textboxb(iw_rowcol,0, "Bins:",cval,4);

    /* create accept boxes  ---------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    wid_stdbut(iw_rowcol, iw_lahemen, 
               &iw_dums, &iw_pushc, &iw_pusha, 
               "CA",fin_cb,fin_cb,lahemen_buta, NULL);
    }

 XtManageChild(iw_lahemen);
 }

 /********************* lahe accept button callback *******************/

 void lahemen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char           filtertype[2];

 if (strtoint(iw_bins,"bins",&bins) == FALSE) return;
 if (bins  < 2 ) 
    { spout("*** Bins range is: 2...");  return; }

 if (strtoint(iw_border,"border", &border) == FALSE) return;
 if (border < 1 || 
     border >= (nsamold) /2 || border >= (nrowold)/2) 
    { spout("*** Border range is: 1...min(nsam/2, nrow/2)"); return; }

 XtUnmanageChild(iw_lahemen);

 filtertype[0] = 't';
 filtertype[1] = '\0';
 filter_but(iw_temp, (XtPointer) filtertype, call_data);
 }

