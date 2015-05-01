/**
C++*******************************************************************
C
C    rcaver.c -- CONVERTED NOV., 1993 BY JINGSU
C
C		 FROM RCAVER.FOR
C
C    PURPOSE:    CUTS OUT A RECTANGULAR IMAGE SECTION AND DETERMINES
C		 AVERAGE
C
C    VOID RCAVER(FPTR,NSAM,NROW,NSAMW1,NROWW1,NSAM2,NROW2,AVER)
C
C    VARIABLES:  FPTR           FILE POINTER OF INPUT IMAGE
C                NSAM,NROW      DIMENSIONS OF INPUT PICTURE
C                NSAMW1,NROWW1  COORDINATES, WITH RESPECT TO INPUT IMAGE,
C                               OF TOP LEFT CORNER OF WINDOW
C                NSAM2,NROW2    DIMENSIONS OF WINDOW = DIMENSIONS AREA
C				FOR AVERAGE
C
C    CALLED BY   BACKP.C
C
C--*******************************************************************/

#include "std.h"
#include "routines.h"


 /*****************************  rcaver  *****************************/

 void rcaver(FILEDATA * fptr, int nsam, int nrow, int nsamw,
	     int nroww, int ixul, int iyul, float * aver)
 {
 int icol, irow;

 * aver = 0;

 for (irow = iyul; irow < iyul + nroww; irow++)
     {
     if (redlin(fptr, fptr -> buf, nsam, irow) == TRUE)
        {
        for (icol = ixul; icol < ixul + nsamw;  icol++)
           *aver = *aver + (fptr -> buf[icol]);
        }
     }
 * aver = (* aver) / (nsamw * nroww);
 return;
 }
