
/*$Header: /usr16/software/web/src/RCS/pixel_sc.c,v 1.21 2018/12/07 17:03:33 leith Exp $*/
/*
 C pixel_sc.c
 C               Pixel rewrite                   Jun 2011 ArDean Leith
 C               Doc file close bug              Jul 2015 ArDean Leith
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
 C PURPOSE:  Find location and value of pixel on screen    
 C 
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "pixel.h"

 // Internal functions 
 static void    pixel_sc_pop (Widget, XEvent *, String *, Cardinal *);

 // Externally defined common variables 



 // Internally defined common variables 
 FILE            * fpdocpix_sc = NULL;
 int		 * pixellist;             // Used in shift
 int	           pixelnum;              // Used in shift

 // Static file scope variables 
 static int        openitp = TRUE;
 static int	   bufsiz  = 0;

/***************************  pixel  *********************************/

 void pixel_sc(void)
 {
 
 openitp  = TRUE;
 pixelnum = 0;

 /* Open message window with the following strings  */
 showbutx("Query location.", 
          "Recall menu.", 
          "Stop this routine.", FALSE);

 /* Set actions for right, left, and center buttons */
 actions(iw_win, "pixel_pop", pixel_sc_pop,"M123");

 //printf(" ihighx,iwidex: (%d,%d) \n", ihighx,iwidex);
 //printf(" ixulx, iyulx:  (%d,%d) \n", ixulx, iyulx);
 }


 /************************* pixel_sc_pop ********************************/

 void pixel_sc_pop(Widget iw_t, XEvent *event, String *params,
                Cardinal *num_params)
 {
 int    ixs, iys, ipix, i, savdocit;
 float  fval;
 float  dlist[7];
 char   outstr[81];

 getloc(event,'m',&ixs,&iys);
 //printf("  %d,%d  \n",ixs,iys);   

 if (getscreen_sc  && (iys < ihighx ) ) 
    {    /* Query screen value */
    ipix = XGetPixel(imagep_sc, ixs, iys);
    //sprintf(outstr,"Cursor location: (%d,%d) = %d$",ixs,iys,ipix);
    }


 if (!(strcmp(*params, "M")))
    {                           /* List location ------- Mouse moved */
    if (getscreen_sc)
       {      /* Want screen value */     
       sprintf  (outstr,"Cursor location: (%d,%d) = %d$",ixs,iys,ipix);
       }
    else
       { sprintf(outstr,"Cursor location: (%d,%d)$", ixs,iys); }

    spout(outstr);
    } 

 else if (!(strcmp(*params, "1")))
    {                     /*  List location -------- Button 1 pushed */

    if ( ! docit_sc)
       { // Do not save location, etc. in doc file
       if (getscreen_sc)
          { sprintf(outstr,"Location: (%d,%d) = %d",ixs,iys,ipix); }
       else
          { sprintf(outstr,"Location: (%d,%d)", ixs,iys); }
       spout(outstr);
       }

    else 
       {// Save location in doc file

       if (getscreen_sc)
           {sprintf(outstr,"Key: %d  (%d,%d) = %d", ikey,ixs,iys,ipix);}
       else
           {sprintf(outstr,"Key: %d  (%d,%d)", ikey,ixs,iys);}
       spout(outstr);

       //    Put location in doc file 
       for (i=1; i <=6; i++) dlist[i] = 0.0;

       dlist[0]         = ikey;
       dlist[ixreg_pix] = ixs;
       dlist[iyreg_pix] = iys;
       if (getscreen_sc && (isreg_pix > 0))
           { dlist[isreg_pix] = ipix;}
       else
           { dlist[isreg_pix] = 0.0;}

       fpdocpix_sc = savdn1(docnam, datexc, &fpdocpix_sc, dlist, 7, 
                           &openitp, TRUE, TRUE);
       }

    if (leavit_sc)
       {
       /* Check pixel buffer size, if not enough add more */
       if (pixelnum >= bufsiz-1)
          {
          if ((pixellist = (int *) realloc(pixellist, (bufsiz+400)*
                   	sizeof(int))) == NULL)
             {spout("*** Unable to realloc. pixelbuf in pixel.c"); return; }
	  bufsiz = bufsiz + 400;
	  }
	     
       // Record the pixel number (used in shift) 
       pixellist[pixelnum] = ixs;     pixelnum++;
       pixellist[pixelnum] = iys;     pixelnum++;

       // Leave permanent circle at this location 
       xorc(iwin,    icontx, TRUE, ixs, iys, iradius_pix);
       xorc(imagsav, icontx, TRUE, ixs, iys, iradius_pix);
       }

       ikey++;     // Increment doc file key
    }

 else if (!(strcmp(*params, "2")))
    {                    /* Show menu -------------   Button 2 pushed */

    /* Kludge to prevent file name incrementing */
    if (docit_sc)
       pixelmen_sc_doc(NULL, NULL, NULL);
    else
       pixelmen_sc_nod(NULL, NULL, NULL);
    }

 else if (!(strcmp(*params, "3")))
    {                    /* Stop routine ----------    Button 3 pushed */

    pixelmen_sc_buts(NULL,NULL,NULL);

    pixelnum = 0;
    }
 }
