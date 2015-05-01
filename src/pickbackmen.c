
/*$Header: /usr8/web/src/RCS/pickbackmen.c,v 1.10 2011/09/23 19:46:07 leith Exp $*/

/*
C++*************************************************************************
C
C  pickbackmen.c          May 93 al
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
C    PICKBACKMEN()
C
C    PURPOSE:    displays background picking selecting menu
C
C    PARAMETERS: none   
C
C    CALLed BY:  pickmen pickback    
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Text.h>

 /* external function prototypes */
 void          pickbackave (char *);

 /* internal function prototypes */
 void          pickbackmen_buts (Widget, caddr_t, caddr_t);
 void          pickbackmen_buta (Widget, caddr_t, caddr_t);
 void          pickbackmen_butcl(Widget, caddr_t, caddr_t);
  
 /* externally defined global variables */
 extern char       dfil4[12], dfil5[12];
 extern int        numb, openitl, openitr;
 extern FILE*      fpdoc4, * fpdoc5;

 /* internally defined global variables */
 int           firstback;    /* set by fitdoc */
 GC            icontxor = 0;

 /* internal file scope variables */
 static Widget        iw_pickbackmen=(Widget)0, iw_winkey, iw_winsiz;
 
 /***********************   pickbackmen   ********************************/

 void pickbackmen(void)

 {
 static Widget  iw_rowcolv;
 Widget  iw_pushs, iw_pushc, iw_pusha, iw_dums; 
 Widget  iw_buter;

 char    cval[10];

 if (iw_pickbackmen <= (Widget)0)
    {   /* create  background windowing menu first */

    iw_pickbackmen = wid_dialog(iw_win, 0, "Background windowing menu", -1, -1);
    iw_rowcolv = wid_rowcol(iw_pickbackmen, 'v', -1, -1);

    /* create text box for particle key  */
    nsamw = 15; nroww = 15;
    sprintf(cval,"%2d",nsamw);
    iw_winsiz   = wid_textboxb(iw_rowcolv,0,"Window size:",cval,3);

    /* create text box for particle key  */
    sprintf(cval,"%4d",numb);
    iw_winkey   = wid_textboxb(iw_rowcolv,0,"Window number:",
                               cval,4);

    /* create pushbutton for erasing locations and numbers */
    iw_buter = wid_pushg(iw_rowcolv, 0, "Erase windows",
                  pickbackmen_butcl, NULL, -1,-1);

    /* create box for apply  */
    iw_dums = wid_stdbut(iw_rowcolv, iw_pickbackmen, 
                         &iw_pushs, &iw_pushc, &iw_pusha, "SCA",
                         pickbackmen_buts,  pickbackmen_buts,
                         pickbackmen_buta, NULL);
    }
 else
    {
    /* create text box for particle key  */
    sprintf(cval,"%4d",numb);
    iw_winkey   = wid_textboxb(iw_rowcolv,iw_winkey,
                                "Window number:",cval,4);
    }
 XtManageChild(iw_pickbackmen);
 }


/*********** accept button callback **********************************/

 void pickbackmen_buta(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
 char * string;

 /* find window size */
 string = XmTextGetString(iw_winsiz);
 sscanf(string,"%d",&nsamw);
 if(string) free(string);

 if (nsamw < 0) 
    { spout("*** Window size must be >= 0"); return; }
 nroww = nsamw;

 /* find next window key */
 string = XmTextGetString(iw_winkey);
 sscanf(string,"%d",&numb);
 if(string) free(string);

 printf(" pickbackmen: window key numb %d\n",numb);
 
 if (numb < 0) 
    { spout("*** Window number must be >= 0"); return; }

 /*  remove  pickback menu */
 XtUnmanageChild(iw_pickbackmen);

 /* set default cursor */
 setacursor(0, -1, -1);

 /* create an xor graphics context if not currently valid */
 if (!icontxor) icontxor = setxor(icontx);

  /* set default color in xor GC */
  wicolor(icontxor,colorgo+icolorov);

 /* start window background picking */
 pickback(firstback);

 }

/************ erase button callback **********************************/

 void pickbackmen_butcl(Widget iw_temp, caddr_t data, caddr_t call_data)
 {

 /*  remove  pickbackmen menu 
 XtUnmanageChild(iw_pickbackmen);  */

 /* redisplay original images at current shifted location */
 pickdisp(TRUE,TRUE);
 }

/************ stop windowing, continue picking callback ***************/

 void pickbackmen_buts(Widget iw_temp, caddr_t data, caddr_t call_data)
 {

 /*  remove  pickbackmen menu */
 XtUnmanageChild(iw_pickbackmen);

 if (fpdoc4) 
    {
     fclose(fpdoc4); 
     pickbackave(dfil4);
     openitl = TRUE;
    }
 if (fpdoc5) 
    {
     fclose(fpdoc5); 
     pickbackave(dfil5);
     openitr = TRUE;
    }
 
 /* free the xor context if currently valid */
 if (icontxor) XFreeGC(idispl,icontxor); 
 icontxor = 0;

/* set x-hairs cursor */
 setacursor(76,-1,-1);

 /* restart the particle picker again */
 pickp(FALSE);
 }


