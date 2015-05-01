/*
C++*********************************************************************
C
C STACKMEN -- CREATED Jan 1998 Yanhong Li
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                             *
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
C STACKMEN
C
C PURPOSE:   DISPLAY MENU FOR STACK FILES
C
C VARIABLES:       
C
C CALLED BY:       
C
C***********************************************************************
*/

#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#include "common.h"
#include "routines.h"

/* Internal function prototypes */
 void          stackmen_buta(Widget, XtPointer, XtPointer);
 void          stackmen_butc(Widget, XtPointer, XtPointer);
 void          stackmen_toga(Widget, XtPointer, XtPointer);
 void          stackmen_togb(Widget, XtPointer, XtPointer);
 void          stackmen_togc(Widget, XtPointer, XtPointer);

 /* File variables */
 static Widget   iw_all, iw_single, iw_section;
 static Widget   iw_rowcol1, iw_rowcol2;
 static Widget   iw_single1, iw_stkgo, iw_stkend;
 static Widget   iw_stackmen = (Widget)0, iw_pushc, iw_pusha;

 /* Externally used variables */
 int             stkgo;            /* Used in stack display */
 int             stkend;           /* Used in stack display */
 static int      all=TRUE, single=FALSE, section=FALSE;    

 extern int      montaverage;      /* Want montage average */

 /************************************************************/

 void stackmen(FILEDATA* filedata)

 { 
 Widget iw_rowcol;
 Widget iw_dums;
 char   cval[6],cval1[6];

 if (iw_stackmen == (Widget)0)
    {   /* Create a menu widget first */
    stkgo  = 1;
    stkend = filedata->maxim;

    iw_stackmen = wid_dialog(iw_win, 0, "stack display options", -1, -1);
    iw_rowcol   = wid_rowcol(iw_stackmen, 'v', -1, -1);

    /* Create a toggle box for image selection ------------------ all */
    iw_all = wid_toggleg(iw_rowcol,iw_all,"Display All Images",
	    all,stackmen_toga, NULL, -1,-1);

    /* Create a toggle box for image selection --------------- single */
    iw_single = wid_toggleg(iw_rowcol,iw_single,
	    "Display Single Image",single,stackmen_togb,NULL, -1,-1);

    /* Create text box for single image number  */
    iw_rowcol1   = wid_rowcol(iw_rowcol, 'h', -1, -1);

    sprintf(cval,"%6d",stkgo);
    iw_single1 = wid_textboxb(iw_rowcol1, iw_single1, 
                        "     Image Number: ", cval,6);
    if (!single) 
        {XtUnmanageChild(iw_rowcol1); }

    /* Create a toggle box for image selection -------------- subset */
    iw_section = wid_toggleg(iw_rowcol,iw_section,
	    "Display Subset From Stack",section,
		stackmen_togc, NULL,-1,-1);

    /* Create text boxes for stkgo & stkend  */
    iw_rowcol2   = wid_rowcol(iw_rowcol, 'h', -1, -1);

    sprintf(cval, "%6d",stkgo);
    iw_stkgo  = wid_textboxb(iw_rowcol2,0,"Start",cval,6);
    sprintf(cval,"%6d",stkend);
    iw_stkend = wid_textboxb(iw_rowcol2,0,"End ", cval,6);

    if (!section) 
        { XtUnmanageChild(iw_rowcol2); }

    /* Create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcol, iw_stackmen, &iw_dums, &iw_pushc, 
                        &iw_pusha,  "CA",
                        fin_cb, stackmen_butc ,stackmen_buta, NULL);
    }

 else
    {   /* Stack menu widget already exists, update it */
    /* update text boxes for stkgo & stkend ----------------- section */

    if (!samefile || (!single && !section))
       {
       stkgo  = 1;
       stkend = filedata->maxim;
       }

    if (single)
       {
       sprintf(cval,"%6d",stkgo);
       iw_single1 = wid_textboxb(iw_rowcol,iw_single1,
                                 "Image Number:",cval,6);
       }
    else if (section)
       {
       sprintf(cval, "%6d",stkgo);
       iw_stkgo  = wid_textboxb(iw_rowcol2,iw_stkgo,"Start",cval,6);

       sprintf(cval,"%6d",stkend);
       iw_stkend = wid_textboxb(iw_rowcol2,iw_stkend,"End ", cval,6);
       }
    }
 XtManageChild(iw_stackmen);
 }


 /***********  stackmen_toga  (toga  button callback) ********/

 void stackmen_toga(Widget iw_temp, XtPointer data, XtPointer call_data)
     {
     XmToggleButtonGadgetSetState(iw_all,    TRUE,   FALSE);
     XmToggleButtonGadgetSetState(iw_single, FALSE,  FALSE);
     XmToggleButtonGadgetSetState(iw_section, FALSE, FALSE);

     XtUnmanageChild(iw_rowcol1);
     XtUnmanageChild(iw_rowcol2);
     }

 /***********  stackmen_togb  (togb  button callback) ********/

 void stackmen_togb(Widget iw_temp, XtPointer data, XtPointer call_data)
     {
     XmToggleButtonGadgetSetState(iw_all,     FALSE,  FALSE);
     XmToggleButtonGadgetSetState(iw_single,  TRUE,   FALSE);
     XmToggleButtonGadgetSetState(iw_section, FALSE,  FALSE);

     XtManageChild(iw_rowcol1);
     XtUnmanageChild(iw_rowcol2);
     }

 /***********  stackmen_togc  (togc  button callback) ********/

 void stackmen_togc(Widget iw_temp, XtPointer data, XtPointer call_data)
     {
     XmToggleButtonGadgetSetState(iw_all, FALSE,  FALSE);
     XmToggleButtonGadgetSetState(iw_single, FALSE,  FALSE);
     XmToggleButtonGadgetSetState(iw_section, TRUE,  FALSE);

     XtUnmanageChild(iw_rowcol1);
     XtManageChild(iw_rowcol2);
     }

 /***********  stackmen_buta  (accept button callback) ********/

 void stackmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *string = NULL;
 int  vval;

 /* Get the state of toggle */
 all     = XmToggleButtonGadgetGetState(iw_all);
 single  = XmToggleButtonGadgetGetState(iw_single);
 section = XmToggleButtonGadgetGetState(iw_section);

 /* No question for this in this menu, so default it to FALSE */
 montaverage = FALSE;

 /* Get the num for stkgo and stkend */
 if (single)
    {
    string = XmTextGetString(iw_single1);
     
    /* Find file numbers wanted */
    if (sscanf(string,"%d",&vval) <= 0 || vval < 0)
       {   /* Can not interpret the image number */
       spout("*** Can not parse image number! ");
       if (string) XtFree (string); return;
       }
    stkgo  = vval;
    stkend = vval;
    }
 else if (section) 
    {
    /* Get stkgo */
    string = XmTextGetString(iw_stkgo);

    if (sscanf(string,"%d",&vval) <= 0 || vval < 0)
        {   /* Can not interpret vval */
        spout("*** Can not parse starting image number! ");
        if (string) XtFree (string); return;
        }
    else 
	stkgo = vval;

    /* Get stkend */
    string = XmTextGetString(iw_stkend);
    if (sscanf(string,"%d",&vval) <= 0 || vval < 0)
       {   /* Can not interpret vval */
       spout("*** Can not parse ending image number ending! ");
       if (string) XtFree (string); return;
       }
    else 
       stkend = vval;

    if (string) XtFree (string);
    }

 /*  Remove the stack menu */
 XtUnmanageChild(iw_stackmen);
 expos_cb(NULL,NULL,NULL);

 /*  Display the montage, imagego set in imagemen */
 stackdis(imagego,imageend);
 }

 /***********  stackmen_butc  (Cancel button callback) ********/

 void stackmen_butc(Widget iw_temp, XtPointer data, XtPointer call_data)
    {
    XtUnmanageChild(iw_stackmen);
    closefile(filedata);
    filedata = NULL;
    }

