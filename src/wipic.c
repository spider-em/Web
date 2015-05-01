
/*$Header: /usr8/web/src/RCS/wipic.c,v 1.16 2011/09/21 13:42:19 leith Exp $*/

/***********************************************************************
 *
 * wipic.c
 *
 ***********************************************************************
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
 ***********************************************************************
 *
 * wipic(icontx, cbuf,  ixul,iyul, nsam,nrow, showit, keepimage, image)
 * 
 * PURPOSE:    Displays an image 
 *
 * RETURNS:    
 * 
 * PARAMETERS:  keepimage  deallocate XImage structure if false.   
 *
 ***********************************************************************
*/

#include "common.h"

 /* Common variables used here */
 extern Display       *idispl;
 extern Screen        *iscreen;
 extern Visual        *visual;
 extern Window        iwin;
 extern Pixmap        imagsav;

 /*******************  wipic   **************************/

 void wipic(GC icontx, unsigned char * cbuf, 
              int ixul, int iyul, int nsam, int nrow, int showit,
              int keepimage, XImage ** imaget)
 { 
 XImage * image;

 int   i, j, k, Bpp;
 char  *excbuf;

 Bpp = (int) pow(2, (DefaultDepthOfScreen(iscreen) / 8) - 1);
 //Bpp = (int) pow(2, 2);
 //Bpp = 4;

 excbuf = calloc(nsam * nrow * Bpp, sizeof(char));

 for (i=0; i<nrow; i++)

    for (j=0; j<nsam; j++)

      for (k=0; k<Bpp; k++)

         if (k == 3)
            excbuf[((i*nsam) + j)*Bpp + k] = 0;
         else
            excbuf[((i*nsam) + j)*Bpp + k] = cbuf[(i*nsam) + j];

 image = XCreateImage(idispl,visual,DefaultDepthOfScreen(iscreen),
            ZPixmap, 0, (char *) excbuf,
            (unsigned int) nsam, (unsigned int) nrow, Bpp*4, nsam*Bpp);

 if (showit)
    {
    XPutImage(idispl,iwin,   icontx,image,0,0,ixul,iyul,
              (unsigned int)nsam, (unsigned int)nrow); 

    XPutImage(idispl,imagsav,icontx,image,0,0,ixul,iyul,
              (unsigned int)nsam, (unsigned int)nrow); 
    }

 if (!keepimage) 
    {
    /* Do not deallocate data memory ever!! XDestroyImage will!! */
    if (image) XFree((void *)image); 
    image = NULL;  
    }

 neednewmap = TRUE;

 *imaget = image;
 }

