
/*$Header: /usr8/web/src/RCS/pixel_in.c,v 1.21 2015/09/08 18:09:02 leith Exp $*/

/*
 C**********************************************************************
 C pixel_in.c
 C              Pixel rewrite                   Jun 2011 ArDean Leith
 C              Doc file close bug              Jul 2015 ArDean Leith
 C              Bell                            Sep 2015 ArDean Leith
 C
 C**********************************************************************
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
 C PURPOSE:  Find location of pixel inside image     
 C           Optionally save location in doc file
 C
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* Internal function prototypes */
 void              pixel_in_pop   (Widget, XEvent *, String *, Cardinal *);

 // Externally defined common variables 
 extern char       outstr[80];
 extern FILEDATA * filedatap;

 extern int        ixreg_pix, iyreg_pix;    // From: pixelmen_sc
 extern int        isreg_pix, ivreg_pix;    // From: pixelmen_sc
 extern int        iradius_pix;             // From: pixelmen_sc 
 
 extern XImage *   imagep_in;               // From: pixelmen_in
 extern int        docit_in;                // From: pixelmen_in 
 extern int        getscreen_in;            // From: pixelmen_in 
 extern int        leavit_in;               // From: pixelmen_in 
 extern int        ikey_in;                 // From: pixelmen_in 

 extern float *    fimage;                  // From: imagemen 

 /* Internally defined common  variables */
 FILE *            fpdocpix_in = NULL;
 int *             pixellist;
 int	           pixelnum;

 /* Static variables */
 static int        openitp   = TRUE;
 static int	   bufsiz    = 0;
 static char       comment[] = "                X            Y       Screen         File"; 

/***************************  pixel_in  *********************************/

 void pixel_in(void)
 {
 
 openitp  = TRUE;
 pixelnum = 0;

 /* Open message window with the following prompts  */
 showbutx("Query location.", 
          "Show menu.", 
          "Stop this routine.", FALSE);

 /* Set actions for right, left, and center buttons */
 actions(iw_win, "pixel_in_pop", pixel_in_pop,"M123");
 }


 /************************* pixel_in_pop ********************************/

 void pixel_in_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {
 int   ixi, iyi, ixs, iys, ipix, irect, i, savdocit, inside;
 int   iloc;
 float fpix;
 float dlist[7];

 getloc(event,'m',&ixs,&iys);
 
 /* Find location relative to last displayed image */
 ixi    = ixs - ixul + 1;
 iyi    = iys - iyul + 1;
 inside = iskipadj(ixi,iyi,nsam,nrow,iskip,&ixi,&iyi);

 if ( inside ) 
    {    // Inside image,  query file value 
    iloc = (islice -1)*nrow*nsam + (iyi-1)*nrow + ixi -1;
    fpix = fimage[iloc];
    }

 if (getscreen_in  && (iys < ihighx ) ) 
    {    /* Query screen value */
    ipix = XGetPixel(imagep_in, ixs, iys);
    if (inside) 
        ipix = (ipix * 255 / 65535) - imagego ;
    else
        ipix = (ipix * 255 / 65535) ;

    }

 if (!(strcmp(*params, "M")))
    {                           /* List location ------- mouse moved */

    if (!inside && getscreen_in && (iys < ihighx) )
       {       /* Cursor outside of image, only want inside */
       sprintf(outstr,"Outside: (%d,%d) = %d$",ixi,iyi,ipix);          
       }

    else if (inside && getscreen_in )
       {      /* Cursor inside image and want screen value */     
       sprintf(outstr,"Location: (%d,%d) = %d  Image value: %f At: %d$",
                         ixi,iyi,ipix,fpix,iloc);  
       }

    else if ( inside )
       {     /* Want position relative to image*/
       sprintf(outstr,"Location in image: (%d,%d)  Image value: %f At: %d$", 
                       ixi,iyi,fpix,iloc);
       }
    spout(outstr);
    } 

 else if (!(strcmp(*params, "1")))
    {                     /*  List location -------- button 1 pushed */

    //printf("Screen location: (%d,%d) \n", ixs,iys);
    //printf("location ii: (%d,%d) \n", ixi,iyi);

    if (!inside)
       {    /* Cursor is outside of image, only want inside */
       sprintf(outstr,"*** Outside image: (%d,%d)", ixi,iyi);
       spout(outstr);
       XBell(idispl,50);
       }

    else 
       {   /* Inside.  Want to record location, etc */

       if (getscreen_in && (iys < ihighx) )
          {      /* Record location, screen and file values */
          sprintf(outstr,"Key: %d  (%d,%d) = %d  Image value: %f",
                            ikey_in,ixi,iyi,ipix,fpix);
          }
       else 
          {      /* Record location only */
          sprintf(outstr,"Key:%d  (%d,%d)",
                          ikey_in,ixi,iyi);
          }
      spout(outstr);

      if (docit_in)
          {  /* Save info in doc file */ 

          for (i=1; i <=6; i++) dlist[i] = 0.0;

          /*     Put this marker in the file */
          dlist[0]         = ikey_in;
          dlist[ixreg_pix] = ixi;
          dlist[iyreg_pix] = iyi;
          dlist[ivreg_pix] = fpix;  
          if (getscreen_in) dlist[isreg_pix] = ipix;

          if (openitp)

             fpdocpix_in = savdnc(docnam, datexc, 
                               &fpdocpix_in, dlist, 7, 
                               &openitp, TRUE, TRUE, 
                               comment);
          else
             fpdocpix_in = savdn1(docnam, datexc, 
                               &fpdocpix_in, dlist, 7, 
                               &openitp, TRUE, TRUE);
          }

      if (leavit_in)
          {
	  /* Check pixel buffer size, if not enough add more */
 	  if (pixelnum >= (bufsiz-1) )
             {
             if ((pixellist = (int *) realloc(pixellist, (bufsiz+400)*
                    	sizeof(int))) == NULL)
                {
                spout("*** Unable to realloc pixelbuf in pixel.c");
                XBell(idispl,50);  return;
                }
	     bufsiz = bufsiz + 400;
	     }
	     
	  /* Record the pixel number */
	  pixellist[pixelnum] = ixi;    pixelnum++;
	  pixellist[pixelnum] = iyi;    pixelnum++;

	  /* Leave permanent circle at this location */
          xorc(iwin,    icontx, TRUE, ixs, iys, iradius_pix);
          xorc(imagsav, icontx, TRUE, ixs, iys, iradius_pix);
          }
       }
       ikey_in++;
    }

 else if (!(strcmp(*params, "2")))
    {                    /* Show menu ------------  Button 2 pushed */
    /* Kludge to prevent file name incrementing */
    if (docit_in)
       pixelmen_in_doc(NULL, NULL, NULL);
    else
       pixelmen_in_nod();
    }

 else if (!(strcmp(*params, "3")))
    {                    /* Stop routine ---------  Button 3 pushed */
    pixelmen_in_buts(NULL,NULL,NULL);
    pixelnum = 0;

    if (fimage)
        { free(fimage); fimage = NULL; }
    }
 }
