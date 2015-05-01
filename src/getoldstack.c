
/*$Header: /usr8/web/src/RCS/getoldstack.c,v 1.12 2011/08/01 16:02:27 leith Exp $*/

/*
C++********************************************************************
C
C getoldstack.c -- Created Sept 99 Ardean leith
C 
C *********************************************************************
C *  AUTHOR: ARDEAN LEITH                                             *
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
C *********************************************************************
C
C    getoldstack
C
C    PURPOSE:      Set next image in a stack      
C
C    PARAMETERS:     
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

/****************************  getoldstack  **************************/

 int getoldstack(FILEDATA * filedatat, int imgnum, int wantnext, int sayit)

 {   
 float	    header[256];
 char	    outstr[100];
 int        inuse, nsam, nrow, headbyt, nslice;

  //printf("imgnum   %d \n", imgnum);
  if (imgnum > filedatat->maxim)
     { return 0; }

  /* Set offset for reading header from file */
  headbyt = filedatat->headbyt;
  nsam    = filedatat->nsam;
  nrow    = filedatat->nrow;
  nslice  = filedatat->nslice;

  filedatat->offset = headbyt + 
     (imgnum - 1) *  (headbyt + nsam * nrow * nslice * 4);

  inuse = FALSE;

  while (!inuse)
     {    
     /* Read header of stacked image file */
     if (!redlin(filedatat,header,256,0)) return -1;

     inuse = (header[26] != 0);

     if (!inuse && !wantnext) 
        {  /* Desired image does not exist, have error */
        printf("Desired image not in use. \n");
        return -1;
        }
     else if (!inuse && wantnext)
        {  /* Image does not exist, try next one */
        imgnum++; 
        if (imgnum > filedatat->maxim) return 0;
        }
     }  /* Continue until an image is obtained */
 
  filedatat -> imami    = header[5];
  filedatat -> fmax_spi = header[6];
  filedatat -> fmin_spi = header[7];
  filedatat -> av       = header[8];
  filedatat -> sig      = header[9];
	
  filedatat -> iangle   = header[13];
  filedatat -> phi      = header[14];
  filedatat -> theta    = header[15];
  filedatat -> gamma    = header[16];
	
  if (filedatat->imami != 1)  
        { norm(filedatat); }
	
  fmin_spi = filedatat->fmin_spi;
  fmax_spi = filedatat->fmax_spi;
  if (sayit)
     {
     sprintf(outstr, "@ %d:  Range: %f.... %f", imgnum, fmin_spi, fmax_spi);
     spout(outstr);
     }

  return imgnum;
}


 
