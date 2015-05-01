/*
C wid_movie.for  Created                       feb 93
C                Ported to: c               7/14/94 Mahieddine Ladjadj *
C
C
C **************************************************************************
C *  author:  ardean leith                                                 *
C *  copyright (c)1993, wadsworth center for laboratories and              *
C *  research, new york state department of health, albany, ny 12201.      *
C *    the contents of this document are proprietary to the center for     *
C *    laboratories and research and are not to be disclosed to others or  *
C *    used for purposes other than intended without written approval of   *
C *    the center for laboratories and research			           *
C **************************************************************************
C
C***************************************************************************
*/

#include "common.h"
#include "routines.h"

void         movie_pop      (Widget, XEvent *, String *, Cardinal *);
void         wid_movierun   (void);
void         movie_doit     (Widget, XtIntervalId *);
extern void  moviemen_buts  (Widget, caddr_t, caddr_t);
extern void  glass          (Window , Window , int , int ,
	                    int , int , int *, int *, int );


extern int  ixulmon,iyulmon;          /* First image of montage */ 
extern int  numfil;		      /* Last file number+1     */ 
extern int  setupbuttons, moviedelay, moviemagni, moviecycl, imno, forward,
            movie_step, twoway,moviemenu, ndone, ixulmv,iyulmv;
int         ixulm, iyulm, iskipm, ixoffm, iyoffm, ixulmm, iyulmm; 
int	    icycles, movierun, nisams, nirows, checkmag, cyclnbr;


XtIntervalId  intval; 

/*************************  wid_movie  ********************************/
void wid_movie(void)
  {
  if (setupbuttons)
     {
     showbutx("Step thru movie.",
              "Menu/(start movie).",
              "Stop movie.",FALSE);

     actions(iw_win,"movie_pop",movie_pop,"M123");
     setupbuttons = FALSE;
     }

  XFlush(idispl);

  /* Find display location */
  ixulmm = (iwidex - ixoffm) / 2;
  iyulmm = (ihighx - iyoffm) / 2;

  /* Total number of frames for a continous movie */
  if (twoway)
      cyclnbr = moviecycl * ndone * 2 - 1 ;
  else
      cyclnbr = moviecycl * ndone ;
  
  /* Start movie running  */
  checkmag = TRUE;   movierun = TRUE;   movie_step = FALSE;
  wid_movierun();
  }

/**********************************  wid_movierun  *******************/
void wid_movierun(void)
   {
   
   /* names_height is the number of pixel needed to write filename
          under image, defined in imoptmen.c */
	  
   int    imcol, imrow, idelay, ixnn,iynn;
   char   outstr[10]="Frame:   ";
   int    nlft;
   int    old_magx, old_magy, old_mfac, x11, y11;

   if (iskip >= -1 && iskip <= 1)
      { /* Display image full-sized  */
      iskip = 1;
      nisams = (nsam2 - nsam1 + 1) ; nirows = (nrow2 - nrow1 + 1) ;
      }
   else if (iskip > 1)
      { /* Shrink image by factor of iskip */
      nisams = 1 + (nsam2 - nsam1)/iskip; nirows = 1 + (nrow2 - nrow1)/iskip;
      }
   else if (iskip < 1) 
      {  /* Magnify image by factor of iskip */
      nisams = (nsam2-nsam1+1)*(-iskip); nirows = (nrow2-nrow1+1)*(-iskip);
      }

   /* Find correct subimage location, ndone : total number of images */
   if (!twoway)
      {
      imno++;
      if (imno > ndone || imno <= 1)
         {
         imno   = 1; 
         ixulmv = ixulmon; iyulmv = iyulmon;
	 }
      else
         { /* find the location */  
         ixulmv = ixulmon + ((imno -1) % nprow ) * (nisams + marx);
         iyulmv = iyulmon + ( (imno -1) / nprow ) * (nirows + mary ) ; 
	 }

      if( !(imno % nprow ))
          iyulmv += (( imno / nprow ) - 1 ) * names_height ;
      else
	  iyulmv += ( imno / nprow ) * names_height ;
      }
   else if (twoway && forward)
      {
      imno++;
      if (imno > ndone) 
         {
         imno    = ndone - 1; forward = FALSE;
         ixulmv = ixulmon + ((imno -1) % nprow ) * (nisams + marx);
         iyulmv = iyulmon + ( (imno -1) / nprow ) * (nirows + mary ) ;

         if( !(imno % nprow ))
	      iyulmv += (( imno / nprow ) - 1 ) * names_height ;
         else
	      iyulmv += ( imno / nprow ) * names_height ;
	 
         imno--; 
         }            
      else
         { /* Find the location */  
         ixulmv = ixulmon + ((imno -1) % nprow ) * (nisams + marx);
         iyulmv = iyulmon + ( (imno -1) / nprow ) * (nirows + mary ) ;
 
         if( !(imno % nprow ))
              iyulmv += (( imno / nprow ) - 1 ) * names_height ;
         else
	      iyulmv += ( imno / nprow ) * names_height ; 
	 }
                          
      }
   else if (twoway)
      {
      imno--;
      if (imno < 0) 
          {
          imno    = 1;  forward = TRUE;
          ixulmv = ixulmon; iyulmv = iyulmon;
          }           
      else
         { /* find the location */  
         ixulmv = ixulmon + ((imno -1) % nprow ) * (nisams + marx);
         iyulmv = iyulmon + ( (imno -1) / nprow ) * (nirows + mary ) ;
         if( !(imno % nprow ))
              iyulmv += (( imno / nprow ) - 1 ) * names_height ;
         else
	      iyulmv += ( imno / nprow ) * names_height ; 
	 }
      }
      
  
   /* Copy window to center of screen */
   if(moviemagni <= 1)
      { /* no magnification, just copy */
      XCopyArea(idispl,imagsav,iwin,icontx,ixulmv,iyulmv,
                       nisams,nirows+names_height,ixulmm,iyulmm);
      neednewmap = TRUE;

/* this is temp . replace it back */
/*                       nisams,nirows,ixulmm,iyulmm);
*/
      }
   else
      { /* new magnify on the fly                        */
      /* could save this somewhere else                  */
      /* preserve the OLD image size and magnification   */
      old_magx = magsizx; old_magy = magsizy; old_mfac = magfac;
      magsizx = nisams; magsizy = nirows; magfac = moviemagni;

      /* Compute new position for movie image */
      if(checkmag)
         {
         checkmag = FALSE;
         ixulmm -= magsizx * magfac / 2;
         iyulmm -= magsizy * magfac / 2;
         }

      glass(imagsav, iwin, ixulmv, iyulmv, FALSE, FALSE,
            &ixulmm,&iyulmm,FALSE);
      neednewmap = TRUE;

      /* Put back values of image size and magnification */
      magsizx = old_magx; magsizy = old_magy; magfac = old_mfac;
      }

   XFlush(idispl);

   if(forward)
       { sprintf(&outstr[7],"%2d", imno); spout(outstr); }
   else
       { sprintf(&outstr[7],"%2d", (imno + 1)); spout(outstr); }

   /* Stop if this is single frame stepping */
   if(!movie_step) 
       {
       intval = XtAppAddTimeOut(XtWidgetToApplicationContext(iw_win),
                       (unsigned long) moviedelay, 
                       (XtTimerCallbackProc) movie_doit,
		       (XtPointer) NULL);
       }
   } /* wid_movierun(void) */


/* ******************* movie_doit ************************************/

void movie_doit(Widget w, XtIntervalId *id)
     {
     int doit;
     
     /* Strangely enough some extra timeout seem to occur. execute
        this routine only if it is caused by moviemen timeout */

     if(*id != intval) 
	doit=0;
     else
        doit=1;

     if  (movierun)
         {
         if (moviemenu)
            {	
            moviemenu = FALSE;
            moviemen(FALSE);  			/* movie menu   */
            }
         else
	    {
	    cyclnbr--;
            if(!cyclnbr) {cyclnbr = moviecycl * ndone; movie_step = TRUE; }
            if(doit) wid_movierun();            /* Continue run */
	    }
	 }
     else 				        /* Stop movie   */
         moviemen_buts(NULL,NULL,NULL);
	 
     }

/* ******************* movie_pop **************************************/

void movie_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
  {                        
  if (!(strcmp(*params,"1")))                 /* Button 1 pushed */
     {                         /* restart movie  ------- Restart */
     movie_step = TRUE;   wid_movierun(); 
     }
  else if (!(strcmp(*params,"2")))            /* Button 2 pushed */
     {                        /* menu callup   ------------ Menu */
     moviemenu = TRUE;
     if (movie_step)
        {   movie_step = FALSE;   wid_movierun();  }		
     }
  else if (!(strcmp(*params,"3")))            /* Button 3 pushed */
     {
     movierun = FALSE;  
     movie_step = TRUE;  moviemen_buts(NULL,NULL,NULL);
     }

  } /* end of: movie_pop  */
