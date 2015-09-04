
/*$Header: /usr8/web/src/RCS/openold.c,v 1.50 2015/09/01 17:53:27 leith Exp $*/

/***********************************************************************
 *                                                                     *
 * openold.c   MRC support                      Nov 2012  ArDean Leith *
 *             64 bit support                   Sep 2015  ArDean Leith *
 *                                                                     *
 ***********************************************************************
 C   AUTHOR: A. LEITH                                                  *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright 1985-2013  Health Research Inc.,                        *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but without any warranty; without even the implied warranty of    *
 c=* merchantability or fitness for a particular purpose.  See the GNU *
 C=* General Public License for more details.                          *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program. If not, see <http://www.gnu.org/licenses>*
 C=*                                                                   *
 ***********************************************************************
 *
 * openold(filnam, nsamptr, nrowptr, nsliceptr, iformptr, disp)
 *
 * PURPOSE:     Open an existing data file for random access I/O
 *
 * PARAMETERS:         
 *    filnam          File name                      (Sent)
 *    nsamptr,nrowptr Dimensions of file             (Returned)
 *    nsliceptr       No. of slices in file          (Returned)
 *    iformptr        File type specifier.           (Unused)
 *                      = +3    3-D SPIDER image
 *	                = +1    2-D SPIDER image
 *                      = -1    2-D SPIDER Fourier transform
 *                      = -3    3-D SPIDER Fourier transform
 *                      = +8    8  bit raw black and white image
 *                      = 11    8  bit color image
 *                      = 108   8  bit MRC byte    image signed -128..127
 *                      = 116   16 bit MRC integer image
 *                      = 132   32 bit MRC real    image
 *    disp            Character specifying readonly or not  (sent)
 *                      = 'o' - Read/write. 
 *                      = 'q' - Read/write, no error msg if non-existing 
 *                      = 'r' - File exists, open readonly.
 *
 * RETURNS:        filedata * or null if error reading file
 *
 *********************************************************************/

#include "files.h"
#include "routines.h"

// Byte flipping 
#define CONVERT_4( A, B )                        \
             (A) =                               \
            ((unsigned int)(B) >> 24) |          \
            ((unsigned int)(B) << 24) |          \
           (((unsigned int)(B) >> 8) & 0xff00) | \
           (((unsigned int)(B) << 8) & 0xff0000)

 /* Externally defined global variables */
 extern int   nsam8, nrow8, nslice8;
 extern int   nstack, inuse, maxim, imginstack;

 /* Internal function prototypes */
 int          opensmall8      (char *, FILEDATA  *, int *,   int * ,  int * , 
                               int ,   int , int , int * );
 int          is_spider_header(float[]);
 int          is_mrc_header   (float[]);
 int          IsLittleEndian  ();
 int          IsBigEndian     ();
 float        ReverseFloat    ( const float inFloat );


 /* Internal file scope variables */
 int needrawsize = TRUE;  /* Force it to run image size widget for raw images on first call */

/*********************** openold ************************************/

 FILEDATA * openold(char filnam[], int *nsamptr, int *nrowptr, 
                   int *nsliceptr, int *iformptr, char disp[])
 { 
 union  BUFFER 
        {
        float hbuf[256];
        char  cbuf[1024];
        int   ibuf[256];
        } header;

 FILEDATA  *fileptr;

 char           type[4], output[160], ctemp[10], typefl[1];
 FILE *         fp;
 int            nsam,nrow,nslice,iform,headbyt,headrec;
 int            lentitle, k1, locat, lennum, iflag;
 char *         cptr;
 unsigned int * uiptr;
 int            flip, flipped, spiderkind, mrckind;
 float          hbuf[256];

 /* Retrieve stacked image number, if present */ 
 nstack     = 0;
 imginstack = 0;
 inuse      = 0;
 maxim      = 0;
 strcpy(ctemp,"          ");

 locat = strcspn(filnam,"@");
 if (locat < strlen(filnam))
    { /* Has @ in filnam denoting image stack */
    lennum     = strspn(&filnam[locat+1],"0123456789");
    if (lennum > 0)
       {  /* Has number after @ in filnam, stacked image requested */
       strncpy(ctemp,&filnam[locat+1],lennum);
       imginstack = atoi(ctemp);
       }
    /* Remove @ and stacked image number from filnam */
    strcpy(&filnam[locat],&filnam[locat+1+lennum]);
    }

 /* Open file */
 if (strcmp(disp,"r") == 0) 
    fp = fopen(filnam,"rb");   // "r" Never used? al dec 2012

 else if (strcmp(disp,"o") == 0 || strcmp(disp,"q") == 0) // Q from mont2 al dec 2012
    fp = fopen(filnam,"r+b");

 else 
    { spout("*** Unknown disp in openold"); return FALSE; }

 if (fp ==  NULL)
     {    /* File does not exist, or is write protected try readonly */
     fp = fopen(filnam,"rb");
     if (fp ==  NULL)
        {  /* Does not exist, etc. */

       //printf(" *** null in openold: fp: %d disp:%s file: %s \n",fp,disp,filnam);
       if (strcmp(disp,"q") != 0)
           {  /* Give error output (used in montage increment) */
           sprintf(output,"*** Unable to open: %s",filnam);
           spout(output);  
           }
        return FALSE;
        }
     else
        {  /* Opened readonly */
        spout("Opening file as read only!!:");
        }
     } 

 /* Allocate space for a spider file info structure */
 if ((fileptr = (FILEDATA *) malloc(sizeof(FILEDATA))) ==
    (FILEDATA *) 0) 
    {spout("*** Unable to malloc filedata in openold."); return FALSE; }

 /* Set offset for reading initial header from file */
 fileptr -> offset     = 0;
 fileptr -> iform      = 0;
 fileptr -> flip       = 0;
 fileptr -> headbyt    = 0;
 fileptr -> fp         = fp;
 fileptr -> imginstack = imginstack;

 /* Read initial header of image file and determine image format ---*/

 spiderkind = 0;
 mrckind    = 0;

 if (redlin(fileptr,&header.hbuf[0],256,0))
    {
    // Determine image/volume format.  May be SPIDER image? 
    // Flips header.hbuf if this is a flipped SPIDER header

    //printf(" Read hbuf[1]: %f   \n", hbuf[1]);
    spiderkind = is_spider_header(&header.hbuf[0]);

    //printf(" spiderkind: %d   hbuf[1]: %f   \n",spiderkind,header.hbuf[1]);

    if (spiderkind == 0) 
       {
       // Not a SPIDER header.  Is this a MRC header?  
       // Flips header.hbuf if this is a flipped MRC header
       mrckind = is_mrc_header(&header.hbuf[0]);
       }
    }
  
 /* Parse initial header of image file--------------------------- */
 if (spiderkind == 0 && mrckind == 0)
    {  /* Not a SPIDER or MRC image, treat as 8 bit raw */

    iflag = opensmall8(filnam, fileptr, &nsam, &nrow, &nslice, 
                    nstack, inuse, maxim, &lentitle);
    if (iflag != TRUE) return(FALSE);
    }
 else if (spiderkind != 0)
    {  /* Standard SPIDER image, may be flipped */

    nslice = header.hbuf[0];
    if (nslice < 0) nslice = - nslice;
    fileptr -> nslice   = nslice;

    fileptr -> nrow     = header.hbuf[1];

    fileptr -> nlabel   = header.hbuf[3];
    fileptr -> iform    = header.hbuf[4];
    fileptr -> imami    = header.hbuf[5];
    fileptr -> fmax_spi = header.hbuf[6];
    fileptr -> fmin_spi = header.hbuf[7];
    fileptr -> av       = header.hbuf[8];
    fileptr -> sig      = header.hbuf[9];

    fileptr -> nsam     = header.hbuf[11];
    fileptr -> headrec  = header.hbuf[12];
    fileptr -> iangle   = header.hbuf[13];
    fileptr -> phi      = header.hbuf[14];
    fileptr -> theta    = header.hbuf[15];
    fileptr -> gamma    = header.hbuf[16];

    fileptr -> headbyt  = header.hbuf[21];
    fileptr -> reclen   = header.hbuf[22];

    fileptr -> nstack   = header.hbuf[23];
    fileptr -> inuse    = header.hbuf[24];
    fileptr -> maxim    = header.hbuf[25];

    fileptr -> flip     = 0;
    if (spiderkind < 0) fileptr -> flip = 1;

    maxim   = fileptr -> maxim;
    nstack  = fileptr -> nstack;

    /* Set error correction for bad headbyt and reclen SPIDER files */
    iform = fileptr -> iform;
    if (iform < 4 && 
       (fileptr -> reclen < (fileptr -> nsam) * 4)) 
       {
       fileptr -> reclen = (fileptr -> nsam) * 4;
       } 

    headrec = 1024 / (fileptr -> reclen);
    if (fileptr->reclen < 1024 &&
       (1024 % (fileptr -> reclen)) != 0) headrec++;
    headbyt = headrec * (fileptr -> reclen);

    if (iform < 4 && ((fileptr -> headbyt) < headbyt))
       { fileptr -> headbyt = headbyt;  }

    /* Set date in current file information */
    uiptr = (unsigned int *) &header.hbuf[211];
    /* Flip the bytes for date and time in header */
    for (k1 = 211; k1 <= 216 ; k1++, uiptr++)
       { CONVERT_4(*uiptr,*uiptr);  }

    strncpy(&fileptr -> date[0], &header.cbuf[844],11);

    if (fileptr -> date[1] == '-')
       {
       uiptr = (unsigned int *) &header.hbuf[211];
       /* Flip the bytes for date....  in header */
       for (k1 = 211; k1 <= 216 ; k1++, uiptr++)
          { CONVERT_4(*uiptr,*uiptr);  }

       strncpy(&fileptr -> date[0], &header.cbuf[844],11);
       }
    fileptr -> date[11] = '\0';

    /* Set time in current file information */
    strncpy(&fileptr -> time[0], &header.cbuf[856],9);
    fileptr -> time[8] = '\0';

    /* set title in current file information */
    strncpy(&fileptr -> title[0], &header.cbuf[864], 160);
    fileptr -> title[159] = '\0';

    lentitle = strlen(fileptr -> title);
    }

 else if (mrckind != 0)
    {  /* Standard MRC image */

    fileptr -> nsam     = header.ibuf[0];
    fileptr -> nrow     = header.ibuf[1];
    fileptr -> nslice   = header.ibuf[2];
    nslice              = fileptr -> nslice;

    if (header.ibuf[3] == 0)
       { // 8 bit MRC
       fileptr -> iform  = 108;
       fileptr -> reclen = (fileptr -> nsam) ;
       }
    else if (header.ibuf[3] == 1)
       { // 16 bit MRC
       fileptr -> iform  = 116;
       fileptr -> reclen = (fileptr -> nsam) * 2 ;
       }
    else if (header.ibuf[3] == 2)
       { // 32 bit MRC
       fileptr -> iform  = 132;
       fileptr -> reclen = (fileptr -> nsam) * 4 ;
       }
    else
       {
       sprintf(output,"***  %s    Unsupported MRC image format: %d",
               filnam,header.ibuf[3]);
       spout(output);  
       return FALSE;
       }

    if (header.ibuf[16] != 1  || 
        header.ibuf[17] != 2  || 
        header.ibuf[18] != 3)
       { // Not X,Y,Z voxel order
       sprintf(output,"*** %s    Web only supports MRC voxel order: X, Y, Z",filnam);
       spout(output);  
       return FALSE;
       }
   
    fileptr -> fmin_spi = header.hbuf[19];
    fileptr -> fmax_spi = header.hbuf[20];
    fileptr -> av       = header.hbuf[21];
    fileptr -> sig      = 0;

    fileptr -> imami    = 0;
    if (fileptr -> fmin_spi != 0.0 ||
        fileptr -> fmax_spi != 0.0 ||
        fileptr -> av       != 0.0) 
        {fileptr -> imami = 1;}

    fileptr -> nlabel   = header.ibuf[55];
    fileptr -> headrec  = 1;
    fileptr -> headbyt  = 1024;
    fileptr -> iangle   = 0;
    fileptr -> phi      = 0;
    fileptr -> theta    = 0;
    fileptr -> gamma    = 0;
    fileptr -> nstack   = 0;
    fileptr -> inuse    = 1;
    fileptr -> maxim    = 0;

    fileptr -> flip     = 0;
    if (mrckind < 0) fileptr -> flip = 1;

    fileptr -> date[0]  = '\0';
    fileptr -> time[0]  = '\0';
    fileptr -> title[0] = '\0';

    maxim    = 0;
    nstack   = 0;
    lentitle = 0;
    }

 *nsamptr   = fileptr -> nsam;
 *nrowptr   = fileptr -> nrow;
 *nsliceptr = fileptr -> nslice;

 /* Getoldstack may use buf if it needs to normalize */
 if ((fileptr -> buf = (float *) malloc(*nsamptr * sizeof(float))) == 
      (float *) 0) 
    {spout("*** Unable to malloc buf in openold."); return FALSE; }
  
 /* Getoldstack adjusts offset for reading header from stacked file */
 fileptr -> offset = 0;
 if (fileptr -> nstack > 0  && fileptr -> imginstack > 0)
     {  /* Set header pointer to current image in stack */
     if (getoldstack(fileptr, imginstack, FALSE, TRUE) != imginstack)
         {return FALSE;}
     }

 /* Initialize ximage pointer */
 fileptr -> ximage = NULL;

 /* Print file opening information */
 strcpy(typefl," ");
 if (fileptr -> flip > 0) strcpy(typefl,"F" );
 
 iform = fileptr -> iform;

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
 else if (iform == 108)
     strcpy(type,"MRC0");
 else if (iform == 116)
     strcpy(type,"MRC1");
 else if (iform == 132)
     strcpy(type,"MRC2");
 else
     strcpy(type,"r ");

 if (imginstack > 0 && 
    (lentitle > 0)  && (lentitle + strlen(filnam)) < 70)
    /* Print filename and title on one line */
    {
    sprintf(output," %s@%d  %s", filnam,imginstack, fileptr -> title);
    spout(output);
    }
 else if(imginstack > 0)
    /* Print filename */
    {
    sprintf(output," %s@%d", filnam,imginstack);
    spout(output);
    }
 else if ((lentitle > 0) && (lentitle + strlen(filnam)) < 70)
    /* Print filename and title on one line */
    {
    sprintf(output," %s %s", filnam,fileptr -> title);
    spout(output);
    }
 else
    /* Print filename */
    {
    sprintf(output," %s",filnam);
    spout(output);
    }

 if (iform == 8 && fileptr -> headbyt <= 0)
     {  /* Opened a raw 8 bit image (no header) */
     if (nslice > 1)
        sprintf(output," (raw) %d %d %d   Header bytes: 0",
           *nsamptr, *nrowptr, *nsliceptr);
     else
        sprintf(output," (raw) %d %d    Header bytes: 0",
           *nsamptr, *nrowptr);
     spout(output);
     }

 else
     { /* Print file parameters */
     if (fileptr -> nstack > 0 && fileptr -> imginstack == 0)
       { /* Overall stack header */
       if (nslice > 1)
          sprintf(output," (%s) %d %d %d  (...%d) Old: %s At %s  Header bytes: %d",
             type, *nsamptr, *nrowptr, *nsliceptr, maxim, fileptr -> date, 
             fileptr -> time, fileptr -> headbyt);
       else
          sprintf(output," (%s) %d %d (...%d)     Old: %s At %s  Header bytes: %d",
             type, *nsamptr, *nrowptr, maxim, fileptr -> date, fileptr -> time, 
             fileptr -> headbyt);
       }
    else if (fileptr -> iform > 99)
       {
        /* Print file parameters for MRC file*/
       if (nslice > 1)
          sprintf(output," (%s %s) %d %d %d", type, typefl,
                          *nsamptr, *nrowptr, *nsliceptr);
       else
          sprintf(output," (%s %s) %d %d", type, typefl,
                          *nsamptr, *nrowptr);
       }
  
    else
       {
        /* Print file parameters */
       if (nslice > 1)
          sprintf(output," (%s) %d %d %d  Old: %s At %s  Header bytes: %d",
             type, *nsamptr, *nrowptr, *nsliceptr, fileptr -> date, 
             fileptr -> time, fileptr -> headbyt);
       else
          sprintf(output," (%s) %d %d     Old: %s At %s  Header bytes: %d",
             type, *nsamptr, *nrowptr, fileptr -> date, fileptr -> time, 
             fileptr -> headbyt);
       }
    spout(output);

    if ((lentitle > 0) && (lentitle + strlen(filnam)) >= 70)
       {    /* Must print rest of title on separate lines */
       strncpy(&output[0],&fileptr -> title[0],80);
       output[80] = '\0';
       spout(output);
       if (lentitle > 80)
          {
          cptr = fileptr -> title + 80;
          strncpy(output,cptr,80);
          spout(output);
          }
       }
    }
 
 return fileptr;
 }

 /*********************** ReverseFloat ***********************************/

float ReverseFloat( const float inFloat )
{
   float retVal;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat    = ( char* ) & retVal;

   // Swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}

 /*********************** is_spider_header ***********************************/


//#define CONVERT_4( A, B )                        \
//             (A) =                               \
//            ((unsigned int)(B) >> 24) |          \
//            ((unsigned int)(B) << 24) |          \
//           (((unsigned int)(B) >> 8) & 0xff00) | \
//           (((unsigned int)(B) << 8) & 0xff0000)



 int is_spider_header(float hbuf[])
 { 
 /* Is this header a SPIDER header?  Flips if hbuf if not native SPIDER header */

 unsigned int * uiptr;
 float *        fptr;
 int            i;
 int            flipped;
 float          ftemp, a, b;
 float          h_1, h_4, h_11;

 union  {
        float hbuf[256];
        char  cbuf[1024];
        } temp ;

 if (hbuf[1]  < 1.0   || hbuf[1]  > 100000.0 ||
     hbuf[11] < 1.0   || hbuf[11] > 100000.0 ||
     hbuf[4]  < -20.0 || hbuf[4] ==  0.0     || hbuf[4]  > 32 )

    { /* Not native SPIDER try flipping header */
    //printf(" hbuf[1]: %f Not native SPIDER try byte swapping \n", hbuf[1]);
        
    h_1  =  ReverseFloat( hbuf[1] );
    h_4  =  ReverseFloat( hbuf[4] );
    h_11 =  ReverseFloat( hbuf[11] );
    //printf(" 1  h_1:  %f \n",  h_1);       
    //printf(" 1  h_4:  %f \n",  h_4);       
    //printf(" 1  h_11:  %f \n",  h_11);     

    if (h_1 < 1.0    || h_11 > 100000.0 ||
        h_1  < 1.0   || h_1  > 100000.0 ||
        h_4  < -20.0 || h_4 ==  0.0     || h_4  > 32 )

       {/* Neither native SPIDER nor flipped SPIDER!  Returns original hbuf! */
       //printf(" 4 temp.hbuf[1]:  %f   NOT SPIDER \n", temp.hbuf[1]);

       flipped = 0 ; 
       return flipped;
       }

    // Needs flipping, Returns flipped hbuf
    uiptr = (unsigned int *) &hbuf[0];
    for (i = 0; i < 256 ; i++, uiptr++)
       { CONVERT_4(*uiptr,*uiptr);  }

    //printf(" 5 hbuf[1]:  %f   FLIPPED: %d \n",hbuf[1] ,flipped);

    flipped = -1 ; 
    return flipped;
    }
 
 //printf(" 5 hbuf[1]: %f OK SPIDER \n",hbuf[1]);

 // No need for flipping, Returns original hbuf

 flipped = 1; 
 return flipped;
 }   



 /*********************** is_mrc_header ***********************************/

 /*C  MAP/IMAGE HEADER FORMAT
 C 	LENGTH = 1024 BYTES, ORGANIZED AS 56 LONG WORDS FOLLOWED
 C                BY SPACE FOR 10 80 BYTE TEXT LABELS.
 C  1	NX		# OF COLUMNS	(FASTEST CHANGING IN MAP)
 C  2	NY		# OF ROWS
 C  3	NZ		# OF SECTIONS 	(SLOWEST CHANGING IN MAP)
 C  4	MODE		DATA TYPE
 C			0	IMAGE : SIGNED 8-BIT BYTES RANGE -128  TO 127				*
 C			1	IMAGE : 16-BIT HALFWORDS		
 C			2	IMAGE : 32-BIT REALS			
 C			3	TRANSFORM : COMPLEX 16-BIT INTEGERS	
 C			4	TRANSFORM : COMPLEX 32-BIT REALS	
 C  5	NXSTART		NUMBER OF FIRST COLUMN  IN MAP (Default = 0)
 C  6	NYSTART		NUMBER OF FIRST ROW     IN MAP       
 C  7	NZSTART		NUMBER OF FIRST SECTION IN MAP       
 C  8	MX		NUMBER OF INTERVALS ALONG X
 C  9	MY		NUMBER OF INTERVALS ALONG Y
 C 10	MZ		NUMBER OF INTERVALS ALONG Z
 C 11-13                CELLA           CELL DIMENSIONS IN ANGSTROMS
 C 14-16                CELLB           CELL ANGLES IN DEGREES	
 C 17	MAPC		WHICH AXIS CORRESPONDS TO COLUMNS  (1,2,3 FOR X,Y,Z)
 C 18	MAPR		WHICH AXIS CORRESPONDS TO ROWS     (1,2,3 FOR X,Y,Z)
 C 19	MAPS		WHICH AXIS CORRESPONDS TO SECTIONS (1,2,3 FOR X,Y,Z)
 C 20	DMIN		MINIMUM DENSITY VALUE
 C 21	DMAX		MAXIMUM DENSITY VALUE
 C 22	DMEAN		MEAN    DENSITY VALUE    (AVERAGE)
 C 23	ISPG		SPACE GROUP NUMBER 0 OR 1 (DEFAULT=0)
 C 24	NSYMBT		NUMBER OF BYTES USED FOR SYMMETRY DATA (0 OR 80)
 C 25-49 EXTRA          EXTRA, USER DEFINED STORAGE SPACE. 29 WORDS MAX.
 C 50-52                ORIGIN          ORIGIN IN X,Y,Z USED FOR TRANSFORMS
 C 53	MAP	        CHARACTER STRING 'MAP ' TO IDENTIFY FILE TYPE	
 C 54	MACHST          MACHINE STAMP		
 C 55	RMS	        RMS DEVIATION OF MAP FROM MEAN DENSITY	
 C 56	NLABL	        NUMBER OF LABELS BEING USE	
 C 57-256	        LABEL(20,10) 10 80-CHARACTER TEXT LABELS		
 C SYMMETRY RECORDS FOLLOW - IF ANY - STORED AS TEXT AS IN INTERNATIONAL
 C TABLES, OPERATORS SEPARATED BY * AND GROUPED INTO 'LINES' OF 80
 C CHARACTERS (IE. SYMMETRY OPERATORS DO NOT CROSS THE ENDS OF THE
 C 80-CHARACTER 'LINES' AND THE 'LINES' DO NOT TERMINATE IN A *).
 C DATA RECORDS FOLLOW.
 */

 int is_mrc_header(float hbuf[])

 { /* Is this header a MRC header?  Flips if not native MRC header*/

 unsigned int * uiptr;
 int            i;
 char           machst[4];    // Machine stamp   
 int            flipit;
 int            il,ib;
 int            flipped; 

 union  BUFFER 
        {
        float hbuf[256];
        char  cbuf[1024];
        int   ibuf[1024];
        } temp;

 //il = IsLittleEndian(); ib = IsBigEndian();
 //printf(" lit: %d  big:%d \n",il,ib);

 for (i = 0; i < 256 ; i++)
    {temp.hbuf[i] = hbuf[i];}
 
 //printf(" hbuf[0..2]:%f %f %f \n",hbuf[0],hbuf[1],hbuf[2]);
 //printf(" ibuf[0..2]:%d %d %d %d\n",
 //         temp.ibuf[0],temp.ibuf[1],temp.ibuf[2],temp.ibuf[3]);

 if (temp.ibuf[0]  < 1.0   || temp.ibuf[0]  > 1000000.0 ||
     temp.ibuf[1]  < 1.0   || temp.ibuf[1]  > 1000000.0 ||
     temp.ibuf[2]  < 1.0   || temp.ibuf[2]  > 1000000.0 ||
     temp.ibuf[3]  <   0   || temp.ibuf[3]  > 3          )
    { 
    /* Not native MRC. Image size abnormal or bad MRC format*/

    uiptr = (unsigned int *) &temp.hbuf[0];
    for (i = 0; i < 256 ; i++, uiptr++)
       { CONVERT_4(*uiptr,*uiptr);} 

    //printf(" ibuf[0..2]:%f %f %f %f\n",
    //         temp.ibuf[0],temp.ibuf[1],temp.ibuf[2],temp.ibuf[3]);

    if (temp.ibuf[0]  < 1.0   || temp.ibuf[0]  > 1000000.0 ||
        temp.ibuf[1]  < 1.0   || temp.ibuf[1]  > 1000000.0 ||
        temp.ibuf[2]  < 1.0   || temp.ibuf[2]  > 1000000.0 ||
        temp.ibuf[3]  <   0   || temp.ibuf[3]  > 3          )
       { 
       /* Image size out of normal values or bad MRC format*/
       /* Not native MRC or flipped MRC! Returns unflipped hbuf! */
       flipped = 0 ;
       //printf(" temp.hbuf[1]: %f not mrc  \n",temp.hbuf[1]);
       return flipped;
       }

    // Flip the input buffer for return
    uiptr = (unsigned int *) &hbuf[0];
    for (i = 0; i < 256 ; i++, uiptr++)
       { CONVERT_4(*uiptr,*uiptr);}
 
    flipped = -1 ; 
    //printf(" hbuf[1]: %f flipped mrc:%d \n",hbuf[1],flipped);
    return flipped ;
    }
    
 // No flipping, return un flipped hbuf
 flipped = 1 ; 
 //printf(" hbuf[1]: %f native mrc:%d \n",hbuf[1],flipped);
 return flipped ;
 }


 int IsLittleEndian()
 {
 int n = 1;
 // Little endian if true
 if (*(char *)&n == 1) {return TRUE;}
 return FALSE;
 }

 int IsBigEndian()
 {
 int n = 1;
 // Little endian if true
 if (*(char *)&n == 1) {return FALSE;}
 return TRUE;
 }


 /*********************** opensmall8 *********************************/
 
 int opensmall8(char filnam[], FILEDATA  *fileptr,
               int * nsamptr,   int * nrowptr,  int * nsliceptr, 
               int nstack,      int inuse, int maxim, int * lentitle)
  
 { 
 char   output[160];

 // Not a SPIDER image, treat as 8 bit raw 

 if (nsam8 <= 0 || nrow8 <= 0 || nslice8 <= 0)
    {   // Need correct raw image size 
    sprintf(output,"*** %s  Unknown image format. Raw image size not set in options", filnam);
    spout(output);
 
    // get_imsiz();
    // closefile(fileptr);
    return -1;
    }

 *nsamptr  = nsam8;
 *nrowptr   = nrow8;
 *nsliceptr = MYMAX(nslice8,1);

 fileptr -> nslice   = nslice8;
 fileptr -> nrow     = nrow8;

 fileptr -> nlabel   = 0;
 fileptr -> iform    = 8;
 fileptr -> imami    = 1;
 fileptr -> fmax_spi = 255.0;
 fileptr -> fmin_spi = 0.0;
 fileptr -> av       = 0.0;
 fileptr -> sig      = 0.0;

 fileptr -> nsam     = nsam8;
 fileptr -> headrec  = 0.0;
 fileptr -> iangle   = 0.0;
 fileptr -> phi      = 0.0;
 fileptr -> theta    = 0.0;
 fileptr -> gamma    = 0.0;

 fileptr -> headbyt  = 0;
 fileptr -> reclen   = nsam8;

 fileptr -> nstack   = nstack;
 fileptr -> inuse    = inuse;
 fileptr -> maxim    = maxim;

 /* Set date, time title to NULL in current file information */
 fileptr -> date[0]  =  '\0';
 fileptr -> time[0]  =  '\0';
 fileptr -> title[0] =  '\0';

 *lentitle = 0;
 return TRUE;
 }
  



#ifdef NEVER
    fseek(fp, 0xd0, SEEK_SET);
    if ((my_mrc_header.machst)[0] == ((0x1 << 4) + 0x1) &&
        IsLittleEndian() && flipped == FALSE) flipped = TRUE;
    else if ((my_mrc_header.machst)[0] == ((0x4 << 4) + 0x4) &&
             IsBigEndian() && flipped == FALSE) flipped = TRUE;


#endif


#if BUGGY
c__________________________________________________________
  printf(" *** in open: fp: %d file: %s \n",fp,filnam);
  printf(" *** in open: date: %s \n",fileptr -> date);
  printf(" *** in open: time: %s \n",fileptr -> time);
  printf(" *** in open: header.cbuf: %s \n",&header.cbuf[844]);
  printf(" *** in open: title: %s \n",fileptr -> title);
  printf(" *** in open: lentitle: %d \n",lentitle);
  printf(" *** in open: *nsam: %d \n",*nsamptr);
  printf(" *** in open: fileptr: %d \n",fileptr);
c---------------------------------------------------
#endif

