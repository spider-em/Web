
/*$Header: /usr8/web/src/RCS/showimage.c,v 1.13 2005/10/18 16:59:50 leith Exp $*/

/***********************************************************************
 *
 * showimage.c
 *
 ***********************************************************************
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
 * showimage(
 * 
 * PURPOSE:    displays an image from a file
 *
 * RETURNS:    true if successfull
 * 
 * PARAMETERS:  
 *
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

extern int nsami,nrowi,nslicei; /* from mont3.c */

/***********************  showimage **********************************/

int showimage(FILEDATA *filedatat, int keepcimage, char** cimage,
                               int keepimage,  XImage** imaget, 
                               int keep32,     float**  fimage,
                               int showimaget, float fmint, float fmaxt,
                               int nmint,      int nmaxt)
{   
 float      fn, fncon;
 char       *s, *fname;

 /* find nsami, nrowi in displayed image */
 dispsize(nsam1,nsam2,nrow1,nrow2,1,1,iskip,&nsami, &nrowi, &nslicei);

 if (placed) 
    {  /* use location from ixuln, iyuln */
    ixul = ixuln;
    iyul = iyuln;
    }

 if (!movie)
    {   /* find location for next image */
    if (names)
       locc(&ixul, &iyul, nsami, nrowi+names_height, 
         &ntop, &nbot,  &nrit, marx, mary, marlef, martop,
         iwidex, ihighx, placed , newline, &nuscrn, force);
    else
       locc(&ixul, &iyul, nsami, nrowi, 
         &ntop, &nbot,  &nrit, marx, mary, marlef, martop,
         iwidex, ihighx, placed , newline, &nuscrn, force);
    }

 /* load image from file into new 8 bit buffer and maybe float buffer */
 if (!redvol(filedatat, TRUE, cimage, keep32, fimage, nsam1, nsam2,
            nrow1, nrow2, islice, islice, nmint, nmaxt, fmint, fmaxt,
            &fn,  &fncon, iskip)) 
            return FALSE;

 /* display image at ixul, iyul; deallocate imaget if !keepimage) */
 wipic(icontx,*cimage,ixul,iyul,nsami,nrowi,showimaget,
                     keepimage,imaget);

 if ((!keepcimage) && *cimage)
    {  /* free the char array for the image */
    free(*cimage);
    *cimage = NULL;
    } 

 /* record cbuf and ximage for possible later reference */
 filedatat -> ximage = *imaget;
       
 /* record magnification value for this image */
 lastskip = iskip;

 if (names)
    {     /* print image file name below image */
    wrtname(ixul, iyul+nrowi, filnow, -1);
    }
 placed = FALSE;
 
 return TRUE;
 }

 
