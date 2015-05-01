
/*$Header: /usr8/web/src/RCS/getcorner.c,v 1.3 2005/10/18 16:59:40 leith Exp $*/
/*
C++************************************************************************
C
C GETCORNER
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C GETCORNER
C
C PURPOSE:         GET LOCATION OF CORNER 
C
C PARAMETERS:      
C                  IXULN,IYULN  UPPER LEFT CORNER FOR THIS IMAGE
C
C CALLED BY:       MONTMEN3   MONTMEN2
C
C***********************************************************************
*/

#include "std.h"
#include "x.h"
#include <Xm/Text.h>

 extern int            placed;            /* image position sent flag    */
 extern unsigned int   ihighx, iwidex;    /* display window dimensions   */
 extern int            nsam,nrow,nslice;  /* image size                  */
 extern int            marlef,martop;     /* left and top window margins */
 extern int            ntop,nbot,nrit;    /* top, bottem & rit side      */
 extern int            marx,mary;         /* x & y image margins         */


 /********************** getcorner  *********************************/

 int getcorner(Widget iw_ixul, Widget iw_iyul, int *ixuln, int *iyuln)

 {   
 char *string;

 placed = FALSE;

 if (iw_ixul > (Widget)0)
    {
    string = XmTextGetString(iw_ixul);

    if (strcmp(string,"*") != 0)
       {  /* find value for ixuln */
       *ixuln = atoi(string);
       placed = TRUE;
       }
    XtFree(string);
    }

 if (iw_iyul > (Widget)0)
    {  /* find value for iyuln */
    string = XmTextGetString(iw_iyul);

    if (strcmp(string,"*") != 0)
      {     /* value is specified for iyuln, use it */
      *iyuln = atoi(string);

      if (placed == FALSE)
         {
         /* ixuln was *, so must set ixuln first */
         if ((nsam + marx + nrit) <= iwidex)
            /* enough width for image in this row of images */
            *ixuln = nrit + marx + 1;
         else
            /* put image at left margin */
            *ixuln = marlef + marx + 1;
         placed = TRUE;
         }
      }
    else if (placed == TRUE)
      /* ixuln is set but iyuln is *, so must default iyuln */
      {
      if ((ntop + nrow + mary) <= ihighx)
         /* enough length for image in this row of images */ 
         *iyuln = ntop;
      else
         /* not enough space on this screen, put image at top */
         *iyuln = martop;
      }
    XtFree(string);
    }

 return TRUE;
 }
 
 /*****     if (*iyuln < 1 || *iyuln > nrow) return FALSE;  ***/

