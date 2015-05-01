
/*$Header: /usr8/web/src/RCS/catmen.c,v 1.13 2012/04/17 18:32:04 leith Exp $*/
/*
C++*********************************************************************
C
C  catmen.c          Jan 94 
C
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
C    catmen()
C
C    PURPOSE:    Displays category selecting menu
C
C    PARAMETERS: None   
C
C    CALLED BY:  mont2      
C
C--*********************************************************************
*/
#include "common.h"
#include "routines.h"
#include <Xm/ToggleBG.h>

 /* External function prototypes */
 extern void   cat          (void);

 /* Internal function prototypes */
 void          catmen_buts(Widget, XtPointer, XtPointer);
 void          catmen_buta(Widget, XtPointer, XtPointer);
  
 /* External common variables */
 extern int        ncat;        /* number of categories wanted */

 /* Internal common variables */
 static Widget     iw_catmen= 0, iw_docgg, iw_nkeyc, iw_ncat;
 static int        showing = FALSE;

 /********************   catmen   ****************************/

 void catmen(void)

 {
 static Widget  iw_rowcolv; 
 Widget  iw_pushs, iw_pusha; 

 char    cval[3];

 if (iw_catmen == (Widget)0)
    {   
    /*   Create categories doc file menu first */

    iw_catmen = wid_dialog(iw_win, 0, "Categories document file", 
                           -1, -1);
    iw_rowcolv  = wid_rowcol(iw_catmen, 'v', -1, -1);

    /*   Create a text box for doc file name input */
    iw_docgg = wid_textboxb(iw_rowcolv,0,"Output doc file:  ",
                            docnam,20);

    /*   Create a text box for category numbers input */
    sprintf(cval,"%d",ikeyc);

    /* printf("---ikey = %d\n", ikeyc); */

    iw_nkeyc = wid_textboxb(iw_rowcolv,0,"Starting key number:  ",
                            cval,4);

    /*   Create a text box for category numbers input */
    sprintf(cval,"%d",ncat);
    iw_ncat = wid_textboxb(iw_rowcolv,0,"Categories:  ",cval,2);

    /* Create box for apply  */
    wid_stdbut(iw_rowcolv, iw_catmen, 
                         &iw_pushs, &iw_pushs, &iw_pusha, "SA",
                         catmen_buts, catmen_buts ,catmen_buta, NULL);

    }
 else
    {
    /*   Create a text box for doc file name input */
    iw_docgg = wid_textboxb(iw_rowcolv,iw_docgg,"Output doc file:",
                            docnam,20);

    /*   Recover the key num  */
    sprintf(cval,"%d",ikeyc);
    iw_nkeyc = wid_textboxb(iw_rowcolv,iw_nkeyc,"Starting key number:  ",cval,4);
    }
 XtManageChild(iw_catmen);

 }

/***********  Docfile accept button callback ***********************/

 void catmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 float        fdum;

 if (rdprxc(iw_docgg,&docnam[0],81) == 0) 
    {
    spout("*** You must enter a doc. file name now!");
    return;
    }

 /*  Find new key */
 if (!rdprx(iw_nkeyc,'i',&ikeyc,&fdum)) return;

 /*  Find ncat */
 if (!rdprx(iw_ncat,'i',&ncat,&fdum)) return;
 if (ncat > 9 || ncat < 1) 
    {
    spout("*** Categories range 1 ... 9");
    return;
    }

 /*  Remove doc file menu */
 XtUnmanageChild(iw_catmen);

 /*  Refresh screen  */
 expos_cb(NULL,NULL,NULL);

 /*  Start categorizing routine */
 showing = TRUE;
 cat();

 }


/*********** Stop button callback **********************************/

 void catmen_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 /*  Remove message */
 if (showing)
   {
   showing = FALSE;
   showbutx("","","",TRUE);
   }

 /*  Remove the menu widget */
 XtUnmanageChild(iw_catmen);

 /*  Cancel buttons,  stop this routine */
 XtUninstallTranslations(iw_win);

 }


