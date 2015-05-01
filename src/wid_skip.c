
/*$Header: /usr8/web/new/RCS/wid_skip.c,v 1.3 2011/06/23 14:29:16 leith Exp $*/
/*
C++*********************************************************************
C
C wid_skip.c                                      ArDean Leith  May 2011
C **********************************************************************
C    AUTHOR:  ArDean Leith
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
C  wid_skip
C
C  PARAMETERS:    
C
C  PURPOSE:    Displays skip selecting widget
C
C--*********************************************************************
*/

#include "common.h"
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

 /* External function prototypes */
 extern Widget wid_rowcol   (Widget, char , int, int);
 extern Widget wid_pushg    (Widget, Widget, char *, void (*) (), 
                             char *,int,int);
 extern void   spout        (char *);

 /* Internal function prototypes */
 void     wid_skip_cb   (Widget, XtPointer, XtPointer);

 // File scope variables
 static  Widget  iw_skip = (Widget) 0;

 // ***********************   wid_skip   ****************************
  
 Widget wid_skip(Widget iw_parent, char * label,  int ix, int iy)

 { 

 if (iw_skip <= (Widget) 0)
    {   // Create skip widget first 
 
    iw_skip  = wid_rowcol(iw_parent, 'h', -1, -1);

    wid_labelg(iw_skip, (Widget)0, label, 0, 0);  // Label

    wid_pushg(iw_skip, (Widget)0, "1/5", wid_skip_cb, " 5", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "1/4", wid_skip_cb, " 4", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "1/3", wid_skip_cb, " 3", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "1/2", wid_skip_cb, " 2", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "1",   wid_skip_cb, " 1", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "2",   wid_skip_cb, "-2", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "3",   wid_skip_cb, "-3", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "4",   wid_skip_cb, "-4", -1,-1);
    wid_pushg(iw_skip, (Widget)0, "5",   wid_skip_cb, "-5", -1,-1);
    XtManageChild(iw_skip);
    }

 XtManageChild(iw_skip);

 return iw_skip;
 }

// ***********************  wid_skip_cb ******************************

 void wid_skip_cb(Widget iw_temp, XtPointer data, 
                                  XtPointer call_data)
 {

 //printf(" Iskip: %2.2s \n",data);

 if (sscanf(data,"%d",&iskip) <= 0)
    { spouts("*** Illegal value for: Image scaling");  }
 return;
 }
        
