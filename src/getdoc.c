/*$Header: /usr8/web/src/RCS/getdoc.c,v 1.11 2015/09/18 16:22:28 leith Exp $*/
/*
 C**********************************************************************
 C    getdoc.c
 C            New doc. file format                        Feb  2004 al
 C            No complain if can not open new file        Jun  2015 al
 C            Cosmetic                                    Sep  2015 al
 C
 C**********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C *********************************************************************
 C
 C    PURPOSE:  RECOVERS DOCUMENT FILE INFO.
 C
 C    PARAMETERS:
 C         FPDOC         DOC. FILE FILE POINTER               (SENT)
 C         DOCNAM        DOC. FILE NAME                       (SENT)
 C         DOCEXT        DOC. FILE EXTENSION                  (SENT)
 C         MAXKEY        NUMBER OF ROWS IN DBUF               (SENT)
 C         MAXREG        NUMBER OF COLUMNS IN DBUF            (SENT)
 C         DBUF          BUFFER OF RETRIEVAL FROM DOC. FILE   (RETURNED)
 C         LASTKEY       NUMBER OF HIGHEST KEY FOUND IN FILE  (RETURNED)
 C         WANTFILEMSG   WANT VERBOSE FILE INFO               (SENT)
 C
 C    RETURNS:
 C         LERR          ERROR FLAG (  0 IS NORMAL)           
 C
 C                       -5   EXCESSIVE REGISTERS FOR >= ONE KEY
 C                       -4   KEY > MAXKEY FOR >= ONE KEY
 C                        1   LACK SUFFICIENT REGISTER FOR >= ONE KEY
 C                       20   MEMORY ALLOCATION FAILED
 C                       21   NEW DOC FILE NAME TOO LONG
 C                       10   NEW DOC FILE CAN NOT BE OPENED
 C                       22   ERROR READING DOC FILE LINE
 C                       30   MAXREGP1 TOO LARGE
 C
 C    TYPICAL DOC FILE LINE:
 C    COL: 123456789 123456789 123456789 123456789 123456789 1234565789
 C           1 4   20.070000   17.000000   17.000000   17.000000
 C         KEY #REGS/LINE    VALUES ........
 C
 C    WHEN RETRIEVED THIS LINE IS PLACED IN DBUF STARTING AT 
 C    [(KEY -1)*MAXREG].  THE VALUES PLACED THERE ARE THE NUMBER OF
 C    REGISTERS RETIVED FOR THAT KEY, FOLLOWED BY VALUES OF THE
 C    REGISTERS.  THUS FOR THE ABOVE SAMPLE AT DBUF[0] WE HAVE
 C    2, 20, 17, 17, 17.
 C
 C    TO USE DBUF, COMPUTE ADDRESS OF EACH ENTRY BY USING POINTERS:
 C
 C    FOR J REG IN  KEY3 :    PT = DBUF + (KEY3 - 1) * MAXREG + J;
 C
 C***************************************************************************
*/

#include "std.h"
#include "routines.h"

 /***************************  getdoc_f    *******************************/

 int getdoc_f(FILE * fpdoc, char *docnam, char * docext,  
            int maxkeys, int maxregp1, float **buf, int * lastkeygot,
            int wantfilemsg)
  {
  char        outstr[109], docname[81], reclin[180];
  FILE  *     ptrdoc;
  float *     dbuf;
  float *     lptr;
  int         regsonline, items, message1,  message2, lerr, keygot, i;
  float       regis[12];
  int         message0;

  if (maxregp1 > 12)
     {spout("*** Web can only read 12 registers from doc. file"); return 30; }  

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
        {spouts("*** Doc. file name too long: "); spout(docname);  return 21; }  
     strcat(strcat(docname,"."),docext);
     }

  /* Open a new file if fpdoc is NULL, close it upon return */
  ptrdoc = fpdoc;
  if (!ptrdoc && ((ptrdoc = fopen(docname,"r")) == NULL))
     { 
     if (wantfilemsg) // 2015
        { spouts("*** Unable to open docfile: "); spout(docname); }
     return 10;
     }
  else
     { /* Already have open doc file */
     rewind(ptrdoc);
     }
 
  /* Print document file name */
  if (wantfilemsg) // 2015
     { spouts("Reading docfile: "); spout(docname);}

  /* Run time allocation of memory space for buf */
  if ((dbuf = (float *) calloc(maxkeys * maxregp1, sizeof(float)))
           == (float *) NULL)
     {spout("*** Memory allocation failed in getdoc!"); 
      fclose(ptrdoc); return 20; }


  /* Read next line (until end-of-line or 180 char) from doc file */
  while ((fgets(reclin,180,ptrdoc)) != (char *) NULL)
     {
     if (ferror(ptrdoc))
        { /*  Fatal error */
        spouts("*** Error reading docfile line: "); spout(reclin);
        fclose(ptrdoc); return 22;
        }

     if ((strstr(reclin,";")) == (char *) NULL)
        { /* Skips any  comment line */
 
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

        if (items <= 0) 
	{   /* Try old format */
        items = sscanf(reclin,"%d %1d%12f%12f%12f%12f%12f%12f",
                &keygot, &regsonline, &regis[0],&regis[1],&regis[2],
                                     &regis[3],&regis[4],&regis[5]);
        }

        if(items > 0) 
	{
	if (items < 3 || regsonline < maxregp1 - 1 && message0) 
           {
           /* Not enough registers on this line, possible error */
           sprintf(outstr, 
              "*** Lacks registers for one or more key, e.g. key: %d",
                        keygot);
           spout(outstr); lerr = 1; message0 = FALSE;       
           } 

        if (keygot > maxkeys) 
           {
           if (message1)
              {
              /* This key can not be stored in array, possible error */
              sprintf(outstr,"*** Doc file key: %d > max keys: %d",
                      keygot,maxkeys);
              spout(outstr); lerr = -4; message1 = FALSE; 
              }
           keygot = 0;      
           } 

        if (regsonline > (maxregp1 - 1))  
           {  /* Doc file has more registers than are retrieved */
           if (message2)
              {
              sprintf(outstr, 
              "*** Unused registers on one or more key, e.g. key: %d",
                        keygot);
              spout(outstr);  lerr = -5; message2 = FALSE;
              }
           /* Even though regsonline was >= maxregp1, continue */
           regsonline = maxregp1 - 1;
           }
   
        if (keygot > 0)
           {  /* Put key contents in dbuf */ 

           /* Find pointer into dbuf */
           lptr = dbuf + (keygot - 1) * maxregp1;

           /* store number of registers for key in dbuf */
           *(lptr++) = regsonline;

           for (i = 0; i < regsonline; i++)
              {
              /* Read the register entries */
              *(lptr++) = regis[i]; 
              }
               
           *lastkeygot = MYMAX(*lastkeygot, keygot);
           } /* (keygot > 0)                        */
	   } /* eof, blank line etc...      ML      */
        }    /* End of      if ((strstr(reclin..... */
     }       /* While (read next line of doc file)  */

  /* Close the doc file  */
  if (!fpdoc) 
     {fclose(ptrdoc);}
  else
     { /* Position file at end for append */
     fseek(fpdoc,0L,SEEK_END);
     }

  /* The doc file has been successfully read */
  *buf  = dbuf;

  return lerr;
  }


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

  /* Open a new file if fpdoc is NULL, close it upon return */
  ptrdoc = fpdoc;
  if (!ptrdoc && ((ptrdoc = fopen(docname,"r")) == NULL))
     { return 10;}  // 2015
     //{spouts("*** Unable to open docfile: "); spout(docname); return 10;}
  else
     { /* Already have open doc file */
     rewind(ptrdoc);
     }
 
  /* Print document file name */
  spouts("Reading docfile: "); spout(docname);

  /* run time allocation of memory space for buf */
  if ((dbuf = (float *) calloc(maxkeys * maxregp1, sizeof(float)))
           == (float *) NULL)
     {spout("*** Memory allocation failed in getdoc!"); 
      fclose(ptrdoc); return 20; }


  /* Read next line (until end-of-line or 180 char) from doc file */
  while ((fgets(reclin,180,ptrdoc)) != (char *) NULL)
     {
     if (ferror(ptrdoc))
        { /*  Fatal error */
        spouts("*** Error reading docfile line: "); spout(reclin);
        fclose(ptrdoc); return 22;
        }

     if ((strstr(reclin,";")) == (char *) NULL)
        { /* Skips any  comment line */
 
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
	{   /* Try old format */
        items = sscanf(reclin,"%d %1d%12f%12f%12f%12f%12f%12f",
                &keygot, &regsonline, &regis[0],&regis[1],&regis[2],
                                     &regis[3],&regis[4],&regis[5]);
        }

        if(items > 0) 
	{
	if (items < 3 || regsonline < maxregp1 - 1 && message0) 
           {
           /* Not enough registers on this line, possible error */
           sprintf(outstr, 
              "*** Lacks registers for one or more key, e.g. key: %d",
                        keygot);
           spout(outstr); lerr = 1; message0 = FALSE;       
           } 

        if (keygot > maxkeys) 
           {
           if (message1)
              {
              /* This key can not be stored in array, possible error */
              sprintf(outstr,"*** Doc file key: %d > max keys: %d",
                      keygot,maxkeys);
              spout(outstr); lerr = -4; message1 = FALSE; 
              }
           keygot = 0;      
           } 

        if (regsonline > (maxregp1 - 1))  
           {  /* Doc file has more registers than are retrieved */
           if (message2)
              {
              sprintf(outstr, 
              "*** Unused registers on one or more key, e.g. key: %d",
                        keygot);
              spout(outstr);  lerr = -5; message2 = FALSE;
              }
           /* Even though regsonline was >= maxregp1, continue */
           regsonline = maxregp1 - 1;
           }
   
        if (keygot > 0)
           {  /* Put key contents in dbuf */ 

           /* Find pointer into dbuf */
           lptr = dbuf + (keygot - 1) * maxregp1;

           /* store number of registers for key in dbuf */
           *(lptr++) = regsonline;

           for (i = 0; i < regsonline; i++)
              {
              /* Read the register entries */
              *(lptr++) = regis[i]; 
              }
               
           *lastkeygot = MYMAX(*lastkeygot, keygot);
           } /* (keygot > 0)                        */
	   } /* eof, blank line etc...      ML      */
        }    /* End of      if ((strstr(reclin..... */
     }       /* While (read next line of doc file)  */

  /* Close the doc file  */
  if (!fpdoc) 
     {fclose(ptrdoc);}
  else
     { /* Position file at end for append */
     fseek(fpdoc,0L,SEEK_END);
     }

  /* Doc file has been successfully read */
  *buf  = dbuf;

  return lerr;
  }





