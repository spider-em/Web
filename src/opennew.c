
/*$Header: /usr16/software/web/src/RCS/opennew.c,v 1.22 2018/12/07 17:03:33 leith Exp $*/
/*
 * opennew.c
 *
 *
 *************************************************************************
 * SPIDER - MODULAR IMAGE PROCESSING SYSTEM.  AUTHOR: J.FRANK 
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
 **************************************************************************
 *
 * opennew(FILNAM,NSAM,NROW,NSLICE,IFORM,DISP)
 *
 * PURPOSE:     TO CREATE A NEW SPIDER DATA FILE FOR RANDOM ACCESS IO
 *
 * PARAMETERS:     FILNAM     CHARACTER ARRAY, CONTAINING FILE NAME 
 *      LUN        LOGICAL UNIT NUMBER TO BE ASSIGNED TO FILNAM.
 *      NSAM,NROW  DIMENSIONS OF FILE - INPUT OR OUTPUT PARAMETERS
 *                 OF OPEN, DEPENDING ON DISP.
 *      DISP       CHARACTER CONTAINING ONE OF THE FOLLOWING 
 *                 DISPOSITION SPECIFICATIONS:
 *
 *                  U -         IT IS NOT KNOWN IF THE FILE EXISTS.  
 *                              IF IT DOES NOT EXIST, NON-ZERO NSAM AND
 *                              NROW VALUES HAVE TO BE AVAILABLE; THE
 *                              FILE IS CREATED WITH THESE DIMENSIONS.
 *                              IF THE FILE ALREADY EXISTS, IT WILL BE 
 *                              OVERWRITTEN. 
 *
 *        NF                    ERROR RETURN FLAG.
 *                              NF = 0    NORMAL RETURN 
 *                              NF = 1    ERROR RETURN
 *
 *	 NSLICE	                NUMBER OF PLANES, EACH CONTAINING NSAM*NROW 
 *                              ELEMENTS. 
 *
 *    VARIABLES:   IFORM  = FILE TYPE SPECIFIER. 
 *                        = +3    3-D IMAGE
 *	                  = +1    2-D IMAGE
 *                        = -1    2-D FOURIER TRANSFORM
 *                        = -3    3-D FOURIER TRANSFORM
 *                        = +8    8 BIT BLACK AND WHITE IMAGE
 *                        = 11    8 BIT COLOR IMAGE
 *
 *   NOTE:         opennew does not set a title in the file nor does
 *                 it set angles in the file (unlike fortran version)
 *
 *********************************************************************/

#include "files.h"
#include "routines.h"

 /* external file prototypes */


/*************************  opennew  ****************************/


 FILEDATA *opennew(char * filnam, int nsam, int nrow, int nslice, 
                    int iform, char disp[]) 
 { 

 /* header union for equivalencing floating point and character buffers */
 union  BUFFER 
        {
        float hbuf[256];
        char  cbuf[1024];
        } header;


 FILEDATA * fileptr;
 time_t     now;
 char     * datentime ,type[4], output[80];
 FILE     * fp;
 int        nlabel,reclen,headrec,headbyt;
 int        k,imami,iangle,k1;
 
 if (strcmp(disp,"u") != 0 && strcmp(disp,"n") != 0) 
    { spout(" *** Invalid disp in opennew"); return 0; }

 if (strcmp(disp,"n") == 0 && (fp = fopen(filnam,"rb")) != NULL)
    {spout(" *** New file already exists"); fclose(fp); return 0;}

 /* open the file */
 if ((fp = fopen(filnam,"w+b")) == NULL) 
    { spout(" *** Unable to open new file"); return 0; } 

 if (nsam == 0 || nrow == 0) 
     /*   for new files, non-zero nsam, nrow need to be supplied  */
     {spout(" ***  Need size info to open file."); return 0; }     

 else if (nslice == 0) 
     {
     /*   new files need a non zero slice number for file format flag */
     spout(" Warning:  nslice set to 1.");
     nslice = 1;
     }

 else if (nslice < 0) 
     {
     /*   want to create an short label format file  */
     spout(" Warning:  short label files no longer  supported.");
     nslice = -nslice;
     }
       
 /*adjust number of header records to have >= 256*4 bytes in header */

 if (iform < 6)
     /*   standard SPIDER floating point files */
     reclen = nsam * 4;

 else if (iform == 8)
     /*   for 8 bit files     */
     reclen = nsam ;

 else if (iform == 10)
     /*   pixel by pixel 8 bit color image file  */
     reclen = nsam * 3;

 else if (iform == 11)
     /*   color by color 8 bit color image file  */
     reclen = nsam * 3;

 else if (iform == 12)
     /*   8 bit runlength color file */
     reclen = nsam * 4;

 else if(iform == 16)
     /*   for 16 bit integer files */
     reclen = nsam * 2;

 else 
     { spout(" *** UNKNOWN IFORM IN OPENNEW"); return 0; }

 /* allocate space for a spider file structure */
 fileptr = (FILEDATA *) malloc(sizeof(FILEDATA));

 if (fileptr == (FILEDATA *) 0) 
     { spout(" *** Unable to allocate memory in opennew."); 
        printf(" *** Unable to allocate memory in opennew.\n");
        return 0; }

 fileptr -> fp = fp;
 
 /* set number of header records and length of header */
 headrec = 1024 / reclen;
 if ((1024 % reclen) != 0) headrec = headrec + 1;
 headbyt = headrec * reclen;

 header.hbuf[0] = -nslice;
 header.hbuf[1] = nrow;
 header.hbuf[2] = headbyt;
 nlabel         = 0;
 header.hbuf[3] = nlabel;
 header.hbuf[4] = iform;
 imami          = 0;
 header.hbuf[5] = imami;

 /*     zero unused buf variables */
 for (k = 5; k < 212 ;k++)
          header.hbuf[k] = 0.0;

 header.hbuf[11] = nsam;
 header.hbuf[12] = headrec;

 /* angles are not set in opennew */
 iangle = 0;
 header.hbuf[13] = iangle;
 header.hbuf[14] = 0.0;
 header.hbuf[15] = 0.0;
 header.hbuf[16] = 0.0;

 header.hbuf[21] = headbyt;
 header.hbuf[22] = reclen;

 fileptr -> offset  = 0;
 fileptr -> nsam    = nsam;
 fileptr -> nrow    = nrow;
 fileptr -> nslice  = nslice;
 fileptr -> nlabel  = nlabel;
 fileptr -> iform   = iform;
 fileptr -> flip    = 0;
 fileptr -> imami   = imami;
 fileptr -> fmin_spi    = 0.0;
 fileptr -> fmax_spi    = 0.0;
 fileptr -> av      = 0.0;
 fileptr -> sig     = 0.0;
 fileptr -> headrec = headrec;
 fileptr -> headbyt = headbyt;
 fileptr -> reclen  = reclen;
 fileptr -> iangle  = iangle;

 /* find current date and time */
 now       = time(0);
 datentime = ctime(&now);

 /* set date for file header */
 strncpy(&header.cbuf[844],datentime + 8,  2);
 header.cbuf[846] = '-';
 strncpy(&header.cbuf[847],datentime + 4,  3);
 header.cbuf[850] = '-';
 strncpy(&header.cbuf[851],datentime + 20, 4);
 header.cbuf[855] = ' ';

 /* set date in current file information */
 strncpy(fileptr -> date, &header.cbuf[844],11);
 fileptr -> date[11] = '\0';

 /* set time in file header */
 strncpy(&header.cbuf[856],datentime + 11, 8);
 header.cbuf[864] = '\0';

 /* set time in current file information */
 strncpy(fileptr -> time, &header.cbuf[856],9);

 /* initialize ximage pointer */
 fileptr -> ximage = NULL;

 /* write header to open file */

 if (wrtlin(fileptr,header.hbuf,256,0) != TRUE)
    { closefile(fileptr); return 0; }

 if ((fileptr -> buf = (float *) malloc(nsam * sizeof(float))) == 
     (float *) 0) 
     {     
     spout(" *** Unable to allocate read/write buffer in opennew.");
     closefile(fileptr); return 0;
     }
 
 /* print file opening information */
 if (iform == -1)
     strcpy(type,"f ");
 else if (iform == -11)
     strcpy(type,"fo");
 else if (iform == -12)
     strcpy(type,"fe");
 else if (iform == -21)
     strcpy(type,"3fe");
 else if (iform == -22)
     strcpy(type,"3fe");
 else if (iform == 0)
     strcpy(type,"d ");
 else if (iform == 3)
     strcpy(type,"r3");
 else if (iform == 8)
     strcpy(type,"8 ");
 else if (iform == 16)
     strcpy(type,"16");
 else
     strcpy(type,"r ");

 /* print filename */
 sprintf(output," %s",filnam);
 spout(output);
     
 /* print file parameters */
 if (nslice > 1)
     {
     sprintf(output," (%s) %d %d %d  New: %s At %s  Header bytes: %d",
           type, nsam, nrow, nslice, fileptr -> date, fileptr -> time, headbyt);
     spout(output);
     }
 else
     {
     sprintf(output," (%s) %d %d     New: %s At %s  Header bytes: %d",
           type, nsam, nrow, fileptr -> date, fileptr -> time, headbyt);
     spout(output);
     }

 return fileptr;

}
