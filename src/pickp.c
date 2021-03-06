
/*$Header: /usr16/software/web/src/RCS/pickp.c,v 1.36 2018/12/07 17:03:33 leith Exp $*/

/*
 C**********************************************************************
 C                                                                     *
 C pickp.c                                                             *
 C               Improved                        Jun 2011 ArDean Leith *
 C               Can start pick either side      Jun 2015 ArDean Leith *
 C               Altered particle deletion       Jul 2015 ArDean Leith *
 C               Altered ouput messages          Aug 2015 ArDean Leith *
 C                                                                     *
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
 *
 * pickp
 * 
 * PURPOSE:     Interactive tilt-pair particle selecting      
 * 
 * PARAMETERS:	 
 *
 * VARIABLES:   xim       Imaged number             dfil1 + 2
 *              xu0,yu0   Untilted picked points    dfil1
 *              xs,ys     Tilted picked points      dfil2
 *              xs2,ys2   Tilted predicted points   
 *
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "pick.h"
#include "fit.h"

 /* External function prototypes */

 /* Internal function prototypes */
 static void      pick_pop (Widget, XEvent *, String *, Cardinal *);
 static Widget    showbuts (Widget, char *, char *, char *, int );
           
 /* Externally defined global variables */

 
 // Internally defined global variables 
 int              openit1, openit2;
 int              nsamsl, nrowsl, nsamsr, nrowsr;
 int              iradi         = 4;          // Particle radius on screen           
 int              numm          = 1;          // Current particle number   
 int              numm1         = 0;          // Current particle number   
 int              numm2         = 0;          // Current particle number   
 int              fitted        = FALSE;      // Assume unfitted start
 int              leftside      = FALSE;      // Start with either image   
 int              rightside     = FALSE;      // Start with either image   
 Widget           iw_but_lefrit = (Widget) 0; // For: pickmen
 Widget           iw_but_str    = (Widget) 0; // For: pickmen

 //                0026 6         26          242           61          242           61            1
 char             strcom1[] = 
                  " (Untilted)  Particle    Picked-X,Y-location       Original-X,Y-location ";
 char             strcom2[] = 
                  " (Tilted)    Particle    Picked-X,Y-location       Original-X,Y-location ";

 /* Internal file scope  variables */
 static FILE    * fpdoc1  = NULL;
 static FILE    * fpdoc2  = NULL;

 /***********************  pick  ***********************************/

 void pickp(int firstrun)

 {
 if (firstrun)
    {
    /* Retrieve any existing tilted and untilted points, & fit angles */ 
    if (fitdoc(TRUE,TRUE,TRUE,TRUE) > 0)
       { 
       spout("*** Can not continue, fix document files!");
       XBell(idispl,50); XBell(idispl,50); XBell(idispl,50); 
       return;
       }

    /* Set next particle number based on number already in doc file */
    numm = maxpart + 1;

   /* Initialize the first back picking flag */
    firstback = TRUE;

    // printf(" maxpart: %d \n", maxpart); 

    if (maxpart > 0)
       {/* Draw any existing particle numbers & their locations */
       pickdraw(TRUE, TRUE, FALSE, TRUE, FALSE, maxpart);
       }
    }

 // Set circle and # color to green
 wicolor(icontx,colorgo+2);

 openit1 = TRUE;
 openit2 = TRUE;

 /* Find displayed size of both images */
 nsamsl = nsam2l - nsam1l + 1;
 nsamsr = nsam2r - nsam1r + 1;
 nrowsl = nrow2l - nrow1l + 1;
 nrowsr = nrow2r - nrow1r + 1;


 // Display first button assignment window
 showbuts_str(&iw_but_lefrit, &iw_but_str,
              "Select left or right particle.", 
              "Show menu.", 
              "Delete a particle pair.", TRUE);

 //printf(" iw_but_lefrit: %d  iw_but_str: %d \n",iw_but_lefrit, iw_but_str);

 leftside    = FALSE;
 rightside   = FALSE;

 pickmen();

 /* Set  actions for right, left, and center buttons */
 actions(iw_win, "pick_pop", pick_pop,"M123");

 }


 /************************* pick_pop **************************/

 void pick_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 int           ixi, iyi, ixs, iys,iok, ixt, iyt, derror, iflag;
 char          outstr[100];
 float         dlist[8];
 float         fxi,fyi;
 float         x1,y1, x2,y2, xt,yt;
 char        * string;
 static int    ix1, iy1, ix2, iy2;

 int           nsaytilt      = 48;  /* Say angle for this many particles */
 int           predict_error = 10;  /* Max expected difference from predicted location */

 if (!(strcmp(*params, "M")))
   {  /************************************************ Mouse movement only */
   getloc(event,'m',&ixs,&iys);

   if (leftside && 
       (ixs < ixull || ixs >= ixull + nsamsl || 
        iys < iyull || iys >= iyull + nrowsl ))
       {    /* Cursor is outside of displayed left image */
       spout("*** Not in left image.$");
       }

   else if (ixs >= ixull && ixs < ixull + nsamsl && 
            iys >= iyull && iys < iyull + nrowsl )
       {    /* Cursor inside displayed left image */
       ixi = ixs - ixulli + 1;
       iyi = iys - iyulli + 1;
       sprintf(outstr,"In left  image #%d: (%d,%d)$", numm,ixi,iyi);
       spout(outstr);
       }

    else if (rightside &&
       (ixs < ixulr || ixs >= ixulr + nsamsr || 
        iys < iyulr || iys >= iyulr + nrowsr ))
       {    /* Cursor outside displayed right image */
       spout("*** Not in right image.$");
       }

    else if (ixs >= ixulr && ixs < ixulr + nsamsr && 
             iys >= iyulr && iys < iyulr + nrowsr )
       {    /* Cursor inside displayed right image */
       ixi = ixs - ixulri + 1;
       iyi = iys - iyulri + 1;
       sprintf(outstr,"In right image #%d: (%d,%d)$", numm,ixi,iyi);
       spout(outstr);
       }
    else
       {    /* Cursor is outside both images */
       ixi = ixs - ixulri + 1;
       iyi = iys - iyulri + 1;
       spout("*** Not in either image.$");
       }
    }    // End of: if (!(strcmp(*params, "M")))

    /* Should add position indicator in right side window !!!!! */ 

 /******************************************************************** Left button */

 else if ( !(strcmp(*params, "1")))
    {   /*  Cursor anywhere -- button 1 pushed */ 

    getloc(event,'B',&ixs,&iys);
    iflag = 0;

    //printf("Button 1: %d %d (%d,%d)\n", leftside, rightside, ixs,iys);
    //printf("ixull,nsamsl: (%d,%d) \n", ixull,nsamsl);
    //printf("iyull,nrowsl: (%d,%d) \n", iyull,nrowsl);
    //printf("ixulr,nsamsr: (%d,%d) \n", ixulr,nsamsr);
    //printf("iyulr,nrowsr: (%d,%d) \n", iyulr,nrowsr);

    if (leftside && 
       (ixs < ixull || ixs >= ixull + nsamsl || 
        iys < iyull || iys >= iyull + nrowsl ))
       {    /* Cursor is outside of displayed left image */
       spout("*** Not in left image.$"); XBell(idispl,50); XBell(idispl,50);
       }

    else if ( rightside &&
       (ixs < ixulr || ixs >= ixulr + nsamsr || 
        iys < iyulr || iys >= iyulr + nrowsr ))
       {    /* Cursor outside displayed right image */
       //printf("Button 1: %d %d (%d,%d)\n", leftside, rightside, ixs,iys);
       spout("*** Not in right image.$"); XBell(idispl,50); XBell(idispl,50);
       }

    else if (ixs >= ixull && ixs < ixull + nsamsl &&  
             iys >= iyull && iys < iyull + nrowsl ) 
       {    /* Cursor inside displayed left image, Save location */
       ixi = ixs - ixulli + 1;
       iyi = iys - iyulli + 1;
       // sprintf(outstr,"In left image: (%d,%d)$", ixi,iyi); spout(outstr);
       // printf("Left button: %d %d (%d,%d) \n", leftside,fitted,ixs,iys);
       // printf("ixull,nsamsl: (%d,%d) \n", ixull,nsamsl);
       // printf("iyull,nrowsl: (%d,%d) \n", iyull,nrowsl);

       derror = 0; 
       if (leftside || (!leftside && !rightside))
          { // In left image -- button 1 pushed, record current location  

          /* Check for possible bad leftside location */
          if (leftside && fitted)
             { 
             derror = (int) (sqrt((float)( (ixi - ix1) * (ixi - ix1) + 
                                           (iyi - iy1) * (iyi - iy1)) ));
             }    

          if (fitted)
             { 
             /* Use angles to get predicted location in tilted rightside image */
             fxi = (float)ixi;   fyi = (float)iyi;
             witran(&fxi,&fyi,  &x2,&y2, 1, gammaff, thetaf, phif);
             ix2 = x2;   iy2 = y2;
             }   // End of: if (fitted)


          if (!leftside && fitted)
             {   /* Check for future bad rightside location */
             if (ix2 < 1 || ix2 > nsamr || iy2 < 1 || iy2 > nrowr)
                {    /* Predicted cursor loc. is outside of right image, */
                sprintf(outstr,"*** Tilted cursor not in right image: (%d,%d)$",
                                ix2,iy2);
                spout(outstr); XBell(idispl,50);
                return;
                }
             }   // End of: if (!leftside && fitted)


          /* Save location in array: xu,yu */
          iok =  fitdoc_addpart(numm,  1, ixi,iyi, 0,0 );

          // printf(" Leftside: %d   rightside: %d   numm: %d\n",leftside,rightside,numm);
          // printf(" Predicted: %d, %d  Picked: %d, %d \n", (int)xt,(int)yt, ixi,iyi);

          if (leftside) 
             {    /* Save info in doc files */ 
             dlist[0] = numm;
             dlist[1] = numm ;
             dlist[2] = ixi * iredu;
             dlist[3] = iyi * iredu;
             dlist[4] = ixi;
             dlist[5] = iyi;       
             dlist[6] = 1.0; 

             if (numm == 1)       
                { fpdoc1 = savdnc(dfil1, datexc, &fpdoc1,
                              dlist, 7, &openit1, TRUE, TRUE,strcom1); }
             else
                { fpdoc1 = savdn1(dfil1, datexc, &fpdoc1, 
                              dlist, 7, &openit1, TRUE, TRUE);}

             dlist[2] = xs[numm] * iredu;
             dlist[3] = ys[numm] * iredu;
             dlist[4] = xs[numm];
             dlist[5] = ys[numm];  
     
             if (numm == 1)       
                { fpdoc2 = savdnc(dfil2, datexc, &fpdoc2,
                              dlist, 7, &openit2, TRUE, TRUE,strcom2); }
             else
                { fpdoc2 = savdn1(dfil2, datexc, &fpdoc2,
                              dlist, 7, &openit2, TRUE, TRUE);}
             }

          /* Leave circle at this leftside location */
          xorc(iwin,    icontx, TRUE, ixs, iys, iradi);
          xorc(imagsav, icontx, TRUE, ixs, iys, iradi);

          /* Write particle number at this leftside location */
          string = itoa(numm);
          witext(icontx, string, ixs, iys, 1, 0, -1, 2, FALSE);
          if (string) free(string);
         
          spoutfile(TRUE);

          /* Find predicted location in right image */
          if (fitted)
             {   /* Move cursor to predicted location on right tilted side */
             ixt = ix2 + ixulri;
             iyt = iy2 + iyulri;
             movecur(ixt-ixs,iyt-iys);
             }
          else   //  if (!fitted)
             { /* No tilt angle available, Move cursor to center of right side */
             ixt = ixulr + nsamsr / 2;
             iyt = iyulr + nrowsr / 2;
             movecur(ixt-ixs,iyt-iys);
             }

          if (numm > 2 && numm < nsaytilt && leftside)
             { /* Determine tilt and axis, Sets: fitted */
             iflag = det_tilt(FALSE,TRUE);
             }

          if ( iflag == - numm || derror > predict_error ) 
             { 
             if (derror > predict_error ) printf("*** Check location: %d  for bad position? \n",numm);
             sprintf(outstr,
                  "Picked left:   #%d  (%d,%d)  Distance: %3d  Current Tilt: %-.2f  Axis: %-.2f, %-.2f  BAD??",
                   numm,ixi,iyi, derror, thetaf,phif,gammaff);
             XBell(idispl,50);
             }
          else
             {
             sprintf(outstr,
                  "Picked left:   #%d  (%d,%d)  Distance: %3d  Current Tilt: %-.2f  Axis: %-.2f, %-.2f",
                   numm,ixi,iyi, derror, thetaf,phif,gammaff);
             }
          spout(outstr);
          spoutfile(FALSE);

          if (numm > maxpart) maxpart = numm;
          if (leftside || rightside) {numm++;}

          if (leftside) 
             { rightside = FALSE;}
          else  
             { rightside = TRUE; }
          leftside  = FALSE;

          // Show button message  
          if (leftside)
             { 
             showbuts_str(&iw_but_lefrit, &iw_but_str,
                          "Select left particle.         ", 
                          "Show menu.", 
                          "Delete a particle pair.", TRUE);
             }
          else if (rightside)
             { 
             showbuts_str(&iw_but_lefrit, &iw_but_str,
                       "Select right particle.        ", 
                       "Show menu.", 
                       "Delete a particle pair.", TRUE);
             }
          else
             { 
             showbuts_str(&iw_but_lefrit, &iw_but_str,
                       "Select left or right particle.", 
                       "Show menu.", 
                       "Delete a particle pair.", TRUE);
             }
         }
       //printf(" Left maxpart: %d, numm: %d\n", maxpart,numm);

       } // End of: if (leftside || (fitted && !leftside && !rightside))

    else if (ixs >= ixulr && ixs < ixulr + nsamsr &&  
             iys >= iyulr && iys < iyulr + nrowsr )
       {    /* Cursor is inside displayed right image ------------------------------ */
       ixi = ixs - ixulri + 1;
       iyi = iys - iyulri + 1;
       // sprintf(outstr,"In right image: (%d,%d)$", ixi,iyi);
       // spout(outstr);

       if (rightside || (!leftside && !rightside))
          { // In right image -- button 1 pushed, record current location  

          /* Check for possible bad location */
          derror = 0; 
          if (rightside && fitted)
             {
             derror = (int) (sqrt((float)( (ixi - ix2) * (ixi - ix2) + 
                                           (iyi - iy2) * (iyi - iy2)) ));
             }   

          if (fitted)
             { 
             /* Use angles to get predicted location in untilted leftside image */
             fxi = (float)ixi;   fyi = (float)iyi;
             witran_rev(&x1,&y1,  fxi,fyi, 1, gammaff, thetaf, phif);
             ix1 = x1;   iy1 = y1;
             }   // End of: if (fitted)

          if (!rightside && fitted)
             {  /* Check for future bad leftside location */
             if (ix1 < 1 || ix1 > nsaml || iy1 < 1 || iy1 > nrowl)
                {    /* Predicted cursor loc. is outside of left image, */
                sprintf(outstr,"*** Untilted cursor not in left image: (%d,%d)$",
                                ix1,iy1);
                spout(outstr); XBell(idispl,50);
                return;
                }
             }   // End of: if (!rightside && fitted)

          /* Save location in array: xs,ys */
          iok =  fitdoc_addpart(numm,  2, 0,0, ixi,iyi );

          // printf(" Leftside: %d   rightside: %d   numm: %d\n",leftside,rightside,numm);
          // printf(" Predicted: %d, %d  Picked: %d, %d \n", (int)xt,(int)yt, ixi,iyi);

          if (rightside) 
             {    /* Save info in doc files */ 
             dlist[0] = numm;
             dlist[1] = numm ;
             dlist[2] = ixi * iredu;
             dlist[3] = iyi * iredu;
             dlist[4] = ixi;
             dlist[5] = iyi;       
             dlist[6] = 1.0; 

             if (numm == 1)       
                { fpdoc2 = savdnc(dfil2, datexc, &fpdoc2,
                              dlist, 7, &openit2, TRUE, TRUE,strcom2); }
             else
                { fpdoc2 = savdn1(dfil2, datexc, &fpdoc2, 
                              dlist, 7, &openit2, TRUE, TRUE);}

             dlist[2] = xu0[numm] * iredu;
             dlist[3] = yu0[numm] * iredu;
             dlist[4] = xu0[numm];
             dlist[5] = yu0[numm];  
     
             if (numm == 1)       
                { fpdoc1 = savdnc(dfil1, datexc, &fpdoc1,
                              dlist, 7, &openit1, TRUE, TRUE,strcom2); }
             else
                { fpdoc1 = savdn1(dfil1, datexc, &fpdoc1,
                              dlist, 7, &openit1, TRUE, TRUE);} 
             }

          /* Leave circle at this rightside location */
          xorc(iwin,    icontx, TRUE, ixs, iys, iradi);
          xorc(imagsav, icontx, TRUE, ixs, iys, iradi);

          /* Write particle number at this rightside location */
          string = itoa(numm);
          witext(icontx, string, ixs, iys, 1, 0, -1, 2, FALSE);
          if (string) free(string);
         
          spoutfile(TRUE);

          if (fitted)
             {   /* Move cursor to predicted location on left untilted side */
             ixt = ix1 + ixulli;
             iyt = iy1 + iyulli;
             movecur(ixt-ixs,iyt-iys);
             }             
          else   
             {   /* No tilt angle available yet */
                 /* Move cursor to center of left untilted side */
             ixt = ixull + nsamsl / 2;
             iyt = iyull + nrowsl / 2;
             movecur(ixt-ixs,iyt-iys);
             }

          if (numm > 2 && numm < nsaytilt && rightside)
             { /* Determine tilt and axis, Sets: fitted */
             iflag = det_tilt(FALSE,TRUE);
             }

          if (iflag == -numm || derror > predict_error ) 
             { 
             if (derror > predict_error ) printf("*** Check location: %d  for bad position? \n",numm);
             sprintf(outstr,
                  "Picked right:  #%d  (%d,%d)  Distance: %3d  Current Tilt: %-.2f  Axis: %-.2f, %-.2f   BAD??",
                   numm,ixi,iyi, derror, thetaf,phif,gammaff); 
             XBell(idispl,50);
             }
          else
             {
             sprintf(outstr,
                  "Picked right:  #%d  (%d,%d)  Distance: %3d  Current Tilt: %-.2f  Axis: %-.2f, %-.2f",
                   numm,ixi,iyi, derror, thetaf,phif,gammaff);
              } 
          spout(outstr);
          spoutfile(FALSE);

          if (numm > maxpart) maxpart = numm;
          if (leftside || rightside) numm++ ;

          if (rightside) 
             { leftside = FALSE;}
          else  
             { leftside = TRUE; }
          rightside  = FALSE;

          // Show button message  
          if (leftside)
             { 
             showbuts_str(&iw_but_lefrit,&iw_but_str,
                          "Select left particle.         ", 
                          "Show menu.", 
                          "Delete a particle pair.", TRUE);
             }
          else if (rightside)
             { 
             showbuts_str(&iw_but_lefrit,&iw_but_str,
                          "Select right particle.        ", 
                          "Show menu.", 
                          "Delete a particle pair.", TRUE);
             }
          else
             { 
             showbuts_str(&iw_but_lefrit,&iw_but_str,
                          "Select left or right particle.", 
                          "Show menu.", 
                          "Delete a particle pair.", TRUE);
             }

          }// End of:  if (leftside || (fitted && !leftside && !rightside))
       }   // End of:  if (leftside || (fitted && !leftside && !rightside))

    // Update label box for next particle number 
    pickmen();

    }      // End of:  else if (ixs >= ixulr && ixs < ixulr + ..

 /***************************************************** Middle button */ 

 else if (!(strcmp(*params, "2")))
    {                          /* Show menu --       Button 2 pushed */
    if (fpdoc1) 
       { fclose(fpdoc1); fpdoc1 = NULL; openit1 = TRUE; }
    if (fpdoc1) 
       { fclose(fpdoc2); fpdoc2 = NULL; openit2 = TRUE; }

    /* Display picking menu */
    pickmen();

    }   // End of: else if (!(strcmp(*params, "2"))) 

 /****************************************************** Right button */ 

 else if (!(strcmp(*params, "3")))
    {                          /* Button 3 pushed */
    getloc(event,'B',&ixs,&iys);

    if (fpdoc1) 
       { fclose(fpdoc1); fpdoc1 = NULL; openit1 = TRUE; }
    if (fpdoc2) 
       { fclose(fpdoc2); fpdoc2 = NULL; openit2 = TRUE; }

    leftside  = FALSE;
    rightside = FALSE;

    /* Display particle deletion menu */
    tiltp_deletemen();

    // Replace button messages  
    showbuts_str(&iw_but_lefrit, &iw_but_str,
              "Select left or right particle.", 
              "Show menu.", 
              "Delete a particle pair.", TRUE);

    }      // End of: else if (!(strcmp(*params, "3"))) 

 }

