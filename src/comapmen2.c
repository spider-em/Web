/*
************************************************************************
** comapmen2.c		
**
** WID_COMAPMEN2.FOR ported to UNIX     8/93    Mahieddine Ladjadj
** Altered menu                         Nov 07  ArDean Leith
**
C **********************************************************************
** *   AUTHOR:  Mahieddine Ladjadj                                     *
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
C **********************************************************************
*/

#include <Xm/ToggleBG.h>
#include "common.h"
#include "std.h"
#include "x.h"
#include "routines.h" 
#include "comap.h"
  
 /* Internal functions */
 static void     comapmen2_but  (Widget, XtPointer, XtPointer);
 static void     ovrlap_cb      (Widget, XtPointer, XtPointer);

 /* External variables */
 Pixmap        msksav;
 Widget        iw_comapmen2;
 int           ovrlap;

/********************** comapmen2 ***********************************/

void comapmen2(void)
  {
  char   cval[15];
  Widget iw_rowcol, iw_p1, iw_p2, iw_p3, iw_p4, iw_p5, iw_p6, iw_p7;

  if (iw_comapmen2 == (Widget) 0) 
      { /* Comap menu widget does not exist, create it */

      /* Create dialog box */
      iw_comapmen2 = wid_dialog(iw_win,(Widget) 0,"corr_map options",20,20);
      iw_rowcol   = wid_rowcol(iw_comapmen2,'v',-1,-1);

      /* Create push button for saving images in doc file */
      iw_p1 = wid_pushg(iw_rowcol, (Widget)0,
                   "Save Images in Doc File", comapmen3,"0",0,0);  

      /* Create push button for displaying average image */
      iw_p2 = wid_pushg(iw_rowcol, (Widget)0,
                   " Display Averaged Image",comapmen2_but,"2",0,0);  

      /* Create push button for saving averaged image */
      iw_p3 = wid_pushg(iw_rowcol, (Widget)0,
                   "   Store averaged Image", comapmen5,"0",0,0);  

      /* Create push button for clearing mask */
      iw_p4 = wid_pushg(iw_rowcol, (Widget)0,
                   "            New Masking",comapmen2_but,"4",0,0);  

      /* Create push button for clearing mask */
      iw_p6 = wid_pushg(iw_rowcol, (Widget)0,
                   "       Continue Masking",comapmen2_but,"5",0,0);  

      /* create toggle box for overlaping image & mask */
      ovrlap = TRUE;
      iw_p7 = wid_toggleg(iw_rowcol,(Widget) 0,
                              "Overlap Avg. Im. & Mask",
                              ovrlap,ovrlap_cb,(XtPointer) &ovrlap,-1,-1);
      
      /* create push button for stop */
      iw_p5 = wid_pushg(iw_rowcol, (Widget)0,
                   "           Stop Mapping",comapmen2_buts,"0",0,0);
      }

  XtManageChild(iw_comapmen2);
  
  /* Enable all planes for drawing or replacing exposure */
  //wisetplanes(idispl,icontx,-1);  // may 2011 al

  /* Unassign buttons */
  XtUninstallTranslations(iw_win);

  /* Remove instructional message */
  showbutx("","","",TRUE);
  
  return;
} /* End of comapmen2 function */


// ***************************** comapmen2_but ************************* 

/* Display button */

void comapmen2_but(Widget iw_t, XtPointer data, XtPointer callpar)
  {  
  char *oper;
  int it;
  char ops[] = "12345";

  /* Remove menu widget */
  XtUnmanageChild(iw_comapmen2);

  /* Refresh screen */
  expos_cb(NULL,NULL,NULL);

  /* itag=2 calculates and displays averaged image
     itag=4 do not clear masks, continue masking
     itag=5 clears masks then continue masking */

  oper = data;
  it   = (int) (strchr(ops,*oper) - ops) + 1;
  comapsav(it);

  }  /* End of comapmen2_but */


// *********************** overlap_cb ************  Overlap toggle  button 

 void  ovrlap_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
   {
   ovrlap = XmToggleButtonGadgetGetState(iw_temp); 
   }


// ***************************** comapmen2_buts ************** Stop button 

void comapmen2_buts(Widget iw_t, XtPointer itag, XtPointer callpar)
  {      
  /* Remove menu widget */
  XtUnmanageChild(iw_comapmen2);

  /* Flush buffer to reset the luts */
  XFlush(idispl);
 
  /* Free space used in comap.c */
  free(bufx);
  free(bufy);
  free(nkey);
  if (icontmx) XFreeGC(idispl,icontmx);
  if (msksav) 
      { XFreePixmap( idispl, msksav ); msksav = 0; }
  
  } /* End of comapmen2_buts */
