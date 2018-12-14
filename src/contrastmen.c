
/*$Header: /usr16/software/web/src/RCS/contrastmen.c,v 1.19 2018/12/07 17:03:31 leith Exp $*/

/*C++*******************************************************************
C                                                                      *
C contrastmen.c     New                        ArDean Leith  Dec  1992 *
C                   Directcolor mods           Erik Vogan    Jul  2001 *
C                   Truecolor mods             ArDean Leith  Sep  2011 *
C                                                                      *
C **********************************************************************
C *  AUTHOR: A. LEITH                                                  *
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
C    FILE:         contrastmen
C
C    PARAMETERS:   
C
C    PURPOSE:      Sets image contast using luts
C
C    CALLED BY:    web_com
C
C    NOTE:         Maybe this should work over imagego...imagend 
C                  instead of mapgo... mapend??
C
C--********************************************************************
*/

#include <Xm/Scale.h>
#include "common.h"
#include "routines.h"
#include "extras.h"


#ifdef WEB_TRUE 
//     WEB_TRUE ------------------------------------------------------

 // Internal function prototypes 
 static void    contrastmen_buta(Widget, XtPointer, XtPointer);

 // File scope static variables 
static Widget  iw_contrastmen = (Widget)0;
static Widget  iw_ngo, iw_nend;

// Common variables used here

 /**********************   contrastmen   ******************************/

 void contrastmen(Widget iw_parent, XtPointer data, XtPointer call_data)

 { 
 Widget iw_lab1, iw_rowcolv, iw_rowcolh;
 Widget iw_dums, iw_pushc, iw_pusha;


  if (iw_contrastmen <= (Widget)0)
    {   /* Create contrast menu widget first */

    iw_contrastmen = wid_dialog(iw_win, 0, "Contrast options", -1,-1);
    iw_rowcolv     = wid_rowcol(iw_contrastmen, 'v', -1,-1);

    /* Create label for scales */
    iw_lab1 = wid_labelg(iw_rowcolv, 0,
                         "Lower and upper end of ramp:", -1,-1);

    /* Create a horizontal row-col widget for scales */
    iw_rowcolh = wid_rowcol(iw_rowcolv, 'h', -1,-1);

    /* Create scale widget for starting value ------------------- ngo */
    iw_ngo  = wid_scale(iw_rowcolh, 0, "", ngo,nend,  ngo, 
                        180,50, -1,-1);

    /* Create scale widget for ending value -------------------- nend */
    iw_nend = wid_scale(iw_rowcolh, 0, "", ngo,nend,  nend, 
                       180,50, -1,-1);

    XtAddCallback(iw_ngo,XmNvalueChangedCallback,
                           (XtCallbackProc) contrastmen_buta,"0");
    XtAddCallback(iw_nend,XmNvalueChangedCallback,
                           (XtCallbackProc) contrastmen_buta,"0");

    /* Create box for cancel --------------------------------- cancel */
    iw_dums = wid_stdbut(iw_rowcolv, iw_contrastmen, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "C",
                        fin_cb,fin_cb ,fin_cb, NULL);
    }
 
 else
    {   /* Contrast menu widget already exists, update it */

    /* Alter scale widget for starting value ------------------ ngo */
    iw_ngo = wid_scale(iw_rowcolh, iw_ngo, "", ngo,nend,  ngo, 
                       180,50, -1,-1);

    /* Alter scale widget for ending value  ------------------ nend */
    iw_nend = wid_scale(iw_rowcolh, iw_nend, "", ngo,nend,  nend, 
                       180,50, -1,-1);
    }
 XtManageChild(iw_contrastmen);

}

 /************* Contrast changed callback ********************************/

 void contrastmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int    ngo_com, nend_com;

 XmScaleGetValue(iw_ngo,  &ngo_com);
 XmScaleGetValue(iw_nend, &nend_com);

 if (nend_com <= ngo_com)
    { spout("*** Upper value must be > lower value"); return;}
 else if (ngo_com < 0)
    { spout("*** Lower value outside of image ramp"); return;}
 else if (nend_com > 255)
    { spout("*** Upper value outside of image ramp"); return;}

 // Use mapgo to signal ramp info
 ngo  = ngo_com;  
 nend = nend_com;

 // Create new vmap
 newvmap(ngo,nend,vmap);

 // Flag for vmap changed
 neednewmap = TRUE;

 // Update display using new vmap.
 expos_cb(NULL,NULL,NULL);

 return;
 }



#else
// WEB_24 --------------------------------------------------------------

#include <Xm/ToggleBG.h>

 /* Internal function prototypes */
 void          contrastmen_buta(Widget iw_temp, XtPointer, XtPointer);

 /* Internal common variables */
 static Widget  iw_contrastmen= (Widget)0, iw_red, iw_blue, iw_green;
 static Widget  iw_ngo, iw_nend;
 static int     redit, greenit, blueit;

 // Common variables used here

 /**********************   contrastmen   ******************************/

 void contrastmen(Widget iw_parent, XtPointer data, XtPointer call_data)

 { 
 Widget iw_lab1, iw_rowcolv, iw_rowcolh;
 Widget iw_dums, iw_pushc, iw_pusha;

  if (iw_contrastmen <= (Widget)0)
    {   /* Create contrast menu widget first */

    iw_contrastmen  = wid_dialog(iw_win, 0, "Contrast options", -1,-1);
    iw_rowcolv      = wid_rowcol(iw_contrastmen, 'v', -1,-1);

    /* Create label for scales */
    iw_lab1 = wid_labelg(iw_rowcolv, 0,
                         "Lower and upper end of ramp:", -1,-1);

    /* Create a horizontal row-col widget for scales */
    iw_rowcolh = wid_rowcol(iw_rowcolv, 'h', -1,-1);

    /* Create scale widget for starting value ------------------- ngo  */
    iw_ngo  = wid_scale(iw_rowcolh, 0, "", mapgo,mapend,  ngo , 
                        180,50, -1,-1);

    /* create scale widget for ending value -------------------- nend  */
    iw_nend = wid_scale(iw_rowcolh, 0, "", mapgo,mapend,  nend , 
                       180,50, -1,-1);

    XtAddCallback(iw_ngo,XmNvalueChangedCallback,
                           (XtCallbackProc) contrastmen_buta,"0");
    XtAddCallback(iw_nend,XmNvalueChangedCallback,
                           (XtCallbackProc) contrastmen_buta,"0");

    /* Create toggle boxes for red, greeen & blue colors */
    redit   = TRUE;
    greenit = TRUE;
    blueit  = TRUE;

    iw_red   = wid_togmen(iw_rowcolv, 0, "Red  ", redit,   -1,-1);
    iw_green = wid_togmen(iw_rowcolv, 0, "Green", greenit, -1,-1);
    iw_blue  = wid_togmen(iw_rowcolv, 0, "Blue ", blueit,  -1,-1);

    /* Create box for cancel --------------------------------- cancel */
    iw_dums = wid_stdbut(iw_rowcolv, iw_contrastmen, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "C",
                        fin_cb,fin_cb ,fin_cb, NULL);
    }
 else
    {   /* Contrast menu widget already exists, update it */

    /* Alter scale widget for starting value -------------------- ngo  */
    iw_ngo = wid_scale(iw_rowcolh, iw_ngo, "", mapgo,mapend,  ngo , 
                       180,50, -1,-1);

    /* Alter scale widget for ending value  -------------------- nend  */
    iw_nend = wid_scale(iw_rowcolh, iw_nend, "", mapgo,mapend,  nend , 
                       180,50, -1,-1);
    }

 XtManageChild(iw_contrastmen);

 }


 /************* Accept button callback ********************************/

 void contrastmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int             nindex, ival, iflags, indx;  
 float           fval;
 int             ngo_com, nend_com;

 /* Get ngo_com & nend_com */
 XmScaleGetValue(iw_ngo, &ngo_com);
 XmScaleGetValue(iw_nend,&nend_com);

 if (nend_com < ngo_com)
    { spout("*** Upper value must be > lower value"); return;  }
 else if (ngo_com < mapgo)
    { spout("*** Lower value outside of image ramp"); return; }
 else if (nend_com > mapend)
    { spout("*** Upper value outside of image ramp"); return;  }

 ngo     = ngo_com;
 nend    = nend_com;

 redit   = XmToggleButtonGadgetGetState(iw_red);
 blueit  = XmToggleButtonGadgetGetState(iw_green);
 greenit = XmToggleButtonGadgetGetState(iw_blue);

 iflags = 0;
 if (redit)   iflags = DoRed; 
 if (greenit) iflags = iflags | DoGreen ;
 if (blueit)  iflags = iflags | DoBlue;
                                                                                 
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
       fval = 65535.0 * (float)(nindex - ngo) / (float) (nend - ngo);
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
 
 /* Put the colors in the desired colormap */
 XStoreColors(idispl,map,colort,indx);

 XFlush(idispl);
 return;
 }

#endif




   
  
