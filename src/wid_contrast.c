
/*$Header: /usr16/software/web/src/RCS/wid_contrast.c,v 1.8 2018/12/07 17:03:35 leith Exp $*/

/*C++*******************************************************************
 C                                                                     *
 C wid_contrast.c    New                         ArDean Leith Jun 2011 * 
 C                   if (nend_com <= ngo_com)    ArDean Leith Sep 2011 *
 C                                                                     *
 C *********************************************************************
 C * AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 C *********************************************************************
 C
 C  wid_conrast
 C
 C  PURPOSE:      Menu line widget sets image contrast 
 C
 C  CALLED BY:    web_com
 C
 C--********************************************************************
*/

#include <Xm/Scale.h>

#include "common.h"
#include "routines.h"
#include "extras.h"

#ifdef WEB_TRUE
// WEB_TRUE -----------------------------------------------------------

 // Internal function prototypes 
 static void   wid_contrast_but(Widget, XtPointer, XtPointer);

 // External common variables

 // Common variables defined here

 // File scope static variables 
 static Widget iw_contrast = (Widget)0;
 static Widget iw_ngo, iw_nend;

 /**********************   wid_contrast    ***************************/

 Widget wid_contrast(Widget iw_parent, char * label,  int ix, int iy)

 { 
 Widget iw_rowcolh;
 Widget iw_dums, iw_pushc, iw_pusha;

  if (iw_contrast <= (Widget)0)
    {   /* Create contrast menu widget first */

    // Create a horizontal row-col widget for scales 
    iw_contrast = wid_rowcol(iw_parent, 'h', -1,-1);

    // Create label  
    wid_labelg(iw_contrast, 0, label, -1,-1);

    /* Create scale widget for starting value -------------- ngo  */
    iw_ngo  = wid_scale(iw_contrast, 0, "", 
                        mapgo,mapend,  ngo, 
                        110,29, -1,-1);

    /* Create scale widget for ending value --------------- nend  */
    iw_nend  = wid_scale(iw_contrast, 0, "", 
                       mapgo,mapend,  nend, 
                       110,29, -1,-1);

    XtAddCallback(iw_ngo,XmNvalueChangedCallback,
                       (XtCallbackProc) wid_contrast_but,"0");

    XtAddCallback(iw_nend,XmNvalueChangedCallback,
                       (XtCallbackProc) wid_contrast_but,"0");
    }
 else
    {   /* Contrast menu widget already exists, update it */

    /* Alter scale widget for starting value --------------- ngo  */
    iw_ngo = wid_scale(iw_contrast, iw_ngo, "", 
                       mapgo,mapend,  ngo, 
                       0,0, -1,-1);

    /* Alter scale widget for ending value  --------------- nend  */
    iw_nend = wid_scale(iw_contrast, iw_nend, "", 
                        mapgo,mapend,  nend, 
                        0,0, -1,-1);
    }

 XtManageChild(iw_contrast);

 return iw_contrast;
  }


 /************* Contrast changed callback ************************/

 void wid_contrast_but(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int             nindex, ival, iflags, indx;  
 float           fval;
 int             ngo_com, nend_com;

 XmScaleGetValue(iw_ngo,  &ngo_com);
 XmScaleGetValue(iw_nend, &nend_com);

 if (nend_com <= ngo_com)
    { spout("*** Upper value must be > lower value"); return;}
 else if (ngo_com < imagego)
    { spout("*** Lower value outside of image ramp"); return;}
 else if (nend_com > imageend)
    { spout("*** Upper value outside of image ramp"); return;}

 // Use mapgo to signal ramp info
 ngo        = ngo_com;  
 nend       = nend_com;
 neednewmap = TRUE;

 // Create new vmap
 newvmap(ngo,nend,vmap);

 // Apply vmap
 expos_cb(NULL,NULL,NULL);

 return;
 }




#else
// WEB_24 --------------------------------------------------- WEB_24

 // Internal function prototypes 
 void          wid_contrast_but(Widget, XtPointer, XtPointer);

 // File scope static variables 
 static Widget iw_contrast = (Widget)0;
 static Widget iw_ngo, iw_nend;

// Common variables defined here

 /**********************   wid_contrast    ***************************/

 Widget wid_contrast(Widget iw_parent, char * label,  int ix, int iy)

 { 
 Widget iw_rowcolh;
 Widget iw_dums, iw_pushc, iw_pusha;

  if (iw_contrast <= (Widget)0)
    {   /* Create contrast menu widget first */

    // Create a horizontal row-col widget for scales 
    iw_contrast = wid_rowcol(iw_parent, 'h', -1,-1);

    // Create label  
    wid_labelg(iw_contrast, 0, label, -1,-1);

    /* Create scale widget for starting value -------------- ngo */
    iw_ngo  = wid_scale(iw_contrast, 0, "", 
                        mapgo,mapend,  ngo, 
                        110,29, -1,-1);

    /* Create scale widget for ending value --------------- nend */
    iw_nend  = wid_scale(iw_contrast, 0, "", 
                       mapgo,mapend,  nend, 
                       110,29, -1,-1);

    XtAddCallback(iw_ngo,XmNvalueChangedCallback,
                       (XtCallbackProc) wid_contrast_but,"0");

    XtAddCallback(iw_nend,XmNvalueChangedCallback,
                       (XtCallbackProc) wid_contrast_but,"0");
    }
 else
    {   /* Contrast menu widget already exists, update it */

    /* Alter scale widget for starting value --------------- ngo */
    iw_ngo = wid_scale(iw_contrast, iw_ngo, "", 
                       mapgo,mapend,  ngo, 
                       0,0, -1,-1);

    /* Alter scale widget for ending value  ---------------- nend */
    iw_nend = wid_scale(iw_contrast, iw_nend, "", 
                        mapgo,mapend,  nend, 
                        0,0, -1,-1);
    }

 XtManageChild(iw_contrast);

 return iw_contrast;
  }


 /************* Contrast changed callback ********************************/

 void wid_contrast_but(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int             nindex, ival, iflags, indx;  
 float           fval;
 int             ngo_com, nend_com;

 XmScaleGetValue(iw_ngo,  &ngo_com);
 XmScaleGetValue(iw_nend, &nend_com);

 if (nend_com <= ngo_com)
    { spout("*** Upper value must be > lower value"); return;}
 else if (ngo_com < imagego)
    { spout("*** Lower value outside of image ramp"); return;}
 else if (nend_com > imageend)
    { spout("*** Upper value outside of image ramp"); return;}
 iflags = DoRed | DoGreen | DoBlue;
  
 ngo    = ngo_com;
 nend   = nend_com;
                                                                               
 /* Set colors in the image portion of the colormap, only */ 
 indx = 0;
 for (nindex = mapgo; nindex <= mapend; nindex++)
    {
    colort[indx].pixel = ispicol[nindex];
    colort[indx].flags = iflags;

    if (nindex <= ngo) 
       {
       /* Set this color to black */
       ival = 0;
       }
    else if (nindex < nend)
       {
       /* Interpolate shade */
       fval = 65535.0 * (float)(nindex - ngo) / 
                        (float)(nend - ngo);
       ival = fval;
       }
    else 
       {
       /* Set this color to white */
       ival = 65535;
       }

    colort[indx].red    = ival;
    colort[indx].green  = ival;
    colort[indx++].blue = ival;
    }

 //printf(" Mapped: %d ... %d \n",ngo,nend);
 
 /* Put the colors in the current colormap */
 XStoreColors(idispl,map,colort,indx);

 XFlush(idispl);
 return;
 }


#endif





   
  



