
/*$Header: /usr8/web/src/RCS/pickbackmen.c,v 1.11 2015/09/01 17:53:27 leith Exp $*/

/*
 C**********************************************************************
 C
 C  pickbackmen.c      Ported                      May 93 ArDean Leith
 C                     Improved                    Aug 15 ArDean Leith
 C
 C**********************************************************************
 C   AUTHOR:  ArDean Leith
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
 C  pickbackmen()
 C
 C  PURPOSE:    Displays background window menu
 C
 C  PARAMETERS: None   
 C
 C  CALLED BY:  pickmen pickback    
 C
 C*********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Text.h>

 /* External function prototypes */
 int            pickbackave (char *);

 /* Internal function prototypes */
 void           pickbackmen_buts (Widget, XtPointer, XtPointer);
 void           pickbackmen_buta (Widget, XtPointer, XtPointer);
 void           pickbackmen_butcl(Widget, XtPointer, XtPointer);
  
 /* Externally defined global variables */
 extern char    dfil4[12], dfil5[12];
 extern int     back_win_now;
 extern int     openit1, openit2;
 extern FILE *  fpdoc4 ;
 extern FILE *  fpdoc5;

 /* Internally defined global variables */
 int            firstback;      /* Set by fitdoc */
 GC             icontxor = 0;

 /* Internal file scope variables */
 static Widget  iw_pickbackmen = (Widget)0;
 static Widget  iw_winkey;
 static Widget  iw_winsiz;
 
 /***********************   pickbackmen   ********************************/

 void pickbackmen(void)

 {
 static Widget  iw_rowcolv;
 Widget         iw_pushs, iw_pushc, iw_pusha, iw_dums; 
 Widget         iw_buter;

 char           cval[10];

 if (iw_pickbackmen <= (Widget)0)
    {   /* Create background windowing menu first */

    iw_pickbackmen = wid_dialog(iw_win, 0, "Background windowing menu", -1, -1);
    iw_rowcolv = wid_rowcol(iw_pickbackmen, 'v', -1, -1);

    /* Create text box for window size   */
    nsamw = 15; 
    nroww = 15;
    sprintf(cval,"%2d",nsamw);
    iw_winsiz   = wid_textboxb(iw_rowcolv,0,"Window size:",cval,3);

    /* Create text box for window number  */
    if (back_win_now < 1) back_win_now = 1;
    sprintf(cval,"%4d",back_win_now);
    iw_winkey   = wid_textboxb(iw_rowcolv,0,"Next window number:",
                               cval,4);

    /* Create pushbutton for erasing windows */
    iw_buter = wid_pushg(iw_rowcolv, 0, "Erase window display",
                         pickbackmen_butcl, NULL, -1,-1);

    /* Create box for stop & start  */
    iw_dums = wid_stdbut_str(iw_rowcolv, iw_pickbackmen, 
                         &iw_pushs, &iw_pushc, &iw_pusha, "SA",
                         "STOP WINDOWING", "", "START WINDOWING",
                         pickbackmen_buts, NULL,
                         pickbackmen_buta, NULL);
    }
 else
    {   /* Update background windowing menu */ 

    /* Update text box for window key number */
    sprintf(cval,"%4d",back_win_now);
    iw_winkey   = wid_textboxb(iw_rowcolv,iw_winkey,
                                "Window number:",cval,4);
    }

 XtManageChild(iw_pickbackmen);
 }


/*********** Start button callback **********************************/

 void pickbackmen_buta(Widget iw_temp, XtPointer data, 
                                       XtPointer call_data)
 {
 char * string;

 /* Find window size */
 string = XmTextGetString(iw_winsiz);
 sscanf(string,"%d",&nsamw);
 if (string) free(string);

 if (nsamw <= 0) 
    { spout("*** Window size must be > 0"); XBell(idispl,50); return; }
 nroww = nsamw;

 /* Find next window key */
 string = XmTextGetString(iw_winkey);
 sscanf(string,"%d",&back_win_now);
 if (string) free(string);

 //printf(" pickbackmen: back_win_now %d\n",back_win_now);
 
 if (back_win_now <= 0) 
    {back_win_now = 1;}

 /* Set default cursor */
 setacursor(0, -1, -1);

 /* Create an xor graphics context if not currently valid */
 if (!icontxor) icontxor = setxor(icontx);

 /* Set default color in xor GC */
 wicolor(icontxor,colorgo+icolorov);

 /* Start background window picking */
 pickback(firstback);

 }

/************ Erase button callback **********************************/

 void pickbackmen_butcl(Widget iw_temp, XtPointer data, 
                                        XtPointer call_data)
 {
 /* Redisplay original images at current shifted location */
 pickdisp(TRUE,TRUE);
 }

/************ Stop windowing, resume picking particles ***************/

 void pickbackmen_buts(Widget iw_temp, XtPointer data, 
                                       XtPointer call_data)
 {
 /*  Remove  pickbackmen menu */
 XtUnmanageChild(iw_pickbackmen);

 /*  Remove button message */
 showbutx("","","",TRUE); 

 /* Remove any current boxes */
 xorboxt(imagsav, icontxor, TRUE, FALSE, TRUE, 1, 1, nsamw, nroww);

 /* Save overall averages in doc files */
 if (fpdoc4) 
    {
    fclose(fpdoc4); 
    fpdoc4 = NULL;
    pickbackave(dfil4);
    openit1 = TRUE;
    }
 if (fpdoc5) 
    {
    fclose(fpdoc5); 
    fpdoc5 = NULL;
    pickbackave(dfil5);
    openit2 = TRUE;
    }
 
 /* Free the xor context if currently valid */
 if (icontxor) XFreeGC(idispl,icontxor); 
 icontxor = 0;

 /* Reset x-hairs cursor */
 setacursor(76,-1,-1);

 /* Restart particle picker */
 pickp(FALSE);
 }


