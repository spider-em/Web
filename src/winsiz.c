
/*$Header: /usr16/software/web/src/RCS/winsiz.c,v 1.17 2018/12/07 17:06:44 leith Exp $*/

/*
 C++********************************************************************
 C
 C WINSIZ -- CREATED JAN 90 al
 C           CONVERTED TO C -- AUG 92 al
 C *********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=*                                                                   *
 C=* HEALTH RESEARCH INCORPORATED (HRI),                               *   
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=*                                                                   *
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
 C winsiz
 C
 C PURPOSE:       Returns window coordinates and size
 C
 C VARIABLES:     iwintyp       window type ifs
 c                ioper         operation
 C
 C CALLED BY:     web_com  
 C
 C
 C                     / winsiz_pop --> winsiz_pop2 --> winsel --> movearea
 C winsiz --> winsiz1 -- wid_posiz  --> winmov      /           
 C                     \ wid_posiz  --> winsiz3    /
 C
 C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "posiz.h"
#include "extras.h"

 /* Internal function prototypes */
 static void winsiz_pop (Widget, XEvent *, String *, Cardinal *);
 static void winsiz_pop2(Widget, XEvent *, String *, Cardinal *);
 static void winsiz3    (void);
 static void winsiz4    (void);

 /* Externally defined variables */

 /* Internally or externally used variables */
 static GC       icontxx;
 int             iwintyp;
 static Widget   iw_posiz = (Widget) 0;

 /********************************* winsiz ***************************/

 void winsiz(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 char ctemp[3];

 if (data != NULL) 
    {
    strncpy(ctemp, (char*) data, 1); ctemp[1]= '\0'; 
    ioper   = atoi(&ctemp[0]);

    strncpy(ctemp, (char*)data, 2); ctemp[2]= '\0';
    iwintyp = atoi(&ctemp[1]);
    }

 if (iwintyp < 1 || iwintyp > 3)
    {
    printf(
        "in winsiz, iwintyp: %d ioper: %d ctemp: %c%c Notify Dean!\n",
        iwintyp,ioper,ctemp[0],ctemp[1]);
    } 
 winsiz1();
 }



 /***************************** winsiz1 ******************************/

 void winsiz1(void)

 {

 /* Make a xor graphics context */
 icontxx = setxor(icontx);

 /* Set default color for box in xor GC */
 wicolor(icontxx,colorgo+icolorov);

 if (iwintyp == 1) 
    {   /* Interactive windowing by rubber box */

    /* Set upper left corner cursor */
    setacursor(67,-1,-1);

    actions(iw_win, "winsiz_pop", winsiz_pop, "13");

    //printf(" winsiz1 1\n");  // used to DELAY AFTER this

    showbutx("Sets upper left window corner.",
             NULL, 
             "Stop this routine.", FALSE);

    //printf(" winsiz1 2\n"); // used to DELAY Befor this


    XFlush(idispl);
    }

 else if (iwintyp == 2) 
    {    /* Want fixed size window */
    if (nsamw > 0 && nroww > 0)
       iw_posiz = wid_posiz(iw_win, iw_posiz, ixulw, iyulw, nsamw, nroww,
               'S', winsiz4);
    else
       iw_posiz = wid_posiz(iw_win, iw_posiz, 
                            ixulw, iyulw, icols_posiz, irows_posiz,
                            'S', winsiz4);
    }

 else if (iwintyp == 3) 
    {    /* Want specified coordinate window */
    iw_posiz = wid_posiz(iw_win, iw_posiz, 
                         ixulw, iyulw, nsamw, nroww,
                         'B', winsiz3);
    }
 else
    {
    printf("iwintyp: %d in winsiz1 Notify Dean!\n", iwintyp);
    } 
 
 return;
 }


 /******************* Set interactive UL corner *********************/

 void winsiz_pop(Widget iw_t, XEvent *event, String *params,
                  Cardinal *num_params)
 {

 /* Uninstall translations */
 XtUninstallTranslations(iw_t);
   
 if (!(strcmp(*params,"1")))            /* Button 1 pushed */
    {
    // printf(" winsiz_pop 12        \n");

    getloc(event,'B',&ixulw,&iyulw);

    // For "record file'. If cursor not in the last image
    if( ioper == 4 && (ixulw < ixul || iyulw < iyul || 
	ixulw > (ixul + nsam *iskip) || iyulw > (iyul + nrow *iskip)))
        {
 	spout(" The cursor is not in the last image.");
        actions(iw_win, "winsiz_pop", winsiz_pop, "13");
	return;
	}

    /* Set lower right corner cursor */
    setacursor(7,-1,-1);

    /* Set up action procedure to set lower right corner */
    actions(iw_win, "winsiz_pop2", winsiz_pop2, "M123");

    //printf(" winsiz_pop 13\n");// DELAYS HERE

    showbutx("Set lower right window corner.",
             "Change upper right window corner.", 
             "Stop this routine.", FALSE);
    //printf(" winsiz_pop 14\n");// DELAYS HERE
    }

 else if (!(strcmp(*params,"3")))               
    {                                       /* Button 3 pushed */
    /* Set default cursor */
    setacursor(0,-1,-1);

    if (icontxx > (GC)0) XFreeGC(idispl,icontxx);

    /* Unmanage information box */
    showbutx("","","",TRUE);
    }
 }

 /**************** Set interactive lower right corner ***************/

 void winsiz_pop2(Widget iw_t, XEvent *event, String *params,
                  Cardinal *num_params)
 {
 int ix,iy;

   
 if (!(strcmp(*params,"M")))            /* cursor moved */
    {
    /* Get mouse position */
    getloc(event,'M',&ix,&iy);

    nsamw = ix - ixulw + 1;
    nroww = iy - iyulw + 1;

    /* Mouse moved so draw new box */
    xorbox(iwin,icontxx,FALSE,ixulw,iyulw,nsamw,nroww);
    }

 else if (!(strcmp(*params,"1")))       /* Button 1 pushed */
    {
    getloc(event,'B',&ix,&iy);

    nsamw = ix - ixulw + 1;
    nroww = iy - iyulw + 1;

    /* Erase old box and set source window to this location */
    xorbox(iwin,icontxx,TRUE,ixulw,iyulw,nsamw,nroww);

    /* If 'record the last file', keep the window */
    if (ioper == 4)	
    	xorbox(imagsav,icontx,TRUE,ixulw,iyulw,nsamw,nroww);

    /* Set default cursor */
    setacursor(0,-1,-1);

    /* Unmanage information box */
    showbutx("","","",TRUE);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);

    expos_cb(NULL,NULL,NULL);// Shortens delay!!

    /* Proceed to specific operation on window */
    winsel();
    return;
    }


 else if (!(strcmp(*params,"2")))       /* Button 2 pushed */
    {   /* Resize window */

    /* Erase old box and set source window to this location */
    xorbox(iwin,icontxx,TRUE,ixulw,iyulw,nsamw,nroww);

    /* Set default cursor */
    setacursor(0,-1,-1);

    /* Unmanage information box */
    showbutx("","","",TRUE);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);

    winsiz1();
    return;
    }

 else if (!(strcmp(*params,"3")))       /* Button 3 pushed */
    {   /* Abort window */

    /* Erase old box and set source window to this location */
    xorbox(iwin,icontxx,TRUE,ixulw,iyulw,nsamw,nroww);

    /* Set default cursor */
    setacursor(0,-1,-1);

    /* Unmanage information box */
    showbutx("","","",TRUE);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);

    XFreeGC(idispl,icontxx);

    /******** if (docit)  CLOSE(LUNDOCW) */
    return;
    }

 }

 /****************** Set size window    *************************/


 void winsiz4(void)
 {
 nsamw = icols_posiz;
 nroww = irows_posiz;

 winmov();
 }

 /****************** Set coordinate window    *******************/


 void winsiz3(void)
 {
 ixulw = ix_posiz;
 iyulw = iy_posiz;
 nsamw = icols_posiz;
 nroww = irows_posiz;

 winsel();
 }
