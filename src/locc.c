
/*$Header: /usr8/web/src/RCS/locc.c,v 1.4 2005/10/18 17:00:04 leith Exp $*/
/*
C++*************************************************************************
C
C  LOCC
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C LOCC(IXUL,IYUL,NSAM,NROW,NTOP,NBOT,NRIT,MARX,MARY,
C           MARLEF,MARTOP,IXRES,IYRES,SENT,NEWLIN,NUSCRN,FORCE)
C
C    PURPOSE:      FIND NEXT POSITION FOR IMAGE
C
C    PARAMETERS:   IXUL   = COL FOR IMAGE
C                  IYUL   = ROW FOR IMAGE
C                  NSAM   = IMAGE COLUMNS
C                  NROW   = IMAGE ROWS
C                  NTOP   = PRESENT DISPLAY ROW
C                  NBOT   = PRESENT BOTTEM OF THIS DISPLAY ROW
C                  NRIT   = PRESENT RIGHT SIDE OF DISPLAY ROW
C                  MARX   = X MARGIN BETWEEN IMAGES
C                  MARY   = Y MARGIN BETWEEN IMAGES
C                  MARLEF = LEFT MARGIN
C                  MARTOP = TOP MARGIN
C                  IXRES  = WIDTH OF DISPLAY (0 ORIGIN AT BOT LEFT)
C                  IYRES  = HEIGHT OF DISPLAY
C                  SENT   = FLAG TO PLACE IMAGE AT IXUL, IYUL
C                  NEWLIN = TRUE FROM CALLER FORCES A NEW LINE OF IMAGES
C                  NUSCRN = RETURNS LOGICAL FLAG THAT THE NEXT LOCATION
C                           OVERWRITES OLD IMAGES AT UPPER LEFT
C                  FORCE  = FORCE DESIRED NUMBER OF IMAGES / LINE EVEN IF
C                           TRUNCATED 
C                  
C    CALLS:        NONE
C
C    CALLED BY:    WID_IMAGE
C
C--*************************************************************************
*/

#include "std.h"

/******************************  locc  ***************************/

  void locc(int *ixul,  int *iyul,  int nsam,   int nrow, 
               int *ntop,  int *nbot,  int *nrit,  int marx,
               int mary,   int marlef, int martop, int ixres,   
               int iyres,  int sent,   int newlin, int *nuscrn, 
               int force)
 { 
 int ntall;

/* printf("locc, nsam: %d, nrow: %d, ixres: %d ,iyres: %d,marx: %d, mary: %d \n",
                nsam, nrow, ixres, iyres, marx, mary); */

  if (sent)
     {     /* location sent in ixul and iyul by calling pgm */
     *nbot   = 0;
     }

  else if (*nuscrn)
     {     /* want to start again at upper left origin */
     *ixul   = marlef;
     *iyul   = martop;
     *nbot   = -1;
     }

  else
     {
     if (newlin == TRUE || ((nsam + marx + *nrit) > ixres) &&
        (force  == FALSE))
        {    /* try next row of images */
        *iyul = *nbot + mary + 1;       
        *ixul = marlef;

        if ((*iyul + nrow) > iyres)
           {  /* this row does not have enough height, place at origin */
           *ixul   = marlef;
           *iyul   = martop;
           *nbot   = -1;
           }
        }

     else
       {   /* this row of images has enough width for this image */

       if ((*ntop + nrow + mary) < iyres)
         { /* enough height for image in this row of images */
         *ixul   = *nrit + marx + 1;
         *iyul   = *ntop;
         }
       else
         { /* not enough space on this screen overlap image at origin */
         *ixul   = marlef;
         *iyul   = martop;
         *nbot   = -1;
         }
       }
     }

  /* update next image locator */
  *nrit  = *ixul + nsam - 1;
  *ntop  = *iyul;
  ntall  = *ntop + nrow - 1;
  if (ntall > *nbot) *nbot = ntall;
       
 /* printf("locc, ixul: %d, iyul: %d, nrit: %d, nbot: %d, ntop: %d\n",
                *ixul,     *iyul,     *nrit,     *nbot,    *ntop); */

  return;
 }
