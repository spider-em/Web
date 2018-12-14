/*                                                                            
C***********************************************************************   
C                                                                      *
C   redvol8_reform.c  -- Created                        Oct 18 2011 al *                                         
C                                                                      *
C **********************************************************************      
C *  AUTHOR: ArDean Leith                                                  *  
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *      
 C=* Copyright (C) 1992-2011  Health Research Inc.                     *      
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
C  redvol8_reform(filedata, cptr, nxn,nyn, nzn, nmint, nmaxt,fn, fncon)                                                          
C                                                                             
C  PURPOSE:     Reads spider image into 8bit array normalized from            
c               nmin to nmax.  Returns normaliztion constants also.           
C                                                                             
C  PARAMETERS:  nsam1,nsam2     Sample range                                  
c               nrow1,nrow2     Row range                                     
c               nslice1,nslice2 Slice range                                   

C  RETURNS:     fn, fncon  
C               
C  NOTES:       
C               
C--*********************************************************************
*/                                                                      

#include "files.h"
#include "routines.h"

 /* External function prototypes */
                                   
 /*************************   redvol8   ****************************/

 int redvol8_reform(FILEDATA *filedata, unsigned char * cptr,
         int nxn, int nyn, int nzn, int nmint, int nmaxt, 
         float * fn, float * fncon)                      

 {
 int           nsam,   nrow, nslice, irec;
 int           islice, irow, isam ;     
 float         * buf;                   
 unsigned char * cptrt;                   
 unsigned char * cptrx;                   
 unsigned char * cptry;                   
 unsigned char * cptrz;                   

 // Original dimensions
 nsam   = filedata -> nsam;
 nrow   = filedata -> nrow;
 nslice = filedata -> nslice;

 /* Find existing file read buf for redlin */
 buf    = filedata -> buf;

 // Find image intensity scaling values for float --> 8 bits.
 *fn    = (nmaxt - nmint ) / 
          ((filedata-> fmax_spi) - (filedata -> fmin_spi));
 *fncon = (-filedata -> fmin_spi * *fn) + nmint;

 /* Fill volume with 8 bit data */
 irec = 0;

 for (islice = 0; islice < nslice; islice++)
    {
    for (irow = 0; irow < nrow; irow++)
       {
       if (!redlin(filedata,buf,nsam,++irec))
          {return FALSE;}

       for (isam =0; isam < nsam; isam++)
          {
          cptrt  = cptr + irow*nxn*nyn + isam*nxn + islice;   
          *cptrt = (unsigned char) (buf[isam] * *fn + *fncon);

          }  // End of: for (isam   = 0
       }     // End of: for (irow   = 0
    }        // End of: for (islice = 0
 return TRUE;
 }
