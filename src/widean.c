
/*$Header: /usr8/web/new/RCS/widean.c,v 1.8 2011/05/17 17:29:14 leith Exp $*/
/*
C++*************************************************************************
C
C WID_DEAN.FOR -- CREATED JUNE 91
C widean.c                NOV  92
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
C  widean
C
C  PARAMETERS:    
C
C  PURPOSE:    Displays marker number selecting widget
C
C--*********************************************************************
*/

#include "common.h"
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>


 /* External function prototypes */
 extern Widget wid_rowcol   (Widget, char , int, int);
 extern Widget wid_textboxb (Widget, Widget, char *, char *,  int);
 extern Widget wid_dialog   (Widget, Widget, char *, int, int);
 extern Widget wid_pushg    (Widget, Widget, char *, void (*) (), 
                             char *,int,int);
 extern void   spout        (char *);


 /* Internal function prototypes */
 void     widean_cb   (Widget, caddr_t, caddr_t);

 Widget  iw_deant;
 char    cval[12] = "b0123456789";

 /***********************   widean   ****************************/
  
 Widget widean(Widget iw_parent, Widget iw_dean, 
               char * label, char * string, int ix, int iy, int icol)

 { 
 int    i;
 Widget iw_pb[11], iw_rowcolh;
 char   ctemp[2];

 if (iw_dean <= (Widget)0)
    {   /* Create dean widget first */
 
    iw_dean     = wid_dialog(iw_parent, 0, label, ix, iy);
    iw_rowcolh  = wid_rowcol(iw_dean, 'h', -1, -1);

    ctemp[1] = '\0';
    for (i=0; i< 11; i++)
       {
       strncpy(&ctemp[0],&cval[i],1);
       iw_pb[i] = wid_pushg(iw_rowcolh, (Widget)0, ctemp,
                  widean_cb, &cval[i], -1,-1);
       }

    /*  Create the text input part of the box */
    iw_deant = wid_textboxb(iw_rowcolh,0,"",string,icol);
    }

 XtManageChild(iw_dean);
 return iw_dean;
 }

/***********************  widean_cb ************************/

 void widean_cb(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
 char opert[2];
 int    ip;

 strncpy(opert,data,1);
 opert[1] = '\0'; 

 //spouts("value:"); spout(opert); 
 //printf(" value: %1.1s \n",opert);


 if (*opert == 'b')
    {
    /*      Clear the text box string */
    XmTextSetString(iw_deant,"0");
    }
 else
    {
    /*      Append this digit to text in text box */

    /*      Get present string length */
    ip = (int) XmTextGetLastPosition(iw_deant);

    /*      Find insertion point for new text */ 
    if (*opert != '-' || ip == 0)
       {
       XmTextReplace(iw_deant,ip,ip,opert);            
       }
    }
 return;
 }
        
