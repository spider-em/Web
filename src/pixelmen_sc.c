
/*$Header: /usr8/web/src/RCS/pixelmen_sc.c,v 1.23 2015/07/27 16:23:57 leith Exp $*/

/*
 C**********************************************************************
 C
 C PIXELMEN -- Created                           MAY 91   ArDean Leith 
 C             Converted to C                    Oct 92   ArDean Leith      
 C             24 bit display bug fixed          11/8/07  ArDean Leith
 C             Pixel rewrite                     Jun 2011 ArDean Leith
 C             Doc file close bug                Jul 2015 ArDean Leith
 C *********************************************************************
 C *  AUTHOR:  ArDean Leith                                            *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C *********************************************************************
 C
 C  pixelmen
 C
 C  PURPOSE:     Set various parameters relating to pixel reading
 C               Not for pixels within a file
 C
 C  CALLED BY:   web_com
 C      
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#define  MAXREGM1 6

 // Internal functions 
 void          pixelmen_sc_buta(Widget, XtPointer, XtPointer);
 void          pixelmen_sc_buts(Widget, XtPointer, XtPointer);
 void          pixelmen_sc_l   (Widget, XtPointer, XtPointer);
 void          pixelmen_sc_d   (Widget, XtPointer, XtPointer);
 void          pixelmen_sc_s   (Widget, XtPointer, XtPointer);

 // Common variables defined here
 int           ixreg_pix    = 1, iyreg_pix = 2;
 int           isreg_pix    = 3, ivreg_pix = 4;
 int           iradius_pix  = 5;

 // Externally defined variables used here
 extern FILE   * fpdocpix_sc;             // From pixel_sc

 // Common variables defined here
 XImage     *  imagep_sc    = 0;
 int           docit_sc     = FALSE;
 int           getscreen_sc = TRUE;
 int           leavit_sc    = TRUE;
 int           ikey_sc      = 1;

 Widget        iw_radius_sc_nod, iw_getscreen_sc_nod;
 Widget        iw_docit_sc_nod;

 Widget        iw_doc_sc_doc,  iw_key_sc_doc; 
 Widget        iw_xreg_sc_doc, iw_yreg_sc_doc;
 Widget        iw_sreg_sc_doc;

 Widget        iw_radius_sc_doc, iw_getscreen_sc_doc;
 Widget        iw_docit_sc_doc;

 // Static file scope variables 
 static Widget  iw_pixelmen_sc_nod = (Widget)0;
 static Widget  iw_pixelmen_sc_doc = (Widget)0;

 /************************* pixelmen_sc_nod **********************************/

 void pixelmen_sc_nod(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval[5];
 static   Widget   iw_rowcol, iw_rowcol1;
 static   Widget   iw_pushc, iw_pusha, iw_pushs;
 static   Widget   iw_dum = 0;

 char   * cdum = 0;        // Data for a callback is unused here 


 if (iw_pixelmen_sc_nod == (Widget)0)
    {   // Create pixel menu widget first

    iw_pixelmen_sc_nod  = wid_dialog(iw_win,iw_pixelmen_sc_nod,
                             "pixel info options",20,20);

    iw_rowcol           = wid_rowcol(iw_pixelmen_sc_nod,
                                     'v',-1,-1);

    iw_radius_sc_nod    = (Widget) 0;
    iw_getscreen_sc_nod = (Widget) 0;
    iw_docit_sc_nod     = (Widget) 0;

    iw_pushc            = (Widget) 0;   
    iw_pusha            = (Widget) 0;
    iw_pushs            = (Widget) 0;   
    }

 /* Create text box for radius input  --------------------- radius */
 sprintf(cval,"%4d",iradius_pix);
 iw_radius_sc_nod = wid_textboxb(iw_rowcol,
                              iw_radius_sc_nod,
                              "Marker radius:",cval,4);

 /* Create toggle box for getscreen --------------------- getscreen */
 iw_getscreen_sc_nod = wid_toggleg(iw_rowcol,
                             iw_getscreen_sc_nod,
                             "Get screen value",
                             getscreen_sc, pixelmen_sc_s,
                             "XXX",0,0);

 /* Create toggle box for docit ---------------------------- docit */
 iw_docit_sc_nod = wid_toggleg(iw_rowcol,
                            iw_docit_sc_nod,
                            "Save selections in doc. file",
                            docit_sc, pixelmen_sc_d,
                            "XXX",-1,-1);

 // Create accept boxes  ------------------------------------- apply 
 if (iw_dum  == (Widget) 0) 
    iw_dum    = wid_stdbut(iw_rowcol, iw_pixelmen_sc_nod, 
                          &iw_pushs, &iw_pushc, &iw_pusha, 
                          "SA",
                          pixelmen_sc_buts,
                          fin_cb,
                          pixelmen_sc_buta, "XXX");

 XtManageChild(iw_pixelmen_sc_nod);
 }


 /************************* pixelmen_sc_doc *******************************/

 void pixelmen_sc_doc(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval[5];
 static   Widget   iw_rowcol, iw_rowcol1;
 static   Widget   iw_pushc,  iw_pusha, iw_pushs;
 static   Widget   iw_dum = 0;

 char   * cdum = 0;           // Data for a callback is unused here 

 if (iw_pixelmen_sc_doc == (Widget)0)
    {   // Create pixel menu widget first

    iw_pixelmen_sc_doc  = wid_dialog(iw_win,iw_pixelmen_sc_doc,
                             "pixel info options",20,20);

    iw_rowcol           = wid_rowcol(iw_pixelmen_sc_doc,
                                   'v',-1,-1);

    iw_radius_sc_doc    = (Widget) 0;
    iw_getscreen_sc_doc = (Widget) 0;
    iw_docit_sc_doc     = (Widget) 0;
    iw_doc_sc_doc       = (Widget) 0;   

    iw_key_sc_doc       = (Widget) 0;   
    iw_xreg_sc_doc      = (Widget) 0;
    iw_yreg_sc_doc      = (Widget) 0;   
    iw_sreg_sc_doc      = (Widget) 0;

    iw_rowcol1          = (Widget) 0;
    iw_pushc            = (Widget) 0;   
    iw_pusha            = (Widget) 0;
    iw_pushs            = (Widget) 0;   
    }

 /* Create text box for radius input  -------------------- radius */
 sprintf(cval,"%4d",iradius_pix);
 iw_radius_sc_doc = wid_textboxb(iw_rowcol,
                                iw_radius_sc_doc,
                                "Marker radius:",cval,4);

 /* Create toggle box for getscreen -------------------- getscreen */
 iw_getscreen_sc_doc = wid_toggleg(iw_rowcol,
                                iw_getscreen_sc_doc,
                                "Get screen value",
                                getscreen_sc, pixelmen_sc_s,
                                 "DOC",0,0);

 /* Create toggle box for docit ---------------------------- docit */
 iw_docit_sc_doc = wid_toggleg(iw_rowcol,iw_docit_sc_doc,
                            "Save selections in doc. file",
                            docit_sc, pixelmen_sc_d,
                            "DOC",-1,-1);

 /* Create text box for doc file name input  -------------- docnam */
 iw_doc_sc_doc = wid_textboxb(iw_rowcol,
                            iw_doc_sc_doc,
                            "Doc. File:",docnam,12);

 // Create text box for key number input  ------------------ ikey 
 sprintf(cval,"%4d",ikey_sc);
 iw_key_sc_doc = wid_textboxb(iw_rowcol,
                              iw_key_sc_doc,
                              "Key No.:",cval,4);

 if (iw_rowcol1 == (Widget) 0)
    { iw_rowcol1 = wid_rowcol(iw_rowcol,'h',-1,-1); } 

 // Create text box for X register input  ----------------- ixreg 
 sprintf(cval,"%4d",ixreg_pix);
 iw_xreg_sc_doc = wid_textboxb(iw_rowcol1,
                               iw_xreg_sc_doc,
                               "X Reg.:",cval,4);
      
 // Create text box for Y register input  ----------------- iyreg 
 sprintf(cval,"%4d",iyreg_pix);
 iw_yreg_sc_doc = wid_textboxb(iw_rowcol1,
                               iw_yreg_sc_doc,
                               "Y Reg.:",cval,4);
  
 // Create text box for screen value register input ------- isreg 
 sprintf(cval,"%4d",isreg_pix);
 iw_sreg_sc_doc = wid_textboxb(iw_rowcol,
                               iw_sreg_sc_doc,
                               "Screen Value Reg.:",cval,4);

 // Create accept boxes  ------------------------------------- Apply 
 if (iw_dum == (Widget) 0) 
    iw_dum = wid_stdbut(iw_rowcol, iw_pixelmen_sc_doc, 
                        &iw_pushs, &iw_pushc, &iw_pusha, 
                        "SA",
                        pixelmen_sc_buts,
                        fin_cb,
                        pixelmen_sc_buta, "DOC");

 XtManageChild(iw_pixelmen_sc_doc);
 }

 /************** pixelmen_sc_s (getscreen toggle callback) ***********/

 void pixelmen_sc_s(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 if (!strcmp(data, "DOC"))
    {
    getscreen_sc = XmToggleButtonGadgetGetState(iw_getscreen_sc_doc);

    /* Restart the pixel menu */                       
    XtUnmanageChild(iw_pixelmen_sc_doc);                       
    pixelmen_sc_doc(NULL, NULL, NULL);
    }
 else
    {
    getscreen_sc = XmToggleButtonGadgetGetState(iw_getscreen_sc_nod);

    /* Restart the pixel menu */                       
    XtUnmanageChild(iw_pixelmen_sc_nod);                       
    pixelmen_sc_nod(NULL, NULL, NULL);
    }
 }

 /************** pixelmen_sc_d (docit toggle callback) *************/

 void pixelmen_sc_d(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * stringt;
 int    old_docit;

 old_docit = docit_sc;   // Preserve

 if (!strcmp(data, "DOC"))
    {
    //printf(" doc setting toggle %s \n",data);
    docit_sc = XmToggleButtonGadgetGetState(iw_docit_sc_doc);
    }
 else
    {
    //printf(" non doc setting toggle \n");
    docit_sc = XmToggleButtonGadgetGetState(iw_docit_sc_nod);
    }
                     
 if (!old_docit && docit_sc) 
    {  // Display docit menu instead
    if( iw_pixelmen_sc_nod > 0) XtUnmanageChild(iw_pixelmen_sc_nod); 

    pixelmen_sc_doc(NULL,NULL,NULL);
    ikey_sc = 1;
    }
 else if (  old_docit && ! docit_sc)
    {  // Display non-docit menu instead
    if( iw_pixelmen_sc_doc > 0) XtUnmanageChild(iw_pixelmen_sc_doc); 

    pixelmen_sc_nod(NULL,NULL,NULL);
    ikey_sc = 1;
    }
 }


 /************ Accept button callback *********************************/

 void pixelmen_sc_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * cdum;
 int    iform, iplanes;
 char * stringt;

 /* If last doc file is opened, close it first */
 if (fpdocpix_sc)
    pixelmen_sc_buts(NULL,NULL,NULL);

 if (getscreen_sc)
    {
    // Create new imagep 
    // printf("%d %d   %d %d \n",ixulx,iyulx, iwidex, ihighx); 
   
    iplanes   = pow(2,16) - 1;
    imagep_sc = XGetImage(idispl, imagsav, 0,0,
                          iwidex, ihighx, iplanes, ZPixmap);
    }
 else
    { /* Make sure image structure pointer is NULL */
    imagep_sc = (XImage *) NULL;
    }


 if (docit_sc) 
    {
    stringt = XmTextGetString(iw_doc_sc_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);

    if (strlen(docnam) == 0) 
       {spout("*** Must specify doc file name."); return;}

    /* Get registers */
    if (rdpriw(&ikey_sc, 1,INT_MAX, iw_key_sc_doc, 
             "key",        cdum) == FALSE) return;

    if (rdpriw(&ixreg_pix,1,MAXREGM1,iw_xreg_sc_doc,
             "x register",cdum) == FALSE) return;
    if (rdpriw(&iyreg_pix,1,MAXREGM1,iw_yreg_sc_doc,
             "y register",cdum) == FALSE) return;
    if (ixreg_pix == iyreg_pix) 
           spout("Warning: X & Y registers are same");

    if (getscreen_sc)
       if (rdpriw(&isreg_pix,1,MAXREGM1,iw_sreg_sc_doc,
           "screen value register",cdum) == FALSE) return;
    }

 /* Get radius for marker */
 if (!strcmp(data, "DOC"))
    { if (rdpriw(&iradius_pix,0,1000,iw_radius_sc_doc,
                 "radius",cdum) == FALSE) return;}
 else
    { if (rdpriw(&iradius_pix,0,1000,iw_radius_sc_nod,
                 "radius",cdum) == FALSE) return; }
 leavit_sc = (iradius_pix > 0);              

 if (iw_pixelmen_sc_nod && XtIsManaged(iw_pixelmen_sc_nod)) 
                       XtUnmanageChild(iw_pixelmen_sc_nod);
 if (iw_pixelmen_sc_doc && XtIsManaged(iw_pixelmen_sc_doc)) 
                       XtUnmanageChild(iw_pixelmen_sc_doc);

 // Query pixels with mouse  
 pixel_sc();
 }

 /************ Stop button callback *********************************/

 void pixelmen_sc_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 int    numdoc, nlet;

 if (docit_sc) 
     {  // Using a doc file 

     if (strlen(docnam) != 0) 
        {   /* Increment next doc file name */
        filinc(docnam, docnam, &nlet, &numdoc);
        }

     // Close the doc file 
     if (fpdocpix_sc != NULL) fclose(fpdocpix_sc); 
     fpdocpix_sc = NULL;

     // Set new starting key
     ikey_sc     = 1; 
     }

 if (imagep_sc)
   {    // Free up screen image storage  
   XDestroyImage(imagep_sc);
   imagep_sc = (XImage *) 0;
   }

 // Remove pixelmen menu
 if (iw_pixelmen_sc_nod  && XtIsManaged(iw_pixelmen_sc_nod)) 
            XtUnmanageChild(iw_pixelmen_sc_nod);
 if (iw_pixelmen_sc_doc  && XtIsManaged(iw_pixelmen_sc_doc)) 
            XtUnmanageChild(iw_pixelmen_sc_doc);

 // Remove button assignments
 XtUninstallTranslations(iw_win);

 // Remove button assignment instructions 
 // printf(" removing button assignment instructions \n");
 showbutx("","","",TRUE);  
 }

