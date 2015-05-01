
/*$Header: /usr8/web/src/RCS/wid_stdbut.c,v 1.4 2011/10/21 12:22:00 leith Exp $*/
/*
C++*************************************************************************
C
C  WID_STDBUT -- CREATED MAY 91
C                CONVERTED TO C -- JULY 92 AL
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
C    WID_STDBUT(iw_parent, iw_pushs, iw_pushc,iw_pusha, type, 
C               cbs, cbc,  cba, data)
C
C    PARAMETERS:   iw_parent     Parent for this box
C                  iw_pushs      Stop button widget   
C                  iw_pushc      Cancel button widget
C                  iw_pusha      Accept button widget                 
C                  type          Type of buttons wanted (SCAF)
C                  cbs           Stop button callback
C                  cba           Accept button callback
C                  cbc           Cancel button callback
C                  data          Accept button data
C                  
C--*********************************************************************
*/

#include "x.h"
#include "std.h"

 extern void web_man ( Widget, caddr_t, caddr_t );

 extern Widget wid_pushg  (Widget, Widget, char *, void (*)(), 
                           char *, int, int);
 extern Widget wid_rowcol (Widget, char , int, int);

/************************************************************/

 Widget wid_stdbut(Widget iw_parent, Widget iw_bigparent,
                 Widget *iw_pushs, Widget *iw_pushc,  Widget *iw_pusha,
                 char * type,
                 void (*cbs)(), void (*cbc)(), void (*cba)(), 
                 char * data)
 { 
 Widget  iw_rowcol, iw_pushh;

         /*  printf("Creating iw_rowcol \n"); */

 iw_rowcol  = wid_rowcol(iw_parent,'h',-1,-1);

 if (strchr(type,'s') != NULL  || strchr(type,'S') != NULL)
    {   /* Make a push button for stop */
    *iw_pushs = wid_pushg(iw_rowcol, 0, "   STOP   ",   cbs, 
                        (char *) iw_bigparent, 0,0);
    }
         /** printf("Created iw_pushs \n"); */

 if (strchr(type,'k') != NULL  || strchr(type,'K') != NULL)
    {   /* Make a push button for save file (uses callback s !!) */
    *iw_pushs = wid_pushg(iw_rowcol, 0, "   SAVE   ",   cbs, 
                        (char *) iw_bigparent, 0,0);
    }

 if (strchr(type,'c') != NULL  || strchr(type,'C') != NULL)
    {   /* Make a push button for cancel */
    *iw_pushc = wid_pushg(iw_rowcol, 0, "  CANCEL  ",   cbc,
                        (char *) iw_bigparent ,0,0);
    }

 if (strchr(type,'a') != NULL || strchr(type,'A') != NULL)
    {   /* Make a push button for accept */
    *iw_pusha = wid_pushg(iw_rowcol, 0, "  ACCEPT  ",   cba, 
                         data, 0,0);
     }

 if (strchr(type,'f') != NULL || strchr(type,'F') != NULL)
    {   /* Make a push button for finished (uses callback a !!) */
    *iw_pusha = wid_pushg(iw_rowcol, 0, " FINISHED ", cba, 
                        (char *) iw_bigparent, 0,0);
    }

 if (strchr(type,'h') != NULL || strchr(type,'H') != NULL)
    {   /* Make a push button for help (uses fixed callback web_man !!) */
    iw_pushh = wid_pushg(iw_rowcol, 0, " HELP ", web_man, 
                        data, 0, 0);
    }

 return  iw_rowcol;
 }
