
/*$Header: /usr16/software/web/src/RCS/montmen2.c,v 1.14 2018/12/07 17:03:33 leith Exp $*/
/*

C++************************************************************************
C
C MONTMEN2 -- CREATED JUNE 91 FROM WIMONT
C          -- CONVERTED TO C  -- OCT 92 al
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
C MONTMEN2
C
C PURPOSE:         DISPLAY MONTAGE MENU FOR 2-D FILES
C
C VARIABLES:       NAMES      LOGICAL FLAG TO PRINT NAMES
C                  IXUL,IYUL  UPPER LEFT CORNER FOR LAST IMAGE
C                  MARX,MARY  X & Y MARGINS FOR MONTAGE
C
C CALLED BY:       
C
C**************************************************************************
*/

#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#include "common.h"
#include "routines.h"
#include "mont.h"

/* internal function prototypes */
 static void   montmen2_buta(Widget iw_temp, XtPointer, XtPointer);
 static void   montmen2_butc(Widget iw_temp, XtPointer, XtPointer);

 /* file variables */
 static Widget   iw_ixul, iw_iyul, iw_nprow, iw_numfil, iw_movie;
 static Widget   iw_montmen2 = (Widget)0, iw_pushc, iw_pusha;

 /* externally used variables */
 int             lastfile;           /* used in mont2 */

 /* externally defined variables */

 /************************************************************/

 void montmen2(void)

 { 
 Widget iw_rowcol, iw_rowcol3;
 Widget iw_dums;
 char   cval[9];

 if (iw_montmen2 == (Widget)0)
    {   /* create 2-D montage menu widget first */

    iw_montmen2 = wid_dialog(iw_win, 0, "montage options", -1, -1);
    iw_rowcol   = wid_rowcol(iw_montmen2, 'v', -1, -1);

    /* create text box for numfil input ---------------------- numfil */
    strcpy(cval,"*");
    iw_numfil = wid_textboxb(iw_rowcol, 0, "LAST FILE NUMBER:", 
                            cval, 6);

    /* create label box for location ----------------------- location */
    iw_rowcol3  = wid_rowcol(iw_rowcol,'h',-1,-1);
                  wid_labelg(iw_rowcol3,0, "LOCATION:",-1,-1);


    /* create text boxes for ixuln & iyuln -------------------- ixuln */
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

    /* create text box for pictures / row input --------------- nprow */
    strcpy(cval,"*AS FITS");
    iw_nprow = wid_textboxb(iw_rowcol, 0, "FRAMES / ROW:", 
                            cval,8);
    /* create toggle box for movie ---------------------------- movie */
    iw_movie = wid_togmen(iw_rowcol,0,"MOVIE",movie,-1,-1);

    /* create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcol, iw_montmen2, &iw_dums, &iw_pushc, 
                        &iw_pusha,  "CA",
                        fin_cb, montmen2_butc ,montmen2_buta, NULL);

    }

 else
    {   /* montage menu widget already exists, update it */
    /* update text boxes for ixuln & iyuln -------------------- ixuln */
    if (keepxy) 
       { /* do not want to reset ixuln & iyuln */
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

    /* update text box for pictures / row input --------------- nprow */
    if (samefile)  
       sprintf(cval,"%4d",nprow);
    else
       strcpy(cval,"*AS FITS");
    iw_nprow = wid_textboxb(iw_rowcol, iw_nprow, "FRAMES / ROW:", 
                            cval,8);

    }
    /* movie option not relevant for categorize */
    if (ioper == 11)
       { XtUnmanageChild(iw_movie); }
    else
       { XtManageChild(iw_movie);   }

 XtManageChild(iw_montmen2);
 }



 /***********  montmen2_buta  (accept button callback) ********/

 void montmen2_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *string = NULL;
 int  lastfilt, marlefo;

 /* find next image postion (if set)  */
 if (getcorner(iw_ixul,iw_iyul,&ixuln,&iyuln) == FALSE) 
       {   /* can not interpret image position */
       spout("*** Can not interpret image location! ");
       return;
       }

 if (ioper != 20)
    {
    /* interrogate movie toggle */
    movie = XmToggleButtonGadgetGetState(iw_movie);
    }
 
 /* find nprow value (number of images per row) */
 string = XmTextGetString(iw_nprow);

 if (sscanf(string,"%d",&nprow) <= 0 || nprow == 0) 
    {  /* use default for nprow */

    /* adjust marlefo for lef side position */
    marlefo = marlef;
    if (placed) marlefo = ixuln;

    if (iskip > 1)
       nprow = (iwidex - marlefo) / ((nsam / iskip)  + marx);
    else if (iskip < -1)
       nprow = (iwidex - marlefo) / ((nsam * -iskip) + marx);
    else
       nprow = (iwidex - marlefo) / ((nsam         ) + marx);
    }

 if (string) XtFree (string);

 /* find file numbers wanted */
 string = XmTextGetString(iw_numfil);

 if (strncmp(string,"*",1) == 0)
    {
    /* want all files in this series until first missing file */
    lastfile = -1;
    lastfilt = -1;
    }
 else
    {
    if (sscanf(string,"%d",&lastfilt) <= 0 || lastfilt < 0)
       {   /* can not interpret lastfilt */
       spout("*** Can not interpret number of last file in montage! ");
       if (string) XtFree (string); return;
       }
    lastfile = lastfilt;
    }
 if (string) XtFree (string);

 /* remove the montage menu */
 XtUnmanageChild(iw_montmen2);
 expos_cb(NULL,NULL,NULL);

 /* display the montage, imagego set in imagemen */
 mont2(imagego,imageend);
 }



 /***********  montmen2_butc  (cancel button callback) ********/

 void montmen2_butc(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 XtUnmanageChild(iw_montmen2);
 closefile(filedata);
 filedata = NULL;
 }
