
/*$Header: /usr8/web/src/RCS/tiltang.c,v 1.14 2015/06/11 13:27:53 leith Exp $*/

/*
 ***********************************************************************
 C                                                                     *
 C tiltang.c                                                           *
 C               Improved                        Jun 2015 ArDean Leith *
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
 C
 C   tiltang(x0,y0,xt,yt,npointss,theta,iarea,arealim,wantmsg)
 C
 C   PURPOSE:  Calculate the tiltangle between two images
 C
 C   CALLED BY: 
 C
 C   RETURNS:  0   OK
 C             1   Not enough locations or area betweeen locations
 C            -1   Some locations are bad
 C  
 C   ALSO RETURNS:  theta      Computed tilt angle
 C                             (Theta only altered if ok)
 C                  iarea      Number of tiangles used to get tilt             
 C                         
 **********************************************************************
*/

#include "common.h"
#include "routines.h"

int tiltang(float *x0, float *y0, float *xt, float *yt,
	    int npoints, float * theta, 
            int * iarea, float arealim, int wantmsg)

{
char	    outmsg[80];
int	    i, k, ntot, iflag;
float	    area0, areat, w, sum, temp;
float       x01, x02, y01, y02, xt1, xt2, yt1, yt2;

const       float pi = 3.14159;

sum    = 0.0;
*iarea = 0;
ntot   = 0;

if (npoints < 3)
   { 
   sprintf(outmsg,"*** Unable to compute tilt (theta); Have only: %d points.  Need > 2 points",npoints); 
   spout(outmsg); 
   XBell(idispl,50);
   return 1;
   }

iflag = 0;
for (i = 0; i < npoints; i++)
    {
    for (k = i + 1; k < npoints-1; k++)
	{
        /* Area in untilted image: */
	x01    = x0[k]   - x0[i];
	y01    = y0[k]   - y0[i];
	x02    = x0[k+1] - x0[i];
	y02    = y0[k+1] - y0[i];
	area0  = (float) fabs( (double)(x01 * y02 - x02 * y01));
        ntot++;

        /* Default arealim is 5000 sq. pixels */
	if (area0 >= arealim)
	    {  /* only triangles > arealim are considered */
            /* area in tilted image: */
	    xt1   = xt[k]   - xt[i];
	    yt1   = yt[k]   - yt[i];
	    xt2   = xt[k+1] - xt[i];
	    yt2   = yt[k+1] - yt[i];
            areat =  (float) fabs( (double)(xt1 * yt2 - xt2 * yt1));

	    if (areat >= arealim)
		{
                /* Area in tilted image should be <= area in untilted */ 
		w = areat / area0;
 
		if (w <= 1.0)
		   {
		   *theta    = acos(w);
		   sum       = sum + *theta;
		   (*iarea)++;

                   /***** Output removed
		   temp      = *theta / pi * 180;
	           sprintf(outmsg,
                     " %6dth tilt angle: %5.1f  Area: %8.1f ---> %8.1f",
		      *iarea,temp,area0,areat);
		   spout(outmsg);
                   ***********/
		   }
		else
		   { /* Set bad location return flag */
                   iflag = -1;
		   sprintf(outmsg,
                     "*** Check keys: %4d, %4d & %d  for a bad point",
			i+1, k+1, k+2); 
		   printf("%s \n",outmsg);
		   spout(outmsg);
       	             
                   /* ***************** removed output
		   spout("*** COORDINATES IN UNTILTED IMAGE:");
		   sprintf(outmsg, " (%7.2f,%7.2f) (%7.2f,%7.2f) (%7.2f,%7.2f)",
			   x0[i],y0[i], x0[k],y0[k], x0[k+1],y0[k+1]);
		   printf("%s \n",outmsg);
                   spout(outmsg);

  		   spout("COORDINATES IN TILTED IMAGE:");
		   sprintf(outmsg, " (%7.2f,%7.2f) (%7.2f,%7.2f) (%7.2f,%7.2f)",
                            xt[i],yt[i], xt[k],yt[k], xt[k+1],yt[k+1]);
		   printf("%s\n",outmsg);
 		   spout(outmsg); 
                   sprintf(outmsg,
                   "Untilted Area: %8.1f < tilted area:  %8.1f",area0,areat);
		   printf("%s\n",outmsg);
	           spout(outmsg);
    
		   spout("*** WARNING, ARGUMENT OF ARCCOS > 1"); 
		   spout("DIFFERENCE VECTORS: ");
		   sprintf(outmsg, " %7.2f %7.2f %7.2f %7.2f",x01,y01, x02,y02);
		   printf("%s\n",outmsg);
		   spout(outmsg);

		   spout("DIFFERENCE VECTORS:"); 
		   sprintf(outmsg, "%7.2f %7.2f %7.2f %7.2f", xt1,yt1, xt2,yt2);
		   printf("%s\n",outmsg);
 		   spout(outmsg);
                   ***************** */
		   }
		}
	    }
	}
    }

if (*iarea == 0) 
   {
   spout(" *** Unable to compute tilt angle; Need > 2 points with area > arealim!");
   return 1;
   }

*theta = sum    / (*iarea);
*theta = *theta / pi * 180.0;

if (wantmsg)
   {
   sprintf(outmsg, "Points: %d   Areas used for theta: %d, out of possible: %d", 
                 npoints, *iarea, ntot);
   spout(outmsg);
   } 

return iflag;
}

                      
