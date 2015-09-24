
/*$Header: /usr8/web/src/RCS/tiltang.c,v 1.16 2015/09/22 16:33:57 leith Exp $*/

/*
 C**********************************************************************
 C                                                                     *
 C tiltang.c                                                           *
 C         Improved                              Jun 2015 ArDean Leith *
 C         Considers all possible locations      Sep 2015 ArDean Leith *
 C         Improved bad location determination   Sep 2015 ArDean Leith *
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
 C
 C   tiltang(x0,y0, xt,yt, npoints,theta,iarea,arealim, wantmsg)
 C
 C   PURPOSE:  Calculate the tiltangle between two images
 C
 C   CALLED BY: 
 C
 C   RETURNS:  0   OK
 C             1   Not enough locations or area betweeen locations
 C            -n   Last bad location = n
 C  
 C   ALSO RETURNS:  theta      Computed tilt angle
 C                             (Theta only altered if ok)
 C                  iarea      Number of triangles used to get tilt             
 C                         
 C*********************************************************************
*/

#include "common.h"
#include "routines.h"

 int tiltang(float *x0,   float *y0, 
             float *xt,   float *yt,
	     int npoints, float * theta, 
             int * iarea, float arealim, int wantmsg)
 {
 char        outmsg[80];
 int         i, j, k, ntot, iflag;
 float	     areau, areat, w, sum, temp;
 float       x01, x02, y01, y02, xt1, xt2, yt1, yt2;
 int         nbads[npoints+1];
 int         maxbad, locbad;

 const float pi = 3.14159;

 sum    = 0.0;
 *iarea = 0;
 ntot   = 0;

 // zero bad location tracking array
 for (i = 0; i <= npoints; nbads[i]= 0, i++);

 if (npoints < 3)
    {             //123456789 123456789 123456789 123456789 123456789 123456789 1234567890
    sprintf(outmsg,"*** Unable to compute tilt;  Need > 2 locations, Have only: %d", npoints); 
    spout(outmsg); 
    XBell(idispl,50);
    return 1;
    }

 /* Set good locations return flag */
 iflag = 0;

 for       (i = 0;     i < npoints-2; i++)
    {
    for    (j = i + 1; j < npoints-1; j++)
       {
       for (k = j + 1; k < npoints;   k++)
          {
          /* Find area in untilted image: */
	  x01   = x0[j] - x0[i];
	  y01   = y0[j] - y0[i];
	  x02   = x0[k] - x0[i];
	  y02   = y0[k] - y0[i];
	  areau = (float) fabs( (double)(x01 * y02 - x02 * y01));

          ntot++;

          //printf("*** Triangle: %d, %d, & %d  Area: %f \n", i+1, k+1, k+2, areau ); 

          /* Default arealim is 5000 square pixels */
	  if (areau >= arealim)
	      {  /* Only triangles > arealim are considered */

              /* Find area in tilted image: */
              xt1   = xt[j] - xt[i];
              yt1   = yt[j] - yt[i];
              xt2   = xt[k] - xt[i];
              yt2   = yt[k] - yt[i];
              areat = (float) fabs( (double)(xt1 * yt2 - xt2 * yt1));

              //printf("*** %d %d %d Areas: %6.2f, %6.2f   \n", i+1,j+1,k+1, areau,areat); 

	      if (areat >= arealim)
                 {  /* Only triangles > arealim are considered */

                 /* Area in tilted image should be <= area in untilted */ 
                 w = areat / areau;
              
                 if (w > 1.0)
                    {     /* Set bad location return flag */
                    nbads[i+1]++;
                    nbads[j+1]++;
                    nbads[k+1]++;

                    //printf("*** Check locations: %4d, %4d & %d  for a bad value %d,%d,%d \n", i+1, j+1, k+1, 
                    //        nbads[i+1],nbads[j+1],nbads[k+1]); 
                    //printf("*** Areas: %6.2f, %6.2f  Lim: %6.2f \n", areau,areat,arealim); 
                    }
	         else
	            {
	   	    *theta = acos(w);
		    sum    = sum + *theta;
		    (*iarea)++;
		    }
	         }   // End of: if (areat >= arealim)
              }      // End of: if (areau >= arealim)
           }         // End of: for (k = j + 1; k < npoints-1; k++)
        }            // End of: for (j = i + 1; k < npoints-2; j++)
    }                // End of: for (i = 0;     i < npoints;   i++)

 if (*iarea == 0) 
    {
    spout(" *** Unable to compute tilt angle, pick more locations.");
    spout(" *** Choose locations distributed in both X & Y");
    XBell(idispl,50);
    return 1;
    }

 /* Find maximum bad */
 maxbad = nbads[0];
 locbad = 0;
 for (i=1; i <= npoints; i++)
    {
    if (nbads[i] >= maxbad)
      {
      maxbad = nbads[i];
      locbad = i;
      }
    }
 
 if (locbad > 0 && maxbad > 0)
    { // Bad location?
    //for (i=0; i <= npoints; i++)
    //      {printf("Location: %d  nbads: %d \n",i,nbads[i]);}
    //sprintf(outmsg, "*** Check location: %d  for bad position!", locbad);
    //spout(outmsg);
    //XBell(idispl,50);

    printf("*** Check location: %d  for bad position! \n",locbad);
    iflag = -locbad;
    } 

 *theta = sum    / (*iarea);
 *theta = *theta / pi * 180.0;

 if (wantmsg)
    {
    sprintf(outmsg, "Locations: %d   Areas used for tilt: %d, out of possible: %d", 
                    npoints, *iarea, ntot);
    spout(outmsg);
    } 

 //printf("maxbad:   %d  Locbad: %d  \n",maxbad,locbad);
 //printf("   \n"); 

 return iflag;
 }
