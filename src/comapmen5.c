/*
*****************************************************************************
** comapmen5.c		8/93
**
** Purpose:   Display menu for image data in a file.
**
**
** WID_COMAPMEN5.FOR transported to UNIX
**
C **********************************************************************
** *   AUTHOR:  Mahieddine Ladjadj                                          *
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
*****************************************************************************
*/

#include "common.h"
#include "std.h"
#include "x.h"
#include <Xm/Text.h>
#include "routines.h"

  extern char   datexc[4];         /* file extension           */

  static Widget iw_im = (Widget)0;
  char imnamm[81] = "avgimg001"; 
  extern Widget iw_comapmen2;
  void     comapmen5_buta  (Widget iw_t, caddr_t itag, caddr_t call_data);
                                                                         
/********************** comapmen5 ***********************************/

void comapmen5(Widget iw_t, caddr_t itag, caddr_t callpar)
  {
  /* create a text box for image name input */
  iw_im = wid_text(iw_win, iw_im, "New Average Image", imnamm,
                   *comapmen5_buta,(caddr_t) NULL,20,20);
  }
                                                               
/********************** comapmen5_buta ***********************************/

void comapmen5_buta(Widget iw_t, caddr_t itag, caddr_t call_data)
  {
  XmSelectionBoxCallbackStruct * cbs;

  cbs = (XmSelectionBoxCallbackStruct *) call_data;
  strcpy(imnamm,extractstr(cbs -> value));

  if (strlen(imnamm) == 0)
      { spout(" *** WARNING: no doc file name: imnamm "); return; }
 
  /* remove menu widget */
  XtUnmanageChild(iw_comapmen2);
 
  /* refresh screen */
  expos_cb(NULL,NULL,NULL);

  /* start comap doc saving and averaging routine */
  comapsav(3);

  }
