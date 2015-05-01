/*$Header: /usr8/web/src/RCS/unsdal.c,v 1.13 2007/11/21 15:07:03 leith Exp $*/
/*
*****************************************************************************
**    UNSDAL2.FOR (VAX) converted to C.
**    unsdal.c      7/1993 Mahieddine Ladjadj
**                  Removed continuation & increased regs   8/2000 al
**                  New doc. file format                 Feb. 2004 al
**                  Increased relin length               Nov. 2007 al
**
C **********************************************************************
** *  SPIDER - MODULAR IMAGE PROCESSING SYSTEM.  AUTHOR: J.FRANK            *
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
**
**
**    PURPOSE:  RECOVERS ARRAY OF REGISTERS FROM DOCUMENT FILES.  IF IKEY
**              IS <0 RECOVERS THE LINE OF REGISTERS FROM THE COMMENT
**              LINE HAVING THAT COMMENT KEY ONLY.  UP TO 2 DOC FILES
**
**
**    PARAMETERS:
**         DOCNAM        DOC. FILE NAME                       (SENT)
**         UNUSED        UNUSED                               (SENT&RET)
**         IKEY          KEY FOR LINE OF REGISTERS FOR PLIST  (SENT)
**         NLIST         NUMBER OF REGISTERS TO BE READ IN PLIST (SENT)
**         PLIST         ONE ROW OF DBUF POINTED TO BY IKEY   (RETURNED)
**         DBUF          BUFFER OF RETRIEVAL FROM DOC. FILE   (RETURNED)
**         DBUF[IKEY][0] NUMBER OF REGISTERS FOR KEY "IKEY"   (RETURNED)
**         KEY_TOT       NUMBER OF HIGHEST KEY FOUND IN FILE  (RETURNED)
**         MAXKEY        NUMBER OF ROWS IN DBUF               (SENT)
**         MAXREG        NUMBER OF COLUMNS IN DBUF            (SENT)
**
**    RETURNS:
**
**         LERR          ERROR FLAG (  0 is normal)           
**
**                       -47  blank or bad regular line
**                       -10  fewer registers than wanted for this key (incore)
**                       -9   fewer registers than wanted for this key (new read)
**                       -5   > registers than maxreg for >= one key
**                       -4   key > maxkey for >= one key

**                        1   memory allocation failed
**                        2   requested key > maxkey
**                        3   new doc file can not be opened
**                        4   error reading doc file line
**                        7   requested key not in docfile

**                       10   requested key > maxkey
**                       11   requested key not in docfile
**                       12   requested impossible key
**                       13   empty buffer for data
**                       49   error reading a register line

**
**    TYPICAL DOC FILE LINE:
**    COL: 123456789 123456789 123456789 123456789 123456789 1234565789
**           1 4   20.070000   17.379999   17.379999   17.379999
**         KEY #REGS/LINE    VALUES ........
**
**    WHEN RETRIEVED THIS LINE IS PLACED IN LINE 1 (KEY) OF DBUF AND
**    THE FIRST VALUE ON LINE ONE IS THE KEY: 1 AND THE SECOND VALUE
**    OF LINE ONE IN DBUF IS: 20.07
**
**    THE LINE OF DBUF CORRESPONDING TO IKEY IS RETURNED IN PLIST
**    IF IKEY IS NOT FOUND AN ERROR MESSAGE IS PRINTED BUT DBUF
**    IS RETURNED OK.
**
**   NOTE:
**
**   We want to allocate memory at run time for arrays.
**   The document file is read, and stored in a 1 D array.
**   the array DBUF is created as follow:.
**
**      dbuf is a 1 D array of floats
**      the size of registers will be the same for all keys: maxreg;
**      only (maxreg -1) entries will store values of registers.
**      the first entry in the row of registers will be the number of
**      registers that are used for that particular key.
**
**      example : maxreg = 6
**
**
**         key 1       key1+1                 Key1+3
**          |           |                       |
**      --------------------------------------------------------------
**         | | . . . . |N|1.2.3.-.-| | . . . . | |
**      ---------------------------------------------------------------
**                      |
**                      -- stores the number of registers used for (key1+1)
**                         N = 3 for this example.
**
**      SO COMPUTE ADDRESS OF EACH ENTRY BY USING POINTERS
**
**      for j reg in  key3 :    pt = dbuf + (key3 - 1) * maxreg + j;
**
**
*****************************************************************************
*/

#include "std.h"
#include "routines.h"

 /* external variables */
 extern char           datexc[4];         /* file extension  */

/***************************  unsdal    *******************************/

 int unsdal(char *docnam, int *unused, int ikey, float *plist,
             int nlist, float **buf, int *lastkeygot,
             int maxkeys, int maxreg)
 {
  register int i;
  char        outstr[109], docname[81], ptline[361];
  char      * reclin;
  FILE      * ptrdoc;
  float     * dbuf, *ptr, *lptr;
  int         nc, pkey, regsonline, 
              ilast, nkey, message, message1, message2, lerr;
  
  *lastkeygot = 0;
  message  = message1 = TRUE;   /* display warning only once     */
  message2 = TRUE;
  lerr     = 0;                 /* error in unsdal if lerr != 0  */

  /*  Add datexc (name extension ) to docnam if not already there  */
  strcpy(docname,docnam);
  if (strstr(docname,datexc) == 0)
      {
      if ((strlen(docname) + strlen(datexc) + 1) > (size_t) 80)
         { spout("*** Doc. file name too long:"); spout(docname); 
         return 3; }  
      strcat(strcat(docname,"."),datexc);
      }

  /* open the document file */
  if ((ptrdoc = fopen(docname,"r")) == NULL)
      { sprintf(outstr,"*** Unable to open docfile: %s",docname);
      spout(outstr); return 3; }
  
  /* print document file name */
  sprintf(outstr,"Reading docfile: %s",docname);
  spout(outstr);

  ptline[360] = '\0';
  reclin      = &ptline[0];

  /* run time allocation of memory space for buf */
  if ((dbuf= (float *) calloc(maxkeys * maxreg, sizeof(float)))
           == (float *) NULL)
     { spout("*** Memory allocation failed in unsdal"); return 1; }

  /* read next line (until end-of-line or 360 char) from doc file */
  while ((fgets(reclin,360,ptrdoc)) != (char *) NULL)
      {
      if (ferror(ptrdoc))
         {spout("*** Error reading docfile line:"); spout(reclin);
          fclose(ptrdoc); return 4;}

      if ((strstr(reclin,";")) != (char *) NULL)
         { /* skip this comment line */ ;  }

      else if (*(reclin + 1) == '-')
          { /* skip this continue line (aug 00 al) */ ;  }

      else    /******************* new line *************************/
          {
          /* normal keyed line */

          pkey = regsonline = 0;
          /* sscanf stops when it can not read but has no useful
          ** return warning. pkey & regsonline have to be read. */

          if (((sscanf(reclin,"%d %d %n", &pkey, &regsonline, &nc)) <= 0) 
               || pkey == 0 || regsonline == 0)
              {  /* empty line or end of previous line or bad line */
              if (reclin[0] != '\n' && nc > 1 && message1)
                  {
                  spout("*** Bad line in doc file"); spout(reclin);
                  lerr     = -47;  message1 = FALSE;
                  }
              }
          else if (pkey > maxkeys ) 
              {
              /* part of this line can not be stored in array */
              sprintf(outstr,
                "*** Doc file has key: %d > max keys: %d",pkey,maxkeys);
              spout(outstr); lerr = -4;       
              } 
           else                                   
              {                              
              if (regsonline > (maxreg - 1) )
                  { if (message2)
                     {
                     sprintf(outstr,
                        "*** Unused registers on one or more lines: %d",
                        regsonline-maxreg+1);
                     spout(outstr);  lerr = -5; message2 = FALSE;
                     }
                  }
    
              /* even though regsonline could be > maxreg, continue */
              ilast = MYMIN(regsonline, (maxreg - 1));

              /* lptr : pointer to 1st entry of "pkey" register list
              *lptr : number of registers read for key "pkey" */

              lptr = dbuf + (pkey - 1) * maxreg;
              ptr  = lptr;
              for (i = 1; i < ilast + 1; i ++)
                 {
                 /* read the register entries */
                 reclin += nc;
                 if ((sscanf(reclin,"%f %n", (++ptr), &nc)) <= 0) 
                    { 
                    spout("*** Error reading registers in doc file");
                    spout(reclin); fclose(ptrdoc); return 49;
                    }
                 }

              /* store number of registers for key "pkey" in dbuf[pkey][0] */
              *lptr = ilast;
               
              *lastkeygot = MYMAX(*lastkeygot, pkey);
              }
          }  /* end of new line */

          /* reset reclin pointer to the beginning of the (memory) line */
          reclin = &ptline[0];
      }  /* while (read next line of doc file) */


  /* close the doc file  and stored in dbuf  */
  fclose(ptrdoc);

  /* the doc file has been successfully read */
  *buf      = dbuf;
  if (ikey == 0) return lerr;

  /* the doc file has been read in dbuf, is desired key found?  */
  if (ikey > maxkeys)
      {
      sprintf(outstr,"*** Key: %d > maxkeys: %d",ikey, maxkeys);
      spout(outstr); return 2;
      }
    else if ( (nkey = (int) *(dbuf + (ikey-1) * maxreg)) <= 0) 
      {
      sprintf(outstr,"*** No registers found for key: %d",nkey);
      spout(outstr); return 7;
      }

  ilast = MYMIN(nkey, nlist);
  ptr   = dbuf + (ikey-1) * maxreg;
  for (i=0; i < ilast;  plist[i++] = *(++ptr));

  if ( ilast < nlist)  return -9;

  return lerr;
  }
