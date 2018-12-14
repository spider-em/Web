/*$Header $*/


/*
C++************************************************************************
C
C dentmen2.c  -- 
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
C    dentmen2
C
C    PURPOSE:         DISPLAY DENTIST DRILL PARAMETERS MENU
C
C    CALLED BY:       DENT
C
C***********************************************************************
*/

#include <Xm/Text.h>

#include "common.h"
#include "routines.h"
#include "dent.h"

 /* internal function prototypes */
 static void dentmen2_buts(Widget, XtPointer, XtPointer);
 static void dentmen2_butc(Widget, XtPointer, XtPointer);
 static void dentmen2_cb  (Widget, XtPointer, XtPointer);
 static void dentmen2_save(Widget, XtPointer, XtPointer);
 static void dentfile_cb  (Widget, XtPointer, XtPointer);
 
 /* external global variables  */

 /* external global variables (from dent.c) */

 /* file scope static variables */
 static Widget  iw_dentmen2  = (Widget) 0;
 static char    dentfile[81] = "dentvol" ;

 struct xcall
        { 
        int * iptr;
        int   minvalt;
        int   maxvalt;
        int   odd;
        };

 static struct xcall st_dentdensity, st_dentdepth, st_dentwidth;
 static struct xcall st_dentheight, st_dentmovement;

 /***************************   dentmen2  ****************************/

 void dentmen2()

 { 
 Widget iw_rowcolv,  iw_dentdensity, iw_dentdepth, iw_dentwidth;
 Widget iw_dentheight, iw_dentmovement, iw_pushs, iw_pushc;
 Widget iw_dentfile;
 char   cval[20];

 if (iw_dentmen2 <= (Widget)0)
    {   /* create dentist parameter widget first */

    iw_dentmen2 = wid_dialog(iw_win, 0, "Drill-n-fill parameters",
                              -1, -1);
    iw_rowcolv  = wid_rowcol(iw_dentmen2, 'v', -1, -1);

    /* create text box  for dentdensity -------------------  density */
    st_dentdensity.iptr    = &dentdensity;
    st_dentdensity.minvalt = 0;
    st_dentdensity.maxvalt = 255;
    st_dentdensity.odd     = 0;
    
    sprintf(cval,"%d",dentdensity);
    iw_dentdensity = wid_textboxb(iw_rowcolv,0,"tool density: ",cval,6);
    XtAddCallback(iw_dentdensity,XmNactivateCallback,
                  dentmen2_cb, &st_dentdensity);


    /* create text box  for dentwidth -------------------  dentwidth */
    st_dentwidth.iptr    = &dentwidth;
    st_dentwidth.minvalt = 1;
    st_dentwidth.maxvalt = nrow;
    st_dentwidth.odd     = 1;
    sprintf(cval,"%d",dentwidth);
    iw_dentwidth = wid_textboxb(iw_rowcolv,0,"tool width   :",cval,6);
    XtAddCallback(iw_dentwidth,XmNactivateCallback,
                  dentmen2_cb, &st_dentwidth);

    /* create text box  for dentheight -----------------  dentheight */
    st_dentheight.iptr    = &dentheight;
    st_dentheight.minvalt = 1;
    st_dentheight.maxvalt = nslice;
    st_dentheight.odd     = 1;
    sprintf(cval,"%d",dentheight);
    iw_dentheight = wid_textboxb(iw_rowcolv,0,"tool height:  ",cval,6);
    XtAddCallback(iw_dentheight,XmNactivateCallback,
                  dentmen2_cb,&st_dentheight);

    /* create text box  for dentdepth -------------------  dentdepth */
    st_dentdepth.iptr    = &dentdepth;
    st_dentdepth.minvalt = 0;
    st_dentdepth.maxvalt = nsam;
    st_dentdepth.odd     = 0;
    sprintf(cval,"%d",dentdepth);
    iw_dentdepth = wid_textboxb(iw_rowcolv,0,"tool length:  ",cval,6);
    XtAddCallback(iw_dentdepth,XmNactivateCallback,
                  dentmen2_cb, &st_dentdepth);


    /* create text box  for dentmovement -------------  dentmovement */
    st_dentmovement.iptr    = &dentmovement;
    st_dentmovement.minvalt = -nsam;
    st_dentmovement.maxvalt = nsam;
    st_dentmovement.odd     = 0;
    sprintf(cval,"%d",dentmovement);
    iw_dentmovement = wid_textboxb(iw_rowcolv,0,"tool movement:",cval,6);
    XtAddCallback(iw_dentmovement,XmNactivateCallback,
                  dentmen2_cb, &st_dentmovement);

    /* create text box for file name input  --------------- dentfile */
    iw_dentfile = wid_textboxb(iw_rowcolv,0,"Output file:",
                               dentfile,20);
    XtAddCallback(iw_dentfile,XmNactivateCallback,
                  dentfile_cb,NULL);

    /* create save pushbutton --------------------------------  save */
    wid_pushg(iw_rowcolv, 0, " SAVE IN OUTPUT FILE", dentmen2_save, 
              NULL, 0, 0);

    /* create box for apply ----------------------------------- apply */
    wid_stdbut(iw_rowcolv, iw_dentmen2, 
               &iw_pushs, &iw_pushc, NULL,  "SC",
               dentmen2_buts, fin_cb, NULL, NULL);
    }  


 XtManageChild(iw_dentmen2);
 }

 /*********** textbox altered callback *******************************/

 void dentmen2_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char *string;
 int   ival;
 struct xcall * xptr;
 char outstr[81];

 /* find threshold */
 string = XmTextGetString(iw_temp);
 sscanf(string,"%d",&ival);
 if (string) free(string);

 xptr = data;

 if ((ival < xptr->minvalt) || (ival > xptr->maxvalt))
    {
    sprintf(outstr,"*** Input value out of range: %d...%d",
            xptr->minvalt, xptr->maxvalt);
    spout(outstr);
    XBell(idispl,50);
    return;
    }

 if (xptr->odd && ival % 2 == 0)
     {
    sprintf(outstr,"*** Input value must be odd: %d", ival);
    spout(outstr);
    XBell(idispl,50);
    return;
    }

 /* return the acccepted value */
 *(xptr->iptr) = ival;
 }

 /*********** dentfile altered callback ******************************/

 void dentfile_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * string;

 /* find filename */
 string = XmTextGetString(iw_temp);
 strncpy(dentfile,string,80);

 if (string) free(string);
 }
 

 /*********** save button callback **********************************/

 void dentmen2_save(Widget iw_temp, XtPointer data, XtPointer call_data)
   {
   FILEDATA       *filedatad;
   float          * fptr;
   int            irow;
   unsigned char  * cptr;

   if (dentfile == NULL)
      {
      spout(" No file name specified!");
      XBell(idispl,50);
      }
   else
      {
      /* set waitcursor */
      waitcursor(iwin, TRUE, 79);

      /* open output file */
      if ((filedatad = opennew(dentfile, nsam, nrow, nslice, 3, "u")) ==
          (FILEDATA *) NULL)
          { 
          waitcursor(iwin, FALSE, 79); 
          spout("*** Unable to open output file!!");
          XBell(idispl,50);
          return;
          }

      /* convert single bit volume to spider format */
      cptr = (unsigned char *) dentvol;
      for (irow=1; irow <= nrow*nslice; irow++)
         {
         /* use default read / write buffer from filedatad */
         for (fptr = filedatad -> buf; 
              fptr < (filedatad -> buf)+nsam; fptr++)
            {
            *fptr = *cptr++;
            }
         wrtlin(filedatad, filedatad -> buf, nsam, irow);
         }

      /* close output file */
      closefile(filedatad);

     /* cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
      }
   }

 /*********** cancel button callback **********************************/

 void dentmen2_butc(Widget iw_temp, XtPointer data, XtPointer call_data)
   {
   XtUnmanageChild(iw_dentmen2);
   }


 /*********** stop button callback **********************************/

 void dentmen2_buts(Widget iw_t, XtPointer data, XtPointer call_data)
    {
    if( filedata && filedata->fp ) closefile(filedata);

    /* unmanage secondary menu box */
    XtUnmanageChild(iw_dentmen2);

    /* unmanage button information box */
    showbutx("","","",TRUE);

    /* uninstall translations */
    XtUninstallTranslations(iw_win);

    if (dentvol)     free(dentvol);     dentvol     = NULL;
    if (dentrefmap)  free(dentrefmap);  dentrefmap  = NULL;
    if (dentdistmap) free(dentdistmap); dentdistmap = NULL;
    }

 

	
