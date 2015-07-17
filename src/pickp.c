
/*$Header: /usr8/web/src/RCS/pickp.c,v 1.30 2015/06/11 13:24:37 leith Exp $*/

/*
 ***********************************************************************
 C                                                                     *
 C pickp.c                                                             *
 C               Improved                        Jun 2011 ArDean Leith *
 C               Can start pick either side      Jun 2015 ArDean Leith *
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
 ***********************************************************************
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
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* External function prototypes */
 extern void show_tilt(int wantmsg, int wantlabel);
 extern void witran_rev(float *, float *, float, float, int,
                float, float, float);

 /* Internal function prototypes */
 void   pick_pop (Widget, XEvent *, String *, Cardinal *);
 Widget showbuts (Widget, char *, char *, char *, int );
           
 /* Externally defined global variables */
 extern int       firstback;
 extern int       nsaml, nrowl, nsamr, nrowr;
 extern int       nsam1l,nrow1l,nsam2l,nrow2l;
 extern int       nsam1r,nrow1r,nsam2r,nrow2r;
 extern int       ixull, iyull, ixulr, iyulr;
 extern int       ixulli,iyulli,ixulri,iyulri;
 extern int       ixullmin,ixlrlmax,iyullmin,iylrlmax;
 extern int       ixulrmin,ixlrrmax,iyulrmin,iylrrmax;
 extern float     phif, thetaf, gammaff;
 extern char      dfil1[12], dfil2[12];
 extern int       maxpart;        // Max marker in doc file 
 extern int       iredu ;         // Image reduction factor 
 extern Widget    iw_pickmen;     // For: pickmen
 
 // Internally defined global  variables 
 int              openitl, openitr;
 int              nsamsl, nrowsl, nsamsr, nrowsr;
 int              iradi   = 4;      // Marker radius           
 int              numm    = 1;      // Current marker number   
 int              fitted  = FALSE;

 Widget           iw_but_lef0   = 0;     // For: pickmen
 Widget           iw_but_lef1   = 0;     // For: pickmen
 Widget           iw_but_rit0   = 0;     // For: pickmen
 Widget           iw_but_rit1   = 0;     // For: pickmen
 Widget           iw_but_lefrit = 0;     // For: pickmen

 /* Internal file scope  variables */
 static FILE    * fpdoc1  = NULL;
 static FILE    * fpdoc2  = NULL;
 int              left    = TRUE;   // Start with left image   
 int              right   = FALSE;  // Start with left image   

 char     strl[] = 
  "            Particle    Reduced-X,Y-location      Original-X,Y-location   (in untilted image)";
 char     strr[] = 
  "            Particle    Reduced-X,Y-location      Original-X,Y-location   (in   tilted image)";
//0026 6           26          242           61          242           61            1

 /***********************  pick  ***********************************/

 void pickp(int firstrun)

 {
 if (firstrun)
    {
    /* Set next marker to number already in doc file */
    numm = maxpart + 1;
    /* Initialize the first back picking flag */
    firstback = TRUE;
    left      = TRUE;

    //printf("left,numm,maxpart: %d %d \n", left,numm,maxpart);

    if (maxpart > 0) 
       {
       /* Retrieve tilted and untilted points, & fit angles */
       //printf("calling fitdoc: %d %d \n", left,numm,maxpart);
       fitdoc(FALSE);
       }
    }

 // Set circle and # color to green
 wicolor(icontx,colorgo+2);

 openitl = TRUE;
 openitr = TRUE;

 /* Find displayed size of both images */
 nsamsl = nsam2l - nsam1l + 1;
 nsamsr = nsam2r - nsam1r + 1;
 nrowsl = nrow2l - nrow1l + 1;
 nrowsr = nrow2r - nrow1r + 1;

 /* Create button assignment dialogs with following strings  */
 
 iw_but_lef0 = showbuts(iw_but_lef0,
                       "Select left particle.", 
                       "Menu.", 
                       "Reselect previous particle pair", FALSE);

 iw_but_lef1 = showbuts(iw_but_lef1,
                       "Select left particle.", 
                       "Menu.", 
                       "Reselect right particle.", FALSE);

 iw_but_rit0 = showbuts(iw_but_rit0,
                       "Select right particle.", 
                       "Menu.", 
                       "Reselect previous particle pair", FALSE);

 iw_but_rit1 = showbuts(iw_but_rit1,
                       "Select right particle.", 
                       "Menu.", 
                       "Reselect left particle", FALSE);

 iw_but_lefrit = showbuts(iw_but_lefrit,
                       "Select left or right particle.", 
                       "Menu.", 
                       "Reselect previous particle pair", FALSE);

 // Display first button assignment window

 XtManageChild(iw_but_lefrit);

 left    = FALSE;
 right   = FALSE;

 pickmen();
 //XtUnmanageChild(iw_pickmen);

 /* Set  actions for right, left, and center buttons */
 actions(iw_win, "pick_pop", pick_pop,"M123");

 }


 /************************* pick_pop **************************/

 void pick_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 int           ixr, iyr, ixi, iyi, ixs, iys;
 int           ixt, iyt;
 static int    ixp, iyp;
 char          outstr[100];
 float         dlist[8];
 float         xt,yt,  fx,fy;
 char        * string;


 if (!(strcmp(*params, "M")))
   {  /****************************************** Mouse movement only */
   getloc(event,'m',&ixs,&iys);

   if (left && 
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
       sprintf(outstr,"In left image #%d: (%d,%d)$", numm,ixi,iyi);
       spout(outstr);
       }

    else if (right &&
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

 /****************************************************** Left button */

 else if ( !(strcmp(*params, "1")))
    {   /*  Cursor anywhere -- button 1 pushed */ 

    getloc(event,'B',&ixs,&iys);

    //printf("Button 1: %d %d (%d,%d)\n", left, right, ixs,iys);
    //printf("ixull,nsamsl: (%d,%d) \n", ixull,nsamsl);
    //printf("iyull,nrowsl: (%d,%d) \n", iyull,nrowsl);
    //printf("ixulr,nsamsr: (%d,%d) \n", ixulr,nsamsr);
    //printf("iyulr,nrowsr: (%d,%d) \n", iyulr,nrowsr);

    if (left && 
       (ixs < ixull || ixs >= ixull + nsamsl || 
        iys < iyull || iys >= iyull + nrowsl ))
       {    /* Cursor is outside of displayed left image */
       spout("*** Not in left image.$"); XBell(idispl,50); XBell(idispl,50);
       }

    else if ( right &&
       (ixs < ixulr || ixs >= ixulr + nsamsr || 
        iys < iyulr || iys >= iyulr + nrowsr ))
       {    /* Cursor outside displayed right image */
       //printf("xxxButton 1: %d %d (%d,%d)\n", left, right, ixs,iys);
       spout("*** Not in right image.$"); XBell(idispl,50); XBell(idispl,50);
       }
    else if (ixs >= ixull && ixs < ixull + nsamsl &&  
             iys >= iyull && iys < iyull + nrowsl ) 
       {    /* Cursor inside displayed left image */
       ixi = ixs - ixulli + 1;
       iyi = iys - iyulli + 1;
       // sprintf(outstr,"In left image: (%d,%d)$", ixi,iyi);
       // spout(outstr);

       //printf("Left button: %d %d (%d,%d) \n", left,fitted,ixs,iys);
       //printf("ixull,nsamsl: (%d,%d) \n", ixull,nsamsl);
       //printf("iyull,nrowsl: (%d,%d) \n", iyull,nrowsl);

       if (left || (!left && !right))
          {   /* In left image -- button 1 pushed */
              /* Want to record this left particle location */

          /* Save info in doc file */ 
          dlist[0] = numm;
          dlist[1] = numm ;
          dlist[2] = ixi * iredu;
          dlist[3] = iyi * iredu;
          dlist[4] = ixi;
          dlist[5] = iyi;       
          dlist[6] = 1.0; 

          if (openitl)       
             { fpdoc1 = savdnc(dfil1, datexc, &fpdoc1,
                          dlist, 7, &openitl, TRUE, TRUE,strl); }
          else
             { fpdoc1 = savdn1(dfil1, datexc, &fpdoc1,
                          dlist, 7, &openitl, TRUE, TRUE);}

          /* Leave permanent circle at this location */
          xorc(iwin,    icontx, TRUE, ixs, iys, iradi);
          xorc(imagsav, icontx, TRUE, ixs, iys, iradi);

          /* Write marker number at this location */
          string = itoa(numm);
          witext(icontx, string, ixs, iys, 1, 0, -1, 2, FALSE);
          if (string) free(string);

          spoutfile(TRUE);

          /* Find predicted location in right image */
          if (fitted)
             {
             /* Transform the x values */
             fx = (float)ixi;
             fy = (float)iyi;
	     
             /* Use angles to get predicted location in tilted image */
             witran(&fx, &fy,  &xt, &yt, 1, gammaff, thetaf, phif);
   
             ixt = xt;
             iyt = yt;
   
             if (ixt < 1 || ixt > nsamr || iyt < 1 || iyt > nrowr)
                {    /* Predicted cursor loc. is outside of right image, */
                sprintf(outstr,"*** Tilted cursor not in right image: (%d,%d)",
                                ixt,iyt);
                spout(outstr); XBell(idispl,50);
                }
             else
                {   /* Move cursor to predicted location on right tilted side */
                ixt = ixt + ixulri;
                iyt = iyt + iyulri;
                movecur(ixt-ixs,iyt-iys);
                }
             }
          else   //  if (!fitted)
             { /* No tilt angle available yet */
               /* Move cursor to center of right tilted side */
             ixt = ixulr + nsamsr / 2;
             iyt = iyulr + nrowsr / 2;
             movecur(ixt-ixs,iyt-iys);
             }

          if (numm > 3 && numm < 32 && left)
             { 
             show_tilt(FALSE,TRUE);
             sprintf(outstr,"Picked left:  #%d  (%d,%d)   Current Tilt: %f6.2",numm,ixi,iyi,thetaf);
             }
          else
             { 
             sprintf(outstr,"Picked left:  #%d  (%d,%d)",  numm,ixi,iyi);
             }
          spout(outstr);
          spoutfile(FALSE);


          if (left || right) {numm++;}
          if (numm > maxpart) maxpart = numm;

          if (left) 
             { right = FALSE;}
          else  
             { right = TRUE; }
          left  = FALSE;

          // Show button message  
          XtUnmanageChild(iw_but_lef0);
          XtUnmanageChild(iw_but_lef1);
          XtUnmanageChild(iw_but_rit0);
          XtUnmanageChild(iw_but_rit1);
          XtUnmanageChild(iw_but_lefrit);
          if (left) 
             { XtManageChild(iw_but_lef1); }
          else if (right)
             { XtManageChild(iw_but_rit1); }
          else
             { XtManageChild(iw_but_lefrit);}

          /* Record undo location */
          ixp  = ixs;
          iyp  = iys;
          }

       } // End of: if (left || (fitted && !left && !right))

    else if (ixs >= ixulr && ixs < ixulr + nsamsr &&  
             iys >= iyulr && iys < iyulr + nrowsr )
       {    /* Cursor is inside displayed right image */
       ixi = ixs - ixulri + 1;
       iyi = iys - iyulri + 1;
       // sprintf(outstr,"In right image: (%d,%d)$", ixi,iyi);
       // spout(outstr);

       if (right || ( !left && !right))
          {   /* In right image -- button 1 pushed */
              /* Want to record this right particle location */
          spoutfile(TRUE);

          /* Save info in doc file */ 
          dlist[0] = numm;
          dlist[1] = numm;
          dlist[2] = ixi * iredu;
          dlist[3] = iyi * iredu;
          dlist[4] = ixi;
          dlist[5] = iyi;       
          dlist[6] = 1.0;   
	    
          if (openitr)       
             {fpdoc2 = savdnc(dfil2, datexc, &fpdoc2,
                             dlist, 7, &openitr, TRUE, TRUE,strr);}
          else
             {fpdoc2 = savdn1(dfil2, datexc, &fpdoc2,
                             dlist, 7, &openitr, TRUE, TRUE);}

          /* Leave permanent circle at this location */
          xorc(iwin,    icontx, TRUE, ixs, iys, iradi);
          xorc(imagsav, icontx, TRUE, ixs, iys, iradi);

          /*   Write marker number at this location */
          string = itoa(numm);
          witext(icontx, string, ixs, iys, 1, 0, -1, 2, FALSE);
          if (string) free(string);

          /* Find predicted location in left image */
          if (fitted)
             {
             /* Transform the x values */
             fx = (float)ixi;
             fy = (float)iyi;
	     
             /* Use angles to get predicted location in un-tilted image */
             witran_rev(&xt, &yt,  fx, fy, 1, gammaff, thetaf, phif);
   
             ixt = xt;
             iyt = yt;
   
             if (ixt < 1 || ixt > nsaml || iyt < 1 || iyt > nrowl)
                {    /* Predicted cursor loc. is outside of left image, */
                sprintf(outstr,"*** Untilted cursor not in left image: (%d,%d)",
                                ixt,iyt);
                spout(outstr); XBell(idispl,50);
                }
             else
                {   /* Move cursor to predicted location on left untilted side */
                ixt = ixt + ixulli;
                iyt = iyt + iyulli;
                movecur(ixt-ixs,iyt-iys);
                }             
             }
          else   //  if (!fitted)
             {   /* No tilt angle available yet */
                 /* Move cursor to center of left untilted side */
             ixt = ixull + nsamsl / 2;
             iyt = iyull + nrowsl / 2;
             movecur(ixt-ixs,iyt-iys);
             }

          if (numm > 3 && numm < 32 && right)
             { 
             show_tilt(FALSE,TRUE);
             sprintf(outstr,"Picked right: #%d  (%d,%d)   Current tilt: %f6.2",numm,ixi,iyi,thetaf);
             }
          else
             { 
             sprintf(outstr,"Picked right: #%d  (%d,%d)", numm,ixi,iyi);
             }
          spout(outstr);
          spoutfile(FALSE);

          if (left || right) numm++ ;
          if (numm > maxpart) maxpart = numm;

          if (right) 
             { left = FALSE;}
          else  
             { left = TRUE; }
          right  = FALSE;

          // Show button message  
          XtUnmanageChild(iw_but_lef0);
          XtUnmanageChild(iw_but_lef1);
          XtUnmanageChild(iw_but_rit0);
          XtUnmanageChild(iw_but_rit1);
          XtUnmanageChild(iw_but_lefrit);
          if (left) 
             { XtManageChild(iw_but_lef1); }
          else if (right)
             { XtManageChild(iw_but_rit1); }
          else
             { XtManageChild(iw_but_lefrit);}

          /* Record undo location */
          ixp  = ixs;
          iyp  = iys;
          }// End of:  if (left || (fitted && !left && !right))
       }   // End of:  if (left || (fitted && !left && !right))
    }      // End of:  else if (ixs >= ixulr && ixs < ixulr + nsamsr &&  iys >= iyulr && iys < iyulr + nrowsr )

 /***************************************************** Middle button */ 

 else if (!(strcmp(*params, "2")))
    {                          /* Show menu --       Button 2 pushed */
    if (fpdoc1) 
       { fclose(fpdoc1); fpdoc1 = NULL; openitl = TRUE; }

    if (fpdoc2) 
       { fclose(fpdoc2); fpdoc2 = NULL; openitr = TRUE; }

    /* Display picking menu */
    pickmen();

    }   // End of: else if (!(strcmp(*params, "2"))) 

 /****************************************************** Right button */ 

 else if (!(strcmp(*params, "3")))
    {                          /* Button 3 pushed */
    getloc(event,'B',&ixs,&iys);

    // Show button message  
    XtUnmanageChild (iw_but_lef0);
    XtUnmanageChild (iw_but_lef1);
    XtUnmanageChild (iw_but_rit0);
    XtUnmanageChild (iw_but_rit1);

   if (right)

       {   // Move cursor back to left image location 
       // spout("Moving cursor back to left"); 
      
       // Show button assignment message  
       XtManageChild (iw_but_lef1);
    
       movecur(ixp-ixs, iyp-iys);

       right = FALSE;
       ixp   = ixs; iyp = iys;
       }

    else if (left)
       {  // Move cursor back to previous right position 
       // spout("Moving cursor back to right");        

       // Show button assignment message  
       XtManageChild (iw_but_rit1);

       left = FALSE;
       movecur(ixp-ixs, iyp-iys);
       ixp  = ixs; iyp = iys;
       }

    else if (!left && !right)
       {   

       // Show button assignment message  
       XtManageChild (iw_but_lefrit);

       numm--;
       ixp     = ixs; iyp = iys;
       }
    }      // End of: else if (!(strcmp(*params, "3"))) 
 }

