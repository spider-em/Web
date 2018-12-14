/*$Header: /usr16/software/web/src/RCS/cat.c,v 1.23 2018/12/07 17:03:30 leith Exp $*/
/*
C++*********************************************************************
C
C  cat.c                                                      Jan 94 
C                 Undefined fpdoc                             Dec 12  
C **********************************************************************
C   AUTHOR:  ArDean Leith                                             *
C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
C=* Copyright 1985-2012  Health Research Inc.,                        *
C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
C=* Email:  spider@wadsworth.org                                      *
C=*                                                                   *
C=* This program is free software; you can redistribute it and/or     *
C=* modify it under the terms of the GNU General Public License as    *
C=* published by the Free Software Foundation; either version 2 of    *
C=* the License, or (at your option) any later version.               *
C=*                                                                   *
C=* This program is distributed in the hope that it will be useful,   *
C=* BUT WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF    *
c=* merchantability or fitness for a particular purpose.  See the GNU *
C=* General Public License for more details.                          *
C=* You should have received a copy of the GNU General Public License *
C=* along with this program. If not, see <http://www.gnu.org/licenses>*
C=*                                                                   *
C **********************************************************************
C
C    cat()
C
C    PURPOSE:     Category selecting program
C
C    PARAMETERS:  None   
C
C    CALLED BY:   catmen_buta     
C
C--*********************************************************************
*/

#include "common.h"

#define MAXCURSOR 75

#include "routines.h"
#include "mont.h"
#include "extras.h"

 /* External subroutines */

 /* Internal subroutine prototypes */
 static void   cat_pop     (Widget, XEvent *, String *, Cardinal *);

 /* Externally defined common variables */

 // Externally used variables  
 int           ncat = 3;         // No. of categories wanted (used in catmen)

 static int    openit, icat, ifil, ifilgo;

 /************************  cat   *******************************/

 void cat(void)

 {
 int   nlet;
 char  filejnk[81];    
 char outstr[81];

 /*  Refresh screen */
 expos_cb(NULL,NULL,NULL);

 if (ioper == 11 && nstack == 0)
    {
    /*  Extract first filenumber from first file name */
    filinc(filego,filejnk,&nlet,&ifilgo);
    ifilgo = ifilgo - 1;
    if (ifilgo < 0)
       {
       spouts("*** Bad file number in cat.c: "); spouti(ifilgo);
       spouts(" from: "); spout(filego);
       return;
       }
    }
 
 /*  Set initial category */
 icat  = 1;

 openit = TRUE;

 /* Set cursor to category */
 setacursor(MAXCURSOR + 1 + icat, -1,-1);

 showbutx("Assign category to file.",
          "Increment current category number.",
          "Stop this routine.",FALSE);

 actions(iw_win, "cat_pop", cat_pop, "M123");

 sprintf(outstr,"Starting category: %d",icat);
 spout(outstr);
 spout(" ");

 }


 /********************* cat_pop **********************************/

 void cat_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {
 int           ixe, ix1, iy1, ixi, iyi, irow, icol, ipane;
 char *        ctemp;
 static FILE  *fpdoc = (FILE *) NULL;
 float         dlist[5];
 char outstr[81];
 
 if (!(strcmp(*params,"M")))              /*  Mouse moved */ 
    {
    /*  Get mouse position */
    getloc(event,'M',&ix1,&iy1);

    /*  Find location relative to montage */
    ixi = ix1 - ixulmon;
    iyi = iy1 - iyulmon;

    /*  Find image number */
    if (iskip < -1)
       {
       irow  = (iyi / (nrow * -iskip + mary + names_height)) + 1;
       icol  = (ixi / (nsam * -iskip + marx)) + 1;
       }
    else if (iskip > 1)
       {
       irow  = (int)((iyi / (nrow / (float)iskip + mary + names_height)) + 1);
       icol  = (int)((ixi / (nsam / (float)iskip + marx)) + 1);
       }
    else
       {
       irow  = (iyi / (nrow  + mary + names_height)) + 1;
       icol  = (ixi / (nsam  + marx)) + 1;
       }

    ipane = (irow -1) * nprow + icol;
    if (ioper != 11  &&  ipane <= docimgtotal) 
       {ifil  = docimgnums[ipane - 1];}
    else if (ioper != 11)
       {ifil  = 0; ipane = 0;}
    else if (ioper == 11  &&  nstack == 2 && ipane <= nimage) 
       {ifil  = imagelist[ipane - 1];
       }  // Seq. stack list
    else
       {ifil  = ipane + ifilgo -1;}

    sprintf(outstr,"Row: %d  Col: %d  Pane: %d File: %d$", 
            irow, icol, ipane, ifil);
    spout(outstr);
    }

 if (!(strcmp(*params,"1")))            /*  Button 1 pushed */ 
    {
    /*  Left button, find image */

    /*  Get mouse position */
    getloc(event,'M',&ix1,&iy1);
 
    /*  Find location relative to montage */
    ixi = ix1 - ixulmon;
    iyi = iy1 - iyulmon;

    /*  Find image number */
    if (iskip < -1)
       {
       irow  = (iyi / (nrow * -iskip + mary + names_height)) + 1;
       icol  = (ixi / (nsam * -iskip + marx)) + 1;
       }
    else if (iskip > 1)
       {
       irow  = (int)((iyi / (nrow / (float)iskip + mary + names_height)) + 1);
       icol  = (int)((ixi / (nsam / (float)iskip + marx)) + 1);
       }
    else
       {
       irow  = (iyi / (nrow  + mary + names_height)) + 1;
       icol  = (ixi / (nsam  + marx)) + 1;
       }

    ipane = (irow -1) * nprow + icol;

    if (ioper != 11 &&  ipane <= docimgtotal) // From doc. file montage
       {ifil  = docimgnums[ipane - 1];}

    else if (ioper == 11  &&  nstack == 2 && ipane <= nimage) 
       {ifil  = imagelist[ipane - 1];}        // From seq. stack list
         
    else
       {ifil  = ipane + ifilgo - 1;}
    
    /* Are we trying to write on the filename of the image */
    ixe = (irow - 1) * (nrow + mary + names_height) + nrow ;
    
    /* Left button, record category if inside an image */
    if ( (ioper == 11 && nstack == 2 && 
         (ipane < 1 || ipane > nimage)) || 
          icol > nprow || iyi > ixe)
       {
       spout("*** Outside montage, unable to categorize location");

       sprintf(outstr,"Row: %d   Col: %d", irow, icol);
       spout(outstr);
       }

    else if (ipane < 1 || 
            (ioper == 11 && nstack == 0 && 
            (ifil > ndone + ifilgo-1)) || 
            (ioper != 11 && (ipane > docimgtotal)) || 
             icol > nprow || iyi > ixe ) 
       {
       spout("*** Outside montage, unable to categorize location");

       sprintf(outstr,"Row: %d   Col: %d", irow, icol);
       spout(outstr);
      }

    else
       {
       /*    Inside montage, put category in doc file */

       sprintf(outstr,"File: %d   Category: %d", ifil, icat);
       spout(outstr);
 
       /*    Write category number at this location of screen */
       ctemp = itoa(icat);
       witext(icontx, ctemp, ix1, iy1, 0, 0, -9, 2, FALSE);
       if (ctemp) free(ctemp);

       /*    Put this category in the file */
       dlist[0] = ikeyc;
       ikeyc++;
       dlist[1] = ifil;
       dlist[2] = icat;

       /*   Allow addition of more data to the same file,
            have append set to TRUE (last argument) */
       fpdoc    = savdn1(docnam, datexc, &fpdoc, dlist, 3, 
                  &openit, TRUE, TRUE);
       }
    }
 else if (!(strcmp(*params,"2")))            /* Button 2 pushed */ 
    {
    /*  Increment present category  */
    icat++;
    if (icat > ncat) icat = 1;

    /*  Make a numbered cursor */
    setacursor(MAXCURSOR + 1 + icat,  -1,-1);

    sprintf(outstr,"New category: %d",icat);
    spout(outstr);
    }


 else if (!(strcmp(*params,"3")))            /* Button 3 pushed */ 
    {
    if (docimgnums) free(docimgnums);
    docimgnums = NULL;

    /*  Close doc file */
    if (fpdoc != NULL) fclose(fpdoc);

    /*  Reset default cursor */
    setacursor(0,-1,-1);

    /*  Uninstall translations */
    XtUninstallTranslations(iw_t);

    /*  Remove message */
    showbutx("","","",TRUE);
    }
 }

