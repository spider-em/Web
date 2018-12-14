
/*$Header: /usr16/software/web/src/RCS/lut.c,v 1.7 2018/12/07 17:03:32 leith Exp $*/
/*
C++*************************************************************************
C                                                                      *
C wid_lut.for -- Created                        Dec    89              *
C lut.c          Ported                         Dec    92              *
C                Directcolor mods               Jul  2001 Erik Vogan   *
C                Truecolor mods                 Sep  2011 ArDean Leith *
C                                                                      *
C **********************************************************************
C    AUTHOR:  ArDean Leith
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
C
C    lut(fillut,map,nmin,nmax)
C
C    PARAMETERS:  filnam        Lut file name                  input
c                 map           Colormap pointer               input
c                 nmin,nmax     Range of indices to be set     input
c
C    CALLED BY:   lutmen
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "extras.h"


 /*************************** lut ************************************/

 int lut(char * filnam, Colormap mapt, int nmint, int nmaxt)
 {
 int             nindex, neg, nchar, indx, ipixel, ival;
 int             ir, ig, ib, ic;
 float           vmax, scalin, facsum, factin, fact, dx;
 FILE            *luncol;
 char            output[80];
 char            ctemp[81];

 /* File name sent from caller */
        
 if (strcmp(filnam,"bwth") == 0) 
    { spout("*** Use contrast option to do this!"); return FALSE; }

 else if ((strcmp(filnam,"flip") == 0)  || 
          (strcmp(filnam,"colo") ==0)) 
    { spout("*** Operation not implemented yet!");  return FALSE; }

 neg = FALSE;
 if (*filnam == '-') 
    {  
    neg     = TRUE;
    *filnam = ' ';
    }
 else if (*filnam == '+')
    *filnam = ' ';

 if ((luncol= fopen(filnam,"r")) ==  NULL)
    {    /* File does not exist */
    sprintf(output," *** Unable to open look-up-table: %s",filnam);
    spout(output);  return FALSE;
    } 

 /* Find out whether the lut specifies the indicies or resets   */
 /* all of the indices one-by-one                               */
 /* if the indicies are specified there are 4 integers per line */
 /* otherwise only 3 integers per line                          */

 vmax   = 65535.0;
 fact   = vmax / 255.0;

 scalin = vmax / (nmaxt - nmint + 1);
 factin = (float) (nmaxt - nmint + 1) / 256.0;
 facsum = nmint;

 nchar = strlen(fgets(ctemp, 80, luncol));
 rewind(luncol);

 /*   printf("nchar: %d \n",nchar); */


 if (nchar <= 16 || nchar >= 79) 
    {
    /* Load color lookup table, indicies not specified */
    indx = 0;
    for (nindex = 1; nindex <=256; nindex++)
       {
       /* Read a line of the lut file */
       if (fscanf(luncol,"%d %d %d",&ir,&ig,&ib) == EOF)
          break;

      /* Determine indx  for this line.  only indices: nmint...nmaxt  */
      /* (usually 0..127) are to be used. so I map lines 1..256 to    */
      /* this range.  this may discard some lines from the lut file!   */

      dx     = (nindex - 1) * factin + facsum;
      ipixel = dx;
      if (neg) ipixel = nmaxt - ipixel;

      /* Make sure ipixel is in range:  nmint...nmaxt */
      ipixel = MYMAX(MYMIN(ipixel,nmaxt),nmint);

#ifdef WEB_TRUE
      // Create virtual colormap
      colvmap[0][indx]     = MYMIN(MYMAX((ir*fact),0),vmax);;
      colvmap[1][indx]     = MYMIN(MYMAX((ig*fact),0),vmax);
      colvmap[2][indx]     = MYMIN(MYMAX((ib*fact),0),vmax);
      colvmap[3][indx]     = 0;
#else
      // Create colormap
      colort[indx].pixel   = ispicol[ipixel];
      colort[indx].red     = MYMIN(MYMAX((ir*fact),0),vmax);
      colort[indx].green   = MYMIN(MYMAX((ig*fact),0),vmax);
      colort[indx].blue    = MYMIN(MYMAX((ib*fact),0),vmax);
      colort[indx++].flags = DoRed|DoGreen|DoBlue;
#endif

      //printf(" %d : %d : %d , %d , %d \n",indx,ipixel,colort[indx-1].red,
      //                           colort[indx-1].green,colort[indx-1].blue);

      }
    }
 else
    {
    /* Load color lut  at indicies given in file  */
    indx = 0;
    for (nindex = 0; nindex < 256; nindex++)
       {
       /* Read a line of the lut file */
       if (fscanf(luncol,"%d %d %d %d",&ir,&ig,&ib,&ival) == EOF)
          break;

       ic   = 0;

       /* Make sure index is within nmint....nmaxt range */
       if (ival >= nmint && ival <= nmaxt) 
          {
          /* Minus sign is for negative vs positive */
          if (neg) ival = nmaxt - ival;

#ifdef WEB_TRUE
          // Create virtual colormap
          colvmap[0][indx]   = ir * fact;
          colvmap[1][indx]   = ig * fact;
          colvmap[2][indx]   = ib * fact;
          colvmap[3][indx]   = 0;
#else
          // Create colormap
          colort[indx].pixel = ispicol[ival];
          colort[indx].flags = DoRed|DoGreen|DoBlue;

          colort[indx].red   = ir * fact;
          colort[indx].green = ig * fact;
          colort[indx].blue  = ib * fact;
          indx++;
#endif
          }
       }
    }

#ifdef WEB_TRUE
 use3colors = TRUE;
#else
 // Put the colors in the desired colormap 
 XStoreColors(idispl,mapt,colort,indx);
#endif

 XFlush(idispl);
 fclose(luncol);

 return TRUE;
 }

