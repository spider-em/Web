
/*$Header: /usr16/software/web/src/RCS/winfile.c,v 1.11 2018/12/07 17:06:44 leith Exp $*/
/*
 C *********************************************************************
 C
 C WINFILE  -- 
 C             CONVERTED TO C -- SEPT 92 al
 C *********************************************************************
 C * AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 C  WINFILE()
 C
 C  PURPOSE:      READSBACK WINDOW, STORES CORRESPONDING 
 C                AREA FROM AN IMAGE FILE IN A NEW IN A IMAGE FILE
 C
 C  PARAMETERS:   NONE
 C
 C  CALLED BY:    WINSEL
 C
--*********************************************************************
*/

#include <Xm/SelectioB.h>

#include "common.h"
#include "routines.h"
#include "extras.h"

 /* internal function prototypes */
 static void winfile_cb(Widget, XtPointer, XtPointer);

 /* external common variables */

 /* file variables */
 static char     prompt[] = "Enter window file name"; 
 static Widget   iw_text  = (Widget)  0;

/************************  winfile  **********************************/

 void winfile(void)
 {
 int nlet, numwin;


if (strlen(filnow) <= 0)
    {
    spout("*** MUST DISPLAY IMAGE FIRST");
    return;
    }

 if (strlen(winnam) > 0) 
    {   /* increment window file name */
    filinc(winnam, winnam, &nlet, &numwin);
    }

 /* create text input box */
 iw_text = wid_text(iw_win, iw_text, prompt, &winnam[0],
                    winfile_cb, NULL, ixulw+nsamw, iyulw+nroww);
 }



 /*************  winfile_cb  (runs after wid_text) *****************/

 void winfile_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 float       *buf, fiskip;
 FILEDATA    *filedatao, *filedatan;
 int         iform, ixi, iyi, nsamf, nrowf;

 XmSelectionBoxCallbackStruct * cbs;

 cbs = (XmSelectionBoxCallbackStruct *) call_data;

 strcpy(winnam,extractstr(cbs -> value));

 /* redraw image in case damaged by window name box */ 
 expos_cb(NULL,NULL,NULL);

 /* open existing image file for windowing */
 if ((filedatao = openold(filnow, &nsam, &nrow, &nslice, &iform, "o")) ==
      (FILEDATA *) NULL)  return;

 /* find corner locations relative to displayed image */
 ixi   = ixulw - ixul + 1;
 iyi   = iyulw - iyul + 1;
 nsamf = nsamw;
 nrowf = nroww;

 if (iskip > 1) 
    {    /* window is from a reduced size image */
    ixi   = ixi   * iskip;
    iyi   = iyi   * iskip;
    nsamf = nsamf * iskip;
    nrowf = nrowf * iskip;
    }

 else if (iskip < 1)
    {   /* window is from expanded size image */
    fiskip = 1.0 / iskip;
    ixi    = ixi   * fiskip;
    iyi    = iyi   * fiskip;
    nsamf  = nsamf * fiskip;
    nrowf  = nrowf * fiskip;
    }

 /* add datexc to winnam if not already there */
 if (strstr(winnam,datexc) == 0)
    {
    strcat(winnam,".");
    strcat(winnam,datexc);
    }

 /* open new output file for window */
 if ((filedatan = opennew(winnam, nsamf, nrowf, 1, 1, "u")) == 
     (FILEDATA *) NULL)
    {closefile(filedatao); winfile(); return; }

 /* set wait cursor */
 waitcursor(iwin, TRUE, 79);

 /* do the windowing */
 windowsp(filedatao, filedatan, nsam, nrow, nslice, 
        ixi, iyi, 1, nsamf, nrowf, 1, filedatao -> fmin_spi);

 /* close the input and output files */
 closefile(filedatao);
 closefile(filedatan);

 /* reset wait cursor */
 waitcursor(iwin, FALSE, 79);

 /* list window positons */
 windata(ixi, iyi, nsamf, nrowf);
 
 if (iwintyp != 3)
    {   /* fixed size or interactive window */
    winmov();
    }
 winsiz1();
 }



