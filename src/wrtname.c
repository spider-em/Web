
/*$Header: /usr8/web/new/RCS/wrtname.c,v 1.4 2005/10/18 16:59:50 leith Exp $*/


/*
C++*********************************************************************
C
C WRTNAME.C -- CREATED MAY 2002 ArDean Leith
C                no / no name bug Jul 04 ArDean Leith
C
C **********************************************************************
C * WEB.    AUTHOR: ARDEAN LEITH                                       *
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
C    WRTNAME
C
C    PURPOSE:    print names below images     
C
C    VARIABLES:       NAMES      LOGICAL FLAG TO PRINT NAMES
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* external variables used here */
 
/******************************  wrtname *****************************/
   
 int  wrtname(int ix, int iy, char * name, int inum)
 {   
 char      label[] = "     ";
 char       *s, *fname;
 int       iypn;

 if (ix < 0 && iy < 0) 
    { /* query names_height */
    return names_height + 2;
    }


 /* set background color */
 wicolor(icontx, -1000000);

 /* find y-offset needed for name label */
 iypn = iy + names_height + 2;

 if (inum >= 0) 
    {
    /* put slice number under each pane */

    sprintf(label,"%5d",inum);
    witext(icontx, label, ix, iypn, 1, 0, 9, 2, TRUE);
    }

 else
    {     /* print image file name below image */

     /* unix specific line follows */
     s = strrchr(name,'/');

    if (s) 
      {    /* strip pathname from filename  */
      s++;
      fname = (char *) malloc(sizeof(char) * (strlen(s) + 1));
      strcpy(fname,s);
      fname = strtok(fname,".");

      /* 7th parameter set to -1 to use currentforeground color */
      witext(icontx, fname, ix, iypn, 1, 0, -1, 2, TRUE);
      free(fname);
      }

    else 
      {
      /* 7th parameter set to -1 to use currentforeground color */
      witext(icontx, name, ix, iypn, 1, 0, -1, 2, TRUE);
      }
   }
 return names_height;
 }
