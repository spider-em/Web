
/*$Header: /usr8/web/src/RCS/wid_scale.c,v 1.5 2011/07/19 18:06:32 leith Exp $*/
/*
C++************************************************************************
C
C wid_scalef
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
C wid_scalef
C
C PURPOSE:      Set up scale widget for real numbers 
C
C PARAMETERS:   iw_parent         Parent widget         Read
c               iw_its            Widget numbers        Write
c               label             Label                 Read
c               imin,imax         Scale min & max       Read
c               inow              Current value         Read
c               iwid,ihi          Scale size            Read
c               scalef, offsetf   Scale factors         Write
c
C CALLED BY:    surfmen
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"
#include <Xm/Scale.h>

#define  MAX_ARGS 12
 
/*****************  wid_scalef  ***************************/

 Widget wid_scalef(Widget iw_parent, Widget iw_its, char * label,
              float fmint,  float fmaxt,   float fnow, 
              int imin,     int imax,    
              int iwid,     int ihi,
              float * scalef, float * offsetf)

 {   
 Arg          args[MAX_ARGS];    /* arg list  */
 Widget       iw_lab;
 Widget       iw_rowcolh;
 int          n;
 char         cval[15];

 // Conversion factor from floating point to integer
 * scal   = (float) (imax - imin) / (fmaxt - fmint);

 *offset = -fmint * (*scal) + imint + 0.5;
 inow   = (int) (fnow * *scal + *offset);
  
 // Create a label and text box for fnow
 sprintf(cval,"%11g",fnow);

 if (iw_its == (void *)0) 
    {  /* Must create new widget  */

    if (strlen(label) > 0)
       {
       /* Need rowcol widget to hold label and scale widget */
       iw_rowcolh  = wid_rowcol(iw_parent,'h',-1,-1);

       /* Create label widget*/
       iw_lab      = wid_labelg(iw_rowcolh, 0, label, -1, -1);
       }
    else
       iw_rowcolh = iw_parent; 

    /* Create scale widget */
    n = 0;
    XtSetArg(args[n], XmNwidth,         iwid);          n++;                 
    XtSetArg(args[n], XmNheight,        ihi);           n++;               
    XtSetArg(args[n], XmNscaleWidth,    iwid-10);       n++;            
    XtSetArg(args[n], XmNscaleHeight,   ihi/2);         n++;       
    XtSetArg(args[n], XmNminimum,       imin);          n++;               
    XtSetArg(args[n], XmNmaximum,       imax);          n++;             
    XtSetArg(args[n], XmNdecimalPoints, 0);             n++;           
    XtSetArg(args[n], XmNvalue,         inow);          n++;                  
    XtSetArg(args[n], XmNorientation,   XmHORIZONTAL);  n++;       
    XtSetArg(args[n], XmNshowValue,     FALSE);          n++;         
    //XtSetArg(args[n], XmNshowValue,     TRUE);          n++;         
      
    Setbgfg(args, MAX_ARGS, &n);  // Handles BGFG Args
 
    iw_its = XmCreateScale(iw_rowcolh, "iw_scale", args, n);

    /* Create a text box for fnow */
    iw_valf = wid_textboxb(iw_rowcolh,iw_valf,"",cval,11);
    }

 else 
    { /* Alter scale size in existing widget */

    n = 0;
    XtSetArg(args[n], XmNminimum, imin);      n++;      
    XtSetArg(args[n], XmNmaximum, imax);      n++;      
    XtSetArg(args[n], XmNvalue,   inow);      n++;
       
    XtSetValues(iw_its, args, n);

    iw_valf = wid_textboxb(iw_rowcolh,iw_valf,"",cval,11);
    }

 XtManageChild(iw_its);
 return iw_its;

 }
