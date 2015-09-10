
/*
 C++********************************************************************
 C
 C tiltp_deletemen.c
 C
 C**********************************************************************
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
 C**********************************************************************
 C
 C  tiltp_deletemen
 C
 C  PURPOSE:     Display tiltp_deletemen menu for deleting particle pair
 C               from tilt-pair particle picking doc files. Delete 
 C               unwanted particles and renumber consecutively.
 C
 C  CALLED BY:   pickp
 C
 C  CALL TREE:   pickp
 C                 v
 C               tiltp_deletemen       
 C                 v
 C               tiltp_deletemen_buta   
 C                 v
 C               tiltp_delete         --> getdoc_f       
 C                                    --> savdnc
 C                                    --> savdn1
 C                                    --> det_tilt
 C                                    --> pickmen
 C                                    --> fitdoc
 C                                    --> pickdisp
 C                                    --> pickdraw  
 C                 v
 C               pickp (continues)
 C
 C**********************************************************************
*/

#include <Xm/Text.h>
#include "common.h"
#include "routines.h"

 /* Externally defined function prototypes */

 /* Internally defined function prototypes */
 void          tiltp_deletemen     (Widget, XtPointer, XtPointer);
 void          tiltp_deletemen_buta(Widget, XtPointer, XtPointer);
 void          tiltp_deletemen_butc(Widget, XtPointer, XtPointer);
 int           tiltp_delete        (int);

 /* External common global variables used here */
 extern char   dfil1[12];       // Doc file 
 extern char   dfil2[12];       // Doc file 
 extern char   strcom1[];       // Doc file comment
 extern char   strcom2[];       // Doc file comment
 extern int    numm ;           // From: fitdoc
 extern int    maxpart;

 /* Common global variables defined here */

 /* Internal file scope  variables*/
 static Widget  iw_tiltp_deletemen = (Widget)0;
 static Widget  iw_numdel;

/*********************************** tiltp_deletemen **********************/

 void tiltp_deletemen(Widget iw_temp, XtPointer data, 
                                      XtPointer call_data)
 { 
 char     cval9[9];
 Widget   iw_rowcol;
 Widget   iw_pushc, iw_pusha, iw_dums;

 if (iw_tiltp_deletemen <= (Widget)0)
    {   /* Create delete menu widget first */

    iw_tiltp_deletemen = wid_dialog(iw_win,iw_tiltp_deletemen,
                           "Delete particle",20,20);
    iw_rowcol    = wid_rowcol(iw_tiltp_deletemen,'v',-1,-1);

    /* Create text box for numdel ----------------------------- numdel */
    sprintf(cval9,"%d",maxpart);     // Use current particle number
    iw_numdel = wid_textboxb(iw_rowcol,iw_numdel,
                          "Particle:",cval9,9);

    /* Create accept boxes  ----------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    wid_stdbut(iw_rowcol, iw_tiltp_deletemen, 
               &iw_dums, &iw_pushc, &iw_pusha, 
               "CD",
               fin_cb,tiltp_deletemen_butc,
               tiltp_deletemen_buta, NULL);
    }

 else
    {   /* Delete menu widget already exists, update it */

    /* Alter text box for numdel ----------------------------- numdel */
    sprintf(cval9,"%d",maxpart);    // Current particle number
    iw_numdel = wid_textboxb(iw_rowcol,iw_numdel,
                            "Particle:",cval9,9);
    }

 XtManageChild(iw_tiltp_deletemen);
 }


 /********************* Cancel button callback *****************/

 void tiltp_deletemen_butc(Widget iw_temp, XtPointer data, 
                                           XtPointer call_data)
 {
 /* Remove the tiltp_deletemen menu*/
 XtUnmanageChild(iw_tiltp_deletemen);  
 }

 /********************* Delete accept button callback *****************/

 void tiltp_deletemen_buta(Widget iw_temp, XtPointer data, 
                                           XtPointer call_data)
 {
 char * string;
 int    numdel;

 /* Find deleted particle number */
 string = XmTextGetString(iw_numdel);
 sscanf(string,"%d",&numdel);
 if (string) free(string);

 /* Remove the tiltp_deletemen menu */
 XtUnmanageChild(iw_tiltp_deletemen); 
 
 //printf("numdel : %d %d \n",numdel,maxpart);

 if (numdel > 0 && numdel <= maxpart) tiltp_delete(numdel);
 }


 /************************ tiltp_delete ****************************/

 int tiltp_delete(int numdel)
 {
 int     i, ipart;
 float   dlist[8];
 char    outstr[240];
 float * ptri1;
 float * ptri2;
 float * dbuf1       = NULL;    /* Doc input buffer */
 float * dbuf2       = NULL;    /* Doc input buffer */
 FILE  * fpdoc1      = NULL;
 FILE  * fpdoc2      = NULL;
 int     openit1     = TRUE; 
 int     openit2     = TRUE; 
 int     maxreg      = 6+1;     /* Set size for array pointed to by dbuf */
 int     maxkeys     = 9999;    /* Set size for array pointed to by dbuf */

 int     highestkey1 = 0;
 int     highestkey2 = 0;
 int     lowestkey   = 0;
 int     igot        = 0;

 //printf("Deleting: %d \n", numdel); 

 /* Retrieve data from dfil1 for untilted points */
 
 if (getdoc_f((FILE *) NULL, dfil1, datexc,maxkeys,maxreg, 
               &dbuf1, &highestkey1, FALSE) > 0) 
    {             /* Problem retrieving doc file */
    if (dbuf1) free(dbuf1); dbuf1 = NULL; 
    spouts("*** Unable to read untilted particle location doc file: "); spout(dfil1); 
    return 1;
    }

 /* Retrieve data from dfil12 for tilted points */
 
 if (getdoc_f((FILE *) NULL, dfil2, datexc,maxkeys,maxreg, 
               &dbuf2, &highestkey2, FALSE) > 0) 
    {             /* Problem retrieving doc file */
    if (dbuf2) free(dbuf2); dbuf2 = NULL; 
    spouts("*** Unable to read tilted particle location doc file: "); spout(dfil2); 
    return 1;
    }
 
 /* Move original dfil1 to backup location dfil1~ */
 strcpy(outstr,"mv ") ; 
 strcat(outstr,dfil1)  ; strcat(outstr,".") ; strcat(outstr,datexc); 
 strcat(outstr," ")   ;
 strcat(outstr,dfil1)  ; strcat(outstr,".") ; strcat(outstr,datexc); strcat(outstr,"~") ;
 system(outstr);
 //printf("outstr : %s \n",outstr);

 /* Move original dfil2 to backup location dfil2~ */
 strcpy(outstr,"mv ") ; 
 strcat(outstr,dfil2)  ; strcat(outstr,".") ; strcat(outstr,datexc); 
 strcat(outstr," ")   ;
 strcat(outstr,dfil2)  ; strcat(outstr,".") ; strcat(outstr,datexc); strcat(outstr,"~") ;
 system(outstr);

 // Put opening comment in doc file: dfil1
 fpdoc1    = savdnc(dfil1, datexc, &fpdoc1, dlist, 0, &openit1, TRUE, TRUE, strcom1);
 fpdoc2    = savdnc(dfil2, datexc, &fpdoc2, dlist, 0, &openit2, TRUE, TRUE, strcom2);

 lowestkey = highestkey1;
 if (highestkey2 < lowestkey) lowestkey = highestkey2; 

 dlist[6] = 1.0;
 
 ptri1    = dbuf1;
 ptri2    = dbuf2;

 for (i= 0; i < lowestkey; i++)
    {
    if (*ptri1 > 0 && *ptri2 > 0) 
       { // A particle exists on both sides

       ipart = i + 1;       // Particle number
       if (ipart  != numdel) 
          { // Non-deleted particle exists on both sides, save info in doc files

          igot++;

          dlist[0] = igot;         /* Key = particle number */
          dlist[1] = igot;         /* Also particle number  */
          dlist[2] = *(ptri1+2);   /* Unreduced untilted x location */
          dlist[3] = *(ptri1+3);   /* Unreduced untilted y location */
          dlist[4] = *(ptri1+4);   /* Reduced   untilted x location */ 
          dlist[5] = *(ptri1+5);   /* Reduced   untilted y location */ 

          //printf("dlist : %f %f %f %f\n",dlist[0],dlist[1],dlist[2],dlist[3],dlist[4],dlist[5]);

          fpdoc1   = savdn1(dfil1, datexc, &fpdoc1,
                            dlist, 7, &openit1, TRUE, TRUE);

          dlist[2] = *(ptri2+2);   /* Unreduced tilted x location   */
          dlist[3] = *(ptri2+3);   /* Unreduced tilted y location   */
          dlist[4] = *(ptri2+4);   /* Reduced   tilted x location   */ 
          dlist[5] = *(ptri2+5);   /* Reduced   tilted y location   */ 

          fpdoc2   = savdn1(dfil2, datexc, &fpdoc2,
                            dlist, 7, &openit2, TRUE, TRUE);

          if (!fpdoc1 || !fpdoc2)
             { 
             spouts("*** Error writing docfiles: "); 
             spouts(dfil1) ; spouts("  & ") ; spout(dfil2); 

             if (fpdoc1) fclose(fpdoc1); fpdoc1 = NULL;
             if (fpdoc2) fclose(fpdoc2); fpdoc2 = NULL;

             /* Recover original dfil1 from backup location dfil1 */
             strcpy(outstr,"mv ")  ; 
             strcat(outstr,dfil1)  ; strcat(outstr,".") ; strcat(outstr,datexc); strcat(outstr,"~");
             strcat(outstr," ")    ;
             strcat(outstr,dfil1)  ; strcat(outstr,".") ; strcat(outstr,datexc); 
             system(outstr);

             /* Recover original dfil2 from backup location dfil2 */
             strcpy(outstr,"mv ")  ; 
             strcat(outstr,dfil2)  ; strcat(outstr,".") ; strcat(outstr,datexc); strcat(outstr,"~");
             strcat(outstr," ")    ;
             strcat(outstr,dfil2)  ; strcat(outstr,".") ; strcat(outstr,datexc); 
             system(outstr);
             return 1;
             }  // End of: if (!fpdoc1 || !fpdoc2)
          }     // End of: if (i  != numdel) 
       }        // End of: if (*ptri1 > 0 && *ptri2 > 0)

    ptri1 += maxreg;
    ptri2 += maxreg;
    }

 /* Free allocated memory */
 if (dbuf1) free(dbuf1); dbuf1 = (float *) NULL;
 if (dbuf2) free(dbuf2); dbuf2 = (float *) NULL;

 /* Close the new doc files */
 if (fpdoc1) fclose(fpdoc1); fpdoc1 = NULL;
 if (fpdoc2) fclose(fpdoc2); fpdoc2 = NULL;

 maxpart   = igot;
 numm      = maxpart + 1;   // Usual next particle

 // Determine new angles using new data set. 
 det_tilt(FALSE,TRUE);

 // Update label box for next particle number (numm) 
 pickmen();

 /* Recalculate tilted and untilted points and fit angles */
 fitdoc(FALSE);

 /* Redisplay images without labels at current shifted location */
 pickdisp(TRUE,TRUE);
   
 /* Draw new particle numbers & their locations */
 pickdraw(TRUE, TRUE, FALSE, TRUE, FALSE, maxpart);

 return 0;
 }
