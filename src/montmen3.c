
/*$Header: /usr8/web/src/RCS/montmen3.c,v 1.16 2005/10/18 16:59:56 leith Exp $*/
/*
C++************************************************************************
C
C MONTMEN3
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
C MONTMEN3
C
C PURPOSE:         DISPLAY MONTAGE MENU
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

#define MAX_ARGS 2

 /* internal function prototypes */
 void          montmen3_buta(Widget iw_temp, XtPointer, XtPointer);
 void          montmen3_butc(Widget iw_temp, XtPointer, XtPointer);

 /* file scope variables */
 static Widget   iw_scales[6];
 static Widget   iw_ixul, iw_iyul, iw_nprow;
 static Widget   iw_montmen3 = 0, iw_pushc, iw_pusha, iw_numfil;
 static Widget   iw_xaxis, iw_yaxis, iw_zaxis, iw_movie, iw_norm;

 /* externally used variables */
 int      endfile;           /* used in mont3 */

 /*************************  montmen3 ********************************/

 void montmen3(void)

 { 
 int    i;
 Widget iw_rowcol1, iw_rowcol2, iw_rowcol3;
 Widget iw_dums, iw_radio;
 char   cval[9];
 Arg    args[2];

 if (iw_montmen3 == (Widget)0)
    {   /* create 3-D montage menu widget first */

    iw_montmen3 = wid_dialog(iw_win, 0, "montage options", -1, -1);
    iw_rowcol1  = wid_rowcol(iw_montmen3, 'v', -1, -1);

    /* create label box for axis -----------------------------  axis  */
    iw_rowcol2  = wid_rowcol(iw_rowcol1,  'h', -1, -1);
                  wid_labelg(iw_rowcol2, 0, "AXIS:   ", -1, -1);

    /* create toggle boxes for axes */
    iw_radio  = wid_rowcol(iw_rowcol2,  'h', -1, -1);
    XtSetArg(args[0], XmNradioBehavior, TRUE);
    XtSetValues(iw_radio, args, 1);

    iw_xaxis = wid_togmen(iw_radio,iw_xaxis,"X  ",xaxis,-1,-1);
    iw_yaxis = wid_togmen(iw_radio,iw_yaxis,"Y  ",yaxis,-1,-1);
    iw_zaxis = wid_togmen(iw_radio,iw_zaxis,"Z  ",zaxis,-1,-1);

    /* create text box for numfil input ---------------------- numfil */
    strcpy(cval,"*");
    iw_numfil = wid_textboxb(iw_rowcol1, 0, "LAST FILE NUMBER:", 
                            cval, 6);

    /* create label box for location ----------------------- location */
    iw_rowcol3  = wid_rowcol(iw_rowcol1,'h',-1,-1);
                  wid_labelg(iw_rowcol3,0, "LOCATION:",-1,-1);

    /* create toggle box for movie ---------------------------- movie */
    iw_movie = wid_togmen(iw_rowcol1,0,"MOVIE",movie,-1,-1);

    /* create toggle box for normalize -------------------- normalize */
    iw_norm = wid_togmen(iw_rowcol1,0,"NORM. BY SLICE",normit,-1,-1);
 

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
    iw_nprow = wid_textboxb(iw_rowcol1, 0, "FRAMES / ROW:", 
                            cval,8);


    /* create scale widgets for col, row & slice ------------- scales */
    for (i=0; i < 6; iw_scales[i++]  = (Widget) 0)
        ;
    wid_win(iw_rowcol1, &iw_scales[0], nsam, nrow, nslice);


    /* create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcol1, iw_montmen3, &iw_dums, &iw_pushc, &iw_pusha,
               "CA", fin_cb, montmen3_butc, montmen3_buta, NULL);

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

    /* create text box for pictures / row input --------------- nprow */
    if (samefile)  
       sprintf(cval,"%4d",nprow);
    else
       strcpy(cval,"*AS FITS");
    iw_nprow = wid_textboxb(iw_rowcol1, iw_nprow, "FRAMES / ROW:", 
                            cval,8);

 
    /* update scale widgets for col, row & slice ------------- scales */
    wid_win(iw_rowcol1, &iw_scales[0], nsam, nrow, nslice);
    }
 XtManageChild(iw_montmen3);
 }


 /*********** accept button callback **********************************/

 void montmen3_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *string;
 int  lastfilt;

 if (getcorner(iw_ixul,iw_iyul,&ixuln,&iyuln) == FALSE) return;

 /* get nsam1 & nsam2 */
 XmScaleGetValue(iw_scales[0],&nsam1);
 XmScaleGetValue(iw_scales[1],&nsam2);
 if (nsam2 < nsam1)
    {
    spout("*** ENDING COL MUST BE > STARTING COL");
    return;
    }

 /* get nrow1 & nrow2 */
 XmScaleGetValue(iw_scales[2],&nrow1);
 XmScaleGetValue(iw_scales[3],&nrow2);
 if (nrow1 > nrow2)
    {
    spout("*** ENDING ROW MUST BE > STARTING ROW");
    return;
    }

 /* get nslice1 & nslice2 */
 XmScaleGetValue(iw_scales[4],&nslice1);
 XmScaleGetValue(iw_scales[5],&nslice2);
 if (nslice1 > nslice2)
    {
    spout("*** ENDING SLICE MUST BE > STARTING SLICE");
    return;
    }

 movie = XmToggleButtonGadgetGetState(iw_movie);
 
 if (iw_norm > (Widget)0)
    normit = XmToggleButtonGadgetGetState(iw_norm);

 if (nslice > 1) 
   {
   xaxis = XmToggleButtonGadgetGetState(iw_xaxis);
   yaxis = XmToggleButtonGadgetGetState(iw_yaxis);
   zaxis = XmToggleButtonGadgetGetState(iw_zaxis);
   }

 /* find file numbers wanted */
 string = XmTextGetString(iw_numfil);

 if(ioper == 2) 
   {   /* 3D montage  */  
   if (strncmp(string,"*",1) == 0)
      {
      /* want all files in this series until first missing file */
      endfile = -1;
      lastfilt = -1;
      }
   else
      {
      if (sscanf(string,"%d",&lastfilt) <= 0 || lastfilt < 0)
         {   /* can not interpret lastfilt */
         spout("*** Can not interpret number of last file in montage! ");
          if (string) XtFree (string);  return;
         }

       endfile = lastfilt;
       }
     }
  else 
     {  /* 2D series of 3D volume  */
     endfile = 0;
     }
 if (string) XtFree (string);

 /* find nprow value */
 string = XmTextGetString(iw_nprow);

 XtUnmanageChild(iw_montmen3);
 expos_cb(NULL,NULL,NULL);

 if (xaxis) 
    {   /* want x axis montage */
    if (sscanf(string,"%d",&nprow) <= 0) 
       {  /* use default for nprow */
       if (iskip > 1)
          nprow = (iwidex - marlef) /
                  (((nrow2 - nrow1 + 1) / iskip) + marx);
       else if (iskip < -1)
          nprow = (iwidex - marlef) /
                  (((nrow2 - nrow1 + 1) * -iskip) + marx);
       else
          nprow = (iwidex - marlef) /
                  (((nrow2 - nrow1 + 1)        ) + marx);
       
       /* if nprow=0, reset nprow to 1 */
       if (nprow == 0) nprow = 1;
       }
       axis = 'x';
       mont3(filedata, imagego, imageend);
    }
 if (yaxis) 
    {   /* want y axis montage */
    if (sscanf(string,"%d",&nprow) <= 0) 
       {  /* use default for nprow */
       if (iskip > 1)
          nprow = (iwidex - marlef) /
                  (((nsam2 - nsam1 + 1) / iskip) + marx);
       else if (iskip < -1)
          nprow = (iwidex - marlef) /
                  (((nsam2 - nsam1 + 1) * -iskip) + marx);
       else
          nprow = (iwidex - marlef) /
                  (((nsam2 - nsam1 + 1)        ) + marx);
       
       /* if nprow=0, reset nprow to 1 */
       if (nprow == 0) nprow = 1;
       }
       axis = 'y';
       mont3(filedata, imagego, imageend);
    }

 if (zaxis) 
    {   /* want z axis montage */
    if (sscanf(string,"%d",&nprow) <= 0) 
       {  /* use default for nprow */
       if (iskip > 1)
          nprow = (iwidex - marlef) /
                  (((nsam2 - nsam1 + 1) / iskip) + marx);
       else if (iskip < -1)
          nprow = (iwidex - marlef) /
                  (((nsam2 - nsam1 + 1) * -iskip) + marx);
       else
          nprow = (iwidex - marlef) /
                  (((nsam2 - nsam1 + 1)        ) + marx);
       
       /* if nprow=0, reset nprow to 1 */
       if (nprow == 0) nprow = 1;
       }
       axis = 'z';
       mont3(filedata, imagego, imageend);
    }
 if (string) XtFree (string);
 if(ioper == 16)
   {
   moviemen(TRUE);
   }
 }

 /***********  montmen3_butc  (cancel button callback) ********/

 void montmen3_butc(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 XtUnmanageChild(iw_montmen3);
 closefile(filedata);
 filedata = NULL;
 }

