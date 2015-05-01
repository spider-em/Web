
/*
***************************************************************************
** comap.c 			7/28/93
** WID_COMAP.FOR (VAX) ported to UNIX
**
C **********************************************************************
** *  AUTHOR:  Mahieddine Ladjadj                                           *
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
**
**    WID_COMAP
**
**    PURPOSE:   Displays correlation map, user can outline variable
**               shaped window from map,  store the image ids that
**               are located inside window in a doc file, find and
**               display average of images located within window
**
*****************************************************************************
*/

#include "common.h"
#include "routines.h"

  /* Externally definied variables */
  extern    char      docxnam[81];
  extern    int       sayno, itrad, key1, key2, itxreg, ityreg;
  extern    float     stndrt;
  double              factxa, factxb,factya, factyb;
  float               *bufx, *bufy;
  int                 *nkey;

  int    nline, ixmar, iymar, ixsize, iysize, gotbin, gotav, gotavim,
         nowbin;

/******************************** comap () ********************************/

void comap(void)
{
  int     k;
  int     maxkeys, icolort,
          maxreg, ix, iy, numtot, numcor, lastkey,
          doc_read, pkey;
  float   *plist, *ptr, *dbuf, xt, yt;
  double  davx, davx2, davy, davy2, bx, by, xmin, xmax, ymin, ymax, xr, yr,
          sigx, sigy, xrange, yrange;
  char    outstr[81];
     
  /* Set wait cursor */
  waitcursor(iwin, TRUE, 79);                                  
  
  /* 
  Recover all the lines from the doc file and store in the dbuf array.
  column 1 is the key, following columns are registers. correspondence
  analysis may have up to 24 registers. */

  doc_read = TRUE;
  pkey     = 0;
  maxkeys  = MYMAX(key1,key2) + 1;
  maxreg   = MYMAX(itxreg,ityreg) + 1;
  dbuf     = (float *) NULL;
  if (unsdal(docxnam, &doc_read, pkey, plist, maxreg, &dbuf, &lastkey,
                   maxkeys,maxreg) > 0)
      {
      /* Problem retrieving doc file, assume it does not exist */
      if (dbuf) free(dbuf);
      sprintf(outstr," Could not read document file: %s",docxnam);
      spout(outstr);

      /* unset wait cursor */
      waitcursor(iwin, FALSE, 0);                                  
      return;
      }
  
  if (lastkey < MYMIN(key1,key2))
      {
      if (dbuf) free(dbuf);
      sprintf(outstr," both keys needed %d & %d > lastkey = %d",
         key1,key2,lastkey);
      spout(outstr);

      /* unset wait cursor */
      waitcursor(iwin, FALSE, 0);                                  
      return;
      } 
      
  /* Determine maximum/minimum x and y coordinates, avg, etc; */
  xmin = ymin = FLT_MAX;
  xmax = ymax = -xmin;

  davx = davx2 = davy = davy2 = 0; 

  /* Run time allocate of memory space for nkey[], bufx, bufy */
  if ( (nkey = (int *) calloc((key1 * key2), sizeof(float)) )
             == (int *) NULL)                                       {
     spout(" *** Could not allocate memory");
     waitcursor(iwin, FALSE, 0);  return;                           }
  if ( (bufx = (float *) calloc((key1 * key2), sizeof(float)) )
             == (float *) NULL)                                     {
     spout(" *** Could not allocate memory");
     waitcursor(iwin, FALSE, 0);  return;                           }
  if ( (bufy = (float *) calloc((key1 * key2), sizeof(float)) )
             == (float *) NULL)                                     {
     spout(" ***Could not allocate memory");
     waitcursor(iwin, FALSE, 0);  return;                           }

  /* Remember current color in icontx */
  icolort = icolor;

  /* Set current color to image almost white */
  wicolor(icontx,colorgo+2);

  /* Clear the screen */
  //clearover(NULL,NULL,NULL);

  ptr   = dbuf + (key1-1) * maxreg;
  nline = 0;

  for (k = key1; k <= key2; k++)
      {
      /* Check that the key has some registers. */
      if ( (*ptr) > 0.0) 
          {
          nkey[nline] = k;          /*  image number      */
          bx = *(ptr + itxreg);     /*  dbuf[k][itxreg]   */
          by = *(ptr + ityreg);     /*  dbuf[k][ityreg]   */

          /* Record bx & by */
          bufx[nline] = bx;
          bufy[nline] = by;
          nline++;
    
          /* Record max and min for bx & by   */
          xmin = MYMIN(bx,xmin);
          xmax = MYMAX(bx,xmax);
          ymin = MYMIN(by,ymin);
          ymax = MYMAX(by,ymax);

          /* record average and standard deviation for bx & by */
          davx  += bx;
          davx2 += (double)(bx * bx);
          davy  += by;
          davy2 += (double)( by * by);
          }
      ptr += maxreg;
      }        /*   for (pkey = key1; pkey <= key2; pkey++)  */

  /* Calculate sigma and average */
  davx  /= (float) nline;
  davy  /= (float) nline;

  sigx   = sqrt((davx2 - (davx * davx / (float) nline)) / (double) (nline - 1));
  sigy   = sqrt((davy2 - (davy * davy / (float) nline)) / (double) (nline - 1));

  xt     = 2.0 * stndrt * sigx;
  yt     = 2.0 * stndrt * sigy;

  xr     = xmax - xmin;
  yr     = ymax - ymin;

  xrange = MYMIN(xt, xr);
  yrange = MYMIN(yt, yr);

  /* Set plot size */
  ixsize = iwidex - 2;
  iysize = ihighx - 2;
  ixmar  = iymar = 1;
  numtot = numcor = 0;

  /* Find scale factors */
  factxa = (float) (ixsize -1) / xrange;
  factxb = - (xmin * factxa) + ixmar + 0.5;

  factya = (float) (iysize-1) / yrange;
  factyb = - (ymin * factya) + iymar + 0.5;

  for (k = 0; k < nline; k++)
    {
      ix = (bufx[k] - xmin) * factxa + ixmar + 0.5;
      iy = (bufy[k] - ymin) * factya + iymar + 0.5;

      if (ix >= ixmar && ix <= ixsize && iy >= iymar && iy <= iysize)
          { /* Image is inside the plot extent; find the label number */

          if (sayno)
              {  /* write image number at this location on plot */
              sprintf(outstr,"%d",nkey[k]);
              //           itsize,just,  inten,ipen,  overprint
              witext(icontx,outstr,ix,iy, 0,0, -9,2,  FALSE);
              }
          else
              { 
              /* Draw a permanent circle at this position */
              xorc(iwin,    icontx, TRUE, ix, iy, itrad);
              xorc(imagsav, icontx, TRUE, ix, iy, itrad);
              }

          /* Increment total number of images within plot extent */
          numcor += 1;
          }
      else
        {
        /* Image is outside of plot extent */
        nkey[k] = - nkey[k];
        }

    numtot += 1;   /* Increment total number of images */
    }
  sprintf(outstr," Images: %d   Images within plot: %d",numtot,numcor);
  spout(outstr);
     
  /* Replace current color in icontx */
  wicolor(icontx,icolort);

  /* Initialize bin counter for this map */
  nowbin = 0;

  /* Set flag for each bin mask */
  gotbin = gotavim = gotav = FALSE;

  /* Cancel wait cursor */
  waitcursor(iwin, FALSE, 0);

  /* Start masking of correspondence map */
  corr_mask(0);
}
