
/*$Header: /usr16/software/web/src/RCS/mag.c,v 1.8 2018/12/07 17:03:32 leith Exp $*/
/*
C++*************************************************************************
C
C mag.c -- Created                            July 92
C          Rewrite                            Nov  92                                                                    
C **********************************************************************
C *  AUTHOR: YANHONG LI                                                      *
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
C    mag(...)
C
C    PARAMETERS:   iw_temp, data, call_data (usual callback data)
C
C    PURPOSE: MAGNIFICATION
C
C    CALLED BY:    web_com
C
C--********************************************************************
*/


 /* Common variables */
#include "common.h"
#include "routines.h"

 /* Internal function  prototypes */
 static void mag_pop(Widget, XEvent *, String *, Cardinal *);

 /* Internal file variables */
 static GC     icontxx;
 static int    first=1, erase=0;
 static int    lasx=0, lasy=0;
 static int    lasmagx=0, lasmagy=0;
 static int    ix3=0, iy3=0;
 static Pixmap iglassav=0;

/*****************************  mag  *********************************/

 void mag(Widget iw_temp, void * data, void * call_data)

 {
 /* Make a xor graphics context (icontxx)	*/
 icontxx = setxor(icontx);

 /* Open a message window with the following strings	*/
 showbutx( "Set temporary glass location.",
           "Set permanent glass location.",
	   "Stop this routine.", FALSE);

 /* Set the action from right, left, and center buttons */
 actions(iw_win, "mag_pop", mag_pop, "M123L");
	
 /* Turn on Xor writing in icontxx	*/
 icontxx = setxor( icontx );
 }


/***********************  mag_pop  **************************/

static int       ix1, iy1;        /* Mouse position */
static int       ix2, iy2;
static Boolean   leftdown = FALSE;

void mag_pop(Widget iw_t, XEvent *event, String *params,
             Cardinal *num_params)
    {

    /* Get mouse position ix1,iy1. */
    getloc( event, 'M', &ix1, &iy1 );
	
	
    if ( !(strcmp(*params,"M")) && !leftdown )
        /* Cursor moved. Erase and draw a new box */
        {
        xorbox(iwin, icontxx, FALSE, ix1, iy1, magsizx, magsizy);    
        }
	
    else if (!(strcmp(*params, "1")))
        /* Left button pushed. 	*/
        /* Erase the old box first, draw a new one */
        {
    	xorbox(iwin, icontxx, 1, ix1, iy1,magsizx, magsizy);
    	if (erase)
      	    XCopyArea(idispl, imagsav, iwin, icontx, ix3,iy3,
		     lasmagx, lasmagy, ix3, iy3);

	/* Show magnified area	*/
    	glass(iwin, iwin, ix1, iy1, FALSE, FALSE, &ix2, &iy2, TRUE);

        leftdown = TRUE;

        }
    else if (!(strcmp(*params,"L")))    /* Left button up */
        {
	/* Redraw the box	*/
    	xorbox(iwin, icontxx, 1, ix1, iy1, magsizx, magsizy);
#ifndef WEB_TRUE
    	xorbox(iwin, icontxx, 1, ix1, iy1, magsizx, magsizy);
#endif

  	/* Remember the last magnification varibles */
    	erase    = 1;
    	first    = 0;
    	lasmagx  = magsizx * magfac;
    	lasmagy  = magsizy * magfac;
    	ix3      = ix2;
    	iy3      = iy2;

        leftdown = FALSE;
        }

    else if (!(strcmp(*params, "2")))
        /* Center button pushed	*/
        /* Make permanent magnification	*/
        {
    	xorbox(iwin, icontxx, 1, ix1, iy1, magsizx, magsizy);
    	if (erase)
      	    XCopyArea(idispl, imagsav, iwin, icontx, ix3,iy3,
                      lasmagx, lasmagy, ix3, iy3);

	/* Show magnified area	*/
    	glass(iwin, iwin, ix1, iy1, 1, FALSE, &ix2, &iy2, TRUE);

	/* Redraw the box	*/
   	xorbox(iwin, icontxx, 1, ix1, iy1,magsizx, magsizy);
    	xorbox(iwin, icontxx, 1, ix1, iy1,magsizx, magsizy);

	/* Remember the last magnification varibles	*/
    	erase    = 0;
    	first    = FALSE;
        leftdown = FALSE;
        }
	
	
    else if (!(strcmp(*params, "3")))
        /* Right button pushed	        */
        /* Stop this function call	*/
        {
    	if (erase)
	    /* Replace the last temporary magnified area	*/
      	    XCopyArea(idispl, imagsav, iwin, icontx, ix3,iy3,
			lasmagx, lasmagy,ix3, iy3);

    	if (!first)
	    /* Erase old box	*/
      	    xorbox(iwin, icontxx, 1, ix1, iy1, magsizx, magsizy );
	
        neednewmap = TRUE;
        leftdown   = FALSE;

    	XFlush( idispl );
    	XtUninstallTranslations(iw_win);
   	glass(iwin, iwin, ix1, iy1, TRUE, 1, &ix2, &iy2, TRUE);

	/* Release GC	*/
    	XFreeGC(idispl, icontxx );
	
    	showbutx(NULL, NULL, NULL, 1);
    	}
    } 

/***********************  glass  **************************/

void glass(Window iwsrc, Window iwdest, int ixuls, int iyuls,
	    int perm, int finished, int *ixuld, int *iyuld, int finddest)
   {
    unsigned int 	ixsiz, iysiz;
    int	 		depth;
    int			i, j;
    int			ix, iy, ixn, iyn;
    int			lmagx, lmagy;

    if (finished )
        {
      	if (iglassav > 0 )
      	    XFreePixmap( idispl, iglassav );
      	iglassav =0;
      	return;
        }

    /* Find the size of magnified image	*/
    ixsiz = magsizx * magfac;
    lmagx = magsizx;
    if (ixsiz > iwidex)
        {
	ixsiz = iwidex;
	lmagx = ixsiz/magfac;
        }

    iysiz = magsizy * magfac;
    lmagy = magsizy;
    if (iysiz > ihighx)
        {
	iysiz = ihighx;
	lmagy = iysiz/magfac;
        }

    if ((ixsiz != lasx || iysiz != lasy) && iglassav != 0 )
        {
 	/* Mag glass has changed, set flag to create new mag pixmap	*/
    	XFreePixmap( idispl, iglassav );
    	iglassav = 0;
        }

    if (iglassav < 1 )
        {
	/* Need to create offscreen pixmap for mag glass	*/
	/* Create an offscreen backing store for the mag. Glass	*/
    	iglassav = XCreatePixmap(idispl,iwdest,ixsiz,iysiz,idepth);

	/* Remember destination window ( magnified ) size	*/
    	lasx = ixsiz;
    	lasy = iysiz;
        }

    if(finddest)
        {  /* Find upper left corner of destination area	*/
        *ixuld = ixuls - ( ixsiz - lmagx )/2;
        if ( *ixuld < 0 )
    	    *ixuld = 0;
        if (( *ixuld + ixsiz - 1 ) >iwidex )
    	    *ixuld = iwidex - ixsiz;

        *iyuld = iyuls - ( iysiz - lmagy )/2;
        if ( *iyuld < 0 )
            *iyuld = 0;
        if (( *iyuld + iysiz - 1 ) >iwidex )
    	*iyuld = iwidex - iysiz;
        }

    /* Copy columns from backing store with magfac duplication	*/
    for ( i=0; i<lmagy; i++ )
            {
    	for ( j=0; j<magfac; j++ )
    	   {
           iy = iyuls + i;
           iyn = i*magfac + j;
           XCopyArea(idispl,iwsrc,iglassav,icontx, ixuls,iy,
		lmagx,1,0,iyn);
    	   }
       }

    /* Copy top rows from glass into glass for magfac duplication*/
    ixn = ixsiz;
    for ( i=lmagx; i>0; i-- )
       {
    	for ( j=0; j<magfac; j++ )
           {
           ix = i-1;
           ixn = ixn - 1;
           XCopyArea(idispl,iglassav,iglassav,icontx, 
		ix,0,1,iysiz,ixn,0 );
           }
       }	
	
    XFlush(idispl);

    /* Copy the glass into the on_screen window	*/
    XCopyArea(idispl,iglassav,iwdest,icontx,0,0, 
		ixsiz,iysiz,*ixuld,*iyuld );
    neednewmap = TRUE;

    if ( perm )
       {
       /* Copy the glass into the off_screen window	*/
       XCopyArea(idispl,iglassav,imagsav,icontx,0,0,
		ixsiz,iysiz,*ixuld,*iyuld );
       neednewmap = TRUE;

       }
    return;
}

