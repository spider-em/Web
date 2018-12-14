
/*$Header: /usr16/software/web/src/RCS/scaleimg.c,v 1.6 2018/12/07 17:03:34 leith Exp $*/

/***********************************************************************
 *
 * scaleimg.c
 *
 ***********************************************************************
 * author : ArDean Leith
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
 ***********************************************************************
 *
 * scaleimg()
 * 
 * PURPOSE:  scales a floating point into 8 bit over range nmint..nmaxt.
 *
 * RETURNS:  true or false
 *
 * PARAMETERS:
 *
 * NOTES: I removed old if (ival > 128 code see previous RCS versions 
 *        to reinstate code if desired. july 99 al 
 *        also removed memory allocation.
 *
 ***********************************************************************
*/

#include "std.h"
#include "common.h"
#include "routines.h"

 /* externally defined variables */

 /****************************  scaleimg  *****************************/
 
 int scaleimg(float * oldimage, int nsam,   int nrow, 
                                int iskipt, int newmin,
                                float scal, float offset,
                                char * cptr)
 { 
 char   *cbuf;
 int    nsams, nrows, npixpimg, ival, miskip, idum;
 int    irow, isam;
 char   ctemp;
 float  * fptrt, *fptrtt;
 char   * cptrt, *cptrtt;

 /* find number of rows & cols in displayed image */
 dispsize(1,nsam, 1,nrow, 1,1, iskipt,&nsams, &nrows, &idum);

 /* set temporary data pointers */
 cptrt = cptr;

 npixpimg = nsam * nrow;

 if (iskipt > -2 && iskipt < 2)
    {    /*******************************************full size */
    if (newmin)
       { /* want to alter min & max  definition*/
       for (fptrtt = oldimage; fptrtt < oldimage+npixpimg; fptrtt++)
          {
          ival     = (char) (*fptrtt * scal + offset);
          *cptrt++ = MYMIN(MYMAX(ival,imagego),imageend);
          }  /* for isam           */
       }     /* if (newmin         */
    else
       {  
       for (fptrtt = oldimage; fptrtt < oldimage+npixpimg; fptrtt++)
          {
          *cptrt++ = (char) (*fptrtt * scal + offset);
          }       /* for (isam   */
       }          /* not newmin  */
    }             /* if (iskipt > -2 && iskipt < 2)  */

 else if (iskipt > 1)     
    {    /********************************* reduced size image */
    for (irow = 0 ; irow < nrow; irow+=iskipt)
       {
       fptrtt = oldimage + (irow * nsam);
       for (isam = 0; isam < nsam; isam+=iskipt)
          {  
          ival    = (char) (*fptrtt * scal + offset);
          fptrtt += iskipt;
                if (newmin)
             { /* want to alter min & max  definition*/
             *cptrt++ = MYMIN(MYMAX(ival,imagego),imageend);
             }  
          else
             {
             *cptrt++ = ival;
             }  
          }     /* for (isam = 0   */
       }        /* for (irow = 0   */
    }           /* if (iskipt > 1) */

 else if (iskipt < -1)
    {    /******************************* magnified size image */

    fptrtt = oldimage;
    for (irow = 0 ; irow < nrow; irow++)
       {
       cptrtt = cptrt;
       for (isam = 0 ; isam < nsam; isam++)
          {
          ival = (char) (*fptrtt * scal + offset);
          fptrtt++;
          if (newmin)
             { /* want to alter min & max  definition*/
             ctemp = MYMIN(MYMAX(ival,imagego),imageend);
             } 
          else
             {
             ctemp = ival;
             }
          /* calculate and insert first row */
          for (miskip = 0 ; miskip < -iskipt; miskip++)
             {  /* duplicate this pixel miskip times */
             *cptrt++ = ctemp;
             }
          }     /* (isam = 0 ; isam < nsam; isam++) */

       /* duplicate the row miskip -1 times now */
       for (miskip = 1 ; miskip < -iskipt; miskip++)
          {  /* duplicate this row miskip times */
          for (isam = 0 ; isam < nsam*(-iskipt); isam++)
             {
             *cptrt++ = *(cptrtt+isam);
             } /* for (isam = 0 ; isam < nsam*(-iskipt) */
 
          }    /* for (miskip = 1 ; miskip < -iskipt;   */
       }       /* (irow = 0 ; irow < nrow; irow++) */
    }          /* if (iskipt < -1) */

 return TRUE;
 }

