
/*
C**************************************************************************
C
C   surfcolor.c  -- CREATED JUNE 1995
C
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C  surfcolor(colorvol)
C
C  PURPOSE:     sets up colorvolume stuff
C
C  PARAMETERS:  colvol 3D texture mapping vol  buffer

C  RETURNS:     VOID
C
C  CALLED BY:   surf
C
C  NOTES:       
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "extras.h"

 /* external function prototypes */

 /* externally defined variables used here */

 /* static variables for use here */
 static Colormap mapsurf;

 /* variables that are used here */

 /* variables that are used elsewhere also */

 /******************************   surfcolor   ***********************/

 int surfcolor(unsigned char * colvol, int nvolcolors, int * ncolrange)

 {
 int nsamc, nrowc, nslicec, iformc;
 FILEDATA   * filedatac;      /* for color volume */
 float      *fdumptr, fdum0, fdum1;
 int        indx, ipixel, nindex, nredcolors, ncolors, i;
 float      fact, colval;
 char       output[80];
 Visual     *visualb;

 if (colvol != (unsigned char *) NULL)
    {
    /* load the color mapping volume into char array */
    /* normalize over 0..nvolcolors                  */

    filedatac = openold(filcolvol,&nsamc,&nrowc,&nslicec, &iformc,"o");

    if (filedatac == (FILEDATA *)FALSE)
       {
       spout("*** Unable to open color volume file");
       waitcursor(iwin, FALSE, 79);
       return FALSE;
       }

    if ((filedatac->imami != 1) && (norm(filedatac) == FALSE))
       {    
       spout("*** Color volume man/max extraction failed.");
       closefile(filedatac);
       waitcursor(iwin, FALSE, 79);
       return FALSE;
       }

    if (!redvol(filedatac, TRUE, (char **)&colvol, FALSE, &fdumptr, 
             1, filedata->nsam, 1,filedata->nrow, 1,filedata->nslice, 
             0, nvolcolors , 
             filedatac->fmin_spi,filedatac->fmax_spi, &fdum0, &fdum1, 1))
        return FALSE;
  
    /* inform user about partitioning of color volume */
    /* find range for each final color by dividing total color range */

    sprintf(output, "Color volume range: %f...%f", 
                  filedatac->fmin_spi, filedatac->fmax_spi);
    spout(output);

    /* we are through with the file now */
    closefile(filedatac);
    }  /* (colvol != (unsigned char *) NULL) */

 /* make a mapsurf colormap for the nvolcolors */
 /* blue - green - red  - yellow - cyan - magenta - white */

 if (mapsurf == 0) 
    {
    /*   create a new surface color map */
    ncolors = MAXCOL;
    visualb = visual;
    mapsurf = XCreateColormap(idispl,iwtop,visualb,AllocAll);       
    }

 if (mapsurf <= 0) 
    { spout("*** In surfcolor, failed to create colormap"); return FALSE; }

 /* recover colors from the normal colormap */
 for ( i=0; i < ncolors; i++)
    {
#ifdef WEB_24
    colort[i].pixel = ispicol[i];
#else
    colort[i].pixel = i;
#endif
    colort[i].flags = DoRed|DoGreen|DoBlue; 
    }
 XQueryColors(idispl,map,colort,ncolors);


 /* find number of intensities available for each colormap color */
 *ncolrange   = (colorend - mapgo) / nvolcolors;

 /* fill colormap with intensity ramp for each color */
 /* actually this should be skipped and let user set colormap */
 /* using the lut operation from a file */

 fact     = 65535.0 / (*ncolrange);
 indx     = -1;
 ipixel   = mapgo;

 /* I do not define more than seven colors */
 nredcolors = MYMIN(nvolcolors,7);

 for (nindex = 0; nindex < *ncolrange; nindex++)
      {
      /* The color cells between mapgo-->colorend can be used for coloring   */
      /* the image.      */

      colval  = nindex  * fact;

      /* set the colormap position for blue*/
      indx++;
      colort[indx].flags   = DoRed|DoGreen|DoBlue;
#ifdef WEB_24
      colort[indx].pixel   = ispicol[ipixel];
#else
      colort[indx].pixel   = ipixel;
#endif
      colort[indx].red     = 0;
      colort[indx].green   = 0;
      colort[indx].blue    = colval;

      /* set the colormap position for green*/
      indx++;
#ifdef WEB_24
      colort[indx].pixel   = ispicol[ipixel + *ncolrange];
#else
      colort[indx].pixel   = ipixel + *ncolrange;
#endif
      colort[indx].flags   = DoRed|DoGreen|DoBlue;
      colort[indx].red     = 0;
      colort[indx].green   = colval;
      colort[indx].blue    = 0;

      if (nredcolors > 2)
         {
         /* set the colormap position for red*/
         indx++;
#ifdef WEB_24
         colort[indx].pixel   = ispicol[ipixel + 2 * (*ncolrange)];
#else
         colort[indx].pixel   = ipixel + 2 * (*ncolrange);
#endif
         colort[indx].flags   = DoRed|DoGreen|DoBlue;
         colort[indx].red     = colval;
         colort[indx].green   = 0;
         colort[indx].blue    = 0;
         }
      if (nredcolors > 3)
         {
         /* set the colormap position for yellow*/
         indx++;
#ifdef WEB_24
         colort[indx].pixel   = ispicol[ipixel + 3 * (*ncolrange)];
#else
         colort[indx].pixel   = ipixel + 3 * (*ncolrange);
#endif
         colort[indx].flags   = DoRed|DoGreen|DoBlue;
         colort[indx].red     = colval;
         colort[indx].green   = colval;
         colort[indx].blue    = 0;
         }
      if (nredcolors > 4)
         {
         /* set the colormap position for cyan*/
         indx++;
#ifdef WEB_24
         colort[indx].pixel   = ispicol[ipixel + 4 * (*ncolrange)];
#else
         colort[indx].pixel   = ipixel + 4 * (*ncolrange);
#endif
         colort[indx].flags   = DoRed|DoGreen|DoBlue;
         colort[indx].red     = 0;
         colort[indx].green   = colval;
         colort[indx].blue    = colval;
         }
      if (nredcolors > 5)
         {
         /* set the colormap position for magenta*/
         indx++;
#ifdef WEB_24
         colort[indx].pixel   = ispicol[ipixel + 5 * (*ncolrange)];
#else
         colort[indx].pixel   = ipixel + 5 * (*ncolrange);
#endif
         colort[indx].flags   = DoRed|DoGreen|DoBlue;
         colort[indx].red     = colval;
         colort[indx].green   = 0;
         colort[indx].blue    = colval;
         }
      if (nredcolors > 6)
         {
         /* set the colormap position for white*/
         indx++;
#ifdef WEB_24
         colort[indx].pixel   = ispicol[ipixel + 6 * (*ncolrange)];
#else
         colort[indx].pixel   = ipixel + 6 * (*ncolrange);
#endif
         colort[indx].flags   = DoRed|DoGreen|DoBlue;
         colort[indx].red     = colval;
         colort[indx].green   = colval;
         colort[indx].blue    = colval;
         }
      ipixel++;
      }

    /************ debug output ************** removed
    printf("colort[%d] pixel: %d,  r:%d  g:%d  b:%d\n",
            indx, colort[indx].pixel,
            colort[indx].red,colort[indx].green,colort[indx].blue);
    *********** debug output ***************/

    /* put colors in the surface colormap */
    XStoreColors(idispl,mapsurf,colort,indx); 

    /* inform user on info needed for color map*/
    sprintf(output,
        "Number of colors:%d   Initial lut index: %d Indices/color: %d",
         nvolcolors, mapgo, (*ncolrange));
    spout(output);

    /* activate the colormap */
    setamap(mapsurf);


    return TRUE; 
 }
