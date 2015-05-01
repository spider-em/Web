
/*$Header: /usr8/web/src/RCS/web_man.c,v 1.2 2005/10/18 16:59:54 leith Exp $*/
/*
C++*************************************************************************
C
C  web_man.c     by Lekan Odesanya
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
C  web_man 
C
C  PURPOSE:  DISPLAYS THE TEXT OF FILE FOR VIEWING ONLY
C
C--*******************************************************************
*/
#include <Xm/Text.h>
#ifdef VMS
#include <stat.h>
#else
#include <sys/stat.h>
#endif

#include "common.h"
#include "routines.h"

#define MAX_ARGS  6
#define MAX_CHARS 81


Widget          iw_manview, iw_manual = 0;
static char     filenameo[135] = "\0";

void  web_man(Widget iw_tempt, caddr_t data, caddr_t call_data )
{
 FILE        *fp = NULL;
 struct stat file_info;
 char        *buffer, *filename;
 Arg         args[MAX_ARGS];
 Widget      iw_dums, iw_pusha, iw_pushc;
 Widget      iw_rowcolv;

 if (iw_manual <= (Widget)0)
  {   /* create scrolled text viewer widget */

      iw_manual  = wid_dialog(iw_win, 0, "Web Manual", -1, -1);
       XtSetArg( args[0], XmNautoUnmanage, False ); 
       XtSetArg( args[1], XmNnoResize, True );  
       XtSetValues(iw_manual, args, 2);

      iw_rowcolv  = wid_rowcol(iw_manual, 'v', -1, -1);

    /* create box for Exiting ----------------------------------- Finished */
/*
      iw_dums = wid_stdbut(iw_rowcolv, iw_manual, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "F",
                        fin_cb, fin_cb, fin_cb, NULL);
*/

/* create the scrolled widget */
      XtSetArg( args[0], XmNrows,               35 );
      XtSetArg( args[1], XmNcolumns,            80 );
      XtSetArg( args[2], XmNeditable,           FALSE );
      XtSetArg( args[3], XmNeditMode,           XmMULTI_LINE_EDIT );
      XtSetArg( args[4], XmNscrollHorizontal,   FALSE );
   
      iw_manview = XmCreateScrolledText(iw_rowcolv, "Web Manual Scroller",
                                       args, 5);
      XtManageChild( iw_manview );

  }
      
     filename = data;

     if(!strlen(filename) ) /* no file name specified */
      {
        spout("There is no text file for the browsing request!");
        return; 
      }

     if(strcmp(filename, filenameo) != 0)  /* different file from previous */
      {
        strcpy(filenameo, filename);
       /* try opening the file  */
        if( (fp = fopen(filename,"r")) == NULL)
        {
          sprintf(filenameo,"Unable to open text file: %s",filename);
          spout(filenameo); return;
        }

         /* get file size  */
          if( stat(filename, &file_info) != 0)
            fclose( fp );
          else
           {
            if( (buffer = (char *) XtMalloc( file_info.st_size + 5)) ==
                  (char *) NULL)
             fclose( fp );
            else
             {
              if( fread(buffer, 1, file_info.st_size, fp) < file_info.st_size)
                {
                 XtFree( buffer );
                 fclose( fp );
                }
               else
                {
                  buffer[file_info.st_size - 1 ] = '\0';  /* truncate */
                  XmTextSetString( iw_manview, buffer );
                  XtFree ( buffer );
                  fclose(fp);
                }
             }  
           }

      }  /* strcmp  */

 XtManageChild(iw_manual);
}
