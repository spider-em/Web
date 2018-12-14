
/*$Header: /usr16/software/web/src/RCS/showbimage.c,v 1.9 2018/12/07 17:03:34 leith Exp $*/

/***********************************************************************
 *
 * showbimage.c
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
 * showbimage
 * 
 * PURPOSE:    displays an image from floating point buffer
 *
 * RETURNS:    true if successfull
 * 
 * PARAMETERS:     
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* externally defined variables used here*/
 
/****************************  showbimage  ****************************/

int showbimage(float * fbuf, int keepcimage, char** cimage,
                             int keepimage,  XImage** imaget, 
                             int nsamt, int nrowt,
                             float fmint, float fmaxt)
{   
 int        nsams, nrows, nrowlab, iytot=0;
 int        idum = 0;
 float      scal, offset;

 /* find nsams, nrows in displayed image */
 dispsize(1,nsamt,1,nrowt,idum,idum,iskip,
         &nsams, &nrows, &idum);

 if (!(*cimage))
    {  /* must allocate space for cbuf */
    if ((*cimage = (char *) malloc(nsams * nrows * sizeof(char))) 
               == (char *) 0)
       { 
       spout(" *** Unable to allocate cimage in showbimage.c");
       return FALSE;
       }
    }
 /* scale existing 32 bit buffer into 8 bit buffer */ 
 scale32to8(fmint, fmaxt, iskip, imagego, imageend, &scal, &offset);
 if (!scaleimg(fbuf, nsamt, nrowt, iskip, FALSE, scal, offset, *cimage))
       return FALSE;

 if (placed) 
    {  /* use location from ixuln, iyuln */
    ixul = ixuln;
    iyul = iyuln;
    }

 if (names) 
    /* find y-offset needed for file name label */
    {
    /*  future wid_imglabeloc(filnow,iyline,iytot); !!!!!!!!!!!!*/
    nrowlab = nrows + iytot;
    }

 if (!movie)
    {   /* find location for next image */
    locc(&ixul, &iyul, nsams, nrows, 
               &ntop, &nbot,  &nrit, marx, mary, marlef, martop,
               iwidex, ihighx, placed , newline, &nuscrn, force);
    }


 /* display image at ixul, iyul; deallocate imaget if !keepimage) */
 wipic(icontx,*cimage,ixul,iyul,nsams,nrows,TRUE,keepimage,imaget);

 if ((!keepcimage) && *cimage)
    {
    free(*cimage);
    *cimage = NULL;
    } 
       
 /* record magnification value for this image */
 lastskip = iskip;

 if (names)
    {     /* print image file name below image */
    /* future wid_imglabel(filnow,ixul,iyul+nrows+iyline) !!!!! */
    }

 placed = FALSE;
 
 return TRUE;
 }

 
