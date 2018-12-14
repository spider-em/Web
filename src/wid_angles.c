
/*$Header: /usr16/software/web/src/RCS/wid_angles.c,v 1.7 2018/12/07 17:03:35 leith Exp $*/
/*
 C++************************************************************************
 C
 C wid_angles
 C
 C **********************************************************************
 C * AUTHOR:  ArDean Leith                                             *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 C *********************************************************************
 C
 C wid_angles
 C
 C PURPOSE:      Set up an angle scale widget for 3-d vols
 C
 C PARAMETERS:   iw_parent         parent widget         read
 C               iw_its            widget numbers        write
 C
 C CALLED BY:    surfmen
 C
 C***********************************************************************
*/

#include "std.h"
#include "x.h"
#include "routines.h"

/*******************  wid_angles  ***************************/

 Widget wid_angles(Widget iw_parent, Widget iw_its[3], int system,
              int iphi, int itheta, int ipsi, int ixdum, int iydum)

 {   
 Widget               iw_rowcolv;
 Widget               iw_rowcolh1,iw_rowcolh2,iw_rowcolh3;
 static  int          iwid, ihi;
 char		      label1[10],label2[10],label3[10];		/* the labels */
 char		      label4[10],label5[10],label6[10];		/* the labels */

 if (iw_its[0] == (Widget)0) 
    {  /* Must create new  widgets  */

    iwid = 300;
    ihi  = 50;

    iw_rowcolv   = wid_rowcol(iw_parent,'v',-1,-1);

    /* create rowcol widgets for each axis */
    iw_rowcolh1   = wid_rowcol(iw_rowcolv,'h',-1,-1);
    iw_rowcolh2   = wid_rowcol(iw_rowcolv,'h',-1,-1);
    iw_rowcolh3   = wid_rowcol(iw_rowcolv,'h',-1,-1);

    /* Euler angles */
    if(system) 
      {
      strcpy(label1, "Phi:  ");
      strcpy(label2, "Theta:");
      strcpy(label3, "Psi:  ");
      }
  
    /* XYZ angles */
    else
      {
      strcpy(label1, "  X:  ");
      strcpy(label2, "  Y:  ");
      strcpy(label3, "  Z:  ");
      }

    strcpy(label4, "  +-X:");
    strcpy(label5, "  +-Y:");
    strcpy(label6, "  +-Z:");

    iw_its[0] =  wid_scale(iw_rowcolh1, iw_its[0], label1,
              0,360,iphi,    iwid,ihi, -1,-1);
 
    iw_its[1] =  wid_scale(iw_rowcolh2, iw_its[1], label2,
              0,360,itheta,  iwid,ihi, -1,-1);

    iw_its[2] =  wid_scale(iw_rowcolh3, iw_its[2], label3,
              0,360,ipsi,    iwid,ihi, -1,-1);

    iwid = 180;
    iw_its[3] =  wid_scale(iw_rowcolh1, iw_its[3], label4,
              -180,180,0,    iwid,ihi, -1,-1);
 
    iw_its[4] =  wid_scale(iw_rowcolh2, iw_its[4], label5,
              -180,180,0,  iwid,ihi, -1,-1);

    iw_its[5] =  wid_scale(iw_rowcolh3, iw_its[5], label6,
              -180,180,0,    iwid,ihi, -1,-1);

    return iw_rowcolv;
    }

    return NULL;
 }
