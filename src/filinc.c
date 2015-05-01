
/*$Header: /usr8/web/src/RCS/filinc.c,v 1.3 2005/10/18 16:59:53 leith Exp $*/
/*
C++*************************************************************************
C
C  FILINC
C             CONVERTED TO C -- JULY 92 al
C                                                             
C **********************************************************************
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
C    FILINC(FILOLD,FILNEW,NLET,NUMFIL)
C
C    PURPOSE:       INCREMENTS AN OLD FILENAME IF POSSIBLE
C
C    PARAMETERS:    FILOLD       OLD FILE NAME                 READ
C                   FILNEW       NEW FILE NAME                 WRITE
C                   NLET         NUMBER CHAR IN FILNEW         WRITE
C                   NUMFIL       NUMBER USED FOR FILE          WRITE
C
C
C   RETURNS         IRTFLG       ERROR FLAG                    
C                                2 = NORMAL
C                                1 = EATS INTO OLD FILENAME
C                                0 = FILENAME LACKED INCREMENTALBE NUMBER
C
C    CALLED BY:     MONT2
C
C--*********************************************************************
*/

#include "files.h"


 /************************  filinc  ************************************/

 int filinc(char * filold, char * filnew, int * nlet, int * numfil)

 { 
 int    ival, errflg, carry1;
 char   ctemp;
 char * cptr, * pext;

 strcpy(filnew,filold);

 /* skip extension at end of filename (if any) */
 if ((pext = strrchr(filnew,'.')) != (char *) NULL)
     *nlet  = (int) (pext - filnew);
 else
     *nlet = strlen(filnew);  


 if (*nlet  <= 0)
    {
    /* empty filename, can not increment name */
    strcpy(filnew,NULL);
    *nlet   = 0;
    *numfil = 0;
    return 0;
    }


 /* CREATE NEW FILE NAME BY INCREMENTING NUMBER AT END OF OLD FILE NAME */

 /* since we are incrementing there is a carry of one to last digit */
 carry1 = TRUE;

 /* set no error flag for return */
 errflg = 2;

 /* go backwards thru filename looking at each character */

 for (cptr = filnew + *(nlet) -1; cptr>=filnew; cptr--)
   {
   ctemp = *cptr;

   if (isdigit(ctemp)) 
      {    /* the character is a digit */
      if (carry1) 
         {
         ctemp++;
         carry1 = FALSE;
         if (ctemp > '9') 
            {
            carry1 = TRUE;
            ctemp  = '0';
            }   /* endif ctemp */
         *cptr  = ctemp;
         }    /* endif carry1 */
      ival  = atoi(cptr);
      }    /* endif isdigit */

   else 
      {  /* character is not a digit */
      if (carry1)
         {
         ctemp  = '1';
         *cptr  = ctemp;
         errflg = 1;
         carry1 = FALSE;
         }   /* endif carry1 */
      break;
      }   /*  endif else */
   }   /* endfor */


 /* if there is still a carry of one then can not increment filename */
 if (carry1)
    errflg = 0;

 /* return with new file number */
 *numfil = ival;

 return errflg;
 }
