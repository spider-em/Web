/*$Header: /usr16/software/web/src/RCS/toggle2_cb.c,v 1.3 2018/12/07 17:03:34 leith Exp $*/
/*
C+**************************************************************************
C
C toggle2_cb.c                  DEC 92
C
C **********************************************************************
C    AUTHOR:  Mahieddine Ladjadj
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
C toggle2_cb            runs when a toggle is pushed to
C                      change value of toggled variable
C
C PARAMETERS:    
C
C--*********************************************************************
*/

#include <Xm/ToggleBG.h>
#include "routines.h"

 /********************  toggle2_cb   ****************************/

 void  toggle2_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
   *(int *)data = XmToggleButtonGadgetGetState(iw_temp); 
 }

