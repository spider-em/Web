
/*$Header: /usr16/software/web/src/RCS/average.c,v 1.3 2018/12/07 17:03:30 leith Exp $*/

/*
 C**********************************************************************
 C
 C   average
 C             
 C *********************************************************************
 C * AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
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
 C  average
 C           
 C  PURPOSE:     averages two images.
 C
 C  PARAMETERS:  
 C
 C  RETURNS:     TRUE  
 C
 C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

 /*************************   sumum   ****************************/

 int sumum(float * first,  float * second, float * output, int npix)
 {
 int ipix;

 for (ipix = 0; ipix < npix ; ipix++)
     {
     * (output++) = * (first++) + *(second++) ;
     }

 return TRUE;
 }


 /*************************   average   ****************************/

 int average(float * input,  float * output, int npix, float divisor)
 {
 int   ipix;
 float overdiv;

 overdiv = 1.0 / divisor;

 for (ipix = 0; ipix < npix ; ipix++)
    {
    * (output++) = (*(input++))  * overdiv ;
    }

 return TRUE;
 }


 /*****************  avgsave  (runs after avgsave) ************/

 int avgsave(char * avgnamt, int nsam, int nrow, float * avgdata)
 {
 FILEDATA      *filedatat;
 
 /* open new output file */
 filedatat = opennew(avgnamt, nsam, nrow, 1, 1, "u");

 /* put newimage in output file */
 wrtvol(filedatat, avgdata);
 closefile(filedatat);

 return TRUE;
 }



