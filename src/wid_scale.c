
/*$Header: /usr16/software/web/src/RCS/wid_scale.c,v 1.6 2018/12/07 17:03:35 leith Exp $*/
/*
 C++********************************************************************
 C
 C wid_scale
 C
 C *********************************************************************
 C * AUTHOR:  ArDean Leith                                             *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
C wid_scale
C
C PURPOSE:      Set up a scale widget 
C
C PARAMETERS:   iw_parent         parent widget         read
c               iw_its            widget numbers        write
c               label             label                 read
c               imin,imax         scale min & max       read
c               inow              current value         read
c               iwid,ihi          scale size            read
c               ixdum,iydum       unused
c
C CALLED BY:    surfmen, contrastmen, wid_contrast
C
C***********************************************************************
*/

#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"

#define  MAX_ARGS 12

/*****************  wid_scale  ***************************/

 Widget wid_scale(Widget iw_parent, Widget iw_its, char * label,
              int imin, int imax, int inow,  int iwid, int ihi,
              int ixdum, int iydum)

 {   
 Arg          args[MAX_ARGS];    /* arg list  */
 Widget       iw_lab;
 Widget       iw_rowcolh;
 int          n;

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
    XtSetArg(args[0], XmNwidth,         iwid);                    
    XtSetArg(args[1], XmNheight,        ihi);                
    XtSetArg(args[2], XmNscaleWidth,    iwid-10);            
    XtSetArg(args[3], XmNscaleHeight,   ihi/2);        
    XtSetArg(args[4], XmNminimum,       imin);                
    XtSetArg(args[5], XmNmaximum,       imax);             
    XtSetArg(args[6], XmNdecimalPoints, 0);            
    XtSetArg(args[7], XmNvalue,         inow);                  
    XtSetArg(args[8], XmNorientation,   XmHORIZONTAL);       
    XtSetArg(args[9], XmNshowValue,     TRUE);           
    n = 10;
      
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb); 
    n = 11;      
#endif
 
    iw_its = XmCreateScale(iw_rowcolh, "iw_scale", args, n);
    }

 else 
    { /* Alter scale size in existing widget */

    XtSetArg(args[0], XmNminimum, imin);       
    XtSetArg(args[1], XmNmaximum, imax);       
    XtSetArg(args[2], XmNvalue,   inow);
       
    XtSetValues(iw_its, args, 3);
    }

 XtManageChild(iw_its);
 return iw_its;

 }
