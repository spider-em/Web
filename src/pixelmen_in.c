
/*$Header: /usr8/web/src/RCS/pixelmen_in.c,v 1.24 2015/09/08 18:13:32 leith Exp $*/

/*
 C++********************************************************************
 C
 C pixelmen      Created                         May 91   ArDean Leith
 C               Converted to C                  Oct 92   ArDean Leith     
 C pixelmen_in   Renamed                         Jun 2011 ArDean Leith 
 C               24 bit display bug fixed        11/8/07  ArDean Leith 
 C               Pixel rewrite                   Jun 2011 ArDean Leith
 C               Doc file close bug              Jul 2015 ArDean Leith
 C               Doc file input bug              Sep 2015 ArDean Leith
 C
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
 C   pixelmen_in_nod
 C   pixelmen_in_doc
 C
 C   PURPOSE:    Set various parameters relating to pixel reading
 C               For use inside displayed image
 C
 C   CALLED BY:  web_com
 C      
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#define  MAXREGM1 6

 /* Internal functions */
 void          pixelmen_in_buta(Widget, XtPointer, XtPointer);
 void          pixelmen_in_buts(Widget, XtPointer, XtPointer);
 void          pixelmen_in_l   (Widget, XtPointer, XtPointer);
 void          pixelmen_in_d   (Widget, XtPointer, XtPointer);
 void          pixelmen_in_sh  (Widget, XtPointer, XtPointer);
 void          pixelmen_in_s   (Widget, XtPointer, XtPointer);

 /* Externally defined  common variables used here */
 extern int    ixreg_pix, iyreg_pix;    // From pixelmen_sc
 extern int    isreg_pix, ivreg_pix;    // From pixelmen_sc
 extern int    iradius_pix;

 extern FILE * fpdocpix_in;             // From pixel_in

 // Common variables defined here
 XImage *      imagep_in    = 0;
 int           docit_in     = FALSE;
 int           getscreen_in = TRUE;
 int           leavit_in    = TRUE;
 int           ikey_in      = 1;

 Widget        iw_radius_in_nod, iw_getscreen_in_nod;
 Widget        iw_docit_in_nod,  iw_shimg_in_nod;

 Widget        iw_doc_in_doc,    iw_key_in_doc; 
 Widget        iw_xreg_in_doc,   iw_yreg_in_doc; 
 Widget        iw_sreg_in_doc,   iw_vreg_in_doc;

 Widget        iw_radius_in_doc, iw_getscreen_in_doc;
 Widget        iw_docit_in_doc,  iw_shimg_in_doc;

 FILEDATA *    filedatap;

 /* Static file scope variables */
 static Widget  iw_pixelmen_in_nod = (Widget)0;
 static Widget  iw_pixelmen_in_doc = (Widget)0;

 /************************* pixelmen_in_nod **********************************/

 void pixelmen_in_nod()
 { 
 char            cval[5];
 static Widget   iw_rowcol;
 static Widget   iw_pushc, iw_pusha, iw_pushs;
 static Widget   iw_dum = 0;

 char *          cdum  = 0;    /* Data for a callback is unused here */

 if (iw_pixelmen_in_nod == (Widget)0)
    {   /* Create pixel menu widget first */

    iw_pixelmen_in_nod = wid_dialog(iw_win,iw_pixelmen_in_nod,
                              "pixel info options",20,20);

    iw_rowcol           = wid_rowcol(iw_pixelmen_in_nod,
                               'v',-1,-1);

    iw_radius_in_nod    = (Widget) 0;
    iw_getscreen_in_nod = (Widget) 0;
    iw_docit_in_nod     = (Widget) 0;
    iw_shimg_in_nod     = (Widget) 0;

    iw_pushc            = (Widget) 0;   
    iw_pusha            = (Widget) 0;
    iw_pushs            = (Widget) 0;   

    }

 /* Create text box for radius input  -------------------- radius */
 sprintf(cval,"%4d",iradius_pix);
 iw_radius_in_nod = wid_textboxb(iw_rowcol,
                                iw_radius_in_nod,
                                "Marker Radius:",cval,4);

 /* Create button for shift image ------------------------ shimg */
 iw_shimg_in_nod = wid_pushg(iw_rowcol,
                            iw_shimg_in_nod,
                            "Shift the last image",
                            pixelmen_in_sh,
                            (char *)iw_shimg_in_nod,0,0);

 
 /* Create toggle box for getscreen ---------------------- getscreen */
 iw_getscreen_in_nod = wid_toggleg(iw_rowcol,
                      iw_getscreen_in_nod,
                      "Get screen value",
                      getscreen_in, pixelmen_in_s,
                      "XXX",0,0);

 /* Create toggle box for docit -------------------------- docit */
 iw_docit_in_nod = wid_toggleg(iw_rowcol,
                       iw_docit_in_nod,
                       "Save selections in doc. file",
                       docit_in, pixelmen_in_d,
                       "XXX",-1,-1);

 /* Create accept boxes  --------------------------------- apply */
 if (iw_dum  == (Widget) 0) 
    iw_dum  = wid_stdbut(iw_rowcol , iw_pixelmen_in_nod, 
                        &iw_pushs, &iw_pushc, &iw_pusha, 
                        "SA",
                        pixelmen_in_buts,
                        fin_cb,
                        pixelmen_in_buta, "XXX");

 XtManageChild(iw_pixelmen_in_nod);
 }


 /************************* pixelmen_in_doc **********************************/

 void pixelmen_in_doc(Widget iw_temp, XtPointer data, 
                                      XtPointer call_data)
 { 
 char     cval[5];
 static   Widget   iw_rowcol, iw_rowcol1, iw_rowcol2;
 static   Widget   iw_pushc,  iw_pusha,   iw_pushs;
 static   Widget   iw_dum = 0;

 char   * cdum = 0;       /* Data for a callback is unused here */

 if (iw_pixelmen_in_doc == (Widget)0)
    {   /* Create pixel menu widget first */

    iw_pixelmen_in_doc = wid_dialog(iw_win,iw_pixelmen_in_doc,
                             "pixel info options",20,20);

    iw_rowcol          = wid_rowcol(iw_pixelmen_in_doc,
                                'v',-1,-1);

    iw_radius_in_doc    = (Widget) 0;
    iw_getscreen_in_doc = (Widget) 0;
    iw_docit_in_doc     = (Widget) 0;
    iw_doc_in_doc       = (Widget) 0;   

    iw_key_in_doc       = (Widget) 0;   
    iw_xreg_in_doc      = (Widget) 0;
    iw_yreg_in_doc      = (Widget) 0;   
    iw_sreg_in_doc      = (Widget) 0;
    iw_vreg_in_doc      = (Widget) 0;   

    iw_shimg_in_doc     = (Widget) 0;

    iw_rowcol1          = (Widget) 0;
    iw_rowcol2          = (Widget) 0;
    iw_pushc            = (Widget) 0;   
    iw_pusha            = (Widget) 0;
    iw_pushs            = (Widget) 0;   

    }

  /* Create button for shift image -------------------------- shimg */
 iw_shimg_in_doc  = wid_pushg(iw_rowcol,
                        iw_shimg_in_doc,
                        "Shift the last image",
                         pixelmen_in_sh,
                        (char *)iw_shimg_in_doc,0,0);

 /* Create text box for radius input  ---------------------- radius */
 sprintf(cval,"%4d",iradius_pix);
 iw_radius_in_doc = wid_textboxb(iw_rowcol,
                        iw_radius_in_doc,
                        "Marker radius:",cval,4);

 
 /* Create toggle box for getscreen --------------------- getscreen */
 iw_getscreen_in_doc = wid_toggleg(iw_rowcol,
                        iw_getscreen_in_doc,
                        "Get screen value", getscreen_in,
                        pixelmen_in_s,cdum,0,0);

 /* Create toggle box for docit ----------------------------- docit */
 iw_docit_in_doc = wid_toggleg(iw_rowcol,
                        iw_docit_in_doc,
                        "Save selections in doc. file",
                        docit_in, pixelmen_in_d,
                        "DOC",-1,-1);

 /* Create text box for doc file name input  --------------- docnam */
 iw_doc_in_doc = wid_textboxb(iw_rowcol,
                         iw_doc_in_doc,
                         "Doc. File:",docnam,12);

 /* Create text box for key number input  -------------------- ikey */
 sprintf(cval,"%4d",ikey_in);
 iw_key_in_doc = wid_textboxb(iw_rowcol,
                              iw_key_in_doc,
                              "Key No.:",cval,4);

 if (iw_rowcol1 == 0)
    { iw_rowcol1 = wid_rowcol(iw_rowcol,'h',-1,-1); }

 /* Create text box for X register input  ------------------ ixreg */
 sprintf(cval,"%4d",ixreg_pix);
 iw_xreg_in_doc = wid_textboxb(iw_rowcol1,
                              iw_xreg_in_doc,
                              "X Reg.:",cval,4);
      
 /* Create text box for Y register input  ------------------ iyreg */
 sprintf(cval,"%4d",iyreg_pix);
 iw_yreg_in_doc = wid_textboxb(iw_rowcol1,
                               iw_yreg_in_doc,
                               "Y Reg.:",cval,4);
  
 if (iw_rowcol2 == 0) 
    {iw_rowcol2 = wid_rowcol(iw_rowcol,'h',-1,-1);}

 /* Create text box for file value register input ---------- ivreg */
 sprintf(cval,"%4d",ivreg_pix);
 iw_vreg_in_doc = wid_textboxb(iw_rowcol2,
                           iw_vreg_in_doc,
                           "File Value Reg.:",cval,4);

 /* Create text box for screen value register input - isreg */
 sprintf(cval,"%4d",isreg_pix);
 iw_sreg_in_doc = wid_textboxb(iw_rowcol2,
                           iw_sreg_in_doc,
                           "Screen Value Reg.:",cval,4);

 /* Create accept boxes  ---------------------------------- apply */
 
 if (iw_dum == (Widget) 0) 
    iw_dum   = wid_stdbut(iw_rowcol, iw_pixelmen_in_doc, 
                 &iw_pushs, &iw_pushc, &iw_pusha, 
                 "SA",
                 pixelmen_in_buts,
                 fin_cb,
                 pixelmen_in_buta, "DOC");

 XtManageChild(iw_pixelmen_in_doc);
 }

/************** pixelmen_in_s (getscreen toggle callback) ***********/

 void pixelmen_in_s(Widget iw_temp, XtPointer data, 
                                    XtPointer call_data)
 {
 if (!strcmp(data, "DOC"))
    {
    getscreen_in = XmToggleButtonGadgetGetState(iw_getscreen_in_doc);

    /* Restart the pixel menu */                       
    XtUnmanageChild(iw_pixelmen_in_doc);                       
    pixelmen_in_doc(NULL, NULL, NULL);
    }
 else
    {
    getscreen_in = XmToggleButtonGadgetGetState(iw_getscreen_in_nod);

    /* Restart the pixel menu */                       
    XtUnmanageChild(iw_pixelmen_in_nod);                       
    pixelmen_in_nod();
    }
 }

/************** pixelmen_d (docit toggle callback) *************/

 void pixelmen_in_d(Widget iw_temp, XtPointer data, 
                                    XtPointer call_data)
 {
 char * stringt;
 int    old_docit;

 old_docit = docit_in;   // Preserve

 if (!strcmp(data, "DOC"))
    {
    //printf(" doc setting toggle %s \n",data);
    docit_in = XmToggleButtonGadgetGetState(iw_docit_in_doc);
    }
 else
    {
    //printf(" non doc setting toggle \n");
    docit_in = XmToggleButtonGadgetGetState(iw_docit_in_nod);
    }
      
 if (!old_docit && docit_in) 
    {  // Display docit menu instead
    if( iw_pixelmen_in_nod > 0) XtUnmanageChild(iw_pixelmen_in_nod); 

        pixelmen_in_doc(NULL, NULL, NULL);
    ikey_in = 1;
    }

 else if (  old_docit && ! docit_in)
    {  // Display non-docit menu instead
    if( iw_pixelmen_in_doc > 0) XtUnmanageChild(iw_pixelmen_in_doc); 

    pixelmen_in_nod();
    ikey_in = 1;
    }
 }

 /************** pixelmen_in_sh (shimg toggle callback) *************/

 void pixelmen_in_sh(Widget iw_temp, XtPointer data, 
                                     XtPointer call_data)
{
 if (!strcmp(data, "DOC"))
    {
    XtUnmanageChild(iw_pixelmen_in_doc);                       
    shift(PIXEL);

    //printf(" doc setting toggle %s \n",data);
    docit = XmToggleButtonGadgetGetState(iw_docit_in_doc);
    }
 else
    {
    XtUnmanageChild(iw_pixelmen_in_nod);                       
    shift(PIXEL);

    //printf(" non doc setting toggle \n");
    docit = XmToggleButtonGadgetGetState(iw_docit_in_nod);
    }
 }
 

 /************ Accept button callback *********************************/

 void pixelmen_in_buta(Widget iw_temp, XtPointer data, 
                                       XtPointer call_data)
 {

 char * cdum;
 int    iform, iplanes;
 char * stringt;

 /* If last doc file is opened, close it first */
 if (fpdocpix_in)
    pixelmen_in_buts(NULL,NULL,NULL);

  /* Destroy any old image structure */
 if (imagep_in) 
    {
    XDestroyImage(imagep_in);
    imagep_in = NULL;
    }

 if (getscreen_in)
    {
    /* Create new imagep */
    /* printf("%d %d %d %d \n",ixulx,iyulx, iwidex, ihighx-iyulx); */
   
    iplanes   = pow(2,16) - 1;
    imagep_in = XGetImage(idispl, imagsav, 0,0,
                          iwidex, ihighx, iplanes, ZPixmap);
    }
 else
    { /* Make sure image structure pointer is NULL */
    imagep_in    = (XImage *) NULL;
    }


 if (docit_in) 
    {
    stringt = XmTextGetString(iw_doc_in_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);

    if (strlen(docnam) == 0) 
       { spout("*** Must specify doc file name."); XBell(idispl,50);  return; }

    /* Get registers */
    if (rdpriw(&ikey_in, 1,INT_MAX,  iw_key_in_doc, 
           "key",       cdum) == FALSE) return;
    if (rdpriw(&ixreg_pix,1,MAXREGM1,iw_xreg_in_doc,
           "x register",cdum) == FALSE) return;
    if (rdpriw(&iyreg_pix,1,MAXREGM1,iw_yreg_in_doc,
           "y register",cdum) == FALSE) return;
    if (rdpriw(&ivreg_pix,1,MAXREGM1,iw_vreg_in_doc,
           "file value register",cdum) == FALSE) return;
    if (ixreg_pix == iyreg_pix) spout("Warning: X & Y registers are same");

    if (getscreen_in)
       if (rdpriw(&isreg_pix,1,MAXREGM1,iw_sreg_in_doc, 
           "screen value register",cdum) == FALSE) return;
    }

 if (strlen(filnow) == 0)
    { spout("*** Must display image to find file value for pixel."); XBell(idispl,50);  return; }

 if ((filedatap = openold(filnow,&nsam,&nrow,&nslice,&iform,"o")) 
       ==  NULL) 
    { spout("*** Can not open image to find file value for pixel."); XBell(idispl,50);  return; }


 /* Get radius for marker */
 if (!strcmp(data, "DOC"))
    { if (rdpriw(&iradius_pix,0,1000,iw_radius_in_doc,
                 "radius",cdum) == FALSE) return;}
 else
    { if (rdpriw(&iradius_pix,0,1000,iw_radius_in_nod,
                 "radius",cdum) == FALSE) return; }
 leavit_in =  (iradius_pix > 0);              
               
 if (iw_pixelmen_in_nod && XtIsManaged(iw_pixelmen_in_nod)) 
                       XtUnmanageChild(iw_pixelmen_in_nod);
 if (iw_pixelmen_in_doc && XtIsManaged(iw_pixelmen_in_doc)) 
                       XtUnmanageChild(iw_pixelmen_in_doc);

 pixel_in();
 }

 /************ Stop button callback *********************************/

 void pixelmen_in_buts(Widget iw_temp, XtPointer data, 
                                       XtPointer call_data)
 {

 int    numdoc, nlet;

  if (docit_in) 
     {   /* Using a doc file */

     if (strlen(docnam) != 0) 
        {   /* Increment next doc file name */
        filinc(docnam, docnam, &nlet, &numdoc);
        }

     // Close the doc file 
     if (fpdocpix_in != NULL) fclose(fpdocpix_in); 
     fpdocpix_in = NULL;

     // Set new starting key
     ikey_in = 1; 
     }

 if (imagep_in)
   {    /* Free up image storage */
   XDestroyImage(imagep_in);
   imagep_in = (XImage *) 0;
   }

 /* Remove the pixelmen */
 if (iw_pixelmen_in_nod && XtIsManaged(iw_pixelmen_in_nod)) 
                       XtUnmanageChild(iw_pixelmen_in_nod);
 if (iw_pixelmen_in_doc && XtIsManaged(iw_pixelmen_in_doc)) 
                       XtUnmanageChild(iw_pixelmen_in_doc);

 /* Close the image file, if opened here */
 if (filedatap != NULL && filedatap->fp != NULL) closefilep(&filedatap); 

 // Remove button assignments
 XtUninstallTranslations(iw_win);

 /* Remove button assignment instructions */
 showbutx("","","",TRUE); 

 }

