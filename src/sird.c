
/*$Header: /usr16/software/web/src/RCS/sird.c,v 1.7 2018/12/07 17:03:34 leith Exp $*/
/*
C***********************************************************************
C
C   sird.c
C                round --> round_macro    May 17 2007 ArDean Leith
C           
C **********************************************************************
C
C  sird(distbuf, sirdmap, nsam,nrow, y, freeit)
C
C  PURPOSE:     Creates single image random dot stereogram from distance
C               map.  Adapted from:  Thimbleby, HW., Inglis, S and I.H. 
C               Whitten.  Displaying 3D Images: Algorithms for Single-
C               Image Random-Dot Stereograms.  Computer, Vol 27 pg 
C               38-48, Oct 94.
C
C  PARAMETERS:  
C
C  RETURNS:     T or F
C
C  CALLED BY:   surf
C
C--*********************************************************************
*/

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include "common.h"
#include "routines.h"

#define round_macro(X)  (int)((X)+0.5)   /* rounding macro            */
#define DPI           72                 /* pixels per inch           */
#define E             round_macro(2.5*DPI)/* eye separation           */
#define mu            (1/3.0)            /* depth of field            */
#define separation(Z) round_macro((1-mu*Z)*E/(2-mu*Z))
                                         /* stereo separation at Z    */
#define far           separation(0)      /* far plane z= 0!!          */

static int * pix  = (int *) 0;           /* pixel color               */
static int * same = (int *) 0;           /* a pixel on right is       */
                                         /* constrained to this color */

 /*************************** sirds ***********************************/

 int sird(float Z[], unsigned char * sirdmap, int maxx, int maxy,
           int y, int freeit )
 {
 int  x;                              /* current point                */
 int  s;                              /* current stereo separation    */
 int  left, right;                    /* x values of left and rit eye */

 if (freeit == -1) 
    {          /* allocate same and pix memory */
    if (((same = (int *) malloc(maxx*sizeof(int))) == (int *) 0))
        {
        spout("*** Unable to allocate same memory in sird1.c");
        return FALSE;
        }

    if (((pix = (int *) malloc(maxx*sizeof(int))) == (int *) 0))
        {
        spout("*** Unable to allocate pix memory in sird1.c");
        return FALSE;
        }
     return TRUE;
     }

 if (freeit == 1) 
    {        /* free same and pix memory */
    if (pix)  free( pix);
    if (same) free( same);
    return TRUE;
    }


 /* each pixel is initially linked with itself */         
 for (x= 0; x< maxx; x++)
    {same[x] = x; }

 for (x= 0; x< maxx; x++)
    {
    s     = separation(Z[x]);
    left  = x - (s+(s&y&1)) / 2;   /* pixels at left and rit       */
    right = left + s;              /* must be the same?            */

    if (0 <= left && right < maxx)
       {
       int visible;              /* Do hidden surface removal       */
       int t = 1;                /* check (x-t) and (x+t)           */
       float zt;                 /* z coord. of ray at these points */
          
       do
          {
          zt = Z[x] + 2*(2-mu*Z[x])*t/(mu*E);
          visible = Z[x-t] < zt && Z[x+t] < zt;   /* FALSE if obscured*/
          t++;
          } while (visible && zt < 1);  /* finished hidden surf. removal */

       if (visible)                 /* pixels at l & r are the same */
          {
          int k;
          for (k=same[left]; k != left && k != right; k= same[left])
             {
             if (k < right)     /* flip the pointers until either   */
                {               /* same[left] = left or             */
                left = k;       /* same[right] = left               */
                }
             else
                {
                left  = right;
                right = k;
                }
             }
             same[left] = right;    /* record same value */
          }
       }
    }

 for (x=maxx-1; x >= 0; x--)
    {
    if (same[x] == x) 
      { /* random choice        */
#ifdef SP_SUNSP
      pix[x] = lrand48()&1;
#else

#ifdef SP_NT
      pix[x] = rand()&1; 
#else
      pix[x] = random()&1;
#endif

#endif
      }
    else 
      {  /* constrained choice   */
      pix[x] = pix[same[x]];
      }

    sirdmap[x] = (char) pix[x];
    }

 /* draw convergence dots at far plane near screen bottem */
 return TRUE;
 }
   
