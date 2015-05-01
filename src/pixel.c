
/*$Header: /usr8/web/src/RCS/pixel.c,v 1.18 2011/05/17 17:50:08 leith Exp $*/
/*
 * pixel.c
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
 * PURPOSE:  Find location of pixel      
 * 
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* Internal functions */
 void    pixel_pop (Widget, XEvent *, String *, Cardinal *);

 /* External variables */
 extern XImage  *  imagep;
 extern char       outstr[80];
 extern int        ixreg, iyreg, isreg, ivreg;
 extern int        pixelmen_showing;
 extern FILEDATA   *filedatap;

 /* Internal common variables */
 FILE             *fpdocpix = 0;
 int		  *pixellist;
 int	          pixelnum;

 /* Static varables */
 static int       openitp = TRUE;
 static int	  bufsiz  = 0;

/***************************  pixel  *********************************/

 void pixel(void)
 {
 
 openitp  = TRUE;
 pixelnum = 0;

 /* Open message window with the following strings  */
 showbutx("Query location.", 
          "Menu.", 
          "Stop this routine.", FALSE);

 /* Set actions for right, left, and center buttons */
 actions(iw_win, "pixel_pop", pixel_pop,"M123");

 }


 /************************* pixel_pop ********************************/

 void pixel_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 int   ixi, iyi, ixs, iys, ipix, irect, i, savdocit, inside;
 float fval;
 float dlist[7];

 if (!(strcmp(*params, "M")))
    {                           /* List location ------- mouse moved */
    getloc(event,'m',&ixs,&iys);
    if (inimag) 
       {     /* Find location relative to last image */
       ixi    = ixs - ixul + 1;
       iyi    = iys - iyul + 1;
       inside = iskipadj(ixi,iyi,nsam,nrow,iskip,&ixi,&iyi);
       }
    else
       {
       ixi    = ixs;
       iyi    = iys;
       }
       


    if (inimag && !inside)
       {    /* Cursor is outside of image, only want inside */
       if (getscreen)
          {      /* List location and screen value anyway */
          if (iys > (ihighx - iyulx))
             {
             sprintf(outstr,"*** Outside: (%d,%d) = (off screen)$",
                    ixi,iyi);
             }
          else
             {
             ipix = XGetPixel(imagep,ixs-ixulx,iys-iyulx);
             sprintf(outstr,"Outside: (%d,%d) = %d$",ixi,iyi,ipix);
             }          
          }
       else
          sprintf(outstr,"*** Outside: (%d,%d)$", ixi,iyi);
       }

    else if (getscreen)
       {      /* Cursor inside image and want screen value */     
       if (iys > (ihighx - iyulx))
          sprintf(outstr,"Location: (%d,%d) = (off screen)$",ixi,iyi);
       else
          {
          ipix = XGetPixel(imagep,ixs-ixulx,iys-iyulx);
          sprintf(outstr,"Location: (%d,%d) = %d$",ixi,iyi,ipix);
          }
       }

    else if (inimag)
       /* Want position relative to image*/
       sprintf(outstr,"Location in image: (%d,%d)$", ixi,iyi);

    else
       /* Cursor inside image*/
       sprintf(outstr,"Screen location: (%d,%d)$", ixi,iyi);

    spout(outstr);
    } 

 else if (!(strcmp(*params, "1")))
    {                     /*  List location -------- button 1 pushed */
    //printf("gettinglocation: \n");
    getloc(event,'B',&ixs,&iys);

    //printf("Screen location: (%d,%d) \n", ixs,iys);

    if (inimag) 
       {     /* Find location relative to last image */
       ixi    = ixs - ixul + 1;
       iyi    = iys - iyul + 1;
       inside = iskipadj(ixi,iyi,nsam,nrow,iskip,&ixi,&iyi);
       }
    else
       {
       ixi    = ixs;
       iyi    = iys;
       }
    //printf("location ii: (%d,%d) \n", ixi,iyi);

    if (inimag && !inside)
       {    /* Cursor is outside of image, only want inside */
       sprintf(outstr,"*** Outside image: (%d,%d)", ixi,iyi);
       spout(outstr);
       }

    else 
       {   /* Want to record this location */

       if (getfile)
          {    /* want value in image at this location */
          irect = (islice -1) * nrow + iyi -1;

          if (redlin(filedatap,filedatap -> buf,nsam,irect) == FALSE) return;
          fval = filedatap -> buf[ixi -1];
          }

       if (getfile && getscreen)
          /* Record location, screen and file values */
          {
          if (iys > (ihighx - iyulx))
             {
             sprintf(outstr,"Key:%d  (%d,%d) = (off screen) from:%f",
                      ikey,ixi,iyi,fval);
             }
          else
             {
             ipix = XGetPixel(imagep,ixs-ixulx,iys-iyulx);
             sprintf(outstr,"Key:%d  (%d,%d) = %d from:%f",
                            ikey,ixi,iyi,ipix,fval);
             } 
          }

      else if (getfile)
          /* Record location and file values */
          sprintf(outstr,"Key:%d  (%d,%d)   from:%f",ikey,ixi,iyi,fval);

      else if (getscreen)
          /* Record location and  screen values */
         if (iys > (ihighx - iyulx))
             {
             sprintf(outstr,"Key:%d  (%d,%d) = (off screen)",
                     ikey,ixi,iyi);
             }
          else
             {
             ipix = XGetPixel(imagep,ixs-ixulx,iys-iyulx);
             sprintf(outstr,"Key:%d  (%d,%d) = %d", ikey,ixi,iyi,ipix);
             }
      else 
          /* Record location */
          sprintf(outstr,"Key:%d  (%d,%d)",ikey,ixi,iyi);

      spout(outstr);

       if (docit)
          {  /* Save info in doc file */ 

          /*     put this marker in the file */
          for (i=1; i <=6; i++) dlist[i] = 0.0;

          dlist[0] = ikey;
          dlist[ixreg] = ixi;
          dlist[iyreg] = iyi;
          if (getscreen) dlist[isreg] = ipix;
          if (getfile)   dlist[ivreg] = fval;  
          fpdocpix    = savdn1(docnam, datexc, &fpdocpix, dlist, 7, 
                     &openitp, TRUE, TRUE);
          }

       if (leavit)
          {
	  /* Check pixel buffer size, if not enough add more */
 	  if (pixelnum >= bufsiz-1) {
             if ((pixellist = (int *) realloc(pixellist, (bufsiz+400)*
                    	sizeof(int))) == NULL)
             {
             spout("*** Unable to reallocate pixelbuf memory in pixel.c");
             return;
             }
	     bufsiz = bufsiz + 400;
	  }
	     
	  /* Record the pixel number */
	  pixellist[pixelnum] = ixi;		pixelnum++;
	  pixellist[pixelnum] = iyi;		pixelnum++;

	  /* Leave permanent circle at this location */
          xorc(iwin,    icontx, TRUE, ixs, iys, iradius);
          xorc(imagsav, icontx, TRUE, ixs, iys, iradius);
          }
       }
       ikey++;
    }

 else if (!(strcmp(*params, "2")))
    {                    /* Show menu --------       Button 2 pushed */
    /* Kludge to prevent file name incrementing */
    savdocit         = docit; docit = FALSE; 
    pixelmen_showing = TRUE;
    docit            = savdocit;
    pixelmen(NULL, NULL, NULL); 
    }

 else if (!(strcmp(*params, "3")))
    {                    /* Stop routine -------    Button 3 pushed */
    pixelmen_showing = TRUE;
    pixelmen_buts(NULL,NULL,NULL);
    pixelnum = 0;
    }
 }
