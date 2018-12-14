/***********************************************************************
 * 
 *    eul_to_xyz.c    created April 24, 1995
 *                    round --> round_spi     May 17 2007 ArDean Leith
 *
 ***********************************************************************
 C   AUTHOR:  YANHONG LI                                               *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1995-2007  Health Research Inc.                     *
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
 * 	PURPOSE: Used to transfer euler angles (Phi, Theta and Psi) to 
 *               XYZ angles(theta1, theta2, theta3) or (t1,t2,t3) 
 *
 *  	CALLED BY:       surfmen
 *
 ***********************************************************************
*/

#include <stdio.h>
#include <math.h>
#include "routines.h"

#define	PI	3.1415926

/* external functions */
float	angle(float x) { x = 180*x/PI; return x; }
float 	antiang(float x) { x = x*PI/180; return x; }
float   round_spi(float x) {
    if ( x>1 ) 		return 1.0;
    else if (x<-1 ) 	return -1.0;
    else		return x;
    }

float   roundang(float x) {
    if (x<0) x=360+x;
    return floor(x*100+0.5)/100.0;
    }

void eul_to_xyz(float *pphi, float *ptheta, float *ppsi)
{
    float 	inphi, intheta, inpsi;	/* Euler angles */
    float	t1,t2,t3;		/* XYZ angles */

    float	s_phi, s_theta, s_psi;	/* sin values for euler angles */
    float	c_phi, c_theta, c_psi;	/* cos values for euler angles */

    float	s1,s2,s3;		/* sin values for XYZ angles */
    float	c1,c2,c3;		/* cos values for XYZ angles */	

    /* change angle to quatum defination */
    inphi	= antiang(*pphi);
    intheta	= antiang(*ptheta);
    inpsi 	= antiang(*ppsi);
  
    /* calculate the sin and cs values */
    s_phi = sin(inphi); 	c_phi = cos(inphi);
    s_theta = sin(intheta); 	c_theta = cos(intheta);
    s_psi = sin(inpsi); 	c_psi = cos(inpsi);

    /* get the xyz angles */
    s2 = c_psi*s_theta;
    c2 = sqrt(1.0-s2*s2);

    if (c2 != 0) {

	/* t2!=0, general calculation... */
	s1 = s_psi*s_theta/c2;
	c1 = c_theta/c2;

	s3 = (c_psi*c_theta*s_phi+s_psi*c_phi)/c2;
	c3 = (c_psi*c_theta*c_phi-s_psi*s_phi)/c2;

	/* calculate the angles */
     	if (c1 >= 0) 	t1 = angle(asin(round_spi(s1)));
	else		t1 = 180 - angle(asin(round_spi(s1)));

     	if (c2 >= 0) 	t2 = angle(asin(round_spi(s2)));
	else		t2 = 180 - angle(asin(round_spi(s2)));

     	if (c3 >= 0) 	t3 = angle(asin(round_spi(s3)));
	else		t3 = 180 - angle(asin(round_spi(s3)));
	}

    else {
       if(s2>0) 
	c1 = s_theta*c_phi;
       else 
	c1 = -s_theta*c_phi;
	s1 = -s_theta*s_phi;

     	if (c1 >= 0) 	t1 = angle(asin(round_spi(s1)));
	else		t1 = 180 - angle(asin(round_spi(s1)));

	if (s2>0)	t2 = 90;
	else		t2 = 270;

	t3=0;
	}

    *pphi = t1   = roundang(t1);
    *ptheta = t2 = roundang(t2);
    *ppsi = t3   = roundang(t3);
}
