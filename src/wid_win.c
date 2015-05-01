
/*$Header: /usr8/web/src/RCS/wid_win.c,v 1.6 2011/07/19 19:41:45 leith Exp $*/
/*
C++************************************************************************
C
C WID_WIN
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
C WID_WIN
C
C PURPOSE:      SET UP A WINDOWING SCALE WIDGET FOR 3-D VOLS
C
C PARAMETERS:   IW_PARENT         PARENT WIDGET         READ
C               IW_ITS            WIDGET NUMBERS        WRITE
C               NSAM,NROW,NSLICE  VOLUME EXTENT         READ
C               IXC               UPPER LEFT X          READ
C               IYT               UPPER LEFT Y,LOWER Y  READ/WRITE
C
C CALLED BY:    SURFMEN
C
C NOTE:         SLIDER IS ONLY CREATED IF CORRESPONDING VALUE OF
C               NSAM, NROW, OR NSLICE IS > 0.
C
C***********************************************************************
*/

#include "common.h"
#include <Xm/Scale.h>

#define  MAX_ARGS 11

 extern Widget wid_labelg   (Widget, Widget, char *, int, int);
 extern Widget wid_rowcol   (Widget, char , int, int);
 

/************************************************************/

 void wid_win(Widget iw_parent, Widget iw_its[6], 
              int nsam, int nrow, int nslice)

 {   
 Arg          args[MAX_ARGS];    /* arg list  */
 Widget       iw_lab1, iw_lab2, iw_lab3;
 Widget       iw_rowcol, iw_rowcolc, iw_rowcolr, iw_rowcols;
 static int   nsamo, nrowo, nsliceo;
 int          iwid, ihi, n;

 if (iw_its[0] == (void *)0 && nsam > 0) 
    {  /* Must create new col widgets ------------------------------- */

    iwid = 180;
    ihi  = 50;

    iw_rowcol   = wid_rowcol(iw_parent,'v',-1,-1);

    /* Create label for cols*/
    iw_rowcolc  = wid_rowcol(iw_rowcol,'h',-1,-1);
    iw_lab2     = wid_labelg(iw_rowcolc, 0, "COLS:  ", -1, -1);

    n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb); n++;
#endif
    XtSetArg(args[n], XmNwidth, iwid);          n++;                   
    XtSetArg(args[n], XmNheight, ihi);          n++;               
    XtSetArg(args[n], XmNscaleWidth, iwid-10);  n++;            
    XtSetArg(args[n], XmNscaleHeight, ihi/2);   n++;        
    XtSetArg(args[n], XmNminimum, 1);           n++;              
    XtSetArg(args[n], XmNmaximum, nsam);        n++;             
    XtSetArg(args[n], XmNdecimalPoints, 0);     n++;           
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL);   n++;       
    XtSetArg(args[n], XmNshowValue, TRUE);      n++;          
    XtSetArg(args[n], XmNvalue, 1);             n++;                
 
    iw_its[0] = XmCreateScale(iw_rowcolc, "iw_scale", args, n);

    XtSetArg(args[n-1], XmNvalue, nsam);       
    iw_its[1] = XmCreateScale(iw_rowcolc, "iw_scale", args, n);
    }

 else if (nsam > 0 && nsam != nsamo)
    { /* Alter scale size in existing widget */
    XtSetArg(args[0], XmNmaximum, nsam); 
      
    XtSetArg(args[1], XmNvalue, 1);   
    XtSetValues(iw_its[0], args, 2);

    XtSetArg(args[1], XmNvalue, nsam);
    XtSetValues(iw_its[1], args, 2);
    }

  if ( iw_its[2] == (void*)0 && nrow > 0) 
    {  /* Must create new nrow widgets ------------------------------ */
   
    /* create label for cols*/
    iw_rowcolr  = wid_rowcol(iw_rowcol,'h',-1,-1);
    iw_lab1     = wid_labelg(iw_rowcolr, 0, "ROWS:  ", -1, -1);

    n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb);    n++;
#endif
    XtSetArg(args[n], XmNwidth, iwid);          n++;                   
    XtSetArg(args[n], XmNheight, ihi);          n++;               
    XtSetArg(args[n], XmNscaleWidth, iwid-10);  n++;           
    XtSetArg(args[n], XmNscaleHeight, ihi/2);   n++;       
    XtSetArg(args[n], XmNminimum, 1);           n++;             
    XtSetArg(args[n], XmNmaximum, nrow);        n++;           
    XtSetArg(args[n], XmNdecimalPoints, 0);     n++;          
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL);  n++;      
    XtSetArg(args[n], XmNshowValue, TRUE);      n++;          
    XtSetArg(args[n], XmNvalue, 1);             n++;                 
 
    iw_its[2] = XmCreateScale(iw_rowcolr, "iw_scale", args, n);

    XtSetArg(args[n-1], XmNvalue, nrow);       
    iw_its[3] = XmCreateScale(iw_rowcolr, "iw_scale", args, n);
    }

 else if (nrow > 0 && nrow != nrowo)
    { /* alter scale size in existing widget */
    XtSetArg(args[0], XmNmaximum, nrow);       
       
    XtSetArg(args[1], XmNvalue, 1);
    XtSetValues(iw_its[2], args, 2);

    XtSetArg(args[1], XmNvalue, nrow);
    XtSetValues(iw_its[3], args, 2);
    }


  if (iw_its[4] == (void *)0 && nslice > 0) 
    {  /* Must create new slice widgets ----------------------------- */

    iw_rowcols  = wid_rowcol(iw_rowcol,'h',-1,-1);
    iw_lab3     = wid_labelg(iw_rowcols, 0, "SLICES:", -1, -1);

    /* create scale widget for starting slice */
    n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb);    n++;
#endif
    XtSetArg(args[n], XmNwidth, iwid);                 n++;                   
    XtSetArg(args[n], XmNheight, ihi);                 n++;               
    XtSetArg(args[n], XmNscaleWidth, iwid-10);         n++;           
    XtSetArg(args[n], XmNscaleHeight, ihi/2);          n++;       
    XtSetArg(args[n], XmNminimum, 1);                  n++;              
    XtSetArg(args[n], XmNmaximum, nslice);             n++;            
    XtSetArg(args[n], XmNdecimalPoints, 0);            n++;          
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL);   n++;      
    XtSetArg(args[n], XmNshowValue, TRUE);             n++;          
    XtSetArg(args[n], XmNvalue, 1);                    n++;                 
    iw_its[4] = XmCreateScale(iw_rowcols, "iw_scale", args, n);

    XtSetArg(args[n-1], XmNvalue, nslice);       
    iw_its[5] = XmCreateScale(iw_rowcols, "iw_scale", args, n);
    }

 else if (nslice > 0 && nslice != nsliceo)
    { /* Alter scale size in existing widget */
    XtSetArg(args[0], XmNmaximum, nslice); 
      
    XtSetArg(args[1], XmNvalue, 1);       
    XtSetValues(iw_its[4], args, 2);

    XtSetArg(args[1], XmNvalue, nslice);       
    XtSetValues(iw_its[5], args, 2);
    }

 if (nsam > 0)
    {
    XtManageChild(iw_its[0]);
    XtManageChild(iw_its[1]);
    }
 if (nrow > 0)
    {
    XtManageChild(iw_its[2]);
    XtManageChild(iw_its[3]);
    }
 if (nslice > 0)
    {
    XtManageChild(iw_its[4]);
    XtManageChild(iw_its[5]);
    }

 nsamo   = nsam;
 nrowo   = nrow;
 nsliceo = nslice;
 }
