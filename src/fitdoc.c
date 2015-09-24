
/*$Header: /usr8/web/src/RCS/fitdoc.c,v 1.35 2015/09/22 14:21:53 leith Exp $*/

/*
 C**********************************************************************
 C
 C  fitdoc.c
 C           Improved                            Jun 2015 ArDean Leith  
 C           Added fitdoc_addpart                Aug 2015 ArDean Leith  
 C           If (maxpart1 <= 0 ) paranoia        Sep 2015 ArDean Leith   
 C           Parameters changed                  Sep 2015 ArDean Leith   
 C
 C**********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C**********************************************************************
 C
 C  fitdoc
 C
 C  PURPOSE:    Input untilted point locations from doc file dfil1
 C              Input   tilted point locations from doc file dfil2
 C              Input tilt parameters from doc file dfil3 (if present)
 C
 C  VARIABLES:  xim       Image  number             dfil1 + 2
 C              xu0,yu0   Untilted picked points    dfil1
 C              xs,ys     Tilted picked points      dfil2
 C              xs2,ys2   Tilted predicted points   dfil6
 C 
 C              dfil1  = Untilted locations
 C              dfil2  = Tilted   locations
 C              dfil3  = Tilt parameters
 C              dfil4  = Untilted backgrounds       (pickback)
 C              dfil4  = Tilted   backgrounds       (pickback)
 C              dfil6  = Tilted predicted points 
 C
 C  RETURNS:    0 If all OK
 C             -1 If no untilted doc file
 C             -2 If no   tilted doc file
 C              1 If unrecoverable error reading or allocating
 C                or some other disaster
 C
 C  CALLED BY:  
 C
 C**********************************************************************
*/

#include "std.h"
#include "x.h"
#include "routines.h"

 // Internal function prototypes 
 int           fitdoc_addpart(int n,  int iwhich, int xu, int yu, int xt, int yt);

 /* External global variables */
 extern char   dfil1[12], dfil2[12], dfil3[12], dfil4[12], dfil5[12];
 extern int    numm, back_win_now, iredu;
 extern int    fitted;
 extern float  phif, thetaf, gammaff;
 extern float  xu0t, yu0t, xs0t, ys0t;
 extern char   datexc[4];                 /* File extension  */

 /* Global variables used elsewhere */
 int           limfit, ifit;
 int           maxtilts, maxpart;
 float       * xu0 = 0, * yu0 = 0, * xs  = 0, * ys = 0;
 float       * xs2 = 0, * ys2 = 0, * xim = 0;
     
 /* Variables used here */
 static int    sizepartarray = 0;

 /********************   fitdoc   ****************************/

 int fitdoc(Boolean want_nofile_msg, 
            Boolean get_loc_doc, 
            Boolean get_tilt_doc, 
            Boolean get_win_doc)

 { 
 int      i,k, iredut, maxpart1,maxpart2;
 int      maxwin1,maxwin2;
 int      back_wins1,back_wins2;
 float  * ptr;
 float  * dbuf    = NULL;
 int      maxreg  = 6+1;   /* Set size for array pointed to by dbuf */
 int      maxkeys = 9999;  /* Set size for array pointed to by dbuf */
 int      isok    = FALSE;

 if (get_loc_doc) 
    {
    /* Retrieve data from dfil1 for untilted points ---------------------------------- */
    isok = getdoc_f( (FILE *) NULL, dfil1, datexc,maxkeys, maxreg, &dbuf, &maxpart1, FALSE);

    if ( maxpart1 <= 0 ) maxpart1 = 0;   // Error recovery

    if ( xim == NULL || maxpart1 > sizepartarray || maxpart1 == 0 )
       {           /* Create initial or larger particle location arrays */
       sizepartarray = maxpart1 + 300;

       if (((xim = (float *) realloc((void *) xim, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((xu0 = (float *) realloc((void *) xu0, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((yu0 = (float *) realloc((void *) yu0, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((xs =  (float *) realloc((void *) xs,  sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((ys =  (float *) realloc((void *) ys,  sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((xs2 = (float *) realloc((void *) xs2, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((ys2 = (float *) realloc((void *) ys2, sizepartarray * sizeof(float))) == (float *) NULL))
           { spout("*** Unable to reallocate memory in fitdoc."); return 1; }
       }

    if (isok != 0) 
       {         /* Problem retrieving doc file. Assume it does not exist */
       if (dbuf) free(dbuf); dbuf = NULL;
       if ( want_nofile_msg )
          {spouts("*** Unable to read untilted doc file: "); spouti(isok); spout(dfil1);} 
       maxpart = 0;
       return   -1;
       }

    /* Fill xim, xu0, yu0 arrays */
    ptr = dbuf;
    for (k=0; k<maxpart1 ; k++)
       {
       xim[k]  = *(ptr+1);  
       xu0[k]  = *(ptr+4);
       yu0[k]  = *(ptr+5);  
       ptr    += maxreg; 
       }

    /* Free dbuf here*/
    if (dbuf) free(dbuf); dbuf = NULL;

    /* Retrieve data from dfil2 for tilted points -------------------------------- */
    if (getdoc_f((FILE *) NULL, dfil2, datexc,maxkeys, maxreg, &dbuf, &maxpart2, FALSE) > 0) 
       {   /* Problem retrieving doc file, assume it does not exist */
       if (dbuf) free(dbuf); dbuf = NULL;
       if ( want_nofile_msg )
          {spouts("*** Unable to read tilted doc file: "); spout(dfil2);} 
       maxpart = 0;
       return   -2;
       }

    if (  maxpart2 > sizepartarray )
       {           /* Create initial or larger particle location arrays */
       sizepartarray = maxpart2 + 300;

       if (((xim = (float *) realloc((void *) xim, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((xu0 = (float *) realloc((void *) xu0, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((yu0 = (float *) realloc((void *) yu0, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((xs =  (float *) realloc((void *) xs,  sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((ys =  (float *) realloc((void *) ys,  sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((xs2 = (float *) realloc((void *) xs2, sizepartarray * sizeof(float))) == (float *) NULL) ||
           ((ys2 = (float *) realloc((void *) ys2, sizepartarray * sizeof(float))) == (float *) NULL))
           { spout("*** Unable to reallocate memory in fitdoc."); return 1; }
       }

    /* Fill xs, ys arrays */
    ptr = dbuf;
    for (k=0; k < maxpart2 ; k++)
       { 
       xs[k]  = *(ptr+4); 
       ys[k]  = *(ptr+5);  
       ptr   += maxreg;
       }

    /* Free dbuf here*/
    if (dbuf) free(dbuf); dbuf = NULL;

    if (maxpart2 < maxpart1)
       {
       /* Tilted file has more points than first! */  
       spout("*** Tilted doc file has fewer particles than untilted.");
       maxpart = 0;
       return 1;
       }

    if (maxpart2 > maxpart1)
       {
       /* Tilted file has more points than first! */  
       spout("*** Tilted doc file has more particles than untilted.");
       maxpart = 0;
       return 1;
       }
 
    maxpart = maxpart1;
    }  // End of: if (get_loc_doc)
 
 if (get_tilt_doc) 
    {
    /* Retrieve data from dfil3 for tilt parameters (if any) ------------------------------- */
    /* Parametes: Coordinates of origin and tilt angles: theta, phi ,gamma */
 
    maxkeys = 125;      /* Set size for array pointed to by dbuf */
    if (getdoc_f((FILE *) NULL, dfil3, datexc, maxkeys, maxreg, &dbuf, &maxtilts, FALSE) > 1) 
       {   /* Problem retrieving doc file. Assume it does not exist */
       if (dbuf) free(dbuf); dbuf = NULL;
       if ( want_nofile_msg)
          {spouts("*** Tilt angle doc file not available yet: "); spout(dfil3);} 
       return 0;
       }

    if (maxtilts < 124)
       {
       spout(" Tilt angle doc. file lacks necessary info.");
       if (dbuf) free(dbuf); dbuf = NULL;
       return 1;
       }

    /* Get last particle number */
    ptr    = dbuf + ((121-1) * maxreg);
    limfit = *(ptr+5);        /* Number of locations used in fitting */

    /* Get fitted flag  */
    ptr    = dbuf + ((122-1) * maxreg);
    fitted = *(ptr+1);

    /* Get origin */
    ptr    = dbuf + ((123-1) * maxreg);
    xu0t   = *(ptr+1);
    yu0t   = *(ptr+2);
    xs0t   = *(ptr+3);
    ys0t   = *(ptr+4);
    iredut = *(ptr+5);

    if (iredut != iredu)
       {
       spouts("Inconsistent image size reduction factor: "); spouti(iredut) ;
       if (dbuf) free(dbuf); dbuf = NULL;
       return 1;
       }

    /* Get tilt angles */
    ptr     = dbuf + ((124-1) * maxreg);
    thetaf  = *(ptr+1);
    gammaff = *(ptr+2);
    phif    = *(ptr+3);
	      
    /* Free dbuf here */
    if (dbuf) free(dbuf); dbuf = NULL;
    }  // End of: if (get_tilt_doc)


 if (get_win_doc)
    {
    /* Retrieve data from dfil4 & dfil5 for # of background windows (if any) --------------- */
 
    back_win_now = 1;        /* Used for pickback */

    maxkeys = 9999;          /* Set size for array pointed to by dbuf */

    if (getdoc_f((FILE *) NULL, dfil4, datexc, maxkeys, maxreg, &dbuf, &maxwin1, FALSE) > 1) 
       {   /* Problem retrieving doc file. Assume it does not exist */
       if (dbuf) free(dbuf); dbuf = NULL;
       //{spouts("*** Background window doc file not available yet: "); spout(dfil4);} 
       return 0;
       }

    back_wins1 = 0;
    ptr        = dbuf;
    for (i= 0; i < maxwin1; i++)
       {
       //if (i < 20) printf("at ptr:   %f %f \n",*ptr,*(ptr+1));
       if (*ptr > 0 && i < 998) back_wins1 = i + 1; 
       ptr += maxreg;
       }

    /* Free dbuf here */
    if (dbuf) free(dbuf); dbuf = NULL;


    if (getdoc_f((FILE *) NULL, dfil5, datexc, maxkeys, maxreg, &dbuf, &maxwin2, FALSE) > 1) 
       {   /* Problem retrieving doc file. Assume it does not exist */
       if (dbuf) free(dbuf); dbuf = NULL;
       // {spouts("*** Background window doc file not available yet: "); spout(dfil5);} 
       return 0;
       }

    back_wins2 = 0;
    ptr        = dbuf;
    for (i= 0; i < maxwin1; i++)
       {
       if (*ptr > 0 && i < 998) back_wins2 = i + 1; 
       ptr += maxreg;
       }

    /* Free dbuf here */
    if (dbuf) free(dbuf); dbuf = NULL;

    if (back_wins1 != back_wins2) 
       {
       spouts("*** Number of Untilted and tilted background windows not same in: "); 
       spouts(dfil4);  spouts(" & ");  spout(dfil5); 
       }

    /* Used for pickback */
    back_win_now    = back_wins1;         
    if (back_wins2 > back_win_now) 
       back_win_now = back_wins2;        
    back_win_now++;         
    }  // End of: if (get_win_doc)

 return 0;
 }


 /********************   fitdoc_addpart   ****************************/

 int fitdoc_addpart(int n, int iwhich, int xu, int yu, int xt, int yt )
 {

 if ( (xu0 == NULL) || (n > (sizepartarray -1)) )
    {        /* Reallocate space for  arrays */

    sizepartarray += 100;

    if (((xim = (float *) realloc((void *) xim, sizepartarray * sizeof(float))) == (float *) NULL) ||
        ((xu0 = (float *) realloc((void *) xu0, sizepartarray * sizeof(float))) == (float *) NULL) ||
        ((yu0 = (float *) realloc((void *) yu0, sizepartarray * sizeof(float))) == (float *) NULL) ||
        ((xs =  (float *) realloc((void *) xs,  sizepartarray * sizeof(float))) == (float *) NULL) ||
        ((ys =  (float *) realloc((void *) ys,  sizepartarray * sizeof(float))) == (float *) NULL) ||
        ((xs2 = (float *) realloc((void *) xs2, sizepartarray * sizeof(float))) == (float *) NULL) ||
        ((ys2 = (float *) realloc((void *) ys2, sizepartarray * sizeof(float))) == (float *) NULL))
        { spout("*** Unable to reallocate memory in fitdoc_addpart."); return 1; }
    }

 /* Put particle data in arrays */
 if (iwhich == 0  || iwhich == 1) 
    {
    xim[n] = n;  
    xu0[n] = xu;
    yu0[n] = yu;  
    }

 if (iwhich == 0 || iwhich == 2) 
    {
    xim[n] = n;  
    xs[n]  = xt;
    ys[n]  = yt;  
    }

 return 0;
 }

