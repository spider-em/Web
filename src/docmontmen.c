
/*$Header: /usr8/web/src/RCS/docmontmen.c,v 1.20 2012/04/17 18:35:31 leith Exp $*/
/*
C++*********************************************************************
C
C DOCMONTMEN -- CREATED JUNE 91 FROM WIMONT
C            -- CONVERTED TO C  -- OCT 92 al
C            -- ADDED MONTAGE RANGE SCALE SELECTORS, AUG 99 al
C
C **********************************************************************
C * AUTHOR:  ArDean Leith                                              
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
C DOCMONTMEN
C
C PURPOSE:         DISPLAY MONTAGE MENU FOR 2-D FILES
C
C VARIABLES:       NAMES      LOGICAL FLAG TO PRINT NAMES
C                  IXUL,IYUL  UPPER LEFT CORNER FOR LAST IMAGE
C                  MARX,MARY  X & Y MARGINS FOR MONTAGE
C
C CALLED BY:   
C
C web_com --> imagemen --> imagemen_cb --> imagemen_cb_com
C                                               v
C                                               v
C  mont2 <-- imagemen1 <-- docmontmen_buta  <-- docmontmen
C     v
C     v (if average)
C     v
C  imagemen1 (sets ioper to: 1)
C
C**************************************************************************
*/

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <Xm/Scale.h>

#include "common.h"
#include "routines.h"

 /* Internal function prototypes */
 void     docmontmen_buta    (Widget, XtPointer, XtPointer);
 void     docmontmen_buta_stk(Widget, XtPointer, XtPointer);
 void     docmontmen_a       (Widget, XtPointer, XtPointer);
 void     docmontmen_a_stk   (Widget, XtPointer, XtPointer);

 /* File variables */
 static Widget   iw_template, iw_nprow, iw_movie;
 static Widget   iw_nprow_stk, iw_movie_stk;
 static Widget   iw_average, iw_avgnam, iw_minfile, iw_maxfile;
 static Widget   iw_average_stk, iw_avgnam_stk;
 static Widget   iw_ixul, iw_iyul;
 static Widget   iw_ixul_stk, iw_iyul_stk;
 static Widget   iw_docmontmen = (Widget) 0,  iw_pusha;
 static Widget   iw_docmontmen_stk = (Widget) 0,  iw_pusha_stk;
 static int      regno = 1;  /* No longer variable, aug 99 al */
 static char     *filnewdoc;
 static char     filnewimg[128];
 static int      maxvalfound;
 static int      minvalfound;

 /* Externally used variables (For use in mont2) */
 int * docimgnums;
 int   docimgtotal, numnext, numnextout;
 int   montaverage = FALSE;
 char  avgnam[128];

 /* Externally defined variables (For use in mont2) */
 extern int    lastfile;           
 extern char   filego[81];     /* Name of first image in montage   */
 extern int    minfile, maxfile;

 /***********  docmontmen0  (called from imagemen) ********/

 void docmontmen0(char * filnewt)
 { 
 int         maxreg, maxkeys, i, maxkeygot;
 float       *dbuf = NULL;

 /* Remember doc file name */
 filnewdoc = filnewt;

 /* Set size for dbuf array */
 maxreg  = 1+1; 
 maxkeys = 99999;

 /* Recover doc file data */
 if (getdoc((FILE *) NULL, filnewdoc, NULL, maxkeys, maxreg, 
             &dbuf, &maxkeygot) > 0) 
    {   /* Problem retrieving doc file data */
    docimgtotal = -1;
    //printf("getdoc BAD  maxkeygot: %d   \n",maxkeygot);  
    
    if (dbuf) free(dbuf); dbuf = NULL;
    return;
    }

 /* Run time allocation of memory space for docimgnums */
 if ((docimgnums = (int *) malloc(maxkeygot * sizeof(int))) == 
     (int *) NULL)
     {spout("*** Memory allocation failed in docmontmen!"); 
     if (dbuf) free(dbuf); dbuf = NULL; return; }

 /* Fill integer array for image numbers (docimgnums)*/
 docimgtotal = 0;
 maxvalfound = 0;
 minvalfound = INT_MAX;

 for (i=0; i < maxkeygot; i++)
    {
    if (*(dbuf + i * maxreg) > 0)
       { /* This key in use in doc file */
       docimgnums[docimgtotal] = * (dbuf + i * maxreg + regno);
       minvalfound = MYMIN(minvalfound,docimgnums[docimgtotal]);
       maxvalfound = MYMAX(maxvalfound,docimgnums[docimgtotal]);
       docimgtotal++;
       }
    }

 /* Set slider limits for file number selection */
 minfile  = minvalfound;
 maxfile  = maxvalfound;
 lastfile = maxfile;

 /* Dbuf no longer needed */
 if (dbuf) free(dbuf); dbuf = NULL;

 /* Cancel waitcursor */
 waitcursor(iwin, FALSE, 79);
 }

 /**************************  docmontmen  **********************/

 void docmontmen(char * filnewt)
 {  /* Sets menu for document file based image montage */

 Widget iw_rowcol, iw_rowcol3, iw_rowcolh;
 Widget iw_dums;
 char   cval[9];
 char * cdum = 0;

 if (iw_docmontmen == (Widget)0)
    {   /* Create doc montage menu widget first */

    iw_docmontmen = wid_dialog(iw_win, 0, "montage parameters", -1, -1);
    iw_rowcol     = wid_rowcol(iw_docmontmen, 'v', -1, -1);

    /* Create text box image file template ----------------- template */
    strcpy(cval,"avgstk@*");
    iw_template = wid_textboxb(iw_rowcol, 0, 
                 "Image file template:", cval,32);

    /* Create horizontal rowcol for min & max    filenumber -------- */
    iw_rowcolh = wid_rowcol(iw_rowcol, 'h', -1, -1);

    /* Create scale widget for min filenumber --------------- minfile */
    iw_minfile = wid_scale(iw_rowcolh, 0, "Min. filenumber: ",
                 minvalfound,maxvalfound, minfile,200,50,-1,-1); 

    /* Create scale widget for max filenumber --------------- maxfile */
    iw_maxfile = wid_scale(iw_rowcolh, 0, "Max. filenumber: ",
                 minvalfound,maxvalfound, maxfile,200,50,-1,-1); 

    /* Create label box for location ----------------------- location */
    iw_rowcol3  = wid_rowcol(iw_rowcol,'h',-1,-1);
                  wid_labelg(iw_rowcol3,0, "LOCATION:",-1,-1);


     /* Create text boxes for ixuln & iyuln -------------------- ixuln */
    if (keepxy) 
       { /* do not want to reset ixuln & iyuln */
       sprintf(cval,"%4d",ixuln);
       iw_ixul = wid_textboxb(iw_rowcol3,0,"X",cval,4);
       sprintf(cval,"%4d",iyuln);
       iw_iyul = wid_textboxb(iw_rowcol3,0,"Y",cval,4);
       }
    else
       {
       iw_ixul = wid_textboxb(iw_rowcol3,0,"X","*",4);
       iw_iyul = wid_textboxb(iw_rowcol3,0,"Y","*",4);
       }

     /* Create text box for pictures / row input --------------- nprow */
    strcpy(cval,"*AS FITS");
    iw_nprow = wid_textboxb(iw_rowcol, 0, "Frames / Row:", cval,8);

    /* Not relevant for categorize */
    /* Create toggle box for movie ---------------------------- movie */
    iw_movie = wid_togmen(iw_rowcol,0,"Movie",movie,-1,-1);

    /* Create toggle box for average -------------------- montaverage */
    iw_average = wid_toggleg(iw_rowcol,0,"Compute average",
                         montaverage,docmontmen_a,cdum,-1,-1);

    /* Create text box for doc file name input  -------------- avgnam */
    iw_avgnam = wid_textboxb(iw_rowcol,0,"Average file:", avgnam,32);
    XtSetSensitive(XtParent(iw_avgnam), montaverage);

    /* Create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcol, iw_docmontmen, &iw_dums, &iw_dums, 
                        &iw_pusha,  "CA",
                        fin_cb, fin_cb ,docmontmen_buta, NULL);
    }
else
    {   /* Montage menu widget already exists, update it */
    /* update text boxes for ixuln & iyuln -------------------- ixuln */
    if (keepxy) 
       { /* Do not want to reset ixuln & iyuln */
       sprintf(cval,"%4d",ixuln);
       iw_ixul = wid_textboxb(iw_rowcol3,iw_ixul,"X",cval,4);
       sprintf(cval,"%4d",iyuln);
       iw_iyul = wid_textboxb(iw_rowcol3,iw_iyul,"Y",cval,4);
       }
    else
       {
       iw_ixul = wid_textboxb(iw_rowcol3,iw_ixul,"X","*",4);
       iw_iyul = wid_textboxb(iw_rowcol3,iw_iyul,"Y","*",4);
       }

    /* Update scale widget for min filenumber --------------- minfile */
    iw_minfile = wid_scale(iw_rowcolh, iw_minfile, "Min. filenumber: ",
                       minvalfound,maxvalfound, minfile,200,50,-1,-1); 

    /* Update scale widget for max filenumber --------------- maxfile */
    iw_maxfile = wid_scale(iw_rowcolh, iw_maxfile, "Max. filenumber: ",
                       minvalfound,maxvalfound, maxfile,200,50,-1,-1); 


    /* Update text box for pictures / row input --------------- nprow */
    if (samefile)  
       sprintf(cval,"%4d",nprow);
    else
       strcpy(cval,"*AS FITS");
       iw_nprow = wid_textboxb(iw_rowcol, iw_nprow, "FRAMES / ROW:", 
                            cval,8);
    }

    if (ioper == 11 || ioper == 23)
       {  /* Not relevant for categorize */
       XtUnmanageChild(iw_movie);
       XtUnmanageChild(iw_average);
       XtUnmanageChild(iw_avgnam);
       XtUnmanageChild(XtParent(iw_avgnam));
       }
    else
       {
       XtManageChild(iw_movie);
       XtManageChild(iw_average);
       XtManageChild(iw_avgnam);
       }
 XtManageChild(iw_docmontmen);
 }


 


 /************** docmontmen_a (montaverage toggle callback) ***********/

 void docmontmen_a(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 montaverage = XmToggleButtonGadgetGetState(iw_average);
 XtSetSensitive(XtParent(iw_avgnam),montaverage);
 }


 
/***********  docmontmen_buta  (accept button callback) ********/

 void docmontmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char        *string   = NULL;
 char        *template = NULL;
 int         ifile;

 /* Find image file name template, put it in filego for mont2 */
 template = XmTextGetString(iw_template);
 strcpy(filego,template);
 if (!addext(filego, datexc, 81)) return;

 /* Get minfile & maxfile */
 XmScaleGetValue(iw_minfile,&minfile);
 XmScaleGetValue(iw_maxfile,&maxfile);
 if (minfile > maxfile)
    { spout("*** ENDING FILE MUST BE > STARTING FILE"); return; }

 /* find next image postion (if set)  */
 if (getcorner(iw_ixul,iw_iyul,&ixuln,&iyuln) == FALSE) 
    {   /* can not interpret image position */
    spout("*** Can not interpret image location! ");
    return;
    }

 if (ioper != 11 && ioper != 23)
    {
    /* Interrogate movie toggle (not for categorize operation) */
    movie = XmToggleButtonGadgetGetState(iw_movie);
    }

 /* Find nprow value (number of images per row) */
 string = XmTextGetString(iw_nprow);

 if (sscanf(string,"%d",&nprow) <= 0 || nprow == 0) 
    {  /* Use default for nprow, calcualte in mont2.c */
    nprow = 0;
    }

 if (string) XtFree (string);

 if (ioper != 11 && ioper != 23)
    { /* Not relevant for categorize from doc file operation */
    montaverage = XmToggleButtonGadgetGetState(iw_average);                   
    if (montaverage) 
       {
       string = XmTextGetString(iw_avgnam);
       if (strlen(string) <= 0) 
          {
          pop_errmsg("*** You must enter a filename.");
          return;
          }

       else if (strlen(string) >= 128) 
          {
          pop_errmsg("*** File name too long.");
          return;
          }

       strcpy(avgnam,string);
       XtFree(string);
       }
    }

 /* Remove the montage menu */
 XtUnmanageChild(iw_docmontmen);
 expos_cb(NULL,NULL,NULL);

 /* Create file name for first file in series (filnew) */
 numnextout = 0;

 /* Update first filname in case < minfile .... */
 for (numnext = 0 ; numnext < docimgtotal ; numnext++)
    {  /* Search over all doc file entries */
    ifile      = docimgnums[numnext];
    if (ifile >= minfile && ifile <= maxfile)
       {  /* Use this file number */
       docimgnums[numnextout++] = ifile;
       }
    }
 docimgtotal = numnextout;
 numnextout  = 0;
 numnext     = 1;
 ifile       = docimgnums[0];

 filget(template,filnewimg,127,ifile);
 if (template) free(template);

 if (!addext(filnewimg, datexc, 128))return;

 /* Call imagemen1 to activate montage display */
 imagemen1(filnewimg, FALSE);

 /* Cancel waitcursor */
 waitcursor(iwin, FALSE, 79);
 }



