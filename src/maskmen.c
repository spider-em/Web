
/*$Header: /usr16/software/web/src/RCS/maskmen.c,v 1.16 2018/12/07 17:03:32 leith Exp $*/
/*
 C+*********************************************************************
 C
 C WID_MASKMEN.FOR -- CREATED MAY 91
 C maskmen.c                                                   DEC 92
 C
 C *********************************************************************
 C=* AUTHOR:  ArDean Leith                                             *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email: spider@health.ny.gov                                       *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 C=* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  *
 C=* General Public License for more details.                          *
 C=*                                                                   *
 C=* You should have received a copy of the GNU General Public License *
 C=* with this program. If not, see <http://www.gnu.org/licenses>      *
 C=*                                                                   *
 C *********************************************************************
 C
 C maskmen
 C
 C PARAMETERS:  none    
 C
 C--*******************************************************************
*/

#include <Xm/ToggleBG.h>
#include <Xm/SelectioB.h>
#include "common.h"
#include "routines.h"
#include "mask.h"

 /* Internal function prototypes */
 static void   maskmen_butcl  (Widget, XtPointer, XtPointer);
 static void   maskmen_buts   (Widget, XtPointer, XtPointer);
 static void   maskmen_butshf (Widget, XtPointer, XtPointer);
 static void   maskmen_butn   (Widget, XtPointer, XtPointer);
 static void   maskmen1       (Widget, XtPointer, XtPointer);
 static void   maskmen1_buta  (Widget, XtPointer, XtPointer);
 static void   maskread       (char *, Pixmap, int, int, unsigned long);
  
 /* External common variables used here */

 /* File variables */
 static Widget   iw_maskmen  = 0;
 Widget          iw_maskmen1 = 0;
 char            masknam[81] = "\0";  

 // -------------------------   maskmen   --------------------------------------

 void maskmen(void)

 {
 Widget  iw_rowcolv, iw_pushs, iw_pushc, iw_pusha; 

 if (iw_maskmen == (Widget)0)
    {   /* Create masking menu first */

    iw_maskmen = wid_dialog(iw_win, 0, "Masking menu", -1, -1);

    iw_rowcolv = wid_rowcol(iw_maskmen, 'v', -1, -1);

                 wid_toggleg(iw_rowcolv,0,"Fill inside",filler,
                        toggle2_cb, &filler, -1,-1);

                 wid_pushg(iw_rowcolv, 0, "Shift the image",
                           maskmen_butshf, NULL, -1,-1);

                 wid_pushg(iw_rowcolv, 0, "Clear mask",
                     maskmen_butcl, (XtPointer) iw_maskmen, -1,-1);

                 wid_pushg(iw_rowcolv, 0, "Save mask",
                     maskmen1, NULL, -1,-1);

                 wid_pushg(iw_rowcolv, 0, "Next input image",
                     maskmen_butn, NULL, -1,-1);

    /* Create box for apply  */
    wid_stdbut(iw_rowcolv, iw_maskmen, 
               &iw_pushs, &iw_pushc, &iw_pusha, "SC",
               maskmen_buts, fin_cb, fin_cb, NULL);
 
    }
 XtManageChild(iw_maskmen);

 }

// -------------------   maskmen_butshf ------------ Shift button callback  

 void maskmen_butshf(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 // Cancel current masking buttons 
 XtUninstallTranslations(iw_win);

 // Remove the mask menu widget 
 XtUnmanageChild(iw_maskmen);

 XFlush(idispl);

 // Check translations setting in shift 
 shift(MASK);
 
 // printf(" drawing after shift \n");

 // Draw empty polygons on the backing store  
 mask_draw_polys(imagsav,    icontx, TRUE, FALSE);

 // printf(" shifted poly draw finished \n");

 //  Refresh screen  */
 expos_cb(NULL,NULL,NULL);
 }

// ------------------  maskmen_butcl ------------------- Clear masks callback 

 void maskmen_butcl(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int    ixs, iys;

 // Erase masks by redrawing image from the Ximage stuff  

 // Find current location of the image 
 ixs = ixul - ixullmin;
 iys = iyul - iyullmin;

 // Redraw the image (without mask on it) 
 XPutImage(idispl,imagsav,icontx, imagel, nsam1,nrow1,
           ixul,iyul, nsam-ixs, nrow-iys);

 XPutImage(idispl,iwin   ,icontx, imagel, nsam1,nrow1,
          ixul,iyul, nsam-ixs, nrow-iys);

 neednewmap = TRUE;

 // Zero mask indices, etc.
 mask_reset();

 //  Remove menu */
 XtUnmanageChild((Widget) data);

 //  Refresh screen  */
 expos_cb(NULL,NULL,NULL);
 }


// ----------------- maskmen_buts -------------------- Stop button callback 

 void maskmen_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 /* Add a trap here to be sure user wants to stop without storing */
 /******* !!!!!!! *****/

 /*  Remove message */
 showbutx("","","",TRUE);

 /*  Remove the menu widget */
 XtUnmanageChild(iw_maskmen);

 /*  Cancel menu,  stop masking mouse button assignments */
 XtUninstallTranslations(iw_win);

 /* Reset to current foreground color */
 wicolor(icontx,icolor);

 XFreePixmap(idispl,masksav);
 masksav = (Pixmap) NULL;
 //printf(" null in stop masksav: %d \n",masksav);
 }


// ---------------  maskmen_butn ---------------------- Next image callback 

 void maskmen_butn(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 int          nlet, numfili;

 /* Put in a trap here to be sure user wants to stop without storing */
 /******* !!!!!!! *****/

 /* Remove menu */
 XtUnmanageChild(iw_maskmen);

 /* Remove button assignment message */
 showbutx("","","",TRUE);             

 /* Refresh screen  */
 expos_cb(NULL,NULL,NULL);

 if (masksav != ((Pixmap) NULL)) XFreePixmap(idispl,masksav);
 masksav = (Pixmap) NULL;
 // printf(" null masksav in next image: %d \n",masksav);

 // Zero mask indices, etc. to start new mask
 mask_reset();

 /* Increment input image file name */
 filinc(filnow,filnext,&nlet,&numfili);

 /* Ask for next filename to be masked */
 imagemen(NULL,"6",NULL);  
 }

// ----------------  maskmen1 --------------------  Save mask button callback 

 void maskmen1(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 /* Create mask file name text input widget */
 iw_maskmen1 = wid_text(iw_win, iw_maskmen1, "Mask file", &masknam[0],
                        *maskmen1_buta, NULL, ixul+nsam, iyul+nrow);

 XtManageChild(iw_maskmen1);
 }


// -------------------  maskmen1_buta ---------- Mask filename accept callback 

 void maskmen1_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 int                            nlet,numfild;
 XmSelectionBoxCallbackStruct * cbs;
 char *                         string = NULL;
 unsigned long                  ipmask ;
 int                            iwintyp = 3;

 cbs    = (XmSelectionBoxCallbackStruct *) call_data;
 string = extractstr(cbs -> value);
 strcpy(masknam,string);
 if (string) XtFree(string);

 if (masknam[0] == '\0') 
    {   /*  This program requires an image file name */
    spout("*** You must enter a mask file name now!");
    return;
    }

 // Remove options menu 
 XtUnmanageChild(iw_maskmen);

 //  Remove mask file-name menu 
 XtUnmanageChild(iw_maskmen1);

 // Draw filled polygons on mask image with no shift offset 
 mask_draw_polys(masksav, icontxmask, FALSE, TRUE);

 // Store mask image in mask image file 
 ipmask  = -1;
 maskread(masknam, masksav, nsam, nrow, ipmask);
 
 // Increment mask image file name 
 filinc(masknam, masknam, &nlet, &numfild);

 // Zero mask indices, etc. to start new mask
 mask_reset();
 }


 // ----------------------  maskread   --------------------------------


 void maskread(char * winnamt, Pixmap mask, int nsamt, int nrowt,
               unsigned long ipmask)
 {
 float          *buf;
 FILEDATA       *filedatat = NULL;
 int            i, j, k, ipad, itemp, bpl, byord;
 unsigned char  cit;
 XImage         *imagerec = NULL;
 char           outstr[81];

/* Add datexc to winnam if not already there */
 if (strstr(winnamt,datexc) == 0)
    { strcat(winnamt,".");    strcat(winnamt,datexc);  }

 /* Open new output file for window */
 filedatat = opennew(winnamt, nsamt, nrowt, 1, 1, "u");
 if (filedatat == (FILEDATA *) NULL) return;

 /* Set wait cursor */
 waitcursor(iwin, TRUE, 79);

 //printf(" maskread recording masksav: %d Size: %d %d Planes: %#x \n",
 //         masksav,nsamt,nrowt,ipmask);
 
 imagerec = XGetImage(idispl, mask, 0,0, nsamt,nrowt,ipmask, XYPixmap);
 if (imagerec == (XImage *) NULL) 
    { spout("*** Mask image formation failed");  return; }

 /* X images are padded to units of four bytes */
 ipad = nsamt % 4;
 if (ipad > 0) ipad = 4 - ipad;

 /* Use default read / write buffer from filedatat */
 buf = filedatat -> buf;

 /* Convert single bit image to spider format */
 byord        = imagerec -> byte_order;
 bpl          = imagerec -> bytes_per_line;

 //int wt     = imagerec -> width;
 //int ht     = imagerec -> height;
 //int tdepth = imagerec -> depth;
 //int bu     = imagerec -> bitmap_unit;
 //printf("  byord: %d depth:%d bpl:%d, w: %d h: %d  bu: %d\n",
 //          byord,   tdepth,   bpl,    wt,  ht,     bu);

 int   jt, jexp;
 float vback = 0;
 float vmask = 1;
 if (!filler)
    {vback = 1; vmask = 0;}

 if (byord == 0)
    { 
    for (i=1; i <= nrowt; i++)
       {
       itemp = (i-1) * bpl;

       for (j=0; j < nsamt; j+=8)
          {
          /* Recover the relevant character of line every 8 cols */
          cit = imagerec -> data[itemp++];

          if (cit == 0) 
             {  // All background can set all to background
             for (jt = j; jt < (j + 8) && jt < nsamt; jt++)
                 { buf[jt] = vback;}
             continue;
             }           

          jt   = j ;
          jexp = 1 ;

          for (jt = j; jt < (j + 8) && jt < nsamt; jt++)
             {if ((cit & jexp) > 0) 
                { buf[jt] = vmask ; }
              else
                { buf[jt] = vback ; }
             jexp = jexp * 2;
             }
       } // End of: for (j=

       // Save data to file                                  
       wrtlin(filedatat, buf, nsamt, i);

       }  // End of: for (i=0; 
    }     // End of: if (byord == 0)

 else
    {
    // byte order
    for (i=1; i <= nrowt; i++)
       {
       itemp = (i-1) * bpl;

       for (j=0; j < nsamt; j+=8)
          {
          /* Recover the relevant character of line every 8 cols */
          cit = imagerec -> data[itemp++];

          if (cit == 0) 
             {  // All background
             for (jt = j; jt < (j + 8) && jt < nsamt; jt++)
                 { buf[jt] = vback;}
             continue;
             }           

          jt   = j ;
          jexp = 128 ;

          for (jt = j; jt < (j + 8) && jt < nsamt; jt++)
             {if ((cit & jexp) > 0) 
                { buf[jt] = vmask ; }
              else
                { buf[jt] = vback ; }
             jexp = jexp / 2;
             }
       } // End of: for (j=

       // Save data to file                                  
       wrtlin(filedatat, buf, nsamt, i);

       }  // End of: for (i=0; 
    }     // End of: if (byord == 0)

 sprintf(outstr,"Mask recorded:  %d x %d  \n",nsamt,nrowt);
 spout(outstr);

 closefile(filedatat); filedatat = NULL;

 /* Free image memory.  This deallocates image buffer also */
 XDestroyImage(imagerec);

 /* Reset wait cursor */
 waitcursor(iwin, FALSE, 79);
 }









