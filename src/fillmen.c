
/*$Header: /usr16/software/web/src/RCS/fillmen.c,v 1.7 2018/12/07 17:03:31 leith Exp $*/

/*
C++*********************************************************************
C
C fillmen
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
C    fillmen
C
C    PURPOSE:         display filling menu
C
C    CALLED BY:       filtermen
C

C***********************************************************************
*/

#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"
#include "filter.h"

 /* external function prototypes */

 /* internal function prototypes */
 static void     fillmen_buta  (Widget, XtPointer, XtPointer);

 /* external common variables used here */

 /* common variables used here and elsewhere*/
 int             ixseed = 2;
 int             iyseed = 2;
 float           threshval, fillval;
 int             fillconnect = 4;

 static Widget   iw_fillmen = (Widget)0,   iw_ixseed = (Widget)0,
                 iw_iyseed = (Widget)0, iw_thresh = (Widget)0, 
	         iw_fillval = (Widget)0, iw_fillconnect =(Widget) 0;

/********************************  fillmen **************************/

 void fillmen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval[5], cval11[11],cval2[2];
 Widget   iw_rowcol, iw_rowcol1;
 Widget   iw_pushc, iw_pusha, iw_dums;

 if (iw_fillmen <= (Widget)0)
    {   /* create fill menu widget first */

    iw_fillmen = wid_dialog(iw_win,iw_fillmen,
                             "fill options",20,20);
    iw_rowcol   = wid_rowcol(iw_fillmen,'v',-1,-1);

    iw_rowcol1  = wid_rowcol(iw_rowcol,'h',-1,-1);
    /* create text box for ixseed input --------------------- ixseed */
    sprintf(cval,"%4d",ixseed);
    iw_ixseed = wid_textboxb(iw_rowcol1,0, "X SEED:",cval,4);

    /* create text box for iyseed input --------------------- iyseed */
    sprintf(cval,"%4d",iyseed);
    iw_iyseed = wid_textboxb(iw_rowcol1,0, "Y SEED:",cval,4);

    /* create text box for threshold -----------------------  thresh */
    /* must set default threshold */
    threshval = fminold + ((fmaxold - fminold) / 2.0);
    sprintf(cval11,"%f",threshval);
    iw_thresh = wid_textboxb(iw_rowcol,iw_thresh,"Threshold:",
                             cval11,10);

    /* create text box for fillval ------------------------  fillval */
    /* must set default fillval */
    fillval = fminold;
    sprintf(cval11,"%f",fillval);
    iw_fillval = wid_textboxb(iw_rowcol,iw_fillval,
                                "Fill value:",cval11,10);

    /* create text box for fillconnect ----------------  fillconnect */
    sprintf(cval2,"%1d",fillconnect);
    iw_fillconnect = wid_textboxb(iw_rowcol,iw_fillconnect,
                                "Connectivity:",cval2,1);

    /* create accept boxes  ---------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    wid_stdbut(iw_rowcol, iw_fillmen,
               &iw_dums, &iw_pushc, &iw_pusha, 
              "CA",fin_cb,fin_cb,fillmen_buta, NULL);
    }

 XtManageChild(iw_fillmen);
 }

 /*********************** fill accept button callback *****************/

 void fillmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *         string;
 char           filtertype[2];

 if (strtoint(iw_ixseed,"X seed",&ixseed) == FALSE) return;

 if (strtoint(iw_iyseed,"Y seed", &iyseed) == FALSE) return;

 if (ixseed  < 1 || ixseed  > nsamold) 
    {
    spout("*** X seed range is: 1...nsam"); 
    return;
    }

 if (iyseed < 1 || iyseed > nrowold) 
    {
    spout("*** Y seed range is: 1...nrow"); 
    return;
    }

 if (strtoint(iw_fillconnect,"Connectivity",&fillconnect) == FALSE) return;
 if (fillconnect != 4 && fillconnect != 8) 
    {
    spout("*** Connectivity must be 4 or 8!"); 
    return;
    }

 /* find threshold */
 string = XmTextGetString(iw_thresh);
 sscanf(string,"%f",&threshval);
 if (threshval > fmaxold) threshval = fmaxold;
 if (threshval < fminold) threshval = fminold;
 if (string) free(string);

 /* find fillval */
 string = XmTextGetString(iw_fillval);
 sscanf(string,"%f",&fillval);
 if (string) free(string);

 XtUnmanageChild(iw_fillmen);

 filtertype[0] = 'x';
 filtertype[1] = '\0';
 filter_but(iw_temp, (XtPointer) filtertype, call_data);
 }

