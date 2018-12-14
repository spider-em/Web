/*$Header: /usr16/software/web/src/RCS/imagedis.c,v 1.21 2007/11/28 16:18:33 leith Exp $*/

/*
 ***********************************************************************
 *
 * imagedis.c  -- renamed from image.c Aug. 95
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
 * imagedis(FILEDATA *filedatat,int keepimage, XImage ** imaget,
 *          int keepimagec, int nmint, int nmaxt)
 * 
 * PURPOSE:    displays an image slice, after freeing existing image
 *             and image buffer (if they had originally been allocated
 *             inside imagedis, only)
 *
 * RETURNS:    true if successfull
 *
 * CALL TREE:  imagemen --> showimage --> redvol  
 *                                    --> wipic
 * PARAMETERS:     
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

/**************************  image  *****************************/

void imagedis(FILEDATA *filedatat, int keepimage, XImage ** imaget,
              int keepimagec, int keepimagef, float ** imagef,
              int nmint, int nmaxt)
 {   
 static char  *cimage  = NULL;

  if (*imaget) 
     {
     /* free any existing allocated memory in imaget */
     XFree((void*) *imaget);
     *imaget = NULL;
     }
 
 if (cimage) 
    { /* free any existing image data */
    free (cimage);
    cimage = NULL;
    }

 showimage(filedatat,keepimagec, &cimage, 
                     keepimage,  imaget, 
                     keepimagef, imagef, TRUE,
                     (float) filedatat->fmin_spi, (float) filedatat->fmax_spi,
                     nmint, nmaxt);

 return;
 }

 
