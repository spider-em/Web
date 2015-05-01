/*
*
*  translated to C by mahieddine ladjadj 7/14/94
*
c++*************************************************************************
c
c wid_moviemen.for -- created feb 93
c
c **************************************************************************
c    author:  ardean leith                                                 *
c *  copyright (c)1993, wadsworth center for laboratories and              *
c *  research, new york state department of health, albany, ny 12201.      *
c *    the contents of this document are proprietary to the center for     *
c *    laboratories and research and are not to be disclosed to others or  *
c *    used for purposes other than intended without written approval of   *
c *    the center for laboratories and research			           *
c **************************************************************************
c    
c--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include <Xm/Scale.h>
#include <Xm/ToggleBG.h>

void   moviemen_buts    (Widget, caddr_t, caddr_t); 
void   moviemen_buta    (Widget, caddr_t, caddr_t);
extern void  glass          (Window , Window , int , int ,
	                    int , int , int *, int *, int );

extern int   ixulmon,iyulmon;   /* first image of montage */
extern int   ndone;   /* total number of images */

Widget    iw_moviemen, iw_delay, iw_magni, iw_cycl, iw_twoway;
int       setupbuttons, moviedelay, moviemagni, moviecycl,
          imno, forward, movie_step, twoway, moviemenu, ixulmv,iyulmv; 

/**********************  moviemen   ************************************/
void moviemen(int first)
   {   
   Widget iw_rowcol, iw_dums, iw_buts,iw_buta, iw_butc;
   int    ixc, iyd, iyc;
   char cval[5];  
 
   if (iw_moviemen <= (Widget) 0)
      { /* movie menu widget does not exist, need to create it */
                                  
      iw_moviemen = wid_dialog(iw_win,0,"Movie options",20,20);
      iw_rowcol = wid_rowcol(iw_moviemen,'v',-1,-1);
                                                   
      /* set default delay */
      moviedelay  = 10;

      /* set default magnification */
      moviemagni = 1;

      /* set default cycles */
      moviecycl = 1;
    
      twoway = FALSE;
      forward = TRUE;
                                                    
      /* create toggle box for twoway --------------------- twoway */
      iw_twoway = wid_toggleg(iw_rowcol,0,"two-way",twoway,    
                        toggle2_cb, (char *) &twoway, -1, -1);

      /* create a scale widget for delay ------------------- delay */
      iw_delay = wid_scale(iw_rowcol,iw_delay,"delay (msecs):",
                           5, 1020, moviedelay, 250, 50, -1, -1);

      /* create a scale widget magnify ------------------- magnify */
      iw_magni = wid_scale(iw_rowcol,iw_magni,"magnification:",
                           1, 10, moviemagni, 250, 50, -1, -1);
			   
      /* create a scale widget cycles --------------------- cycles */
      iw_cycl = wid_scale(iw_rowcol,iw_cycl,  "   run cycles:",
                           1, 10, moviecycl, 250, 50, -1, -1);

      /* create a push button for apply -------------------- apply */
      iw_dums   = wid_stdbut(iw_rowcol, iw_moviemen,
                         &iw_buts, &iw_butc, &iw_buta, "SCA",
                         fin_cb, fin_cb, moviemen_buta, NULL);
     }                                                      

   /* display button menu on first call  */
   setupbuttons = first;

   /* set starting frame */
   imno        = 0; 
   movie_step =  FALSE; moviemenu = FALSE;

   /* show the movie menu */
   XtManageChild(iw_moviemen); XFlush(idispl);

   } /* end of moviemen function */
                                   

/************************************  moviemen_buta **********************/
/*
**   accept button
*/
 
void moviemen_buta(Widget iw_t, caddr_t data, caddr_t call_data)
   {
   /* get delay */
   XmScaleGetValue(iw_delay,&moviedelay);
                  
   /* get moviemagni (magnification counter) */
   XmScaleGetValue(iw_magni,&moviemagni);
   
   /* get moviecycl (runb cycles counter) */
   XmScaleGetValue(iw_cycl,&moviecycl);
   
   
   /* get twoway toggle */
   twoway =  XmToggleButtonGadgetGetState(iw_twoway);

   XtUnmanageChild(iw_moviemen);

   /* start movie going */
   wid_movie();
                                
   } /* end of moviemen_buta  */

/******************************* moviemen_buts **************************/
/*
**  stop button
**/

void moviemen_buts(Widget iw_t, caddr_t data, caddr_t call_data)
    {

    /* remove the movie menu widget */
    XtUnmanageChild(iw_moviemen);
    showbutx(NULL, NULL, NULL, TRUE);

    /* destroy the pixmap iglassav. */
    glass(imagsav, iwin,0,0,FALSE, TRUE,(int *) 0,(int *) 0,FALSE);
    
    /* remove all translations from iw_win */
    XtUninstallTranslations(iw_win);
    
    } /* end of moviemen_buts  */
