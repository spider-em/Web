
/*$Header: /usr8/web/src/RCS/denoisemen.c,v 1.6 2005/10/18 16:59:57 leith Exp $*/

/*
C++********************************************************************
C
C denoisemen
C
C *********************************************************************
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
C *********************************************************************
C
C    denoisemen
C
C    PURPOSE:         display denoise menu
C
C    CALLED BY:       filtermen
C
C    CALL TREE:       denoisemen -> denoisemen_buta -> filter_but
C                                                         |
C                                                      denoise
C
C *********************************************************************
*/


#include "std.h"
#ifndef VMS
#include "float.h"
#endif
#include "common.h"
#include "routines.h"
#include <Xm/Text.h>

 /* externally defined global variables used here */
 extern float      fminold, fmaxold;

 /* internal subroutines */
 void denoisemen_buta(Widget, XtPointer, XtPointer);

 /* global variable defined here */
 float noiselowerlim;

 /* file scope variables */
 static Widget   iw_denoisemen = (Widget) 0;
 static Widget   iw_lower;

/*********************************** denoisemen **********************/

 void denoisemen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval11[12];
 static Widget   iw_rowcol, iw_pushc = 0, iw_pusha = 0, iw_dums;

 if (iw_denoisemen == (Widget)0)
    {   /* create denoise menu widget first */

    iw_denoisemen = wid_dialog(iw_win,iw_denoisemen,
                           "Noise removal",20,20);
    iw_rowcol    = wid_rowcol(iw_denoisemen,'v',-1,-1);

    /* create text box for lower ----------------------------  lower */
    noiselowerlim = fminold;
    sprintf(cval11,"%f",noiselowerlim);
    iw_lower = wid_textboxb(iw_rowcol,iw_lower,
                              "Remove noise below:",cval11,10);

    /* create accept boxes  ---------------------------------- apply */
    wid_stdbut(iw_rowcol, iw_denoisemen, 
               &iw_dums, &iw_pushc, &iw_pusha, 
               "CA", fin_cb, fin_cb, denoisemen_buta, NULL);
    }
 else
    {   /* denoise menu widget already exists, update it */

    /* alter text box for lower -----------------------------  lower */
    noiselowerlim = fminold;
    sprintf(cval11,"%f",noiselowerlim);
    iw_lower = wid_textboxb(iw_rowcol,iw_lower,
                            "Remove noise below:",cval11,10);
  }

 XtManageChild(iw_denoisemen);
 }


 /********************* denoise accept button callback *****************/

 void denoisemen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *         string;
 char           filtertype[2];

 /* find thresholds */
 string = XmTextGetString(iw_lower);
 sscanf(string,"%f",&noiselowerlim);
 if (string) free(string);

 filtertype[0] = 'B';
 filtertype[1] = '\0';
 filter_but(iw_temp, (XtPointer) filtertype, call_data);
 XtUnmanageChild(iw_denoisemen);
 }


 /******************************  denoise *****************************/

 int denoise(float * oldimage, int nsamt,     int nrowt, 
             float * kernal,   int nsamk,     int nrowk, 
             float * newimage, float * fmint, float * fmaxt, 
             float lowerlim)
 {
 int     ix, iy,  ixk, iyk, nrowkd2, nsamkd2, it;
 float   val,valk,vali, ksum, ftemp;
 int     icon1, icon2, icon3, icon4, icon5;
 int     ixo,iyo;
 int     ip;
 float * pf;
 float * endimage;
 float * endimageo;
 float   valatkernal;

 if (nsamk % 2 == 0 || nrowk % 2 == 0)
    {
    spout("*** Filter kernal size must be odd!");
    return FALSE;
    }

 nrowkd2 = nrowk / 2;
 nsamkd2 = nsamk / 2;

 *fmint  = FLT_MAX;
 *fmaxt  = FLT_MIN;

 /* convolve over all positions in the new image except border */

 for (iy = nrowkd2; iy < nrowt - nrowkd2; iy++)
    {  /* all rows except border */
    icon5 = iy * nsamt;
    icon3 = iy - nrowkd2;

    for (ix = nsamkd2; ix < nsamt - nsamkd2; ix++)
       {  /* all cols except border */
       icon2 = ix - nsamkd2;
       val   = 0.0;
       ksum  = 0;

       /* convolve over all positions in the kernal */
       for (iyk = 0; iyk < nrowk; iyk++)
          {
          icon1    = iyk * nrowk;
          /* iyo   = icon3 + iyk; */
          /* icon4 = icon2 + iyo * nrowt; */
          icon4    = icon2 + (icon3 + iyk) * nsamt;

          for (ixk = 0; ixk < nsamk; ixk++)
             {
             /* get current kernal value &  current image value   */
             valatkernal  = oldimage[icon4 + ixk];    
             if (valatkernal >= lowerlim)
                {
                val += kernal[icon1 + ixk] * valatkernal;
                ksum++; 
                }
             }
          }
       /* ftemp is original image value */
       ftemp =  oldimage[icon5 + ix];
       if (ftemp < lowerlim && ksum > 0)
           {
           val   = val / ksum;
           newimage[icon5 + ix] = val;
           *fmaxt = MYMAX(*fmaxt,val);
           *fmint = MYMIN(*fmint,val);
           }
       else
           {
           newimage[icon5 + ix] = ftemp;
           *fmaxt = MYMAX(*fmaxt,ftemp);
           *fmint = MYMIN(*fmint,ftemp);
           }
       }
     }

  /* fill in borders with original values */

  endimage = newimage + nsamt * nrowt - 1;
  endimageo = oldimage + nsamt * nrowt - 1;

  /* fill top and bottem borders */
  for(ip = 0; ip < nsamt * nrowkd2; ip++)
     {
     *(newimage + ip) = *(oldimage  + ip);
     *(endimage - ip) = *(endimageo - ip);
     }

  /* fill left and right borders */
   for(it = 0; it < nsamkd2; it++)
     {    
     for(ip = it; ip < nsamt*nrowt; ip=ip+nsamt)
        {
        *(newimage + ip) = *(oldimage  + ip); 
        *(endimage - ip) = *(endimageo - ip);
        }
     }

  return TRUE;
}

