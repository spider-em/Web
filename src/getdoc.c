/*$Header: /usr8/web/src/RCS/getdoc.c,v 1.9 2005/10/18 16:59:47 leith Exp $*/
/*
*****************************************************************************
**    getdoc.c
**                 New doc. file format   Feb. 2004 al
**
C **********************************************************************
** *  SPIDER - MODULAR IMAGE PROCESSING SYSTEM.  AUTHOR: J.FRANK            *
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
**
**    PURPOSE:  RECOVERS DOCUMENT FILE INFO.
**
**    PARAMETERS:
**         FPDOC         DOC. FILE FILE POINTER               (SENT)
**         DOCNAM        DOC. FILE NAME                       (SENT)
**         DOCEXT        DOC. FILE EXTENSION                  (SENT)
**         MAXKEY        NUMBER OF ROWS IN DBUF               (SENT)
**         MAXREG        NUMBER OF COLUMNS IN DBUF            (SENT)
**         DBUF          BUFFER OF RETRIEVAL FROM DOC. FILE   (RETURNED)
**         LASTKEY       NUMBER OF HIGHEST KEY FOUND IN FILE  (RETURNED)
**
**    RETURNS:
**
**         LERR          ERROR FLAG (  0 IS NORMAL)           
**
**                       -5   EXCESSIVE REGISTERS FOR >= ONE KEY
**                       -4   KEY > MAXKEY FOR >= ONE KEY
**                        1   LACK SUFFICIENT REGISTER FOR >= ONE KEY
**                       20   MEMORY ALLOCATION FAILED
**                       21   NEW DOC FILE NAME TOO LONG
**                       10   NEW DOC FILE CAN NOT BE OPENED
**                       22   ERROR READING DOC FILE LINE
**                       30   MAXREGP1 TOO LARGE
**
**    TYPICAL DOC FILE LINE:
**    COL: 123456789 123456789 123456789 123456789 123456789 1234565789
**           1 4   20.070000   17.000000   17.000000   17.000000
**         KEY #REGS/LINE    VALUES ........
**
**    WHEN RETRIEVED THIS LINE IS PLACED IN DBUF STARTING AT 
**    [(KEY -1)*MAXREG].  THE VALUES PLACED THERE ARE THE NUMBER OF
**    REGISTERS RETIVED FOR THAT KEY, FOLLOWED BY VALUES OF THE
**    REGISTERS.  THUS FOR THE ABOVE SAMPLE AT DBUF[0] WE HAVE
**    2, 20, 17, 17, 17.
**
**    TO USE DBUF, COMPUTE ADDRESS OF EACH ENTRY BY USING POINTERS:
**
**    FOR J REG IN  KEY3 :    PT = DBUF + (KEY3 - 1) * MAXREG + J;
**
*****************************************************************************
*/

#include "std.h"
#include "routines.h"

/***************************  getdoc    *******************************/

 int getdoc(FILE * fpdoc, char *docnam, char * docext,  
            int maxkeys, int maxregp1, float **buf, int * lastkeygot)
 {
  char        outstr[109], docname[81], reclin[180];
  FILE      * ptrdoc;
  float     * dbuf, *lptr;
  int         regsonline, items, message1,  message2, lerr, keygot, i;
  float       regis[12];
  int         message0;

  if (maxregp1 > 12)
     {spout("*** Web can only read 12 registers from doc. file"); 
     return 30; }  

  *lastkeygot = 0;
  message0    = TRUE;
  message1    = TRUE;
  message2    = TRUE;
  lerr        = 0;     

  /*  Add datexc (name extension ) to docnam if not already there  */
  if (strlen(docnam) > (size_t) 81)
     {spouts("*** Doc. file name too long: "); spout(docnam); return 21;}
  
  strcpy(docname,docnam);

  if (docext)
     {
     if ((strlen(docname) + strlen(docext) + 1) > (size_t) 80)
        {spouts("*** Doc. file name too long: "); spout(docname); 
        return 21; }  
     strcat(strcat(docname,"."),docext);
     }

  /* open a new file if fpdoc is NULL, close it upon return */
  ptrdoc = fpdoc;
  if (!ptrdoc && ((ptrdoc = fopen(docname,"r")) == NULL))
     {spouts("*** Unable to open docfile: "); spout(docname); return 10;}
  else
     { /* already have open doc file */
     rewind(ptrdoc);
     }
 
  /* print document file name */
  spouts("Reading docfile: "); spout(docname);

  /* run time allocation of memory space for buf */
  if ((dbuf = (float *) calloc(maxkeys * maxregp1, sizeof(float)))
           == (float *) NULL)
     {spout("*** Memory allocation failed in getdoc!"); 
      fclose(ptrdoc); return 20; }


  /* read next line (until end-of-line or 180 char) from doc file */
  while ((fgets(reclin,180,ptrdoc)) != (char *) NULL)
     {
     if (ferror(ptrdoc))
        { /*  fatal error */
        spouts("*** Error reading docfile line: "); spout(reclin);
        fclose(ptrdoc); return 22;
        }

     if ((strstr(reclin,";")) == (char *) NULL)
        { /* skips any  comment line */
 
        items = sscanf(reclin,
                "%d %d %f %f %f %f %f %f %f %f %f %f %f %f",
                &keygot, &regsonline, &regis[0],&regis[1],&regis[2],
                                      &regis[3],&regis[4],&regis[5],
                                      &regis[6],&regis[7],&regis[8],
                                      &regis[9],&regis[10],&regis[11]);
        /**************
        printf(" items: %d, key: %5d regs: %d %f %f %f \n",
                 items,keygot,regsonline, 
                 regis[0],regis[1],regis[2]);
        ****************/

        if(items <= 0) 
	{   /* try old format */
        items = sscanf(reclin,"%d %1d%12f%12f%12f%12f%12f%12f",
                &keygot, &regsonline, &regis[0],&regis[1],&regis[2],
                                     &regis[3],&regis[4],&regis[5]);
        }

        if(items > 0) 
	{
	if (items < 3 || regsonline < maxregp1 - 1 && message0) 
           {
           /* not enough registers on this line, possible error */
           sprintf(outstr, 
              "*** Lacks registers for one or more key, e.g. key: %d",
                        keygot);
           spout(outstr); lerr = 1; message0 = FALSE;       
           } 

        if (keygot > maxkeys) 
           {
           if (message1)
              {
              /* this key can not be stored in array, possible error */
              sprintf(outstr,"*** Doc file key: %d > max keys: %d",
                      keygot,maxkeys);
              spout(outstr); lerr = -4; message1 = FALSE; 
              }
           keygot = 0;      
           } 

        if (regsonline > (maxregp1 - 1))  
           {  /* doc file has more registers than are retrieved */
           if (message2)
              {
              sprintf(outstr, 
              "*** Unused registers on one or more key, e.g. key: %d",
                        keygot);
              spout(outstr);  lerr = -5; message2 = FALSE;
              }
           /* even though regsonline was >= maxregp1, continue */
           regsonline = maxregp1 - 1;
           }
   
        if (keygot > 0)
           {  /* put key contents in dbuf */ 

           /* find pointer into dbuf */
           lptr = dbuf + (keygot - 1) * maxregp1;

           /* store number of registers for key in dbuf */
           *(lptr++) = regsonline;

           for (i = 0; i < regsonline; i++)
              {
              /* read the register entries */
              *(lptr++) = regis[i]; 
              }
               
           *lastkeygot = MYMAX(*lastkeygot, keygot);
           } /* (keygot > 0)                        */
	   } /* eof, blank line etc...      ML      */
        }    /* end of      if ((strstr(reclin..... */
     }       /* while (read next line of doc file)  */

  /* close the doc file  */
  if (!fpdoc) 
     {fclose(ptrdoc);}
  else
     { /* poition file at end for append */
     fseek(fpdoc,0L,SEEK_END);
     }

  /* the doc file has been successfully read */
  *buf  = dbuf;

  return lerr;
  }

