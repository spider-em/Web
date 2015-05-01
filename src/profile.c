/*$Header: /usr8/web/src/RCS/profile.c,v 1.15 2011/09/23 19:30:08 leith Exp $*/
/*
************************************************************************
*  profile.c         6/10/93
*  wid_profile.for  (in VAX) ported to UNIX
*                   Oversize images            16 May 2011 ArDean Leith
* 
C **********************************************************************
*    AUTHOR:  Mahieddine Ladjadj                                       *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2011  Health Research Inc.                     *
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
*
*    profile()
*
*    PURPOSE:       Plots profile through row or col of image file
*
*    PARAMETERS:    None
*
************************************************************************
*/

#include "common.h"
#include "routines.h"

/* Internal subroutine prototypes */
 void  profile_pop     (Widget, XEvent *, String *, Cardinal *);
 void  profile_plot    (float *);
 void  profilemen      (void);
 void  profilemen_buts (int, int, int);
 void  profile_line    (int);

 /* Internal common variables */
 int   first , horizbar , leavebar, normbar, permbar, doxyaxis, permxyaxis ;
 int   spline;		/* the special line flag */
 int   linenum;		/* the line number of the special line */
 float *buft;
 int   LS0, ls;           /* line style of the icontxm GC */
 int   LW0, lw;           /* line width of the icontxm GC */

 /* External common variables */
 GC     icontxm;

 /***************************  profile  *******************************/

 void profile(void)
   {
   first    = horizbar = normbar = TRUE;
   leavebar = permbar  = spline  = FALSE;
   linenum  = 0;

   /* Avoid drawing X and Y axis more than once */
   doxyaxis = 2; 
   permxyaxis = 2;

   /* Run-time allocation of memory space */
   buft = (float *) calloc(nrow, sizeof(float));
   if (buft == (float *) NULL)
       {  spout("*** Calloc failed in profile"); return; }

   /* Set up a xor graphics context */
   icontxm = setxor(icontx);

   /* Open a message window with the following strings */
   showbutx("Image Profile.",
            "Display menu.",
            "Stop this routine.", FALSE);

   /* Set actions for right, left, and center buttons */
   actions(iw_win, "profile_pop",profile_pop,"M123");

   /* Set color for the overlay */
   wicolor(icontxm,colorgo+icolorov);

   /* Refresh screen */
   expos_cb(NULL,NULL,NULL);
   }

 /********************** profile_pop  *******************/

 void profile_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
   {
   XGCValues     gcv;
   int           ixc, iyc, ixi, iyi, ix2, iy2, ix3, iy3, inside;
   static int    ix0, iy0, ix1, iy1;
   int           irec, irow, idum;
   float         *buf;
   char          outstr[80];

   /* Get current line style and width */
   /* Line_style is LineSolid(0), LineOnOffDash(1), ... */
   /* Line_width is 0,1,...,5                        */
   if (first)
      { /* Save the line width and style of the GC */
      XGetGCValues(idispl, icontx, GCLineWidth | GCLineStyle, &gcv);
      LW0 = gcv.line_width;
      LS0 = gcv.line_style;
      ls  = LS0; 
      lw  = LW0 - 1;
      }

   if (!(strcmp(*params,"M")))                   /* Cursor moved */
       {
       /* Get mouse position ixc,iyc  */
       getloc(event,'M',&ixc,&iyc);

       /*  Find location relative to image */
       ixi    = ixc - ixul + 1;
       iyi    = iyc - iyul + 1;
       inside = iskipadj(ixi,iyi,nsam,nrow,iskip,&ixi,&iyi);

       if ( !inside )
           { /*  Outside of image. */
           ;
           }
       else if (horizbar)
           {
	   /* If draw the special line */
	   if (spline) {
	      iyi = linenum;
	      iyc = linenum + iyul - 1;
	      }

           /* Horizontal bar inside image, report location. */
           sprintf(outstr,"Row inside image: %d$", iyi); 
	   spout(outstr);

           /* Find bar ends */
           ix2 = ixul;             iy2 = iyc;
           ix3 = ixul + nsam -1;   iy3 = iyc;
           if (!first)
              xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
           else
              {
              xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
              first = FALSE;
              }
	   }
       else
	   {
	   /* If draw the special line */
	   if (spline) {
	      ixi = linenum;
	      ixc = linenum + ixul - 1;
	      }

           /* Vertical bar inside image, report location. */
           sprintf(outstr,"Col. inside image: %d$", ixi);
           spout(outstr);

           /* Find bar ends */
           ix2 = ixc;    iy2 = iyul;
           ix3 = ixc;    iy3 = iyul + nrow -1;
           if (!first)
              xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
           else
              {
              xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
              first = FALSE;
              }
           }
       }

   else if (!(strcmp(*params,"1")))            /* Button 1 pushed */
       {
       /* Get mouse position ixc,iyc  */
       getloc(event,'M',&ixc,&iyc);

       /*  Find location relative to image */
       if (!spline) {
           ixi    = ixc - ixul + 1;  
	   iyi    = iyc - iyul + 1;
           inside = iskipadj(ixi,iyi,nsam,nrow,iskip,&ixi,&iyi);
	   }
       else
           {
	   if (horizbar) {
	      ixi = nsam;
	      iyi = linenum;
	      }
	   else {
	      ixi = linenum;
	      iyi = nrow;
	      }
	   }

       if ( !inside )
	   {  /*  Outside of image. can not select this location */
           ;
	   }
       else if (horizbar)
	   {
           /* Horizontal bar inside image, report location. */
           sprintf(outstr,"Profiled row: %d", iyi);  spout(outstr);

           /* Find bar ends */
           ix2 = ixul;              iy2 = iyc;
           ix3 = ixul + nsam -1;    iy3 = iyc;

           if (leavebar)
	      {
              xorline(iwin,icontxm,TRUE, ix2, iy2, ix3, iy3);

              /* Save bar in backing store */
              xorline(imagsav,icontxm,TRUE, ix2, iy2, ix3,iy3);
	      }
           else
              {
              if (!first)
                xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
              else
                 {
                 xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
                 first = FALSE;
                 }
              }

           /* Read row from image file */
	   if (!spline)
               irec = (islice - 1) * nrow + iyi;
	   else 	/* special line */
               irec = (islice - 1) * nrow + linenum;

           buf  = filedata -> buf;
           if (redlin(filedata,buf,nsam,irec))
               profile_plot(buf);

           /* Plotting the profile has changed static: x0,y0 & x1,y1
           ** in xorline. Delete the last bar */
           if (!leavebar)
              xorline(iwin,icontxm,TRUE, ix2, iy2, ix3, iy3);

	   }
       else
	   {
           /* Vertical bar inside image, report location. */
           sprintf(outstr,"Profiled col.: %d", ixi); spout(outstr);

           /* find bar ends */
           ix2 = ixc;     iy2 = iyul;
           ix3 = ixc;     iy3 = iyul + nrow -1;

           if (leavebar)
	      {
              xorline(iwin,icontxm,TRUE, ix2,iy2, ix3,iy3);

              /* Save bar in backing store */
              xorline(imagsav,icontxm,TRUE, ix2,iy2, ix3,iy3);
	      }
           else
              { 
              if (!first)
                 xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
              else
                 {
                 xorline(iwin,icontxm,FALSE, ix2,iy2, ix3,iy3);
                 first = FALSE;
                 }
              }

           /* Read col from image file */
           buf = filedata -> buf;
           for (irow = 0; irow < nrow; irow++)
	      {
              irec = (islice - 1) * nrow + irow;
              redlin(filedata,buf,nsam,irec);
              buft[irow] = buf[ixi];
	      }
           profile_plot(buft);

           /* Plotting the profile has changed static: x0,y0 & x1,y1
           ** in xorline.  Delete the last bar */
           if (!leavebar)
              xorline(iwin,icontxm,TRUE, ix2, iy2, ix3, iy3);

	   }

       }                                       

   else if (!(strcmp(*params,"2")))           /* Button 2 pushed */
       { /* Get mouse position ixc,iyc  */
       getloc(event,'M',&ixc,&iyc);

       if (horizbar) 
           {
           ix2 = ixul;              iy2 = iyc;
           ix3 = ixul + nsam -1;    iy3 = iyc;
           }
       else
           {
           ix2 = ixc;     iy2 = iyul;
           ix3 = ixc;     iy3 = iyul + nrow -1;
           }

       if (!first)
          xorline(iwin,icontxm,TRUE, ix2,iy2, ix3,iy3);
       else
          {
          xorline(iwin,icontxm,TRUE, ix2,iy2, ix3,iy3);
          first = FALSE;
          }
 
       /* menu  callup */
       profilemen();

       if (!permbar) { permxyaxis=2; doxyaxis=2; }
       }

   else if (!(strcmp(*params,"3")))           /* Button 3 pushed */
       {
       /* Free heap memory */
       if (buft) free(buft);

       /* Stop this routine */
       profilemen_buts(idum,idum,idum);
       }

 return;
 }

/*********************  profile_plot  *****************************/

 void  profile_plot(float *buf)
   {
   int    xline, yline, i, xpsize, ypsize;
   int    ixend, ixori, iyori, ix, itick;
   int    iytop, iybot, ixgo, iygo, iy;
   float  bufmin,bufmax,vrange,vscale,hscale;

   xline = TRUE;
   yline = TRUE;

   if (normbar)
       {
       /* Find max y value in the buffer */
       bufmax = buf[0];
       bufmin = buf[0];
       for (i = 1; i < nsam; i++)
	  {
          bufmax = MYMAX(bufmax, buf[i]);
          bufmin = MYMIN(bufmin, buf[i]);
	  }
       }
   else
       {
       /* Use fmin_spi and fmax_spi for scaling */
       bufmax = fmax_spi;
       bufmin = fmin_spi;
       }

   vrange = bufmax - bufmin;
   if (vrange < FLTZER)
      {spout("*** NULL PROFILE");  return; }

   /* Set plot size */
   xpsize = iwidex - 100;
   ypsize = ihighx - 100;

   /* Find x and y scaling factors */
   hscale = xpsize / nsam;
   if (hscale < 1) hscale = (float) xpsize / (float) nsam;

   vscale = (float) ypsize / vrange;
   if (vscale < 1) vscale = ypsize / (float) vrange;

   /* Find origin of plot */
   ixori = ((iwidex - xpsize) / 2) + hscale;
   iyori = ihighx - (ihighx - ypsize) / 2;

   //printf("*** nsam: %d   \n",nsam);
   //printf("*** iwidex: %d  ihighx: %d \n",iwidex,ihighx);
   //printf("*** hscale: %f  xpsize: %d ypsize: %d \n",hscale,xpsize,ypsize);
   //printf("*** ixori: %d iyori: %d \n",ixori,iyori);

   if (xline)
       {
       /* Draw horizontal X axis */
       ixend = nsam * hscale + ixori - hscale;

       if(doxyaxis > 0) 
          {
          xorline(iwin,icontxm,TRUE, ixori, iyori, ixend, iyori);
          doxyaxis--;
          }
       if (permxyaxis > 0)
          {
          if (permbar)
              xorline(imagsav,icontxm,TRUE, ixori, iyori,
                                            ixend, iyori);
          permxyaxis--;
          }

       /* Add tick marks */
       itick = 50;
       if (nsam < 20 )
          itick = 1;
       else if (nsam < 300)
          itick = 10;

       for (i = 0; i < nsam ; i += itick)
	   {
           ix = i * hscale + ixori - hscale;
           xorline(iwin,icontxm,TRUE, ix, iyori, ix, iyori + 4);
           if (permbar)
             xorline(imagsav,icontxm,TRUE, ix, iyori, ix, iyori + 4);
	   }
       }
     
   if (yline)
       {
       /* Draw vertical Y axis */
       iytop = iyori - (vrange * vscale);
       iybot = iyori;

       /* Draw Y axis */
       if (doxyaxis > 0)
          {
          xorline(iwin,icontxm,TRUE, ixori, iybot, ixori, iytop);
          doxyaxis--;
          }

       if (permxyaxis > 0)
          {
          if (permbar)
             xorline(imagsav,icontxm,TRUE, ixori, iybot,
                                           ixori, iytop);
          permxyaxis--;
          }

       /* Put tick at bottom of Y axis */
       xorline(iwin,icontxm,TRUE, ixori-4, iybot, ixori, iybot);
       if (permbar)
         xorline(imagsav,icontxm,TRUE, ixori-4, iybot, ixori, iybot);

       /* Put tick at top of Y axis */
       xorline(iwin,icontxm,TRUE, ixori-4, iytop, ixori, iytop);
       if (permbar)
         xorline(imagsav,icontxm,TRUE, ixori-4, iytop, ixori, iytop);
       }

     /* Plot the profile */
     ixgo = ixori;
     iygo = iyori;

     /* Select line style and width for icontxm GC */

     profile_line(1);
     for (i = 0; i < nsam; i++)
        {
        ix = i * hscale + ixori;
        iy = iyori - ((buf[i] - bufmin) * vscale);

        xorline(iwin,icontxm,TRUE, ixgo, iygo, ix, iy);
        if (permbar)
           xorline(imagsav,icontxm,TRUE, ixgo, iygo, ix, iy);
        ixgo = ix;
        iygo = iy;
        }

     /* Reset line style and width for icontxm GC */
     profile_line(0);

     return;
     }

/*********************  profile_line  *****************************/

 void  profile_line(int doit)
   {
   XGCValues     gcv;
   int ls1, lw1;

   if (doit)
       {  /* Change line style and width if icontxm GC */
       if (lw >= 5) 
           { /* Reset line width and change line style */
           lw = 1;
           if (ls >= 2 ) 
             ls = 0;
           else
             ls++;
           }
       else
           {/* Next line width */
           lw++;
           }
       ls1 = ls; lw1 = lw;
       }
   else
       {  /* Reset line style and width if icontxm GC */
       ls1 = LS0; lw1 = LW0;
       }

   gcv.line_width = lw1;
   switch( ls1 )
       {
       case 0:   gcv.line_style = LineSolid;        break;
       case 1:   gcv.line_style = LineOnOffDash;    break;
       case 2:   gcv.line_style = LineDoubleDash;   break;
       }

   XChangeGC( idispl, icontxm, GCLineWidth | GCLineStyle, &gcv);
   }
//*** nsam: 1205
//*** iwidex: 900  ihighx: 779
//*** hscale: 0.663900  xpsize: 800 ypsize: 679
//*** ixori: 50 iyori: 729
//*** ixgo: 899 iygo: 778
//*** ix: 899 iy: 778
