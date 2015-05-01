
/***************************************************************************
* powermen.c  CREATED FROM wid_powbut.for BY  Mahieddine Ladjadj          5/24/94
*
* WID_POWBUT.FOR  -- FEB 93
*
* **********************************************************************
* *  AUTHOR: ArDean Leith                                                  *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2007  Health Research Inc.                     *
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
* **********************************************************************
*
*    powermen(void)
*
*    PURPOSE:      Moves a power spectrum window around in an image
*
*    CALLED BY:    winsel
*
************************************************************************
*/

#include "common.h"
#include "routines.h"

void  powerbut_pop (Widget, XEvent *, String *, Cardinal *);

static int      erase, chksiz;
static GC       icontxxt;
extern int      iwintyp;
static Boolean  showing = TRUE;

/****************************  powermen  ******************************/

void powermen(void)
  {

  /* Xor graphics context */
  icontxxt = setxor(icontx);

  /* Set default color in temporary GC for box */
  wicolor(icontxxt,colorgo+icolorov);

  chksiz = TRUE;
  erase = FALSE;

  if (iwintyp == 3) 
     {  /* Fixed window size and location */

     /* Put a box */
     xorbox(iwin,icontxxt, TRUE, ixulw, iyulw, nsamw, nroww);

     /* show power spectrum */
     wid_power(TRUE,&chksiz); 
     XFlush(idispl);
     return;
     }

  showbutx("Set temporary power spectrum.",
           "Set permanent power spectrum.",
           "Stop this routine.",FALSE);

  /* Setup movement on pointer motion, button press etc... */
  actions(iw_win,"powerbut_pop",powerbut_pop,"M123L");

  XFlush(idispl);
  }

/***************************  powerbut_pop  ********************************/
void powerbut_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {
 static int      ixp, iyp, ixb, iyb;
 int             ixs, iys;

 if (!(strcmp(*params, "M")))
     {  /**************************************** Mouse movement only */
     getloc(event,'M',&ixs,&iys);
 
     if (!showing)
        {
        /* Box moved, erase old box (if necessary)  */
        xorbox(iwin,icontxxt, FALSE, ixs, iys, nsamw, nroww);
        }
     }

 else if (!(strcmp(*params, "1")))
     {                          /******************   Button 1 pushed */
     getloc(event,'B',&ixs,&iys);

     /* Show temporary power spectrum  */
     /* Erase old box first and put a new one */
     xorbox(iwin,icontxxt, TRUE, ixs, iys, nsamw, nroww);

     /* Replace last spectrum area before continuing */
     if (erase)
         XCopyArea(idispl,imagsav,iwin,icontx,
                   ixp,iyp,nsamw,nroww,ixp,iyp);

     /* Show power spectrum */
     ixulw = ixs; iyulw = iys;
     wid_power(FALSE,&chksiz); 

     /* Remember last spectrum position */
     ixp     = ixulw;  iyp = iyulw;
     erase   = TRUE;
     showing = TRUE; 
     }

 else if (!(strcmp(*params, "L")))
     {                          /******************   Button 1 up */
     showing = FALSE; 

     /* Erase old box (if necessary)  */
     xorbox(iwin,icontxxt, FALSE, ixs, iys, nsamw, nroww);
     }

  else if (!(strcmp(*params, "2")))
     {  /***  Make a permanent power spectrum.        Button 2 pushed */
     getloc(event,'B',&ixs,&iys);
   
     /* Show temporary power spectrum  */
     /* Erase old box first and put a new one */
     xorbox(iwin,icontxxt, TRUE, ixs, iys, nsamw, nroww);

     /* Replace last spectrum area before continuing */
     if (erase)
        XCopyArea(idispl,imagsav,iwin,icontx,
                   ixp,iyp,nsamw,nroww,ixp,iyp);

     /* show power spectrum */
     ixulw = ixs;  iyulw = iys;

     wid_power(TRUE,&chksiz);

     /* Remember last spectrum position */
     ixp = ixulw;  iyp = iyulw;
     erase   = FALSE;
     showing = FALSE;
     }

  else if (!(strcmp(*params, "3")))
     {  /***  Stop this routine.                     Button 3 pushed */
     /* Erase old box first */
     xorbox(iwin,icontxxt, TRUE, ixb, iyb, nsamw, nroww);

     /* Replace last spectrum area before continuing */
     if (erase)
        XCopyArea(idispl,imagsav,iwin,icontx,
                   ixp,iyp,nsamw,nroww,ixp,iyp);  

     XFlush(idispl);

     showing = FALSE;

     /* Uninstall translations and free GC */
     XtUninstallTranslations(iw_t);
     XFreeGC(idispl,icontxxt);

     /* Unmanage information box */
     showbutx("","","",TRUE);
     }
  }
