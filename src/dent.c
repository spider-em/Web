/*$Header: /usr16/software/web/src/RCS/dent.c,v 1.7 2018/12/07 17:03:31 leith Exp $*/

/*
C**************************************************************************
C
C   DENT.FOR  -- 
C
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C DENT()
C
C  PURPOSE:     READS SPIDER 3-D PICTURE FILE, CREATES 2-D 
C               REFLECTION IMAGE OF LEFT SIDE VIEW, DRILLS / FILLS
C               WITHIN THE VOLUME ACCORDING TO USER INPUT
C
C  PARAMETERS:  
C
C  RETURNS:     VOID
C
C  CALLED BY:   DENTMEN
C
C  NOTES:       
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "dent.h"
#include <assert.h>

 /* internal functions */
 static void    dent_actions (Widget, XEvent *, String *, Cardinal *);

 /* external function prototypes */

 /* externally defined variables used here */

 /* static variables for use here */
 static unsigned char lthlev;
 static float         csfact;
 static float         fd1, fd2;
 static int           currentdepth;
 static int           nsams, nrows;
 static int           nsamwt;

 /* variables that are used elsewhere also */
 unsigned char  * dentvol     = NULL, * dentrefmap = NULL;
 int            * dentdistmap = NULL;
 int            dentdensity   = 0, dentdepth  = 1;
 int            dentwidth     = 1, dentheight = 1;
 int            dentmovement  = 1;

 /******************************   dent   ****************************/

 void dent()

 {
 int            islicet, irow, nvoxels, npix;
 float          ivall;
 char           output[100];
 static float   fn, fncon;
 unsigned char  * cptr,  * ptr;
 int            *dentdistptr;

/* find number of pixels in end view of the volume */
 npix    = nrow * nslice;

 /* find original volume size */
 nvoxels = nsam * nrow * nslice;

 nsams   = nrow;
 nrows   = nslice;
 nsamwt  = nsam2 - nsam1 + 1;

 /* activate wait clock cursor (#79) */
 waitcursor(iwin,TRUE,79);
 
 /* allocate space for arrays */
 if (((dentvol   = (unsigned char *) malloc(nvoxels*
       sizeof(unsigned char))) == (unsigned char *) 0) ||
    ((dentdistmap = (int *) malloc(npix*sizeof(int))) 
                               == (int *) 0) ||
    ((dentrefmap  = (unsigned char *) malloc(npix*sizeof(unsigned char)))
                               ==   (unsigned char *) 0))
    {
    spout("*** Unable to allocate memory in dent.c");
    return;
    }

 /* load volume data into 8 bit vol, scaling from 1..254 */
 redvol8(filedata, dentvol, 1, nsam, 1, nrow, 1, nslice, 
         1, 254, &fn, &fncon); 

 /* NOTE: Distance shading is done according to original nsam     */
 /*       not the windowed nsam1 dimension                        */

 /* convert threshold to 8 bits */ 
 lthlev = (unsigned char) (dentthlev * fn + fncon);

 /* create the distance map, containing depth to threshold */
 dentdistptr = dentdistmap;
 cptr        = dentvol + nsam1 - 1;

 for (islicet =  1; islicet <= nslice ; islicet++)
    {
    /* set starting location for this row in reflection image */
    for (irow = 1; irow <= nrow ; irow++)
       {
       /* if threshold never found, set distance to zero */
       ivall = 0;
       for (ptr = cptr; ptr < (cptr + nsamwt); ptr++)
          {
          if (*ptr >= lthlev)
	     {
	     ivall = ptr - cptr + nsam1;
assert(ivall <= nsam);
	     break;   
             }
          }   /* for ptr  */
       /* place distance within distance map */
       *(dentdistptr++) = ivall;

       /* set pointer to next row within dentvol */
       cptr += nsam;

       }  /*  irow      */
    }     /*  islicet   */

 /* find normalization factors for dentrefmap formation */
 fd1        = (127.0 - imagego) * (1.0 - dentrefl) / (1.0 - nsam);
 fd2        = imagego - nsam * fd1;
 csfact     = dentrefl * (127.0 - imagego);

 sprintf(output, "Threshold:%f --> %d", dentthlev, lthlev);
 spout(output);

 /*convert all of distancemap to a relection map for display  */
 distoref(nsam, nsams, nrows, dentdistmap, dentrefmap, 
          dentfct, csfact, fd1, fd2,
          nsam1, nsams, nrow1, nrows,
          imagego);        
          /** nsam1, nsam2, nrow1, nrow2, APR 2005 al*/
          /** imagego);                   */

 /*  find location for reflection map display  */
 locc(&ixul,&iyul,nsams,nrows,&ntop,&nbot,&nrit,
      marx,mary,marlef,martop,iwidex,ihighx,placed,
      newline,&nuscrn,force);

 /*  display the reflection map, keep the image  */
 wipic(icontx,  (char *) dentrefmap, ixul,iyul, nsams,nrows,
       TRUE,TRUE, &imagel);  

 /*  remove clock cursor */
 waitcursor(iwin,FALSE,0);

 /* display button menu (stays up) */
 showbutx("Drills or fills.", 
          "Raises/lowers tool.",
          "Seeks to threshold depth.",FALSE); 

 /* display the dentist parameters menu (stays up) */
 dentmen2();

 /* set the actions for right, left, and center buttons */
 actions(iw_win, "dent_actions",dent_actions,"M123");
 }


 /****************************** dent_actions *************************/

 void dent_actions(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 int           ixi, iyi, ixs, iys, ix, iy, idepth;
 int           ixgo, ixend, iygo,  iyend;
 int           * distptr;
 unsigned char * cptr, *dentvolptr;
 int           ivall;
 char outstr[81];

 if (!(strcmp(*params, "M")))
    {                        /* List location ---- mouse moved */
    getloc(event,'m',&ixs,&iys);
    /* find location relative to image */
    ixi = ixs - ixul;
    iyi = iys - iyul;

    if (ixi < 0 || ixi >= nsams || iyi < 0 || iyi >= nrows)
       {    
       spouto("*** Outside Volume              ");
       }
    else
       {
       /* report depth to threshold and current depth */
       distptr = dentdistmap + iyi * nsams + ixi;
       sprintf(outstr,"Current Depth: %d   Depth at (%d,%d): %d", 
               currentdepth,ixi+1,iyi+1,*distptr);
       spouto(outstr);
       }
    return;
    } 



 /* find location at time of button push */
 getloc(event,'B',&ixs,&iys);

 /* find location relative to volume image */
 ixi = ixs - ixul;
 iyi = iys - iyul;

 if (ixi < 0 || ixi >= nsams || iyi < 0 || iyi >= nrows)
    {    /* cursor is outside of volume, only want inside */
    spouto("*** Outside Volume            ");
    return;
    }

 if (!(strcmp(*params, "1")))
    {                     /*  Drill/fill ---- button 1 pushed */
    /* want to drill or fill this location */

    ixgo  = ixi - dentwidth  / 2; ixgo  = MYMAX(ixgo,0);
    ixend = ixi + dentwidth  / 2; ixend = MYMIN(ixend,nsams);
    iygo  = iyi - dentheight / 2; iygo  = MYMAX(iygo,0);
    iyend = iyi + dentheight / 2; iyend = MYMIN(iyend,nrows);
/* are ends off by one??? al may 96??? */


    for (ix = ixgo; ix <= ixend; ix++)
       {
       for (iy = iygo; iy <= iyend; iy++)
          {
          /* find pointer to start of this row in volume) */
          dentvolptr = dentvol + iy * nsam * nrow + ix * nsam ;

          /* set density values within the volume */
          if (currentdepth > 0 && currentdepth <= nsam)
             {
             /* place current density in volume at currentdepth */
             for (idepth = currentdepth-1; 
               idepth <= currentdepth-2+dentdepth && idepth <= nsam;
               idepth++)
                { *(dentvolptr+idepth) = dentdensity;  }
             }

          /* update distance to threshold map now */

          /* if threshold never found, set distance to zero */
          ivall = 0;
          for (cptr =  dentvolptr + nsam1 - 1; 
               cptr < (dentvolptr + nsam1 - 1 + nsamwt); 
               cptr++)
             {
             if (*cptr >= lthlev)
                {
                ivall = cptr - dentvolptr + nsam1;
                break;   
                }
             }  

          /* place distance within distance map */
          distptr  = dentdistmap + iy * nsams + ix;
          *distptr = ivall;
          }
       }

    /* update reflection map now */
    distoref(nsam, nsams, nrows, dentdistmap, dentrefmap, 
          dentfct, csfact, fd1, fd2,
          ixgo, ixend, iygo, iyend, imagego);        

    /*  destroy old X image  */
    if (imagel) 
       { 
       XFree((void *)imagel); 
       imagel =  NULL;
       }

    /*  display the new reflection map, keep the image  */
    wipic(icontx, (char *) dentrefmap, ixul,iyul, nsams,nrows,
          TRUE,TRUE, &imagel);
    }

 else if (!(strcmp(*params, "2")))
    {                   /* alter currentdepth -------  button 2 pushed */
    currentdepth += dentmovement;; 
    sprintf(outstr,"Current depth now: %d", currentdepth);
    spout(outstr);
    }

 else if (!(strcmp(*params, "3")))
    {       /* set currentdept to threshold depth ---- button 3 pushed */

    currentdepth = *(dentdistmap + iyi * nsams + ixi);
    sprintf(outstr,"Current depth now: %d", currentdepth);
    spout(outstr);
    }
 }

