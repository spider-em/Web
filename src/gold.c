
/*$Header: /usr8/web/src/RCS/gold.c,v 1.27 2011/05/17 17:29:22 leith Exp $*/
/*
C++*********************************************************************
C
C WID_GOLD.FOR -- CREATED JAN 90 FROM LXGOLD.FOR
C                 WIDGETIZED MAY 91 al
C gold.c          NOV 92 al
C **********************************************************************
C *  AUTHOR:  ArDean Leith
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
C    GOLD
C
C    CALLED BY: web_com   
C
C    gold --> gold_buta -------> gongon ------------ savdn1 -- widean 
C              ?                                       ^           |
C              ---> goldmen_butm --> goldmen_mark --> unsdal       |
C                                                                  v
C                                                               pop setup
C
C    gold_pop ---L--> savdn1
C             |
C             |--M--> goldmen --- _butum --> XPutimage
C             |               |
C             |               --- _butm  --> _mark -->unsdal -- savdn1
C             |               |
C             |               --- _butsh --> shift
C             |
C             |
C             |--R--> filinc -- goldmen_buts --> imagemen
C
C--*********************************************************************
*/
#include "common.h"
#include "routines.h"
#include <Xm/Text.h>
#include <stdlib.h>

 /* Externally defined protypes */
 extern void   goldmen_buts(Widget, XtPointer, XtPointer);
 extern int    goldmen_mark(void);

 /* Internal subroutine prototypes */
 void          gold       (void);
 void          gold_pop   (Widget,  XEvent *,  String *, Cardinal *);
 void          gold_buta  (Widget, XtPointer, XtPointer);
 int           gold_delete(int);

 /* External global variables */
 extern Widget    iw_deant;             /* Text part of iw_dean */

 /* Internal file variables */
 static Widget    iw_phi, iw_theta, iw_docg;
 static int       markg;
 static float     fphi, ftheta;         /* Input tilt angles */
 static int       deltaphi = 0, deltatheta = 0;
 static float     phig, thetag;
 static char      docnamepe[81];

 /* Internally defined global variables */
 Widget           iw_dean     = NULL;
 Widget           iw_gold     = NULL;
 char             docnamg[81] = "markerdoc000";
 FILE             *fpdocg     = NULL;
 int              iradg       = 4;

 /****************************  gold   *******************************/

 void gold(void)

 {
 Widget  iw_rowcolv, iw_pushs, iw_pusha; 
 char    cval[10];

 /* Refresh screen */
 expos_cb(NULL,NULL,NULL);

 if (iw_gold == (Widget) 0)
    {   
    /* Create marker doc file menu first */

    iw_gold = wid_dialog(iw_win, 0, "Marker document file", -1, -1);
    iw_rowcolv  = wid_rowcol(iw_gold, 'v', -1, -1);

    /*   Create a text box for doc file name input */
    iw_docg = wid_textboxb(iw_rowcolv,0,"Doc file:  ",docnamg,20);

    wid_labelg(iw_rowcolv,0,"Theta is tilt about y axis. ",
                         -1,-1);
    /*   Create a text box for theta angle input */
    sprintf(cval,"%6.2f",ftheta);
    iw_theta = wid_textboxb(iw_rowcolv,0,"Theta:     ",cval,7);

    wid_labelg(iw_rowcolv,0,
              "Phi is ccw rotation about z axis.",-1,-1);

    /*   Create a text box for phi angle input */
    sprintf(cval,"%6.2f",fphi);
    iw_phi = wid_textboxb(iw_rowcolv,0,"Phi:       ",cval,7);

    /* Create box for apply  */
    wid_stdbut(iw_rowcolv, iw_gold, 
               &iw_pushs, &iw_pushs, &iw_pusha, "SA",
               goldmen_buts, gold_buta ,gold_buta, 
               NULL);
    }
 else
    {
    /*   Alter text box for doc file name input */
    iw_docg = wid_textboxb(iw_rowcolv,iw_docg,"Doc file:",docnamg,20);

    /*   Create a text box for theta angle input */
    ftheta = ftheta + deltatheta;
    sprintf(cval,"%6.2f",ftheta);
    iw_theta = wid_textboxb(iw_rowcolv,iw_theta,"Theta:     ",cval,7);

    /*   Create a text box for phi angle input */
    fphi = fphi + deltaphi;
    sprintf(cval,"%6.2f",fphi);
    iw_phi = wid_textboxb(iw_rowcolv,iw_phi,"Phi:       ",cval,7);

    }
 XtManageChild(iw_gold);

 }

/*************  Docfile accept button callback ***********************/

 void gold_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 int        idum;
 float        alpha, beta, gamma;
 char         outstr[80];
 int          openit;
 char         *ctemp = NULL;
 int          append;
 float        plist[1];
 int          markdoc;

 if (rdprxc(iw_docg,&docnamg[0],81) == 0) 
    {
    /*  This program requires a doc file name */
    spout("*** You must enter a doc. file name now!");
    XBell(idispl,50);
    return;
    }

 /* Find theta */
 if (!rdprx(iw_theta,'f',&idum,&ftheta)) return;
 if (ftheta > 360 || ftheta < -360) 
    {
    spout("*** Theta range -360 ... 360!"); XBell(idispl,50);
    return;
    }

 /* Find phi */
 if (!rdprx(iw_phi,'f',&idum,&fphi)) return;
 if (fphi > 360 || fphi < -360) 
    {
    spout("*** Phi range -360 ... 360!"); XBell(idispl,50);
    return;
    }

 /*  Remove doc file menu */
 XtUnmanageChild(iw_gold);

 /*     Refresh screen  */
 expos_cb(NULL,NULL,NULL);

 deltatheta = ftheta - thetag;
 thetag     = ftheta;
 deltaphi   = fphi - phig;
 phig       = fphi;

 /* Find euler angles */
 gongon(thetag, phig, &alpha, &beta, &gamma);

 /* Add datexc (name extension ) to docname if not there  */
 strcpy(docnamepe,docnamg);
 if (strstr(docnamepe,datexc) == 0)
    {
    if ((strlen(docnamepe) + strlen(datexc) + 1) > (size_t) 80)
       { spout("*** Doc. file name too long:"); spout(docnamepe); 
       return; }  
    strcat(strcat(docnamepe,"."),datexc);
    }

 /* See if there is an existing document file to append to */
 append  = FALSE;
 markg   = 0;
 markdoc = 0;
 if ((fpdocg = fopen(docnamepe,"r")) != NULL )
    {  
    spout("  Doc. file already exists."); 
    fclose(fpdocg); fpdocg = NULL;
 
    /* Display existing markers, recover markdoc  */
    markdoc = goldmen_mark();

    /* Recover existing points */
    if (markdoc <= 0) 
       {  
       spout("*** Unable to read existing doc. file.");
       XBell(idispl,50);
      }
    else
       {
       markg  = markdoc;
       sprintf(outstr,"Recovered %4d markers from: %s", markg,docnamepe);
       spout(outstr);
       append = TRUE;
       }
    }

 /* Open doc file as append to put angles in it, leave it open */
 openit   = TRUE;
 plist[0] = 0.0;      /* necessary to avoid fl. pt. execption on dec */
 fpdocg   = savdn1(&docnamg[0], datexc, &fpdocg, plist, 0, 
                   &openit, append, TRUE);

 if (markdoc == 0)
    {
    /*    Put angles in the doc file */
    fprintf(fpdocg," ; Image file: %s\n", filnow);
    fprintf(fpdocg," ; Theta & phi angles: %10.4f%10.4f\n",thetag,phig);
    /* ramark in hvem had order of angles chaned on 12 may 94 ------
    fprintf(fpdocg," ; Alpha, beta & gamma are on following line:\n");
    fprintf(fpdocg," ;  1 3%12.4f%12.4f%12.4f\n", alpha,beta,gamma);
      *****************************/
    fprintf(fpdocg," ; Beta, gamma & alpha are on following line:\n");
    fprintf(fpdocg," ;  1 3%12.4f%12.4f%12.4f\n",beta,gamma,alpha);
    fflush(fpdocg);
    }

 /*      Put angles on output window also */
 sprintf(outstr," thetag,phig,alpha,beta,gamma:%8.2f%8.2f%8.2f%8.2f%8.2f",
                  thetag,phig,alpha,beta,gamma);
 spout(outstr);
  
 showbutx("Set marker location.",
          "Menu.",
          "Next marker file.",FALSE);

 /*  Display marker number selection widget */
 iw_dean = widean(iw_win,iw_dean,"marker","1",
                  ixul+nsam+1, iyul+nrow+1,5);

 /*  Change marker # in marker input box */
 ctemp = itoa(++markg);
 XmTextSetString(iw_deant,ctemp);
 if (ctemp) free(ctemp);

 /* Define button meanings */
 actions(iw_win, "gold_pop", gold_pop, "123");
 }

 /************************ gold_pop **********************************/

 void gold_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 int        ix1, iy1, ixi, iyi;
 int        nlet, numfili, numfild;
 float      fdum;
 char     * ctemp;
 float      dlist[3];
 char       outstr[80];
 static int openit = FALSE;
 int        nsams, nrows, nslices;	 

 if (!(strcmp(*params,"1")))            /* Button 1 pushed */ 
    {
    /* Left button, record location if inside image */

    /* Get mouse position */
    getloc(event,'M',&ix1,&iy1);
 
    /*         Find location relative to image */
    ixi = ix1 - ixul + 1;
    iyi = iy1 - iyul + 1;

    /* Get the display size of the image */
    dispsize( nsam1, nsam2, nrow1, nrow2, 1,1,iskip, &nsams, &nrows, &nslices);

    if (ixi < 1 || ixi > nsams || iyi < 1 || iyi > nrows) 
       {
       /*  Outside of image. can not select this location */
       spout("*** Outside image, unable to select location"); 
       XBell(idispl,50);
       }
    else
       {
       /* Inside image and want to select this location for doc file */

       /* Get marker number from text box */
       rdprx(iw_deant, 'i', &markg, &fdum);
  
       if (markg < 0) 
          {
          /*    Delete this marker from doc file */
          if (gold_delete(-markg) > 0)
              { 
              sprintf(outstr,"Marker: %d deleted.", markg);
              spout(outstr);
              }
          }
       else
          {
          sprintf(outstr,"Marker: %d (%d,%d)", markg,ixi,iyi);
          spout(outstr);
 
          /*    Draw circle at this location */
          xorc(iwin,icontx,   TRUE,ix1,iy1,iradg);
          xorc(imagsav,icontx,TRUE,ix1,iy1,iradg);

          /*    Write marker number at this location */
	  if (show_num) 
	    {
	    ctemp = itoa(markg);
	    witext(icontx, ctemp, ix1, iy1, 0, 0, -9, 2, FALSE);
	    }

          /*     Put this marker in the file */
          dlist[0] = markg;

          /* Resize the location of markers */
          if (iskip > 1)
             {
             dlist[1] = ixi * iskip;
             dlist[2] = iyi * iskip;
	     }
          else if (iskip < -1)
             {
             dlist[1] = ixi / -iskip;
             dlist[2] = iyi / -iskip;
	     }
          else 
             {
             dlist[1] = ixi;
             dlist[2] = iyi;
	     }

          fpdocg = savdn1(&docnamg[0], datexc, &fpdocg, dlist, 3, 
                  &openit, TRUE, FALSE);

          /*  Change marker # in marker input box */
          ctemp = itoa(++markg);
          XmTextSetString(iw_deant,ctemp);
          free(ctemp);
          }
       }
    }

 else if (!(strcmp(*params,"2")))            /* Button 2 pushed */ 
    {
    /*  Show marker picking options menu */
    goldmen();
    }

 else if (!(strcmp(*params,"3")))            /* Button 3 pushed */ 
    {
    /*  Next doc file wanted */

    /*      Increment image file name */
    filinc(filnow,filnext,&nlet,&numfili);

    /*     Increment doc file name */
    filinc(docnamg,docnamg,&nlet,&numfild);

    /*  Remove message, etc. */
    goldmen_buts(NULL,NULL,NULL);

    /* Display image file widget for next file */
    imagemen(NULL,"5",NULL); 
    }
 }


 /************************ gold_delete ****************************/


int gold_delete(int markg)
 {
 float * ptr;
 int     maxkeys = 9999, maxreg = 3;
 float   dlist[4];
 int     unused, keepgo, i, markers;
 float * dbuf = NULL;
 int     markdoc;
 int     openit = TRUE;
 char    outstr[120];
 char    reclin[101];
 FILE    *fpdocgtmp = NULL;

 /* Vax does not like file opened twice and unsdal opens it*/
 if (fpdocg) fclose(fpdocg); fpdocg = NULL;

 /* Retrieve marker locations from doc file */
 if (unsdal(&docnamg[0], &unused, 0, dlist, 0, &dbuf, &markdoc,
           maxkeys, maxreg) > 0) 
    {   /* problem retrieving doc file */
    if (dbuf) free(dbuf); dbuf = NULL; XBell(idispl,50);
    spout("*** Unable to read marker doc. file.");
    return 0;
    }

 /* Reopen old doc file */
 openit = TRUE;
 fpdocg = savdn1(&docnamg[0], datexc, &fpdocg, dlist, 0, 
                  &openit, TRUE, FALSE);

 /* Open a new temporary doc file */
 openit = TRUE;
 fpdocgtmp = savdn1("web_temp_jnk12345", NULL, &fpdocgtmp, dlist, 0, 
                  &openit, FALSE, FALSE);

  /* Copy the comment lines at top of old file to temporary file*/
  keepgo     = TRUE;
  while ((fgets(reclin,100,fpdocg)) != (char *) NULL  && keepgo)
      {
      if (ferror(fpdocg))
         {
         spout("*** Error reading docfile line:"); spout(reclin);
         fclose(fpdocg); return 0;
         }

      if ((strstr(reclin,";")) != (char *) NULL)
         { /* Copy this comment line */ ;
         fprintf(fpdocgtmp,"%s",reclin);
         fflush(fpdocgtmp);
         }
      else
         {
         keepgo = FALSE;
         }
      }
 
 /* Remove marg from the array, put array in temporary doc file */
 ptr     = dbuf;
 openit  = FALSE;
 markers = 0;
 keepgo = TRUE;

 for (i= 0; i < markdoc  && keepgo; i++)
     {
     if ((*ptr) > 1 && ((i+1) != markg)) 
        {
        /*     This marker exists and is still wanted */
        markers++;
        dlist[0] = i+1;
        dlist[1] = *(ptr + 1);
        dlist[2] = *(ptr + 2);
        ptr  += maxreg; 
        fpdocgtmp = savdn1(docnamg, datexc, &fpdocgtmp, dlist, 3, 
                  &openit, TRUE, FALSE);
        if (!fpdocgtmp)
           {
           spout("*** Error writing docfile.");
           markdoc = 0;
           keepgo  = FALSE;
           }
        }
     }
 fflush(fpdocgtmp);

 if (dbuf) free(dbuf); dbuf = NULL; 

 /* Close the new doc file */
 if (fpdocgtmp) fclose(fpdocgtmp); fpdocgtmp = NULL;

 /* Close the old doc file */
 if (fpdocg) fclose(fpdocg); fpdocg = NULL;

 if (keepgo) 
   { /* copy succeeded */

   /* Move temp doc file to old doc file name */
   strcpy(outstr,"mv ");
   strcat(outstr,"web_temp_jnk12345 ");
   strcat(outstr,docnamepe);
   system(outstr);
   }

 /* Open new doc file with original doc file name */
 openit = TRUE;
 fpdocg = savdn1(&docnamg[0], datexc, &fpdocg, dlist, 0, 
                  &openit, TRUE, FALSE);

 return markers;
 }

