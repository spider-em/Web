/*
 * distmen.c---- Converted from wid_distmen.for  by Yanhong Li
 *
 **********************************************************************
 *  AUTHOR:  ArDean Leith                                                 *
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
 **********************************************************************

    PURPOSE:         SET VARIOUS PARAMETERS RELATING TO DISTANCE MEASURING

    VARIABLES:       

    CALLED BY:       WEB_COM
      
 ***********************************************************************
 */

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#include "common.h"
#include "routines.h"
#include "dist.h"

 /* Internal functions */
 static void   distmen_buta(Widget,XtPointer,XtPointer);
 static void   distmen_d   (Widget,XtPointer,XtPointer);

 /* File scope variables */
 static Widget iw_distmen    = (Widget)0,  iw_singledist = (Widget)0;
 static Widget iw_docit      = (Widget)0,  iw_distdoc    = (Widget)0;
 static Widget iw_distinimag = (Widget)0,  iw_distleavit = (Widget)0;
 static Widget iw_distkey    = (Widget)0;

 /* Global variables used here */

 /* Global variables defined here */
 FILE * 	      fpdocpix;

 /************************* distmen **********************************/

 void distmen(Widget iw_temp, XtPointer data,XtPointer call_data)
 { 
 char             cval[5];
 static   Widget  iw_rowcol;
 static   Widget  iw_pushc = 0, iw_pusha = 0, iw_pushs = 0, iw_dum = 0;
 char             * cdum = NULL;

 if (iw_distmen <= (Widget)0)
    {   /* create dist menu widget first */
    iw_distmen = wid_dialog(iw_win,iw_distmen,
                             "Distance options",20,20);
    iw_rowcol   = wid_rowcol(iw_distmen,'v',-1,-1);
    }

 if (strlen(filnow) == 0) dist_inimag = FALSE;
 /* create toggle box for dist_docit -------------------- dist_docit */
 iw_docit = wid_toggleg(iw_rowcol,iw_docit,
            "Save distances in doc. file",
            dist_docit,distmen_d,cdum,-1,-1);

 /* create text box for doc file name input  ---------------- docnam */
 if (dist_docit || iw_distdoc == (Widget) 0)
 iw_distdoc = wid_textboxb(iw_rowcol,iw_distdoc,"Doc. File:",docnam,12);
 if (!dist_docit) XtUnmanageChild(XtParent(iw_distdoc));

 /* create toggle box for dist_inimag ------------------ dist_inimag */
 iw_distinimag = wid_togmen(iw_rowcol,iw_distinimag,"Inside last image",
                         dist_inimag,-1,-1);
 if (strlen(filnow) <= 0)  XtUnmanageChild((iw_distinimag));

 /* create toggle box for dist_leav--------------------- dist_leavit */
 iw_distleavit = wid_togmen(iw_rowcol,iw_distleavit,"Leave lines",
                            dist_leavit, -1,-1);

 /* create text box for key number input  --------------------- ikey */
 sprintf(cval,"%4d",ikey);
 iw_distkey = wid_textboxb(iw_rowcol,iw_distkey,"Key No.:",cval,4);

 /* create toggle box for singledist -------------------- singledist */
 iw_singledist = wid_togmen(iw_rowcol,iw_singledist,"Single distance",
                         singledist,-1,-1);
 if (!dist_docit) 
    { XtUnmanageChild(XtParent(iw_distkey)); }

 /* create accept boxes  ------------------------------------- apply */
 
 if (iw_dum <= (Widget) 0) 
    iw_dum   = wid_stdbut(iw_rowcol, iw_distmen, &iw_pushs, &iw_pushc, &iw_pusha, 
                     "SA",distmen_buts,fin_cb,distmen_buta, NULL);

 XtManageChild(iw_distmen);
 }

 /************** distmen_d (dist_docit toggle callback) *************/

 void distmen_d(Widget iw_temp,XtPointer data,XtPointer call_data)
 {
 dist_docit = XmToggleButtonGadgetGetState(iw_docit);
                       
 if (dist_docit) 
    {strcpy(docnam,XmTextGetString(iw_distdoc));}

 XtUnmanageChild(iw_distmen);                       
 distmen(NULL, NULL, NULL);
 }


 /************ accept button callback *********************************/

 void distmen_buta(Widget iw_temp,XtPointer data,XtPointer call_data)
 {
 char * cdum = NULL;

 dist_inimag = XmToggleButtonGadgetGetState(iw_distinimag);
 if (dist_inimag && strlen(filnow) == 0)
    {
    spout("*** Must display image first.");
    XBell(idispl,50); return;
    }

 dist_docit = XmToggleButtonGadgetGetState(iw_docit);
                   
 if (dist_docit) 
    {
    strcpy(docnam,XmTextGetString(iw_distdoc));
    if (strlen(docnam) == 0) 
       {
       spout("*** Must specify document file name.");
       XBell(idispl,50); return;
       }

    /* get registers */
    if (rdpriw(&ikey, 1,INT_MAX, iw_distkey, "key",cdum) == FALSE) return;

    opendistdoc = TRUE;

    }

 singledist = XmToggleButtonGadgetGetState(iw_singledist);

 XtUnmanageChild(iw_distmen);
 
 dist();
 }

 /************ stop button callback *********************************/

 void distmen_buts(Widget iw_temp,XtPointer data,XtPointer call_data)
 {
 int    numdoc, nlet;

 if (dist_docit) 
     {   /* close the doc file */
     fclose(fpdocpix); 
     ikey = 1; 
     if (strlen(docnam) > 0) 
        {   /* increment next doc file name */
        filinc(docnam, docnam, &nlet, &numdoc);
        }
     }

 /* remove distmen */
 XtUnmanageChild(iw_distmen);  

 /* uninstall translations */
 XtUninstallTranslations(iw_win);

 /* remove button assignment instructions */
 showbutx("","","",TRUE);
 }

