
/*$Header: /usr16/software/web/src/RCS/spoutfile.c,v 1.9 2018/12/07 17:03:34 leith Exp $*/

/*
 ***********************************************************************
 *
 * spoutfile.c
 *
 ***********************************************************************
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
 ***********************************************************************
 *
 * spoutfile(useit)
 * 
 * PURPOSE:     uses a results file to trace web ouput
 *
 * PARAMETERS:   useit     T/F flag to write to  file
 *
 * RETURNS:      T/F  if successful 
 * 
 ***********************************************************************
 */

#include "std.h"
#include "common.h"
#include "routines.h"
#include "extras.h"

 /* external common variables */

 /* global variables used elsewhere */
 FILE   * resfp = NULL;

 /**************************** spoutfile*******************************/

 int spoutfile (int useit)
   {
   static char     resname[41];
   char            * datentime;
   time_t          now;

   if (!useit)
      {
      trace_web = FALSE;
      }
   else
      {
      if (!resfp)
         {    /* results file not available for restart */

         /* add datexc to web_results.  */
         strcpy(resname,"web_results.");
         strcat(resname,datexc);

         /* open for appending to end of file */
         resfp = fopen(resname,"a");

         if ( resfp == (FILE *) NULL)
            {
            spouts("*** Unable to open a results file: "); spout(resname); 
            trace_web = FALSE;
            return FALSE;
            }

         /* find current date and time */
         now       = time(NULL);
         datentime = ctime(&now);
         /* should this pointer be freed later ? */

         /* set time in file header */
         fprintf(resfp," ; %3.3s  %s  %s", datexc, datentime, resname);
         }
      trace_web = TRUE;
      }

  return TRUE;
  }


 
