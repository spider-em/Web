
/*$Header: /usr16/software/web/src/RCS/imoptmen.c,v 1.15 2018/12/07 17:03:32 leith Exp $*/
/*
C++************************************************************************
C
C IMOPTMEN -- CREATED JUNE 91
C          -- CONVERTED TO C -- AUG 92      
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C IMOPTMEN
C
C PURPOSE:         SET VARIOUS PARAMETERS RELATING TO IMAGE DISPLAY
C
C VARIABLES:       NAMES      LOGICAL FLAG TO PRINT NAMES
C                  IXUL,IYUL  UPPER LEFT CORNER FOR LAST IMAGE
C                  MARX,MARY  X & Y MARGINS FOR MONTAGE
C
C CALLED BY:       WEB_COM
C      
C***********************************************************************
*/

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#include "x.h"
#include "std.h"
#include "common.h"
#include "routines.h"

 /* Function prototypes */
 static void   imoptmen_buta(Widget, void *, void *);

 /* File variables */
 Widget   iw_names,  iw_keepxy, iw_newline, iw_nuscrn, iw_listfi;
 Widget   iw_movieo, iw_marlef, iw_martop,  iw_marx,   iw_mary;
 Widget   iw_iskip;
 Widget   iw_imoptmen;


 /**************************  imoptmen *******************************/

 void imoptmen(Widget iw_temp, void * data, void * call_data)
 { 
 char     cval[5];
 Widget   iw_marlefl, iw_martopl, iw_marxl, iw_maryl, iw_iskipl;
 Widget   iw_dum, iw_rowcol, iw_rowcol1, iw_rowcol2, iw_rowcol3;
 Widget   iw_pushc, iw_pusha, iw_dums, iw_rawsiz;

 if (iw_imoptmen <= (Widget)0)
    {   /* create 3-D montage menu widget first */

    iw_imoptmen = wid_dialog(iw_win,iw_imoptmen,
                             "image display options",20,20);
    iw_rowcol   = wid_rowcol(iw_imoptmen,'v',-1,-1);

    iw_rowcol1  = wid_rowcol(iw_rowcol,'h',-1,-1);
    /* create text box for marlef input ---------------------- marlef */
    sprintf(cval,"%4d",marlef);
    iw_marlef = wid_textboxb(iw_rowcol1,0, "MARGIN    LEFT:",cval,4);
    /* create text box for martop input ---------------------- martop */
    sprintf(cval,"%4d",martop);
    iw_martop = wid_textboxb(iw_rowcol1,0, " TOP:",cval,4);

    iw_rowcol2 = wid_rowcol(iw_rowcol,'h',-1,-1);
    /* create text box for marx input  ------------------------- marx */
    sprintf(cval,"%4d",marx);
    iw_marx = wid_textboxb(iw_rowcol2,0,   "MARGIN       X:",cval,4);
    /* create text box for mary input  ------------------------- mary */
    sprintf(cval,"%4d",mary);
    iw_mary = wid_textboxb(iw_rowcol2,0,   "   Y:",cval,4);

    /* create text box for iskip input  ----------------------- iskip */
    sprintf(cval,"%3d",iskip);
    iw_iskip = wid_textboxb(iw_rowcol,0,   "SIZE REDUCTION:",cval,4);

    /* create toggle box for movie ---------------------------- movie */
    iw_movieo = wid_togmen(iw_rowcol,0,"MONTAGE MOVIE ",movie,0,0);

    /* create toggle box for names ---------------------------- names */
    iw_names = wid_togmen (iw_rowcol,0,"SHOW FILE NAMES ",names,0,0);
 
    /* create toggle box newline ---------------------------- newline */
    iw_newline = wid_togmen(iw_rowcol,0,"START NEW LINE OF IMAGES ",
                             newline,0,0);

    /* create toggle box nuscrn ------------------------------ nuscrn */
    iw_nuscrn = wid_togmen (iw_rowcol,0,"START NEW SCREEN OF IMAGES",
                             nuscrn,0,0);

    /* create toggle box listfi ------------------------------ listfi */
    iw_listfi = wid_togmen(iw_rowcol,0,"LIST FILES IN FILE-SELECTOR",
                             listfi,0,0);

    /* create toggle box for saving x & y set location ------- keepxy */
    iw_keepxy = wid_togmen(iw_rowcol,0,"KEEP X & Y POSITION FOR MONTAGE",
                             keepxy,0,0);

    /* make a push button for changing raw image size  */
    iw_rawsiz = wid_pushg(iw_rowcol, 0, "SET RAW IMAGE SIZE  ",
                  get_imsiz,"0", 0,0);
 
    /* create accept boxes  ----------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    iw_dum   = wid_stdbut(iw_rowcol, iw_imoptmen, &iw_dums, &iw_pushc, &iw_pusha, 
                        "CA",fin_cb,fin_cb,imoptmen_buta, NULL);
    }

 XtManageChild(iw_imoptmen);
 }


 /************ accept button callback *********************************/

 void imoptmen_buta(Widget iw_temp, void * data, void * call_data)
 {

 /* names_height: pixel height of the filename */
 if( (names   = XmToggleButtonGadgetGetState(iw_names)) )
     names_height = 12; 
 else
     names_height = 0;
 
 keepxy  = XmToggleButtonGadgetGetState(iw_keepxy);
 newline = XmToggleButtonGadgetGetState(iw_newline);
 nuscrn  = XmToggleButtonGadgetGetState(iw_nuscrn);
 listfi  = XmToggleButtonGadgetGetState(iw_listfi);
 movie   = XmToggleButtonGadgetGetState(iw_movieo);

 if (strtoint(iw_marlef,"left margin",&marlef) == FALSE) return;

 if (strtoint(iw_martop,"top margin", &martop) == FALSE) return;

 if (strtoint(iw_marx,  "x margin",   &marx)   == FALSE) return;

 if (strtoint(iw_mary,  "y margin",   &mary)   == FALSE) return;

 if (strtoint(iw_iskip, "reduction",  &iskip)  == FALSE) return;

 XtUnmanageChild(iw_imoptmen);
 }


