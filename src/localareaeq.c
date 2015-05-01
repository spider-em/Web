/*
 ***********************************************************************
 *
 * localareaeq.c
 *
 ***********************************************************************
 AUTHOR Ajay Divakaran
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
 ***********************************************************************
 *
 * localareaeq
 * 
 * PURPOSE:     
 *
 * RETURNS:
 * 
 * PARAMETERS:     
 *
 ***********************************************************************
*/

/*************

Local Area Histogram equalization Program
The main routine is
local_area_eq
    It calls
            reflection
            histogram_add
            histogram_sub
            equal
            quantizer
            
------------------------------------------
            reflection does not call any other routine

--------------------------------

            histogram_add calls 
                                quantizer
 
------------------------------------------
            histogram_sub calls
                                 quantizer

------------------------------------------
            equal does not call any routine

-------------------------------------------

                            local_area_eq   
                                |
                                |
                                |
-----------------------------------------------------------------
|                       |              |             |          |
|                       |              |             |          |
reflection        histogram_add    histogram_sub   equal    quantizer
                        |              |
                        |              |
                    quantizer      quantizer

***********************/


#include "std.h"
#include "routines.h"

/* internal function protoypes */
int   reflection     (float *,int, int, float *,int);
float equal          (int *,float,float,int,int, int);
int   histogram_add  (float *, int, float, float, int, int *);
int   histogram_sub  (float *, int, float, float, int, int *);
int   quantizer      (float, float, float, int);


/* common variables used here 
 static float*   newimage   = NULL;
*/

/************************* localareaeq *****************************/

int localareaeq(float *oldimage,int nsam,int nrow, float *ffmin ,
                  float *ffmax ,int Nbins,int border_thickness,
                  float *newimage)

  /*
  This routine carries out a local area histogram equalization on
  the input image. It starts with a local area centered at the (0,0) pixel
  and then moves the local area center a pixel at a time. After each move,
  the histogram is updated and the equalized value is computed. The 
  advantage of this approach lies in the fact that it is only one row or
  column changes when we move the center of the local area. Hence we do
  not need to re-compute the entire histogram of the local area each time.
  There is hence a saving in computation.  */
  /*
  oldimage    is the input image.
  nsam        is the number of rows.
  nrow        is the number of columns.
  fmaxt        is the maximum pixel value.
  fmint        is the minimum pixel value.
  Nbins       is the number of quantization bins for the histogram.
  border_thickness
              is the number of pixels rowwise from the centre to the
              rightmost/leftmost pixel of the local area.
              It is the same as the number of
              pixels columnwise from the centre to the top/bottom pixel
              of the local area.
              Hence the local area size is
              always (2*border_thickness + 1)  x (2* border_thickness +1)
  newimage    is the equalized output image.
  */

   /*
   The Memory requirements are:             
   newimage
   nsam x nrow x sizeof(float)  

   reflected_image
   (nsam+2*border_thickness) x (nrow+2*border_thickness) x sizeof(float) 
 
   incoming_row
   2*border_thickness+1 x sizeof(float)

   outgoing_row
   (2*border_thickness+1) x sizeof(float)

   incoming_column
   (2*border_thickness+1) x sizeof(float)

   outgoing_column
   (2*border_thickness+1) x sizeof(float)  
  
   local_area
   (2*border_thickness+1) x (2*border_thickness + 1) x sizeof(float)

   histogram
   Nbins x sizeof(int)
   */

{


float *reflected_image;
float *incoming_row;
float *outgoing_row;
float *incoming_column;
float *outgoing_column;
float *local_area;
float fmint,fmaxt; 
float temp;
int check;
int   *histogram;
int    ix,iy,local_area_size,LOCAL_AREA_SIZE;
int    ixc,iyc;
int    nsamr,nrowr;
int    index;
int    flag;
int count; 
/* Initialize fmint and fmaxt */ 
fmint  = *ffmin;
fmaxt  = *ffmax;
count = 0;  
/* nsamr and nrowr are the dimensions of the reflected image.
   Hence we need to add twice the border thickness to each of
   the dimensions of the input image. */ 

nsamr           = nsam+border_thickness+border_thickness;
nrowr           = nrow+border_thickness+border_thickness;
local_area_size = 2*border_thickness+1;
LOCAL_AREA_SIZE = local_area_size*local_area_size;

/* Allocate space for all the matrices and vectors */
/* Allocate space for output_image. This is called newimage elsewhere.*/

/* Allocate space for reflected image */
if ((reflected_image = (float *) calloc(nsamr*nrowr,sizeof(float)))
                     == (float *) 0)
    {
 spout("Unable to allocate memory in local_area_eq");
      return FALSE; }

/* Allocate space for incoming and outgoing columns and rows */

if ((incoming_row = (float *) calloc(local_area_size,sizeof(float)))
                 == (float *) 0)
     {
     spout("Unable to allocate memory in local_area_eq");
     free(reflected_image); return FALSE;
     }
                                       
if ((outgoing_row = (float *) calloc(local_area_size,sizeof(float)))
                  == (float *) 0)
     {
     spout("Unable to allocate memory in local_area_eq");
     free(reflected_image); free(incoming_row);
     return FALSE;
     }
                                       
if ((incoming_column = (float *) calloc(local_area_size,sizeof(float)))
                        == (float *) 0)
     {
     spout("Unable to allocate memory in local_area_eq");
     free(reflected_image); free(incoming_row);
     free(outgoing_row); return FALSE;
     }
                                       
if ((outgoing_column = (float *) calloc(local_area_size,sizeof(float)))
                        == (float *) 0)
     {  
     spout("Unable to allocate memory in local_area_eq");
     free(reflected_image); free(incoming_row);
     free(outgoing_row); free(incoming_column); return FALSE;
     }
                 

/* Allocate space for local area and histogram */

if ((local_area = (float *) calloc(LOCAL_AREA_SIZE,sizeof(float)))
                        == (float *) 0)
     {
     spout("Unable to allocate memory in local_area_eq");
     free(reflected_image); free(incoming_row);
     free(outgoing_row); free(incoming_column); free(outgoing_column);
     return FALSE;
     }
if ((histogram = (int *) calloc(Nbins,sizeof(int)))
                        == (int *) 0)
     {
     spout("Unable to allocate memory in local_area_eq");
     free(reflected_image); free(incoming_row); free(outgoing_row);
     free(incoming_column); free(outgoing_column);  free(local_area);
     return FALSE;
     }
/* Initialize histogram vector */
for (ix=0;ix<Nbins;ix++)
  {
  histogram[ix]=0;
  }

reflection(oldimage,nsam,nrow,reflected_image,border_thickness);

/* Let the center be at [ixc,iyc]. For now we start from the
   pixel input_image[0][0] i.e. input_image[0]. */

ixc=0;
iyc=0;


/* Therefore the corresponding coordinates in the reflected image will
   be ixc=ixc+border_thickness and iyc=iyc+border_thickness.
   Hence the coordinates of the top-left corner of the local area
   in the reflected image will be ixc+border_thickness-border_thickness
   and iyc+border_thickness-border_thickness i.e. [ixc][iyc]
*/

/* Form initial local area */

for (ix=0;ix<local_area_size;ix++)
  {
    for (iy=0;iy<local_area_size;iy++)
     {
      
       local_area[ix*local_area_size+iy]=
       reflected_image[(ixc+ix)*(nrowr)+iyc+iy];
     }
  }
  /* Form histogram of local_area. 
   Carry out equalization and get the value of the output pixel. */

   histogram_add(local_area,LOCAL_AREA_SIZE,fmaxt,fmint,Nbins,histogram);
   index=quantizer(oldimage[0],fmaxt,fmint,Nbins);
   temp =newimage[0]=equal(histogram,fmaxt,fmint,index,Nbins,local_area_size);
   count++; 
 if (temp < *ffmin)
 {
   *ffmin=temp;
 }

/* Check to see if the row is even. If it is even we should move right.
   If it is odd, we move left. Each time we hit the end of the row,
   we stop, and increment the row number, find the modified
   histogram for the new starting point  and toggle the flag. This way
   we never have to construct a local area from scratch at the beginning
   of a row, and thus we save that operation. The ASxAS local area is
   created in the beginning and then because of our even-right, odd-left
   movement, we only need AS + AS modifications to the histogram each 
   time we move to a new pixel.
   if flag=0, the row is even. If flag=1, the row is odd.
   Recall that the subscripting is from 0 so the first row is "even." */

flag=0;

for (ixc=0;ixc<nsam;ixc++)
   {
   if (flag==0)
     {
     for (iyc=0;iyc<nrow-1;iyc++)
           {
           for (ix=0;ix<local_area_size;ix++)
              {

              outgoing_column[ix]=reflected_image[(ixc+ix)*nrowr+iyc];
              incoming_column[ix]=reflected_image[(ixc+ix)*nrowr+iyc
                                  +local_area_size];
              }

/* Modify Histogram according to the outgoing and incoming column
   and equalize. Recall that the pixel in question is actually
   input_image[ixc][iyc+1]
*/

   histogram_add(incoming_column,local_area_size,fmaxt,fmint,
                 Nbins,histogram);
   histogram_sub(outgoing_column,local_area_size,fmaxt,fmint,
                 Nbins, histogram);
   index=quantizer(oldimage[ixc*nrow+iyc+1],fmaxt,fmint,Nbins);

  temp= newimage[ixc*nrow+iyc+1]=equal(histogram,fmaxt,fmint,index,Nbins,
                                                  local_area_size);
  count++; 
 if (temp < *ffmin)
 {
      *ffmin=temp;
 }
        }
/* Now that we have reached the end of the row, we need to go down
   to the next row. So we need to increment flag by 1 to reflect
   the change in the row. Next we need to see how the histogram is
   modified by the movement down by a row. i.e. ixc=ixc+1, iyc=Ncols+2
   We will get the histogram for the pixel [ixc+1][Ncols+2]. Then we
   can once again move row-wise but this time we need to move left. So
   we put that in the else part of the if-then-else */

    flag=1;
    if (ixc!=nsam-1)
       {
       for (iy=0;iy<local_area_size;iy++)
          {
          
          outgoing_row[iy]=reflected_image[ixc*nrowr+iyc+iy];
          incoming_row[iy]=reflected_image[(ixc+local_area_size)*nrowr
                           +iyc+iy];
          }

       /* Modify  as before with ixc=ixc+1,iyc=iyc. */

       histogram_add(incoming_row,local_area_size,fmaxt,fmint,
                     Nbins,histogram);
       histogram_sub(outgoing_row,local_area_size,fmaxt,fmint,
                     Nbins,histogram);
       index=quantizer(oldimage[nrow*(ixc+1)+iyc],fmaxt,fmint,Nbins);
       temp=newimage[nrow*(ixc+1)+iyc]=equal(histogram,fmaxt,fmint,index,
                          Nbins, local_area_size);
 count++; 
if (temp < *ffmin)
  {
     *ffmin=temp; 
  }
       }
   }
else
   {
   for (iyc=nrow-1;iyc>0;iyc--)
      {
      for (ix=0;ix<local_area_size;ix++)
         {
         outgoing_column[ix]=reflected_image[(ixc+ix)*nrowr+iyc
                                             +local_area_size-1];
         incoming_column[ix]=reflected_image[(ixc+ix)*nrowr+iyc-1];
         }


     /*
     Modify histogram and equalize as before. Note that the modified
     histogram is of the pixel input_image[ixc][iyc-1] */

     histogram_add(incoming_column,local_area_size,fmaxt,fmint,Nbins,
                   histogram);
     histogram_sub(outgoing_column,local_area_size,fmaxt,fmint,Nbins,
                   histogram);
     index=quantizer(oldimage[ixc*nrow+iyc-1],fmaxt,fmint,Nbins);
 temp=    newimage[ixc*nrow+iyc-1]=equal(histogram,fmaxt,fmint,index,Nbins,
                     local_area_size);
count++;
if (temp < *ffmin)
 {
     *ffmin=temp;
 }

     }

/* Now we have reached the end of the row i.e.the left extreme. Now
   we need to move down one row and now move the local area from left
   to right. So first we set the flag to 0 so as to indicate that now
   the movement will be from left to right. We also carry out the
   resulting row updates as previously in the left to right case. */

   flag=0;
   if (ixc!=nsam-1)
      {
      for (iy=0;iy<local_area_size;iy++)
         {
          
         outgoing_row[iy]=reflected_image[ixc*nrowr+iy+iyc];
         incoming_row[iy]=reflected_image[(ixc+local_area_size)*nrowr
                                            +iy+iyc];
          }
       /*
       Modify histogram as before. Note that the modified histogram is
       that of pixel input_image[ixc+1,0] */

       histogram_add(incoming_row,local_area_size,fmaxt,fmint,
                     Nbins,histogram);
       histogram_sub(outgoing_row,local_area_size,fmaxt,fmint,
                     Nbins,histogram);
       index=quantizer(oldimage[(ixc+1)*nrow+0],fmaxt,fmint,Nbins);
       temp=newimage[(ixc+1)*nrow+0]=equal(histogram,fmaxt,fmint,index,
                           Nbins, local_area_size);
count++; 
if (temp < *ffmin)
 {
   *ffmin=temp;
 }

       }
    }
 }
/*  
*/ 
/* Free up space */
free(reflected_image);
free(incoming_column);
free(outgoing_column);
free(incoming_row);
free(outgoing_row);
free(local_area);
free(histogram);
*ffmax = fmaxt;
return TRUE ;

}



/************************* reflection      ***************************/

int reflection(float *oldimage, int nsam, int nrow, 
               float *reflected_image,int border_thickness)

   /* 
   This routine takes the input image and extends it beyond its
   edges via symmetric reflection. In other words, the reflected
   image is nothing but the input image with an n pixel border
   created all around it via symmetric reflection. If there
   is an n pixel border, the local area size will be 2n+1 x 2n+1.
   We use the reflection routine to take care of the border
   pixels.
   */

   /*
   oldimage         is the input image.
   reflected_image  is the reflected version of the input image.
   nsam             is the number of rows.
   nrow             is the number of columns.
   border_thickness is the thickness of the border in number of pixels.
   */   
{
int ix,iy;
int nsamr,nrowr;
int check;
/* nsamr and nrowr are the dimensions of the reflected image.
The reflected image is of dimension 
nsam + 2*border_thickness x nrow + 2*border_thickness */

nsamr=nsam+border_thickness+border_thickness;
nrowr=nrow+border_thickness+border_thickness;
/* First paste the image in the middle. */
for (ix=border_thickness;ix<nsam+border_thickness;ix++)
   {
   for (iy=border_thickness;iy<nrow+border_thickness;iy++)
      {
       
       reflected_image[ix*nrowr+iy]=oldimage[(ix-border_thickness)*nrow
                                               +iy-border_thickness];
       }
   }

/*  Reflect the Rows.*/

for (ix=border_thickness;ix<nsam+border_thickness;ix++)
   {
   for (iy=0;iy<border_thickness;iy++)
       {
       
       reflected_image[ix*nrowr+iy]=reflected_image[ix*nrowr+
                       border_thickness +border_thickness-1-iy];
       }
   for (iy=nrow+border_thickness;iy<nrow+border_thickness+
                       border_thickness; iy++)
       {
        

       reflected_image[ix*nrowr+iy]=reflected_image[ix*nrowr
            +(nrow+border_thickness-1) +(nrow+border_thickness) -iy];
       }
   }


/* Reflect the columns. */

for (iy=0;iy<nrowr;iy++)
    {
    for (ix=0;ix<border_thickness;ix++)
       {
        
       reflected_image[nrowr*ix+iy]=reflected_image[(border_thickness
                                                    +border_thickness
                                                    -1-ix)*nrowr+iy];
       }
    for (ix=nsam+border_thickness;ix<nsamr;ix++)
       {
        
       reflected_image[nrowr*ix+iy]=reflected_image[((nsam+
                 border_thickness-1) +(nsam+border_thickness) -ix) * 
                 nrowr + iy];
       }
    }
                                                 
return 0;
}

int   histogram_add  (float *, int, float, float, int, int *);

/************************* histogram_add   ***************************/

int histogram_add(float *incoming, int length, float fmaxt, float fmint, 
                                       int Nbins, int *histogram)
/* This routine modifies the input histogram according the input incoming
   column by incrementing the histogram entries corresponding to each of
   the incoming column elements.
   Thus it takes each element of the incoming column, finds its quantizer
   bin index and increments the corresponding histogram entry. */
/*
incoming        is the incoming row/column when we move the local area.
length          is the length of the incoming row/column.
fmaxt            is the maximum pixel value.
fmint            is the minimum pixel value.
Nbins           is the number of quantizer bins.
histogram       is the histogram of the local area. */ 


{
int ix,index;
int quantizer(float pixel, float fmaxt, float fmint, int Nbins);
for (ix=0;ix<length;ix++)
   {
   index = quantizer(incoming[ix],fmaxt,fmint,Nbins);
   histogram[index]++;
   }
return 0;
}


/************************* histogram_sub   ***************************/

int histogram_sub(float *outgoing, int length, float fmaxt, float fmint,
                                           int Nbins, int *histogram)
/* This routine modifies the input histogram according the input outgoing
   column by decrementing the histogram entries corresponding to each of
   the outgoing column pixels. 
   Thus, it finds the quantizer bin index for each element of the outgoing
   column and then decrements the corresponding histogram entry by 1. */
/*
outgoing   is the outgoing column/row when we move the local area.
length     is the length of the outgoing row or column.
fmaxt       is the maximum pixel value.
fmint       is the minimum pixel value.
Nbins      is the number of quantizer bins.
histogram  is the histogram of the local area. */

{
int ix,index;
int quantizer(float pixel, float fmaxt, float fmint, int Nbins);

for (ix=0;ix<length;ix++)
   {
   index = quantizer(outgoing[ix],fmaxt,fmint,Nbins);
   histogram[index]--;
   }
return 0;
}


/************************* quantizer **********************************/
#include <stdio.h>
int quantizer(float pixel, float fmaxt, float fmint,int NBins)
{
   /* This routine finds the quantizer bin index for a given
   input pixel  given the number of bins Nbins and fmaxt and fmint. */
   /*  pixel is the input pixel.
   fmaxt  is the max. pixel value.
   fmint  is the min. pixel value.
   NBins is the number of quantizer bins. */

int i,index;
float binwidth;

/* Divide regions into bins using the min. and max. values */
/*if (pixel < fmint || pixel > fmaxt)
   {printf("Something Wrong!!\n");}
*/
binwidth=(fmaxt-fmint)/NBins;
/* Subtract min. value from pixel value so that min. pixel value=0*/
pixel = pixel-fmint;

/* Locate the appropriate bin for the pixel value */
/* If the pixel value = fmaxt then the bin number is NBin */
/* which is one larger than the largest. This can be */
/*allowed for at the very end by adding all the members of the*/
/*NBin'th group to the (NBin+1)th group. Thus the total number */
/*of bins remains unaltered. */
/*This is necessary because we proceed from the left. Bear in mind */
/* that it could be done from the right as well, in which case */
/* pixel value = fmint would be handled analogously */

for (i=0;i<=(NBins-1);i++)
   {
   if (i*binwidth <= pixel && pixel < (i+1)*binwidth)
       {
       index=i;
       return index;
       }
   }
/*index is the bin number*/

if (pixel==(fmaxt-fmint))
{index=NBins-1;
 return index;
}

index=NBins-1;

return index;
}

/***************************** equal *********************************/

float equal(int *histogram,float fmaxt,float fmint,int index,
            int NBins,int ibox)
{
int i;
float final_value;

final_value=0;
for (i=0;i<=index;i++)
   {
   final_value+=histogram[i];
   }
final_value = final_value/(ibox*ibox);
final_value = final_value*(fmaxt-fmint) + fmint;
return final_value;
}
