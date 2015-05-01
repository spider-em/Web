
/*$Header:    $*/
/*
C **********************************************************************
C
C WINREAD  -- CREATED MAY 91 FROM WIREAD al
C             CONVERTED TO C -- SEPT 92 al
*               24 bit display bug fixed ArDean Leith         11/8/07
C **********************************************************************
C *  AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2007  Health Research Inc.                     *
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
C  WINREAD()
C
C  PURPOSE:      Readsback window, stores in a image file
C
C  PARAMETERS:   None
C
C  CALLED BY:    WINSEL    MASKMEN
C
--**********************************************************************
*/

#include <Xm/SelectioB.h>
#include "common.h"
#include "routines.h"

 /* internal functions */
 void winread_cb(Widget, XtPointer, XtPointer);

 /* External common variables */
 extern char      outstr[80];

 /* File variables */
 static char     prompt[] = "Enter window file name"; 
 static Widget   iw_text  = (Widget) 0;
  
/************************  winread  *********************************/

 void winread(void)
 {
 int nlet, numwin;

 if (strlen(winnam) > 0) 
    {   /* increment window file name */
    filinc(winnam, winnam, &nlet, &numwin);
    }

 /* Create text input box */
 iw_text = wid_text(iw_win, iw_text, prompt, winnam,
                    *winread_cb, &winnam[0],
                    ixulw+nsamw, iyulw+nroww);
 }

 /*************  winread_cb  (Runs after ?) *****************/

 void winread_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 char * string;
 int    iplanes;
 int    iwintyp = 0;

 XmSelectionBoxCallbackStruct * cbs;

 cbs    = (XmSelectionBoxCallbackStruct *) call_data;

 string = extractstr(cbs -> value); 
 strcpy(winnam,string);
 XtFree(string);

 iplanes = pow(2,16) - 1;            // For image ramp
 
 winread1(winnam,TRUE,iwintyp,iplanes);
 }

 /*************  winread1  (Runs after winread_cb) ******************/

 void winread1(char * winnamt,int zmap, int iwintyp,  unsigned long ipmask)
 {
 XImage         *imagerec = NULL;
 float          *buf, value;
 FILEDATA       *filedatat = NULL;
 int            i, j, k, ipad, itemp, ilen;
 char           *text;
 unsigned char  cit;

/* Add datexc to winnam if not already there */
 if (strstr(winnamt,datexc) == 0)
    {
    strcat(winnamt,".");
    strcat(winnamt,datexc);
    }

 /* Open new output file for window */
 filedatat = opennew(winnamt, nsamw, nroww, 1, 1, "u");
 if (filedatat == (FILEDATA *) NULL) return;

 /* Redraw image in case damaged by window name box */ 
 //expos_cb(NULL,NULL,NULL);

 /* Set wait cursor */
 waitcursor(iwin, TRUE, 79);

 if (zmap)
    {
    //printf(" ipmask: %#x = %d,  zmap T: %d \n",ipmask,ipmask,zmap);
    imagerec = XGetImage(idispl, imagsav, ixulw,iyulw, nsamw,nroww,
                         ipmask, ZPixmap);
    }
 else
    {
    printf(" ipmask: %#x = %d,  zmap F: %d \n",ipmask,ipmask,zmap);

    imagerec = XGetImage(idispl, imagsav, ixulw,iyulw, nsamw,nroww,
                         ipmask, XYPixmap);
    }

 if (imagerec == (XImage *) NULL) 
    { spout("*** Image formation failed");  return; }

 /* X images are padded to units of four bytes */
 ipad = nsamw % 4;
 if (ipad > 0) ipad = 4 - ipad;

 /* Use default read / write buffer from filedatat */
 buf = filedatat -> buf;

 if (!zmap)
    {
    /* Convert single bit image to spider format */
    ilen = imagerec -> bytes_per_line;
    for (i=1; i <= nroww; i++)
       {
       itemp = (i-1) * ilen;

       for (j=0; j < nsamw; j++)
          {
          /* Recover the relevant character of line every 8 cols */
          /* USED TO BE: if defined(__osf__) || defined(VMS) jan 2009 al*/
 
#ifdef __BYTE_ORDER
         //printf(" defined __BYTE_ORDER \n"); YES on x86-64
#    if __BYTE_ORDER == __LITTLE_ENDIAN
         //printf(" defined __LITTLE_ENDIAN \n");  YES on x86-64

         if (!(j%8))
              {
              cit    = imagerec -> data[itemp++];
              buf[j] = cit & 1; 
              // if (buf[j] != 0) printf("loc: %4d %4d %f \n",i,j,buf[j]);  
              }
          else
              { 
              cit    = cit >> 1;        
              buf[j] = cit & 1 ;
              }
#else
          if (!(j%8))
              {
              cit    = imagerec -> data[itemp++];
              buf[j] = (cit & 128) >> 7;   
              }
          else
              { 
              cit    = cit << 1;        
              buf[j] = (cit & 128) >> 7;
              }

#endif

#else
          if (!(j%8))
              {
              cit    = imagerec -> data[itemp++];
              buf[j] = (cit & 128) >> 7;   
              }
          else
              { 
              cit    = cit << 1;        
              buf[j] = (cit & 128) >> 7;
              }
#endif

          }
       wrtlin(filedatat, buf, nsamw, i);
       }
    }

 else if (zmap)
    {
    /* Convert image to spider format */
    k = 0;
    for (i=1; i <= nroww; i++)
       {
       for (j=0; j < nsamw; j++)
          {
          value  = (unsigned char) imagerec->data[k++];
          buf[j] = value;
          }
       wrtlin(filedatat, buf, nsamw, i);
       k = k + ipad;
       }
    }

 sprintf(outstr,"A %d x %d window recorded from: (%d,%d)\n",
                        nsamw,nroww,ixulw,iyulw);
 spout(outstr);

 closefile(filedatat); filedatat = NULL;

 /* Free image memory.  This deallocates image buffer also */
 XDestroyImage(imagerec);

 /* Reset wait cursor */
 waitcursor(iwin, FALSE, 79);

 if (iwintyp != 3)
    {   /* Fixed size or interactive window */
    winmov;
    }

 }








