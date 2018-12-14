/*$Header: /usr16/software/web/src/RCS/setbgfg.c,v 1.2 2018/12/07 17:03:34 leith Exp $*/

/*
 ***********************************************************************
 *
 * Setbgfg.c        New                           Feb 2011 ArDean Leith
 *
 ***********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2011  Health Research Inc.                     *
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
 ***********************************************************************
 *
 * Setbgfg(Arg * args, int maxarg, int * n)
 * 
 * PURPOSE:     Initialize bg and fg menu arguments
 *
 * PARAMETERS:    
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"
 
/***********************  Setbgfg  ************************/

 void Setbgfg(Arg * args, int maxarg, int * n)
 { 

#if defined(WEB_BGFG) || defined(WEB_BGFGO)

 if (*n < maxarg)
    {
    XtSetArg(args[*n], XmNbackground, icolorxmb);
    *n = *n + 1; 
    }
 else
    { printf("*** Arg overflow pgm. error: %d \n",*n); }
#endif
 }

