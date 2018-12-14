/*
C
C++*********************************************************************
C
C showmovie.c
C
C **********************************************************************
C * WEB.    AUTHOR: ARDEAN LEITH                                       *
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
C***********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "mont.h"
#include "movie.h"

/* internal function prototypes */
static void  movie_popb       (Widget, XEvent *, String *, Cardinal *);
static void  movie_showframe  (int, int);
static void  movie_wait       (Widget, XtIntervalId *);
static int   movie_setframe   (int , int , int * , int );

/* external function prototypes */

/* external variables used/or set elsewhere */

/* variables also used/set elsewhere */
XImage *    ximagelist[1000];

/* internal variables used in this file */
static int           ixulm, iyulm;
static XtIntervalId  intval; 

/********************* showmovie  ************************************/

void showmovie(void)
  {
  showbutx("Step backwards in movie.",
           "Menu (Start/Stop movie).",
           "Step forwards in movie.",FALSE);

  actions(iw_win,"movie_popb",movie_popb,"123");

  XFlush(idispl);

  /* set movie location in center of window */
  ixulm = (iwidex - nsami) / 2;
  iyulm = (ihighx - nrowi) / 2;

  /* start movie running (using defaults set by showmoviemen)  */
  movie_showframe(movie_frame, TRUE);
  }

/********************************  movie_showframe  ************************/

void movie_showframe(int iframe, int keepgo)
   {
                    /*123456789 123*/
   char   outstr[12]="Frame:     $";
      
   /* display image at desired location */
   XPutImage(idispl,iwin,icontx, ximagelist[iframe] ,0,0,ixulm,iyulm,
              (unsigned int) nsami, (unsigned int) nrowi);
   XFlush(idispl);

   sprintf(&outstr[7],"%4d", movie_frame); 
   spouto(outstr);

   if (keepgo != 0)
     {  /* need to start up movie_popb after movie delay wait */
     intval = XtAppAddTimeOut(XtWidgetToApplicationContext(iw_win),
                       (unsigned long) movie_delay, 
                       (XtTimerCallbackProc) movie_wait,
		       (XtPointer) NULL);
     }

   }


/* ******************* movie_wait ************************************/

void movie_wait(Widget w, XtIntervalId *id)
     {
     String params[3];
     /* Strangely enough some extra timeout seem to occur. Execute
        this routine only if it is caused by showmoviemen timeout */


     if (*id == intval)
        { 
        params[0] = "W";
        movie_popb((Widget) NULL, (XEvent*) NULL, 
                   params, (Cardinal *) NULL);
        }
     }

/* ******************* movie_popb *************************************/

void movie_popb(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
  { 
  int nextframe;
                       
  if (!(strcmp(*params,"1")))   /* button 1 pushed - step backwards*/
     {                         
     /* ndone: total number of images */
     nextframe   = movie_frame - 1;
     movie_inc   = 0;
     movie_frame = movie_setframe(nextframe, movie_twoways, 
                                  &movie_inc, ndone);
     }

  else if (!(strcmp(*params,"2")))          /* button 2 pushed - menu*/
     {  
     movie_inc = 0;                      
     showmoviemen(FALSE);
     return;
     }

  else if (!(strcmp(*params,"3"))) /*  button 3 pushed - step forward */
     {                         
     nextframe   = movie_frame + 1;
     movie_inc   = 0;
     movie_frame = movie_setframe(nextframe, movie_twoways, 
                                   &movie_inc, ndone);
     }

  else
     {
     /* keep going with movie */
     nextframe   = movie_frame + movie_inc;
     movie_frame = movie_setframe(nextframe, movie_twoways, 
                                  &movie_inc, ndone);
     }

  /* display next frame */
  movie_showframe(movie_frame, movie_inc);

  } 

/* ******************* movie_setframe *************************************/

int movie_setframe(int next, int twoways, int * increment, int ntot)
   {     /* finds next frame (1...ntot)   */

   int  iframe;
                       
   if (next < 1)
      { 
      if (twoways)
         {
         iframe = 2;
         if ((*increment) != 0) *increment = 1;
         }
      else
         {iframe = ntot;}
      }

   else if (next > ntot)
      { 
      if (twoways)
         {
         iframe = ntot - 1;
         if ((*increment) != 0) *increment = -1;
         }
      else
         {iframe = 1;}
      }
   else
      {
      iframe = next;
      }

   return iframe;
   }
