/******************************************************************
 * 
 * 	xyz_to_eul.c (created April 24, 1995)
 *                       round --> round_spi    May 17 2007 ArDean Leith
 *
 **************************************************************************
 *  AUTHOR:  YANHONG LI                                                *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1995-2007  Health Research Inc.                     *
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
 **************************************************************************

 	PURPOSE: Used to transfer XYZ angles(theta1, theta2,
 	theta3) or (t1,t2,t3) to euler angles (Phi, Theta and Psi)

    	CALLED BY:       surfmen

***********************************************************************
 */

#include <stdio.h>
#include <math.h>
#include "routines.h"

#define	PI	3.1415926

void xyz_to_eul(float *pt1, float *pt2, float *pt3)
{
    float 	inphi, intheta, inpsi;	/* Euler angles */
    float	t1,t2,t3;		/* XYZ angles */

    float	s_phi, s_theta, s_psi;	/* sin values for euler angles */
    float	c_phi, c_theta, c_psi;	/* cos values for euler angles */

    float	s1,s2,s3;		/* sin values for XYZ angles */
    float	c1,c2,c3;		/* cos values for XYZ angles */	

    /* change angle to quatum defination */
    t1 = antiang(*pt1);
    t2 = antiang(*pt2);
    t3 = antiang(*pt3);
  
    /* calculate the sin and cs values */
    s1 = sin(t1); c1 = cos(t1);
    s2 = sin(t2); c2 = cos(t2);
    s3 = sin(t3); c3 = cos(t3);

    /* get the euler angles */
    c_theta = c1*c2;
    s_theta = sqrt(1.0-c_theta*c_theta);

    if (s_theta != 0) {

	/* Theta!=0, general calculation... */
	s_psi = s1*c2/s_theta;
	c_psi = s2/s_theta;

	s_phi = (c1*s2*s3-s1*c3)/s_theta;
	c_phi = (c1*s2*c3+s1*s3)/s_theta;

	/* calculate the angles */
     	if (c_phi >= 0) 	inphi = angle(asin(round_spi(s_phi)));
	else			inphi = 180 - angle(asin(round_spi(s_phi)));

     	if (c_theta >= 0) 	intheta = angle(asin(round_spi(s_theta)));
	else			intheta = 180 - angle(asin(round_spi(s_theta)));

     	if (c_psi >= 0) 	inpsi = angle(asin(round_spi(s_psi)));
	else			inpsi = 180 - angle(asin(round_spi(s_psi)));
	}

    else {
	if (c_theta >0) 	intheta = 0;
	else			intheta = 180;

	if (c_theta < 0) {
	    c_phi = -1*c2*c3;
	    s_phi = c2*s3;
	    }
	else {
	    c_phi = c2*c3;
	    s_phi = c2*s3;
	    }

     	if (c_phi >= 0) 	inphi = angle(asin(round_spi(s_phi)));
	else			inphi = 180 - angle(asin(round_spi(s_phi)));

	inpsi=0;
	}

    /* get the round cutting angles */
    *pt1 = inphi   = roundang(inphi);
    *pt2 = intheta = roundang(intheta);
    *pt3 = inpsi   = roundang(inpsi);
}
