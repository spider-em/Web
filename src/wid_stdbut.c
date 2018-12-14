
/*$Header: /usr16/software/web/src/RCS/wid_stdbut.c,v 1.6 2018/12/07 17:06:44 leith Exp $*/

/*
 C++********************************************************************
 C
 C  wid_stdbut    Created                                   May 91 AL
 C                Converted to C                            Jul 92 AL
 C                Added delete button                       Aug 15 AL
 C
 C *********************************************************************
 C   AUTHOR:  ArDean Leith              
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C**********************************************************************
 C
 C wid_stdbut(iw_parent, iw_pushs, iw_pushc,iw_pusha, type, 
 C            cbs, cbc,  cba, data)
 C
 C wid_stdbut_str(iw_parent, iw_pushs, iw_pushc,iw_pusha, type, 
 C                lab_s,  lab_c,  lab_a, cbs, cbc,  cba, data)
 C
 C PARAMETERS:   iw_parent     Parent for this box
 C               iw_pushs      Stop button widget   
 C               iw_pushc      Cancel button widget
 C               iw_pusha      Accept button widget                 
 C               type          Type of buttons wanted (SCADFH)
 C               lab_s         Stop   button msg
 C               lab_c         Cancel button msg
 C               lab_a         Accept button msg
 C               cbs           Stop button callback
 C               cba           Accept button callback
 C               cbc           Cancel button callback
 C               data          Accept button data
 C                  
 C*********************************************************************
*/

#include "x.h"
#include "std.h"
#include "routines.h"

 /* Function prototypes */

/******************************** wid_stdbut_str *******************/

 Widget wid_stdbut_str(Widget iw_parent, Widget iw_bigparent,
                   Widget *iw_pushs, Widget *iw_pushc,  Widget *iw_pusha,
                   char * type,
                   char * lab_s,  char * lab_c,  char * lab_a, 
                   XtCallbackProc cbs, XtCallbackProc cbc, XtCallbackProc cba, 
                   void * data)
 { 
 Widget  iw_rowcol, iw_pushh;

 iw_rowcol = wid_rowcol(iw_parent,'h',-1,-1);

 if (strchr(type,'s') != NULL  || strchr(type,'S') != NULL)
    {   /* Make push button for stop */
    *iw_pushs = wid_pushg(iw_rowcol, 0, lab_s, cbs, 
                        (char *) iw_bigparent, 0,0);
    }

 if (strchr(type,'c') != NULL  || strchr(type,'C') != NULL)
    {   /* Make a push button for cancel */
    *iw_pushc = wid_pushg(iw_rowcol, 0, lab_c, cbc,
                        (char *) iw_bigparent ,0,0);
    }

 if (strchr(type,'a') != NULL || strchr(type,'A') != NULL)
    {   /* Make push button for accept */
    *iw_pusha = wid_pushg(iw_rowcol, 0, lab_a, cba, 
                         data, 0,0);
     }

 return  iw_rowcol;
 }



/******************************** wid_stdbut *******************/

 Widget wid_stdbut(Widget iw_parent, Widget iw_bigparent,
                   Widget *iw_pushs, Widget *iw_pushc,  Widget *iw_pusha,
                   char * type,
                   XtCallbackProc cbs, XtCallbackProc cbc, XtCallbackProc cba, 
                   void * data)
 { 
 Widget  iw_rowcol, iw_pushh;

 iw_rowcol  = wid_rowcol(iw_parent,'h',-1,-1);

 if (strchr(type,'s') != NULL  || strchr(type,'S') != NULL)
    {   /* Make push button for stop */
    *iw_pushs = wid_pushg(iw_rowcol, 0, "   STOP   ",   cbs, 
                        (char *) iw_bigparent, 0,0);
    }
         /** printf("Created iw_pushs \n"); */

 if (strchr(type,'k') != NULL  || strchr(type,'K') != NULL)
    {   /* Make push button for save file (uses callback s !!) */
    *iw_pushs = wid_pushg(iw_rowcol, 0, "   SAVE   ",   cbs, 
                        (char *) iw_bigparent, 0,0);
    }

 if (strchr(type,'c') != NULL  || strchr(type,'C') != NULL)
    {   /* Make a push button for cancel */
    *iw_pushc = wid_pushg(iw_rowcol, 0, "  CANCEL  ",   cbc,
                        (char *) iw_bigparent ,0,0);
    }

 if (strchr(type,'a') != NULL || strchr(type,'A') != NULL)
    {   /* Make push button for accept */
    *iw_pusha = wid_pushg(iw_rowcol, 0, "  ACCEPT  ",   cba, 
                         data, 0,0);
     }

 if (strchr(type,'d') != NULL || strchr(type,'D') != NULL)
    {   /* Make push button for delete  (uses callback a !!) */
    *iw_pusha = wid_pushg(iw_rowcol, 0, "  DELETE  ",   cba,
                        (char *) iw_bigparent ,0,0);
    }
 if (strchr(type,'f') != NULL || strchr(type,'F') != NULL)
    {   /* Make push button for finished (uses callback a !!) */
    *iw_pusha = wid_pushg(iw_rowcol, 0, " FINISHED ", cba, 
                        (char *) iw_bigparent, 0,0);
    }

 if (strchr(type,'h') != NULL || strchr(type,'H') != NULL)
    {   /* Make push button for help (uses fixed callback web_man !!) */
    iw_pushh = wid_pushg(iw_rowcol, 0, " HELP ", web_man, 
                        data, 0, 0);
    }

 return  iw_rowcol;
 }
