/*$Header: /usr16/software/web/src/RCS/savdn1.c,v 1.21 2018/12/07 17:03:34 leith Exp $*/

/*
C++*************************************************************************
C
C savdn1.c                                                Nov. 1992 al
C                   Allowed keys > 9999                   Jan. 2004 al
C                   New ' ' doc file format               Feb. 2004 al
C                   New comment routine                   Jun. 2011 al
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
C    savdn1(docnam,datexc,fileptr,dlist,nlist,openit,append)
C
C    PURPOSE:    Save parameters in document file 
C
C    PARAMETERS:   docnam  Name for doc file (without extension!)
c                  datexc  File extension
c                  fileptr Filepointer
c                  dlist   Array containing floating pt. numbers to be saved.
c                  nlist   Number of elements in array dlist
c                          first element is key, others are registers
c                  openit  True = first call (opens file)
c                  append  True = open/append, false = open/discard old
C
C    RETURNS:      fileptr or null
C
C--*******************************************************************
*/

#include "std.h"
#include "routines.h"

/*************************  savdn1  ****************************/


 FILE * savdn1(char * docnam, char * datexct, FILE ** fpt,
               float * dlist, int nlist, int * openit, int append,
               int dateit)
 {
 FILE     * fp;
 time_t     now;
 char     * datentime, * docname;
 int        icount, ikey, i;
 size_t     slen;

 if (*openit) 
    {
    /* first call of this routine */

    /* add datexct to docnam if not already there */
    slen = strlen(docnam) + strlen(datexct) + (size_t) 2;

    if (!(docname = (char *) malloc(slen * sizeof(char))))
       {
       spout("*** Unable to allocate memory in savdn1!");
       return (FILE *) 0;
       }  

    strcpy(docname,docnam);
    if (strstr(docname,datexct) != 
               (docname + strlen(docname) - strlen(datexct)))
       {   /* add extension now */
       strcat(docname,".");  strcat(docname,datexct);
       }

    if (append )
       {fp = fopen(docname,"a+");}
    else
       { fp = fopen(docname,"w+"); }


    if (fp ==  (FILE *) NULL)
       {    /* file can not be opened */
       spouts("*** Unable to open: "); spout(docname);  
       free(docname); return fp;
       }

    if (dateit) 
       {  /* put date, etc in file now */
       /* find current date and time */
       now       = time(NULL);
       datentime = ctime(&now);
       /* should these pointers be freed later ? */

       fprintf(fp," ; %3.3s/%3.3s  %s   %s",
                datexct,datexct, docname, datentime);
       }

    *openit = FALSE;
    if (docname) free(docname);
    *fpt = fp;
    }
 else if (*fpt)
    {
    /* use file pointer sent from caller */
    fp = *fpt;
    }
 else
    {
    spout("*** Pgm error: NULL fp sent to savdn1 without openit");
    return (FILE *) 0;
    }


 ikey = dlist[0] + 0.5;
 if (nlist > 0 && ikey > 99999) 
    {
    spout("*** Document file keys must be < 100000");
    return fp;
    }

 if (nlist < 0)
    {
    spout("*** Continuation lines not supported by savdn1.c");
    return (FILE *) 0;
    }

 else if (nlist > 0 && nlist <= 7) 
    {
    icount = nlist - 1;

    /* ikey is the key number which identifies each line       */
    /* icount is the number of values (registers) in that line */

    if (ikey < 10000 && icount < 10)
       { fprintf(fp," %4.4d %1.1d",ikey, icount); }
    else
       { fprintf(fp,"%d %d",ikey, icount); }

    for (i = 1; i <= icount; i++)
       { fprintf(fp," %12.6g",dlist[i]);  }
    fprintf(fp,"\n");
    }

 fflush(fp);
 return fp;
 }




 FILE * savdnc(char * docnam, char * datexct, FILE ** fpt,
               float * dlist, int nlist, int * openit, int append,
               int dateit, char * comment)
 {
 FILE     * fp;
 time_t     now;
 char     * datentime, * docname;
 int        icount, ikey, i;
 size_t     slen;

 if (*openit) 
    {
    /* First call of this routine */

    /* Add datexct to docnam if not already there */
    slen = strlen(docnam) + strlen(datexct) + (size_t) 2;

    if (!(docname = (char *) malloc(slen * sizeof(char))))
       {
       spout("*** Unable to allocate memory in savdn1!");
       return (FILE *) 0;
       }  

    strcpy(docname,docnam);
    if (strstr(docname,datexct) != 
               (docname + strlen(docname) - strlen(datexct)))
       {   /* Add extension now */
       strcat(docname,".");  strcat(docname,datexct);
       }

    if (append )
       {fp = fopen(docname,"a+");}
    else
       { fp = fopen(docname,"w+"); }


    if (fp ==  (FILE *) NULL)
       {    /* File can not be opened */
       spouts("*** Unable to open: "); spout(docname);  
       free(docname); return fp;
       }

    if (dateit) 
       {  /* Put date, etc in file now */
       /* find current date and time */
       now       = time(NULL);
       datentime = ctime(&now);
       /* Should these pointers be freed later ? */

       fprintf(fp," ; %3.3s/%3.3s  %s   %s",
                datexct,datexct, docname, datentime);
       }

    *openit = FALSE;
    if (docname) free(docname);
    *fpt = fp;
    }
 else if (*fpt)
    {
    /* Use file pointer sent from caller */
    fp = *fpt;
    }
 else
    {
    spout("*** Pgm error: NULL fp sent to savdn1 without openit");
    return (FILE *) 0;
    }

  if (comment)
     { // Add a comment line
     fprintf(fp," ; %s \n",comment);
     }

 ikey = dlist[0] + 0.5;
 if (nlist > 0 && ikey > 99999) 
    {
    spout("*** Document file keys must be < 100000");
    return fp;
    }

 if (nlist < 0)
    {
    spout("*** Continuation lines not supported by savdn1.c");
    return (FILE *) 0;
    }

 else if (nlist > 0) 
    {
    icount = nlist - 1;

    /* ikey is the key number which identifies each line       */
    /* icount is the number of values (registers) in that line */

    if (ikey < 10000 && icount < 10)
       { fprintf(fp," %4.4d %1.1d",ikey, icount); }
    else
       { fprintf(fp,"%d %d",ikey, icount); }

    for (i = 1; i <= icount; i++)
       { fprintf(fp," %12.6g",dlist[i]);  }
    fprintf(fp,"\n");
    }

 fflush(fp);

 return fp;
 }

