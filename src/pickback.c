
/*$Header: /usr16/software/web/src/RCS/pickback.c,v 1.16 2018/12/07 17:03:33 leith Exp $*/

/*
 ***********************************************************************
 *
 * pickback.c
 *
 ***********************************************************************
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
 ***********************************************************************
 * 
 * 
 * PURPOSE:     Interactive tilt-pair background window selection      
 * 
 * PARAMETERS:	 
 *
 * VARIABLES:   
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "pick.h"
#include "fit.h"

 /* Internally defined  functions */
 static void        pickback_pop (Widget, XEvent *, String *, Cardinal *);
          
 /* Externally defined global variables */

 /* Internal file scope variables */
 static int         nsamsl, nrowsl, nsamsr, nrowsr;
 static int         openitl, openitr;
 static int         gotright = FALSE;
 static int         left     = TRUE;    /* Start with left image   */

 /* Global variables defined here */
 int                back_win_now;       /* Current window number   */
 FILE *             fpdoc4;             /* Used by: pickbackmen.c  */
 FILE *             fpdoc5;             /* Used by: pickbackmen.c  */

 /***********************  pick  ***********************************/

 void pickback(int firstrun)

 {
 openitl   = firstrun;
 openitr   = firstrun;
 firstback = FALSE;

 /* Find displayed size of both images */
 nsamsl = nsam2l - nsam1l + 1;
 nsamsr = nsam2r - nsam1r + 1;
 nrowsl = nrow2l - nrow1l + 1;
 nrowsr = nrow2r - nrow1r + 1;

 gotright = FALSE;
 left     = TRUE;

 /* Open a message window with the following strings  */
 showbutx("Select left background window.", 
          "Show background window menu.", 
          " ", FALSE);

 /* Set the actions for right, left, and center buttons */
 actions(iw_win, "pickback_pop", pickback_pop,"M123");
 }

 /************************* pickback_pop **************************/

 void pickback_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {
 char          outstr[60];
 float         dlist[8];
 int           ixr,iyr, ixs, iys, ixt, iyt;
 static int    ixp, iyp;
 float         xt,yt,  fx,fy, aver;

 char          strwin1[] = "Window Locations in Untilted File "; 
 char          strwin2[] = "Window Locations in Tilted File "; 

 char          strwin[] = 
  "Window    Average         Reduced-X,Y-location     Columns        Rows         Flag"; 

 if (!(strcmp(*params, "M")))
   {  // ******************************************** Mouse movement only 

   getloc(event,'m',&ixs,&iys);
   if (left && 
       (ixs < ixull || ixs >= ixull + nsamsl || 
        iys < iyull || iys >= iyull + nrowsl ))
       {    /* Cursor outside of displayed left image */
       spouto("*** Not in left image.");
       }

    else if (!left &&
       (ixs < ixulr || ixs >= ixulr + nsamsr || 
        iys < iyulr || iys >= iyulr + nrowsr ))
       {    /* Cursor outside displayed right image */
       spouto("*** Not in right image.");
       }
    else
       spouto("                        ");

    /* Draw xor box */
    xorbox(iwin,    icontxor, FALSE, ixs, iys, nsamw, nroww);
#ifndef WEB_TRUE    
    xorbox(imagsav, icontxor, FALSE, ixs, iys, nsamw, nroww);
#endif 
    } 

 // ******************************************************** Left button  

 else if (left && !(strcmp(*params, "1")))
   {   /*  Want box in left image -- button 1 pushed */
    getloc(event,'B',&ixs,&iys);

    /* Find location inside whole left image */
    ixulw    = ixs - ixulli + 1;
    iyulw    = iys - iyulli + 1;

   if (ixs < ixull || ixs >= ixull + nsamsl || 
       iys < iyull || iys >= iyull + nrowsl )
       {       /* Cursor is outside displayed left image, want inside */
       spouto("*** Not in left image."); XBell(idispl,50);
       }

    else 
       {   /* Want to record left background window location */
       spoutfile(TRUE);
       sprintf(outstr,"Left window:  %d (%d,%d)",back_win_now,ixulw,iyulw);
       spout(outstr);
       spoutfile(FALSE);

       /* Leave permanent box at this location  */
#ifndef WEB_TRUE    
       xorbox(iwin,    icontxor, TRUE, ixs, iys, nsamw, nroww);
#endif 
       xorbox(imagsav, icontxor, TRUE, ixs, iys, nsamw, nroww);  

       /* Find average within window */
       rcaver(filedatal, nsaml, nrowl, nsamw, nroww, 
              ixulw, iyulw,  &aver);

       /* Save info in doc file */        
       dlist[0] = back_win_now;
       dlist[1] = aver;
       dlist[2] = ixulw * iredu;
       dlist[3] = iyulw * iredu;
       dlist[4] = nsamw;
       dlist[5] = nroww;       
       dlist[6] = 1.0;       
       if (openitl)      
          {
          fpdoc4 = savdnc(dfil4, datexc, &fpdoc4,
                          dlist, 0, &openitl, TRUE, TRUE,strwin1);
          fpdoc4 = savdnc(dfil4, datexc, &fpdoc4,
                          dlist, 0, &openitl, TRUE, FALSE,strwin);
          }
       fpdoc4 = savdn1(dfil4, datexc, &fpdoc4,
                             dlist, 7, &openitl, TRUE, FALSE);

       /* Find predicted location in right image */
       if (fitted)
          {
          /* Transform the values */
          fx = (float)ixulw;
          fy = (float)iyulw;

          /* Use angles to get predicted location in tilted image */
          witran(&fx, &fy,  &xt, &yt, 1, gammaff, thetaf, phif);

          ixt = xt;
          iyt = yt;

          if ((ixt < 1 || ixt > ixulr + nsamsr ||
               iyt < 1 || iyt > iyulr + nrowsr))
             {    /* Predicted  loc. is outside of right image, */
             sprintf(outstr,"*** Window not in right image: (%d,%d)",
                             ixt,iyt);
             spout(outstr);
             }
          else
             {   /* Warp cursor to predicted location on tilted side */
             ixr = ixt + ixulri;
             iyr = iyt + iyulri;

             movecur(ixr-ixs,iyr-iys);

             /* Draw box at this location */
             xorbox(iwin,    icontxor, FALSE, ixr, iyr, nsamw, nroww);
#ifndef WEB_TRUE    
             xorbox(imagsav, icontxor, FALSE, ixr, iyr, nsamw, nroww); 
#endif 
             }
          }
       else
          { /* No tilt angle available yet */

          /* Warp cursor to center of tilted side */
          ixt = ixulr + nsamsr / 2;
          iyt = iyulr + nrowsr / 2;

          movecur(ixt-ixs,iyt-iys);

          /* Draw box at this location  */
          xorbox(iwin,    icontxor, TRUE, ixt, iyt, nsamw, nroww);
#ifndef WEB_TRUE    
          xorbox(imagsav, icontxor, TRUE, ixt, iyt, nsamw, nroww);
#endif 
          } 

       left     = FALSE;
       gotright = FALSE;

       /*  Remove message */
       showbutx("","","",TRUE);

       /* Open a message window with the following strings  */
       showbutx("Select right background window.", 
                "Show background window menu.", 
                "Reselect left window.", FALSE);

       /* Record undo location */
       ixp  = ixs;
       iyp  = iys;
       }
    }

 else if (!(strcmp(*params, "1")))
    {             /*  Want box in right image -- Button 1 pushed */
    getloc(event,'B',&ixs,&iys);

    /* Find location inside whole right image */
    ixulw    = ixs - ixulri + 1;
    iyulw    = iys - iyulri + 1; 

    if (ixs < ixulr || ixs >= ixulr + nsamsr || 
        iys < iyulr || iys >= iyulr + nrowsr )
       {    /* Cursor outside of displayed right image, want inside */
       spouto("*** Window not in right image."); XBell(idispl,50);
       }
 

    else 
       {   /* Want to average this location */
       spoutfile(TRUE);
       sprintf(outstr,"Right window: %d  (%d,%d)",back_win_now,ixulw,iyulw);
       spout(outstr);
       spoutfile(FALSE);

       /* Leave permanent box at this location */ 
#ifndef WEB_TRUE    
       xorbox(iwin,    icontxor, TRUE, ixs, iys, nsamw, nroww);
#endif
       xorbox(imagsav, icontxor, TRUE, ixs, iys, nsamw, nroww);   

       /* Find average within window */
       rcaver(filedatar, nsamr, nrowr, nsamw, nroww,
              ixulw, iyulw,  &aver);

       /* Save info in doc file */ 
       dlist[0] = back_win_now;
       dlist[1] = aver;
       dlist[2] = ixulw * iredu;
       dlist[3] = iyulw * iredu;
       dlist[4] = nsamw;
       dlist[5] = nroww;       
       dlist[6] = 1.0;  
       if (openitr)      
          {
          fpdoc5 = savdnc(dfil5, datexc, &fpdoc5,
                           dlist, 0, &openitr, TRUE, TRUE, strwin2);
          fpdoc5 = savdnc(dfil5, datexc, &fpdoc5,
                           dlist, 0, &openitr, TRUE, FALSE,strwin);
          }
       fpdoc5 = savdn1(dfil5, datexc, &fpdoc5,
                       dlist, 7, &openitr, TRUE, FALSE);
      
       /*  Remove button message */
       showbutx("","","",TRUE);

       /* Open a button message window with following strings */
       showbutx("Select left background window.", 
                "Show background window menu.", 
                "Reselect right window.", FALSE);

       left     = TRUE;
       gotright = TRUE;
       back_win_now++;

       // Update widgit for back_win_now
       pickbackmen();

       /* Warp cursor to last position of untilted side */
       movecur(ixp-ixs,iyp-iys);

       /* Record undo location */
       ixp  = ixs;
       iyp  = iys;

       /* Draw box at previous location on left side */  
       xorbox(iwin,    icontxor, TRUE, ixp, iyp, nsamw, nroww);
#ifndef WEB_TRUE    
       xorbox(imagsav, icontxor, TRUE, ixp, iyp, nsamw, nroww);
#endif 
       }
    }

 /***************************************************** Middle button */ 

 else if (!(strcmp(*params, "2")))
    {     /* Show menu --       Button 2 pushed */
    pickbackmen();
    }

 /****************************************************** Right button */ 

 else if (!left && !(strcmp(*params, "3")))

    {     /*  In right image -- Button 3 pushed */
    getloc(event,'B',&ixs,&iys);

    /* Warp cursor back to left image location */
    spout("Moving cursor back to left");
    movecur(ixp-ixs, iyp-iys);
 
    /* Draw box at previous location on  other side  */ 
    xorbox(iwin,    icontxor, FALSE, ixp, iyp, nsamw, nroww);
#ifndef WEB_TRUE    
    xorbox(imagsav, icontxor, FALSE, ixp, iyp, nsamw, nroww); 
#endif 

    back_win_now--;

    /* Update text box for window key number */
    pickbackmen();

    left = TRUE;
    ixp = ixs; iyp = iys;
    }

 else if (!(strcmp(*params, "3")))
    {                          /*  In left image -- button 3 pushed */
    /* Warp cursor back to previous right position */
    spout("Moving cursor back to right");

    if (gotright) 
       {
       getloc(event,'B',&ixs,&iys);
       left = FALSE;

       movecur(ixp-ixs, iyp-iys);

       /* Draw box at previous location on  other side */ 
       xorbox(iwin,    icontxor, FALSE, ixp, iyp, nsamw, nroww);
#ifndef WEB_TRUE    
       xorbox(imagsav, icontxor, FALSE, ixp, iyp, nsamw, nroww); 
#endif 
       ixp  = ixs; iyp = iys;
       }
    }             
 }
