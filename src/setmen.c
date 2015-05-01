
/*$Header: /usr8/web/src/RCS/setmen.c,v 1.2 2005/10/18 17:00:01 leith Exp $*/

/*
C++*********************************************************************
C
C setmen
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
C    setmen
C
C    PURPOSE:         display setmen menu
C
C    CALLED BY:        
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
   filtermen -->  setmen --> set -->
                 


C***********************************************************************
*/

#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"

#include "std.h"
#ifndef VMS
#include "float.h"
#endif

 int           filset  (float *, int, int, float, float,
                     float *, float *, float *);
 void          setmen       (Widget, XtPointer, XtPointer);
 void          setmen_buta  (Widget, XtPointer, XtPointer);

 /* internal function prototypes */
 void          filter_but    (Widget, XtPointer, XtPointer);

 /* external global variables used here */
 extern float    fminold, fmaxold;
 static float    oldset,newset;

 /* common global used elsewhere */

 /* common variables used here */
 static Widget   iw_setmen,  iw_oldset, iw_newset;

 /******************************  filset *****************************/

 int filset(float * oldimage, int nsamt, int nrowt, 
            float oldsett, float newsett,
             float * newimage, float * fmint, float * fmaxt)
 {
 int     iloc;
 float   fval;

  * fmint = MYMAX(fmaxold,newsett);
  * fmaxt = MYMIN(fminold,newsett);

 /* convolve over all positions in the new image */

 for (iloc = 0; iloc < nsamt * nrowt; iloc++)
     {
     fval = oldimage[iloc];
     if (fval == oldsett)
        {
        newimage[iloc] = newsett;
        * fmint = MYMIN(*fmint,newsett);
        * fmaxt = MYMAX(*fmaxt,newsett);
        }
     else
        {
        newimage[iloc] = fval;
        * fmint = MYMIN(*fmint,fval);
        * fmaxt = MYMAX(*fmaxt,fval);
        }
     }

  return TRUE;
  }
/************************************  setmen **********************/

 void setmen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval[10];
 Widget   iw_rowcol;
 Widget   iw_pushc, iw_pusha, iw_dums;

 if (iw_setmen <= (Widget)0)
    {   /* create set menu widget first */

    iw_setmen = wid_dialog(iw_win,iw_setmen,"set options",20,20);
    iw_rowcol   = wid_rowcol(iw_setmen,'v',-1,-1);

    /* create text box for old value input ---------------------- old */
    sprintf(cval,"%7.2f",oldset);
    iw_oldset = wid_textboxb(iw_rowcol,0,"Old value:",cval,10);

    /* create text box for new value input ---------------------- new */
    sprintf(cval,"%7.2f",newset);
    iw_newset = wid_textboxb(iw_rowcol,0, "New value:",cval,10);

    /* create accept boxes  ---------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    wid_stdbut(iw_rowcol, iw_setmen, 
               &iw_dums, &iw_pushc, &iw_pusha, 
              "CA",fin_cb,fin_cb,setmen_buta, NULL);
    }

 XtManageChild(iw_setmen);
 }

 /********************* set accept button callback *******************/

 void setmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char   filtertype[2];
 char * string;

 string = XmTextGetString(iw_oldset);
 if (sscanf(string,"%f",&oldset) <= 0)  
    {
    spout("*** Illegal value for: Old value");
    XmStringFree((unsigned char *) string);
    return;
    } 

 XmStringFree((unsigned char *) string);

 string = XmTextGetString(iw_newset);
 if (sscanf(string,"%f",&newset) <= 0)  
    {
    spout("*** Illegal value for: New value");
    XmStringFree((unsigned char *) string);
    return;
    } 
/***********************/
 printf(" old & new: %f , %f \n",oldset,newset);
/*****************************/
 XmStringFree((unsigned char *) string);

 XtUnmanageChild(iw_setmen);

 filtertype[0] = 'A';
 filtertype[1] = '\0';
 filter_but(iw_temp, (XtPointer) filtertype, call_data);
 }


