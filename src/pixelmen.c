
/*$Header: /usr8/web/src/RCS/pixelmen.c,v 1.22 2015/07/27 16:23:00 leith Exp $*/

/*
 C++********************************************************************
 C
 C PIXELMEN    Created                          May 91  
 C             Converted to C                   Oct 92      
 C             24 bit display bug fixed         11/8/07  ArDean Leith 
 C             Doc file close bug               Jul 2015 ArDean Leith
 C *********************************************************************
 C * AUTHOR:  ArDean Leith                                             *
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
 C  PIXELMEN
 C
 C  PURPOSE:       Set various parameters relating to pixel reading
 C
 C  VARIABLES:       
 C
 C  CALLED BY:     WEB_COM
 C      
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#define  MAXREGM1 6

 /* Internal functions */
 void          pixelmen_buta(Widget, XtPointer, XtPointer);
 void          pixelmen_buts(Widget, XtPointer, XtPointer);
 void          pixelmen_l(Widget,    XtPointer, XtPointer);
 void          pixelmen_d(Widget,    XtPointer, XtPointer);
 void          pixelmen_sh(Widget,   XtPointer, XtPointer);
 void          pixelmen_s(Widget,    XtPointer, XtPointer);
 void          pixelmen_f(Widget,    XtPointer, XtPointer);
 void          pixelmen_i(Widget,    XtPointer, XtPointer);

 /* Common variables */
 XImage     *  imagep;
 int           ixreg = 1, iyreg = 2;
 int           isreg = 3, ivreg = 4;
 Widget        iw_inimag, iw_leavit, iw_getfile, iw_getscreen, iw_key;
 Widget        iw_xreg,   iw_yreg,   iw_vreg,    iw_sreg,      iw_docit;
 Widget        iw_radius, iw_doc, iw_shimg;
 int           pixelmen_showing = FALSE;
 extern        FILE * fpdocpix_p;
 FILEDATA      *filedatap;

 /* File scope variables */
 static Widget  iw_pixelmen = (Widget)0;
 static int     shimg;

 /************************* pixelmen **********************************/

 void pixelmen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char     cval[5];
 static   Widget   iw_rowcol, iw_rowcol1, iw_rowcol2;
 static   Widget   iw_pushc,  iw_pusha,   iw_pushs,   iw_dum = 0;
 char   * cdum = 0;    /* data for a callback is unused here */

 if (iw_pixelmen == (Widget)0)
    {   /* create pixel menu widget first */

    iw_pixelmen = wid_dialog(iw_win,iw_pixelmen,
                             "pixel information options",20,20);
    iw_rowcol   = wid_rowcol(iw_pixelmen,'v',-1,-1);

    iw_doc     = (Widget) 0;   iw_shimg   = (Widget) 0;
    iw_inimag  = (Widget) 0;   iw_docit   = (Widget) 0;
    iw_leavit  = (Widget) 0;   iw_getfile = (Widget) 0;
    iw_key     = (Widget) 0;   iw_xreg    = (Widget) 0;
    iw_yreg    = (Widget) 0;   iw_sreg    = (Widget) 0;
    iw_vreg    = (Widget) 0;   iw_radius  = (Widget) 0;
    iw_pushc   = (Widget) 0;   iw_pusha   = (Widget) 0;
    iw_pushs   = (Widget) 0;   iw_rowcol1 = (Widget) 0;
    iw_rowcol2 = (Widget) 0;
    imagep     = (XImage *) 0;
    ikey       = 1;
    }

if (strlen(filnow) == 0) inimag = FALSE;
 /* Create toggle box for docit ---------------------------- docit */
 iw_docit = wid_toggleg(iw_rowcol,iw_docit,
           "Save selections in doc. file",
           docit,pixelmen_d,cdum,-1,-1);

 /* Create text box for doc file name input  -------------- docnam */
 if (docit || iw_doc == (Widget) 0)
    iw_doc = wid_textboxb(iw_rowcol,iw_doc,"Doc. File:",docnam,12);
 if (!docit) XtUnmanageChild(XtParent(iw_doc));

 /* Create toggle box for shift image ---------------------- shimg */
 iw_shimg = wid_pushg(iw_rowcol,iw_shimg,"Shift the last image",
                         pixelmen_sh,(char *)iw_shimg,0,0);

 /* Create toggle box for inimag -------------------------- inimag */
 iw_inimag = wid_toggleg(iw_rowcol,iw_inimag,"Inside last image",
                         inimag,pixelmen_i,cdum,-1,-1);
 if (strlen(filnow) <= 0)  XtUnmanageChild((iw_inimag));

 /* Create toggle box for leavit -------------------------- leavit */
 iw_leavit = wid_toggleg(iw_rowcol,iw_leavit,"Leave marker",leavit,
                         pixelmen_l,cdum,-1,-1);

 /* Create toggle box for getfile ------------------------ getfile */
 if (inimag || iw_getfile == (Widget) 0)
    iw_getfile = wid_toggleg(iw_rowcol,iw_getfile,"Get file value",
                  getfile, pixelmen_f,cdum,-1,-1);
 if (!inimag) XtUnmanageChild(iw_getfile);
 
 /* Create toggle box for getscreen ------------------- getscreen */
 iw_getscreen = wid_toggleg(iw_rowcol,iw_getscreen,"Get screen value",
                 getscreen,pixelmen_s,cdum,0,0);

 /* Create text box for key number input  ------------------ ikey */
 sprintf(cval,"%4d",ikey);
 iw_key = wid_textboxb(iw_rowcol,iw_key,"Key No.:",cval,4);

 if (docit || iw_rowcol1 == 0)
    {
    if (iw_rowcol1 == 0) 
        iw_rowcol1 = wid_rowcol(iw_rowcol,'h',-1,-1);
     
    /* create text box for X register input  -------------- ixreg */
    sprintf(cval,"%4d",ixreg);
    iw_xreg = wid_textboxb(iw_rowcol1,iw_xreg,"X Reg.:",cval,4);
      
    /* create text box for Y register input  -------------- iyreg */
    sprintf(cval,"%4d",iyreg);
    iw_yreg = wid_textboxb(iw_rowcol1,iw_yreg,"Y Reg.:",cval,4);
  
    if (iw_rowcol2 == 0) 
        iw_rowcol2 = wid_rowcol(iw_rowcol,'h',-1,-1);

    if (getfile || iw_vreg == (Widget) 0)
       {
       /* create text box for file value register input --- ivreg */
       sprintf(cval,"%4d",ivreg);
       iw_vreg = wid_textboxb(iw_rowcol2,iw_vreg,
                              "File Value Reg.:",cval,4);
       }

    if (getscreen || iw_sreg == (Widget) 0)
       {
       /* Create text box for screen value register input - isreg */
       sprintf(cval,"%4d",isreg);
       iw_sreg = wid_textboxb(iw_rowcol2,iw_sreg,
                              "Screen Value Reg.:",cval,4);
       }
    }

 if (!docit) 
    {
    XtUnmanageChild(XtParent(iw_key));
    XtUnmanageChild(XtParent(iw_xreg));
    XtUnmanageChild(XtParent(iw_yreg));
    ikey = 1;
    }

 if (!docit || !getfile)   XtUnmanageChild(XtParent(iw_vreg));
 if (!docit || !getscreen) XtUnmanageChild(XtParent(iw_sreg));

 if (leavit || iw_radius == 0)
    {    /* Create text box for radius input  ------------ radius */
    sprintf(cval,"%4d",iradius);
    iw_radius = wid_textboxb(iw_rowcol,iw_radius,"Radius:",cval,4);
    }
 if (!leavit) XtUnmanageChild(XtParent(iw_radius));

 /* create accept boxes  ---------------------------------- apply */
 
 if (iw_dum == (Widget) 0) 
    iw_dum   = wid_stdbut(iw_rowcol, iw_pixelmen, &iw_pushs, &iw_pushc, &iw_pusha, 
                     "SA",pixelmen_buts,fin_cb,pixelmen_buta, NULL);


 XtManageChild(iw_pixelmen);
 }


 /************** pixelmen_i (inimag toggle callback) **************/

 void pixelmen_i(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * stringt;

 inimag = XmToggleButtonGadgetGetState(iw_inimag);

 if (docit) 
    {
    stringt = XmTextGetString(iw_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);
    }

 XtUnmanageChild(iw_pixelmen);                       
 pixelmen(NULL, NULL , NULL);  
 }

 /************** pixelmen_f (getfile toggle callback) **************/

 void pixelmen_f(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * stringt;

 getfile = XmToggleButtonGadgetGetState(iw_getfile);

 if (docit)
    {
    stringt = XmTextGetString(iw_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);
    }

 XtUnmanageChild(iw_pixelmen);                       
 pixelmen(NULL, NULL , NULL);  
 }


 /************** pixelmen_s (getscreen toggle callback) ***********/

 void pixelmen_s(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * stringt;

 getscreen = XmToggleButtonGadgetGetState(iw_getscreen);
 if (docit) 
    {
    stringt = XmTextGetString(iw_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);
    }

 /* restart the pixel menu */                       
 XtUnmanageChild(iw_pixelmen);                       
 pixelmen(NULL, NULL, NULL);
 }

 /************** pixelmen_d (docit toggle callback) *************/

 void pixelmen_d(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * stringt;

 docit = XmToggleButtonGadgetGetState(iw_docit);
                       
 if (docit) 
    {
    stringt = XmTextGetString(iw_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);
    }
 else
    ikey = 1;

 XtUnmanageChild(iw_pixelmen);                       
 pixelmen(NULL, NULL, NULL);
 }

 /************** pixelmen_sh (shimg toggle callback) *************/

 void pixelmen_sh(Widget iw_temp, XtPointer data, XtPointer call_data)
    {
    if (inimag) 
       {
       XtUnmanageChild(iw_pixelmen);                       
       shift(PIXEL);
       }
    else
       {
       spout(" Image can only be shifted when 'Inside last image' is active.");
       }
    }

 /************** pixelmen_l (leavit toggle callback) *************/

 void pixelmen_l(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * stringt;

 leavit = XmToggleButtonGadgetGetState(iw_leavit);
                     
 if (docit) 
    {
    stringt = XmTextGetString(iw_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);
    }

 XtUnmanageChild(iw_pixelmen);                       
 pixelmen(NULL, NULL, NULL);  
 }

 /************ accept button callback *********************************/

 void pixelmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 char * cdum;
 int    iform, iplanes;
 char * stringt;

 /* If last doc file is opened, close it first */
 if (fpdocpix_p)
    pixelmen_buts(NULL,NULL,NULL);

 inimag = XmToggleButtonGadgetGetState(iw_inimag);
 if (inimag && strlen(filnow) == 0)
    {
    spout("*** Must display image to find file value for pixel.");
    return;
    }

 getscreen = XmToggleButtonGadgetGetState(iw_getscreen);
 /* Destroy any old image structure */
 if (imagep) 
    {
    XDestroyImage(imagep);
    imagep = NULL;
    }

 if (getscreen)
    {
    /* Create new imagep */
    /* printf("%d %d %d %d \n",ixulx,iyulx, iwidex, ihighx-iyulx); */
   
   iplanes = pow(2,8) - 1;
#ifdef WEB_24
   iplanes = pow(2,16) - 1;
#endif

    imagep = XGetImage(idispl, imagsav, ixulx,iyulx,
                       iwidex, ihighx-iyulx, iplanes, ZPixmap);
    }
 else
    { /* Make sure image structure pointer is NULL */
    imagep = (XImage *) NULL;
    }


 docit = XmToggleButtonGadgetGetState(iw_docit);                   
 if (docit) 
    {
    stringt = XmTextGetString(iw_doc);
    strcpy(docnam,stringt);
    XtFree(stringt);

    if (strlen(docnam) == 0) 
       { spout("*** Must specify document file name."); return; }

    /* Get registers */
    if (rdpriw(&ixreg,1,MAXREGM1,iw_xreg,"x register",cdum) == FALSE) return;
    if (rdpriw(&iyreg,1,MAXREGM1,iw_yreg,"y register",cdum) == FALSE) return;
    if (rdpriw(&ikey, 1,INT_MAX, iw_key, "key",       cdum) == FALSE) return;
    if (ixreg == iyreg) spout("Warning: X & Y registers are the same");
    }

 if (docit && getscreen)
    {  /* Get screen register */
    if (rdpriw(&isreg,1,MAXREGM1,iw_sreg,"screen value register",cdum) == FALSE) return;
    }

 getfile = XmToggleButtonGadgetGetState(iw_getfile);
 if (getfile)
    {     /* Open image file if necessary */
     
    if (strlen(filnow) == 0)
       {
       spout("*** Must display image to find file value for pixel.");
       return;
       }

    if ((filedatap = openold(filnow,&nsam,&nrow,&nslice,&iform,"o")) == NULL) 
        {
        spout("*** Can not open image to find file value for pixel.");
        return;
        }

    inimag = TRUE;   

    if (docit)
       {  /* Get file value register */
       if (rdpriw(&ivreg,1,MAXREGM1,iw_vreg,"file value register",cdum) 
           == FALSE) return;
       }
    }

 if (leavit)
    {  /* Get radius for marker */
    if (rdpriw(&iradius,0,1000,iw_radius,"radius",cdum) == FALSE) return;
    }
               
 XtUnmanageChild(iw_pixelmen);

 pixelmen_showing = TRUE;                     
 pixel();
 }

 /************ Stop button callback *********************************/

 void pixelmen_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 int    numdoc, nlet;

  if (docit) 
     {   /* Close the doc file */
     fclose(fpdocpix_p); 
     fpdocpix_p = NULL;

     ikey = 1; 
     if (strlen(docnam) != 0) 
        {   /* Increment next doc file name */
        filinc(docnam, docnam, &nlet, &numdoc);
        }
     }

 if (imagep)
   {    /* Free up image storage */
   XDestroyImage(imagep);
   imagep = (XImage *) 0;
   }

 /* Remove the pixelmen */
 XtUnmanageChild(iw_pixelmen);  

 /* Close the image file, if opened here */
 if (filedatap != NULL && filedatap->fp != NULL) closefilep(&filedatap); 

 /* Uninstall translations */
 XtUninstallTranslations(iw_win);

 /* Remove button assignment instructions */
 if (pixelmen_showing) 
    { showbutx("","","",TRUE);  pixelmen_showing = FALSE; }
 }

