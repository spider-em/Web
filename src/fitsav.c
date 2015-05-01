
/*$Header: /usr8/web/src/RCS/fitsav.c,v 1.25 2011/06/22 19:09:19 leith Exp $*/

/*
C++*********************************************************************
C
C    fitsav
C
C **********************************************************************
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
C    fitsav
C
C    PURPOSE:    Saves fitted angles in dfil3
C                Saves tilted predicted loactions in dfil6
C
C    CALLED BY:   
C
C **********************************************************************
*/

#include "std.h"
#include "common.h"
#include "x.h"
#include "routines.h"

 /* Function prototypes */

 /* External common variables used here */
 extern char   dfil3[12], dfil6[12];     /* Doc file names     */
 extern int    iredu;                    /* Image reduction */
 extern int    fitted;
 extern float  phif, thetaf, gammaff;    /* Tilt angles       */
 extern float  xu0t, yu0t, xs0t, ys0t;   /* Tilt origins      */
 extern float  * xu0, * yu0, * xs2, * ys2; 
 extern int    lbnum, maxpart;
 extern char   datexc[4];                /* File extension   */    
     

 char     str121[] =
  " Key: 121                                                   #Markers-fitted   #Backgrounds";
 //121 6            0            0            0            0           24            0

 char     str122[] = 
  " Key: 122   Fitted-flag";
//0122 6            1            0            0            0            0            0

 char     str123[] = 
  " Key: 123    Untilted-X,Y-Origin      Tilted-X,Y-Origin       Img-Reduction";
//0123 6          379          211      405.087      245.268            1            0

 char     str124[] = 
  "Key: 124 Tilt-angle  Untilted,tilted-axis-dir (Theta, Gamma,Phi)";
//0124 6      49.1505     -77.9898     -77.7016            0            0            0

 char     strdft[] = 
  "            Particle   Reduced-X,Y-location     Original-X,Y-location           Flag";
//0001 6            1      404.354      244.869      404.354      244.869            0


/*****************************   fitsav   ****************************/

 void fitsav(int limfitt)

 { 
 float          dlist[8];
 FILE           * fpdoc;
 int            i, openit, append_flag;

 /* Save fit info in dfil3 (dcb***) doc file */ 
 dlist[0] = 121;
 dlist[1] = 0.0;
 dlist[2] = 0.0;
 dlist[3] = 0.0;
 dlist[4] = 0.0;
 dlist[5] = limfitt; 
 dlist[6] = lbnum; 
    
 /* dfil3  (dcb***) will be opened and closed here. */
 openit   = TRUE;  /* Openit first time to get fpdoc pointer */
 fpdoc    = savdnc(dfil3, datexc, &fpdoc, dlist, 
                   7, &openit, FALSE, TRUE,str121);
 if (!fpdoc) 
    {  XBell(idispl,50); XBell(idispl,50); return; }

 /* Set fitted flag */
 dlist[0] = 122;
 dlist[1] = fitted;
 dlist[2] = 0.0;
 dlist[3] = 0.0;
 dlist[4] = 0.0;
 dlist[5] = 0.0;
 dlist[6] = 0.0;
      
 fpdoc    = savdnc(dfil3, datexc, &fpdoc,dlist, 
                  7, &openit, TRUE, TRUE,str122);

 /* Save origin info*/
 dlist[0] = 123;
 dlist[1] = xu0t;
 dlist[2] = yu0t;
 dlist[3] = xs0t;
 dlist[4] = ys0t;
 dlist[5] = iredu; 
 dlist[6] = 0.0;
 
 fpdoc    = savdnc(dfil3, datexc, &fpdoc,dlist, 
                   7, &openit, TRUE, TRUE,str123);

 /* Set tilt angle info */
 dlist[0] = 124;
 dlist[1] = thetaf;
 dlist[2] = gammaff;
 dlist[3] = phif;
 dlist[4] = 0.0;
 dlist[5] = 0.0; 
 dlist[6] = 0.0;
      
 fpdoc    = savdnc(dfil3, datexc, &fpdoc, dlist, 
                   7, &openit, TRUE, TRUE, str124);

 fclose(fpdoc); fpdoc = NULL;

 if (fitted)
    {
    openit = TRUE; append_flag = TRUE;

    /* Use angles to get predicted location in tilted image  */
    witran(xu0, yu0, xs2, ys2, maxpart, gammaff, thetaf, phif);

    dlist[6] = 0.0;
       
    for ( i = 0; i < maxpart; i++)
       {
       /* Save predicted right locations in doc file: dfil6 (dft***) */ 
       dlist[0] = i+1;
       dlist[1] = i+1;
       dlist[2] = xs2[i] * iredu;
       dlist[3] = ys2[i] * iredu;
       dlist[4] = xs2[i];
       dlist[5] = ys2[i];       
   
       if (i == 0) 
          { /* First time thru */ 
          fpdoc   = savdnc(dfil6, datexc, &fpdoc,
                         dlist, 7, &openit, append_flag, TRUE, strdft);
          if (!fpdoc) 
              { XBell(idispl,50); XBell(idispl,50); return; }

          append_flag = TRUE;
          }
       else
          {fpdoc   = savdn1(dfil6, datexc, &fpdoc,
                          dlist, 7, &openit, append_flag,TRUE);}

       }
    fclose(fpdoc);
    }
 }

