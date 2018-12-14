
/*
C++************************************************************************
C
C setline
C
C **********************************************************************
C *  AUTHOR:  Lekan Odesanya                                                *
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
C    setline
C
C    PURPOSE:         set the line style and width
C
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /************************   setline   ******************************/

 void setline( Widget widget, void * data, void * call_data )
 {

 int           request;
 XGCValues     gcv;


   request = atoi(data);

   if( request > 2 ) /*  want to change line width  */
    {
      gcv.line_width = request - 3;
      XChangeGC( idispl, icontx, GCLineWidth, &gcv );
    }
   else             /*  want to change line style  */
    {
      switch( request )
       {
         case 0:   gcv.line_style = LineSolid;        break;
         case 1:   gcv.line_style = LineOnOffDash;    break;
         case 2:   gcv.line_style = LineDoubleDash;   break;
       }

      XChangeGC( idispl, icontx, GCLineStyle, &gcv );
    }

}
