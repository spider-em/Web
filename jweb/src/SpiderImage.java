package web;

import java.io.*;
import java.awt.*;
import java.awt.image.*;
import javax.media.jai.*;
import java.awt.color.*;
import javax.swing.*;
import java.text.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class SpiderImage {

   public float              fmin, fmax;
   public int                nsam, nrow, nslice, iform, imami, labbyt;
   private int               inuse, maxim;
   private String            fileName, fileType, fileFormat;
   private float             nsamCheck, nrowCheck, avg, sig, nflagCheck;
   public float []           fpix;
   public byte  []           bpix;
   private byte []           bpixStackedImage;
   private int  []           inuseImageNumbers;
   private DataInputStream   din;
   private RandomAccessFile  dout;
   private UtilParameters    utilParameters;
   private double            fcval, fncon;


   // Constructor used for directly accessing some useful methods in this class.
   // Example : makeTiledImageFromByteArray(....).
   public SpiderImage() {

   }

   // used to make a spider image for image series operations.
   // only nsam, nrow, nslice and byte array bpix[] info is available for
   // the spider image objects that are created using this constructor.
   public SpiderImage(byte [] dataArrayByte, int width, int height, int numOfImage,
                                                   UtilParameters utilParam) {
      bpix = dataArrayByte;
      nsam = width;
      nrow = height;
      nslice = numOfImage;
      utilParameters = utilParam;
   }

   // ******************* SpiderImage *********************************
   // constructs  SPIDER image object for existing file
   public SpiderImage(String sfile, UtilParameters utilParam) {
      utilParameters = utilParam;

      try {

         din = new DataInputStream
                         (new BufferedInputStream(new FileInputStream(sfile)));

         // save the input file name
         fileName = sfile;
      }
      catch (IOException e) {
         String str = "Error reading header in SpiderImage: " + e + "\n";
         utilParameters.outputMessageTextArea.append(str);
      }
      readHeader();
   }

   private void readHeader() {

      int             headlen  = 256;
      float           header[] = new float[headlen];
      // read the header data from file to find if it is a big-endian (UNIX/JAVA)
      // or little-endian (WINDOWS)
      try {
         din.mark(12*4);
         for (int i = 0; i < 12; i++) {
            header[i]   = din.readFloat();
         }

         // rewind to the begining
         din.reset();
         nrowCheck = Math.abs(header[1]);
         nsamCheck = Math.abs(header[11]);
         nflagCheck = header[5];
         System.out.println(nflagCheck);
         // big-endian
//       if ((nflagCheck == 0.0) || (nflagCheck == 1.0)) {
         if ((header[4] == 1.0) || (header[4] == 3.0) || (header[4] == -11.0) || (header[4] == -12.0) || (header[4] == -21.0) || (header[4] == -22.0)) {
            fileFormat = "bigEndian";
            // read the header data from file
            for (int i = 0; i < headlen; i++) {
               header[i]   = din.readFloat();
            }
            // note: format of header of the spider image stacks was changed, so
            // there has been some modification
            if (header[23] > 0) {
               fileType = "imageStack";
            }
            else if (header[23] < 0) {
               String str ="indexed image stack is not supported !";
               System.out.println(str);
               System.exit(0);
            }
            else {
               fileType = "imageFile";
            }

         }
         // little-endian or document file
         else {
           din.mark(12*4);
           for (int i = 0; i < 12; i++) {
               int b1 = din.read();
               int b2 = din.read();
               int b3 = din.read();
               int b4 = din.read();
               header[i] = java.lang.Float.intBitsToFloat(
                                    (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);
            }
            // rewind to the begining
            din.reset();
            nrowCheck = Math.abs(header[1]);
            nsamCheck = Math.abs(header[11]);
            nflagCheck = header[5];
           System.out.println(nflagCheck);
            // little-endian
 //           if ((nflagCheck == 0.0) || (nflagCheck == 1.0)) {
              if ((header[4] == 1.0) || (header[4] == 3.0) || (header[4] == -11.0) || (header[4] == -12.0) || (header[4] == -21.0) || (header[4] == -22.0)) {
               fileFormat = "littleEndian";
               for (int i = 0; i < headlen; i++) {
                  int b1 = din.read();
                  int b2 = din.read();
                  int b3 = din.read();
                  int b4 = din.read();
                  header[i] = java.lang.Float.intBitsToFloat(
                                    (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);
               }
               if (header[23] > 0) {
                  fileType = "imageStack";
               }
               else if (header[23] < 0) {
                  String str ="indexed image stack is not supported !";
                  System.out.println(str);
                  System.exit(0);
               }
               else {
                  fileType = "imageFile";
               }
            }
            // document file
            else {
               fileFormat = "ascii";
               fileType = "documentFile";
               // close file
               din.close();
            }
         }
         if (fileType != "documentFile") {
            // retrieve important header variables
            nsam   = (int) header[11];
            nrow   = (int) header[1];
            nslice = Math.abs((int) header[0]);
            iform  = (int) header[4];
            imami  = (int) header[5];
            labbyt = (int) header[21];
            inuse = (int) header[26];
            // image stack
            if (header[23] > 0) {
               // update this variable. done only when the overal header of the
               // image stack is read
               maxim = (int) header[25];
            }
            fmin   = header[7];
            fmax   = header[6];
            String str = fileName + "\n";
            if (utilParameters.showHeaderInfo == true) {
               utilParameters.outputMessageTextArea.append(str);
            }
            str = " nsam = " + nsam + " nrow = " + nrow;
            if (utilParameters.showHeaderInfo == true) {
               utilParameters.outputMessageTextArea.append(str);
            }
            if (nslice > 1) {
               str = " nslice = " + nslice ;
               if (utilParameters.showHeaderInfo == true) {
                  utilParameters.outputMessageTextArea.append(str);
               }
            }
            int left = labbyt - (256 * 4);
            if (left > 0) {
               din.skipBytes(left);
            }
            str = "labbyt: " + labbyt + " skipping: " + left + "\n";
            //utilParameters.outputMessageTextArea.append(str);
         }
      }
      catch (IOException e) {
         String str = "Error reading header in SpiderImage: " + e + "\n";
         utilParameters.outputMessageTextArea.append(str);
      }
   }

   // ******************* getImage *********************************

   public TiledImage getImage(int nslicet) throws IOException {
      // read image in float data and convert pixels to byte data
      readImage();
      // make a tiled image
      TiledImage tilImg = makeTiledImageFromByteArray(bpix,nsam,nrow) ;
      return tilImg;
   }

   public String getFileType() {
      // returns type of the file
      return fileType;
   }

   public int getnsam() {
      // returns nsam
      return nsam;
   }

   public int getnrow() {
      // returns nrow
      return nrow;
   }

   public int getnslice() {
      // returns nslice
      return nslice;
   }

   public double getfcval() {
      // returns fcval (
      return fcval;
   }

   public double getfncon() {
        // returns fncon
        return fncon;
   }

   public float getmin() {
          // returns minimum pixel value
          return fmin;
   }

   public float getmax() {
             // returns maximum pixel value
             return fmax;
   }

   public int getiform() {
      // returns iform, iform = 1 => 2D image : iform = 3 => 3D image
      return iform;
   }


   public void readImage() throws IOException {

      // stores Image Array (normalized  [0...255]) for given image
      // in a byte array
      int    npix = nsam * nrow * nslice;
      int    slicepix = nsam * nrow;

      double nmin = 0.0;
      double nmax = 255.0;

      //double fcval;
      int pval;
      double sumX = 0.0;
      double sumXSquare = 0.0;
      double sigTop = 0.0;

      DecimalFormat df = new DecimalFormat("####0.00");
      DecimalFormat df2 = new DecimalFormat("####0.00000");
      /*
         normalized pixel value in int array
         int[] ipix = new int[npix];
         for RGB image array length is trippled
         int[] ipix = new int[npix*3];
      */
      // byte array to hold normalized values
      bpix = new byte[npix];
      // records time to read the full array
      // long startTime = System.currentTimeMillis();

      // big-endian
      if ( fileFormat == "bigEndian") {
         // byte array to hold all pixel values for a slice
         byte [] pixBytes = new byte [slicepix*4];
         int arrayIndex = 0;
         // if fmin, fmax, average and standard deviation are not set,
         // find them and write to the header
         if (imami == 0) {
            din.mark(4*npix);
            float temp = 0;
            // read all pixels from one slice at a time
            // data from a slice is read at a time OR ELSE it is extremely
            // slow in SGI. Note: if you read the whole volume at a time it
            // it will be a bit faster but more memory will be required to store
            // the data.

            // do it for each slice
            for (int k = 0; k < nslice ; k++) {
               arrayIndex = 0;
               din.read(pixBytes);
               for (int i = 0; i < slicepix; i++) {
                  temp = java.lang.Float.intBitsToFloat
                     ((pixBytes[arrayIndex]&0xff)<<24 |
                        (pixBytes[arrayIndex+1]&0xff)<<16 |
                                 (pixBytes[arrayIndex+2]&0xff)<<8 |
                                            (pixBytes[arrayIndex+3]&0xff));
                  // add all the pixel values
                  sumX = sumX + temp;

                  sumXSquare = sumXSquare +  temp * temp;

                  // sets fmin and fmax
                  if (i == 0 && k == 0) {
                     fmin = temp;
                     fmax = temp;
                  }
                  // resets fmin and fmax
                  if (temp < fmin) fmin = temp;
                  if (temp > fmax) fmax = temp;
                  arrayIndex = (i+1)*4;
               }
            }
            // find average pixel values
            avg = (float) sumX/npix;
            sigTop = sumXSquare - (sumX * sumX)/npix;
            if ((sigTop < 0) || (npix == 1)) {
               String str =
               " found SQRT of -ve number OR divison by zero in Standard Deviation";
               System.out.println(str);
               System.exit(0);
            }
            sig = (float)Math.sqrt(sigTop/(double)(npix-1));
            din.reset();
         }
         String str1 = df.format(fmin);
         String str2 = df.format(fmax);
         String str = "";
         if (imami == 0) {
            // if found statistics then print average and standard deviation
            String str_1 = df2.format(avg);
            String str_2 = df2.format(sig);
            str = " fmin = "+str1+ " fmax = "+ str2 + " avg = "+ str_1+
                                   " sd = "+str_2+"\n";
         }
         else {
            str = " fmin = "+str1+ " fmax = "+ str2 + "\n";
         }
         if (utilParameters.showHeaderInfo == true) {
            utilParameters.outputMessageTextArea.append(str);
         }
         fcval = (nmax - nmin) / (fmax - fmin);
         fncon = nmin - (fmin * fcval) ;


         // read all pixels from one slice at a time
         // data from a slice is read at a time OR ELSE it is extremely
         // slow in SGI. Note: if you read the whole volume at a time it
         // it will be a bit faster but more memory will be required to store
         // the data.

         int offset = 0;
         float pixFloat;

         //do it for each slice
         for (int k = 0; k < nslice ; k++) {
            din.read(pixBytes);
            // reset arrayIndex (if imami == 0 so arrayIndex has been changed)
            // also needs to be reset for each slice
            arrayIndex = 0;
            for (int i = 0; i < slicepix; i++) {
               //find each pixel values in float
               pixFloat = java.lang.Float.intBitsToFloat
                 ( (pixBytes[arrayIndex] & 0xff) << 24 |
                  (pixBytes[arrayIndex + 1] & 0xff) << 16 |
                  (pixBytes[arrayIndex + 2] & 0xff) << 8 |
                  (pixBytes[arrayIndex + 3] & 0xff));
               pval = (int) (fcval * pixFloat + fncon);
               bpix[i + offset] = (byte)pval;
               /* copy Gray Scale image values to Red, Green and Blue
                ipix[k]   = pval;
                ipix[k+1] = pval;
                ipix[k+2] = pval;
                k = (i+1) * 3;
               */
               arrayIndex = (i + 1) * 4;
            }
            offset = offset + slicepix;
         }
         // release resource
         pixBytes = null;
         Runtime.getRuntime().gc();
         Runtime.getRuntime().gc();
         Runtime.getRuntime().gc();
         // update statistics is not done for image stacks
         if (fileType == "imageFile") {
            // close the spider file
            din.close();
            // if imami is not set then write statistics to the header
            if (imami == 0) {
               imami = 1;
               dout = new RandomAccessFile(fileName, "rw");
               UtilFile utilFile = new UtilFile(utilParameters);
               utilFile.writeFileHeader(dout,nsam,nrow,nslice,imami,avg,sig,
                                                 iform,fmax,fmin,"big-endian");
               dout.close();
            }
         }
      }
      // little-endian
      else {
         // if fmin, fmax, average and standard deviation are not set, find them
         // and write to the header
         if (imami == 0) {
            din.mark(4*npix);
            float temp = 0;
            for (int i = 0; i < npix; i++) {
               int b1 = din.read();
               int b2 = din.read();
               int b3 = din.read();
               int b4 = din.read();
               temp = java.lang.Float.intBitsToFloat((b4 << 24)+
                                                   (b3 << 16)+(b2 << 8)+ b1);
               // add all the pixel values
               sumX = sumX + temp;

               sumXSquare = sumXSquare +  temp * temp ;
               // sets fmin and fmax
               if (i == 0) {
                  fmin = temp;
                  fmax = temp;
               }
               // resets fmin and fmax
               if (temp < fmin) fmin = temp;
               if (temp > fmax) fmax = temp;
            }
            // find average pixel values
            avg = (float) sumX/npix;
            sigTop = sumXSquare - (sumX * sumX)/npix;
            if ((sigTop < 0) || (npix == 1)) {
               String str =
               " found SQRT of -ve number OR divison by zero in Standard Deviation";
               System.out.println(str);
               System.exit(0);
            }
            sig = (float)Math.sqrt(sigTop/(double)(npix-1));
            din.reset();
         }
         String str1 = df.format(fmin);
         String str2 = df.format(fmax);
         String str = "";
         if (imami == 0) {
            // if found statistics then print average and standard deviation
            String str_1 = df2.format(avg);
            String str_2 = df2.format(sig);
            str = " fmin = "+str1+ " fmax = "+ str2 + " avg = "+ str_1+
                                   " sd = "+str_2 +"\n";
         }
         else {
            str = " fmin = "+str1+ " fmax = "+ str2 + "\n";
         }
         if (utilParameters.showHeaderInfo == true) {
            utilParameters.outputMessageTextArea.append(str);
         }

         fcval = (nmax - nmin) / (fmax - fmin);
         fncon = nmin - (fmin * fcval) ;
         for (int i = 0; i < npix; i++) {
            int b1 = din.read();
            int b2 = din.read();
            int b3 = din.read();
            int b4 = din.read();
            float pixelFloat = java.lang.Float.intBitsToFloat((b4 << 24)+(b3 << 16)
                                                                   +(b2 << 8)+ b1);
            pval    = (int) (fcval * pixelFloat + fncon);

            //ipix[i] = pval;
            bpix[i] = (byte)pval;

            //int k = 0;
            /* copy Gray Scale image values to Red, Green and Blue
            ipix[k] = pval;
            ipix[k+1] = pval;
            ipix[k+2] = pval;
            k = (i+1) * 3;
            */
         }
         // update statistics is not done for image stacks
         if (fileType == "imageFile") {
            // close the spider file
            din.close();
            // if imami is not set then write statistics to the header
            if (imami == 0) {
               imami = 1;
               dout = new RandomAccessFile(fileName, "rw");
               UtilFile utilFile = new UtilFile(utilParameters);
               utilFile.writeFileHeader(dout,nsam,nrow,nslice,imami,avg,sig,iform,
                                                     fmax,fmin,"little-endian");
               dout.close();
            }
         }
      }
   }

   public void readImageStack() throws IOException {

      // stores Image Array (normalized -128..127 [0...255]) for given
      // image stack in a byte array
      int noOfImages = 0;
      // by now the overall header is already read
      // find out # of images actually present
      din.mark(labbyt * maxim + nsam * nrow * 4 * maxim);
      for (int i = 1; i <= maxim; i++) {
         // retrieve important header variables
         din.skipBytes(26*4);
         if (fileFormat == "bigEndian")  {
            // read if image is preset
            inuse = (int) din.readFloat();
         }
         //little endian
         else {
            int b1 = din.read();
            int b2 = din.read();
            int b3 = din.read();
            int b4 = din.read();
            inuse = (int)java.lang.Float.intBitsToFloat(
                       (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);
         }

         if (inuse != 0) {
            noOfImages = noOfImages + 1;
         }
         // skip the header
         din.skipBytes(labbyt-27*4);
         // skip the image
         din.skipBytes(nsam*nrow*4);
      }
      din.reset();
      // allocate exact amount of memory
      bpixStackedImage = new byte [nsam*nrow*noOfImages];
      inuseImageNumbers = new int [noOfImages];
      int indexPix = 0;
      int indexImg = 0;
      for (int i = 1; i <= maxim; i++) {
         // retrieve important header values
         din.skipBytes(5*4);

         // check if statistics is set
         //big endian
         if (fileFormat == "bigEndian")  {
            imami = (int) din.readFloat();
            fmax = din.readFloat();
            fmin = din.readFloat();
            din.skipBytes(18 * 4);
            // check if the image is used
            inuse = (int) din.readFloat();
         }
         // little endian
         else {
            int b1 = din.read();
            int b2 = din.read();
            int b3 = din.read();
            int b4 = din.read();
            imami = (int)java.lang.Float.intBitsToFloat(
                              (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);

            b1 = din.read();
            b2 = din.read();
            b3 = din.read();
            b4 = din.read();
            fmax = java.lang.Float.intBitsToFloat(
                                        (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);
            b1 = din.read();
            b2 = din.read();
            b3 = din.read();
            b4 = din.read();
            fmin = java.lang.Float.intBitsToFloat(
                                         (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);
            din.skipBytes(18 * 4);
            // check if the image is used
            b1 = din.read();
            b2 = din.read();
            b3 = din.read();
            b4 = din.read();
            inuse = (int)java.lang.Float.intBitsToFloat(
                                         (b4 << 24)+(b3 << 16)+(b2 << 8)+ b1);
         }
         // skip rest of the header
         din.skipBytes(labbyt-27*4);

         if (inuse != 0) {
            indexImg = indexImg + 1;
            //inuseImageNumbers[indexImg - 1] = i;
            inuseImageNumbers[indexImg - 1] = inuse;
            // read the image data and store in bytes in bpix
            String str ="\n" +  "@" + Integer.toString(i)+":   "+"\n";
            utilParameters.outputMessageTextArea.append(str);
            readImage();
            // copy the pixel values to the larger array which holds
            // pixel values of the whole image stacks
            for (int j = 0; j < nsam*nrow; j++) {
               bpixStackedImage[indexPix + j] = bpix[j];
            }
            indexPix = nsam * nrow * indexImg;
         }
         else {
            din.skipBytes(nsam*nrow*4);
         }
      }
      // close image file
      din.close();
   }

   public byte [] getBpixStackedImage() {

      return bpixStackedImage;
   }

   public int [] getInuseImageNumbers() {

      return inuseImageNumbers;
   }

   public String [] getInuseImageNumbersAsStrings() {
      int arrayLength = inuseImageNumbers.length;
      String [] inuseImageNumbersAsStrings = new String [arrayLength];
      for (int i = 0; i < arrayLength; i++) {
         inuseImageNumbersAsStrings[i] = Integer.toString(inuseImageNumbers[i]);
      }
      return inuseImageNumbersAsStrings;
   }

   public byte [] getByteData(byte [] VolPixByte, String sliceAxis,
            int colLowVal, int colHighVal, int rowLowVal,
                  int rowHighVal, int sliceLowVal, int sliceHighVal,
                                              int volWidth, int volHeight) {
      // given the data array (SPIDER convention), slice direction and the
      // slice intervals, a byte array of the resulting volume will be returned.
      // data will be consistent with SPIDER convention of volume storage.
      byte [] subVolPixByte;
      int m = 0;
      int newWidth  = colHighVal - colLowVal + 1;
      int newHeight = rowHighVal - rowLowVal + 1;
      int newSlice  = sliceHighVal - sliceLowVal + 1;
      int arrayLen = newWidth * newHeight * newSlice;
      subVolPixByte = new byte[arrayLen];

      if (sliceAxis == "z") {
         for (int k = sliceLowVal-1; k <= sliceHighVal-1; k++) {
            for (int j = rowLowVal-1; j <= rowHighVal-1; j++) {
               for (int i = colLowVal-1; i <= colHighVal-1; i++) {
                  subVolPixByte[m] =
                    VolPixByte [i + j * volWidth + k * (volWidth * volHeight)];
                  m++;
               }
            }
         }
      }

      if (sliceAxis == "x") {
         for (int k = colLowVal-1; k <= colHighVal-1; k++) {
            for (int j = sliceLowVal-1; j <= sliceHighVal-1; j++) {
               for (int i = rowLowVal-1; i <= rowHighVal-1; i++) {
                  subVolPixByte[m] =
                     VolPixByte [i * volWidth + j * (volWidth * volHeight) + k];
                  m++;
               }
            }
         }
      }

      if (sliceAxis == "y") {
         for (int k = rowLowVal-1; k <= rowHighVal-1; k++) {
            for (int j = sliceLowVal-1; j <= sliceHighVal-1; j++) {
               for (int i = colLowVal-1; i <= colHighVal-1; i++) {
                  subVolPixByte[m] =
                    VolPixByte [i + j * (volWidth * volHeight) + k * volWidth];
                  m++;
               }
            }
         }
      }

      return subVolPixByte;
   }

   public float [] getFloatData(float [] VolPixFloat, String sliceAxis,
              int colLowVal, int colHighVal, int rowLowVal,
                    int rowHighVal, int sliceLowVal, int sliceHighVal,
                                                int volWidth, int volHeight) {
        // given the data array (SPIDER convention), slice direction and the
        // slice intervals, a float array of the resulting volume will be returned.
        // data will be consistent with SPIDER convention of volume storage.
        float [] subVolPixFloat;
        int m = 0;
        int newWidth  = colHighVal - colLowVal + 1;
        int newHeight = rowHighVal - rowLowVal + 1;
        int newSlice  = sliceHighVal - sliceLowVal + 1;
        int arrayLen = newWidth * newHeight * newSlice;
        subVolPixFloat = new float[arrayLen];

        if (sliceAxis == "z") {
           for (int k = sliceLowVal-1; k <= sliceHighVal-1; k++) {
              for (int j = rowLowVal-1; j <= rowHighVal-1; j++) {
                 for (int i = colLowVal-1; i <= colHighVal-1; i++) {
                    subVolPixFloat[m] =
                      VolPixFloat [i + j * volWidth + k * (volWidth * volHeight)];
                    m++;
                 }
              }
           }
        }

        if (sliceAxis == "x") {
           for (int k = colLowVal-1; k <= colHighVal-1; k++) {
              for (int j = sliceLowVal-1; j <= sliceHighVal-1; j++) {
                 for (int i = rowLowVal-1; i <= rowHighVal-1; i++) {
                    subVolPixFloat[m] =
                       VolPixFloat [i * volWidth + j * (volWidth * volHeight) + k];
                    m++;
                 }
              }
           }
        }

        if (sliceAxis == "y") {
           for (int k = rowLowVal-1; k <= rowHighVal-1; k++) {
              for (int j = sliceLowVal-1; j <= sliceHighVal-1; j++) {
                 for (int i = colLowVal-1; i <= colHighVal-1; i++) {
                    subVolPixFloat[m] =
                      VolPixFloat [i + j * (volWidth * volHeight) + k * volWidth];
                    m++;
                 }
              }
           }
        }

        return subVolPixFloat;
     }







      // orders data such that surface is created by Surf.java as if
      // viewed from top to bottom. Surf.java is written (historically)
      // such that viewing is done from left side of the object.
      public byte [] orderDataForSurfaceViewing(byte [] VolPixByte) {

      byte [] testArray;
      testArray = new byte[nsam*nrow*nslice];
      int m = 0;
      int mm =0;

      for (int k = 0; k <= nrow-1; k++) {
         for (int j = 0; j <= nsam-1; j++) {
            for (int i = 0; i <= nslice-1; i++) {
               // to mirror the surfcae with respect to the projection
               // at phi = 0, theta = and psi =0
               // mm = nsam*nrow*(nslice-1-i) + (nsam-1-j) + (k)*nrow ;

               mm = nsam*nrow*(i) + j + (k)*nsam ;
               testArray[m] = VolPixByte [mm];
               // modified the bytes as required by Surf.java
               // java uses only signed bytes: -128 ...... +127
               // JAI internally uses pixel scaling of 0,1,2,....255.
               // JAI internally takes care of conversion from (byte) casting to its internal 0 .. 255
               // conversion. (possibly by using the byte shift &0xff operator
               // SpiderImage scales the image from       0,   1,   2....127, 128, 129.... 254,255
               // casting   (byte) translates it to       0,   1,   2....127,-128,-127,.... -2, -1
               // Surf.java expects pixel scaling from -128,-127,-126....-1,    0,   1,....126,127
               if (testArray[m] > 0) {
                  testArray[m] = (byte) (testArray[m] - 128);
               }
               else {
                  testArray[m] = (byte) (testArray[m] + 128);
               }

               m++;
            }
         }
      }
      return  testArray;
   }

   public TiledImage makeTiledImageFromByteArray(byte [] pixelByteArray,
                                                     int width, int height) {
      // byte [] pixelBArray = pixelByteArray;
      // create a tiled image from a data array
      // DataBuffer dbuff = new DataBufferInt(ipix,ipix.length);
      // DataBuffer dbuff = new DataBufferFloat(fpix,fpix.length);
      DataBuffer dbuff = new DataBufferByte(pixelByteArray,pixelByteArray.length);

      // ColorSpace cs = ColorSpace.getInstance(ColorSpace.CS_sRGB);
      ColorSpace cs = ColorSpace.getInstance(ColorSpace.CS_GRAY);

      ColorModel cm =
             new ComponentColorModel(cs, new int[]{8},false,false,
                             // Transparency.OPAQUE,DataBuffer.TYPE_INT);
                             // Transparency.OPAQUE,DataBuffer.TYPE_FLOAT);
                                Transparency.OPAQUE,DataBuffer.TYPE_BYTE);
      /* for RGB color model
      ColorModel cm =
             new ComponentColorModel(cs, numBit ,false,false,
                                Transparency.OPAQUE,DataBuffer.TYPE_INT);
      */
      int[] band = {0};

      SampleModel sm =
                  // new PixelInterleavedSampleModel(dbuff.getDataType(),
                  // new PixelInterleavedSampleModel(DataBuffer.TYPE_INT,
                  // new PixelInterleavedSampleModel(DataBuffer.TYPE_FLOAT,
                     new PixelInterleavedSampleModel(DataBuffer.TYPE_BYTE,
                                                       width,height,1,width,band);
      /* for RGB color model
       SampleModel sm =
             new PixelInterleavedSampleModel(DataBuffer.TYPE_INT,
                                             nsam,nrow,3,nsam*3,band);
      */

      /* images with width less than 512 will have tile width = image width;
         images with width greater than 512 will have tile width = 512 pixels.
         This criteria applies to image height too. Tile size 512 seems to be
         the most efficient (as per JAI FAQ).
      */
      int tileX = 0;
      int tileY = 0;
      if (width <= 512) {
         tileX = width;
      }
      else {
         tileX = 512;
      }
      if (height <= 512) {
         tileY = height;
      }
      else {
         tileY = 512;
      }

      SampleModel smTile = sm.createCompatibleSampleModel(tileX,tileY);

      WritableRaster raster =
             Raster.createWritableRaster(sm, dbuff, new Point(0,0));

      // Use the following if img is type BufferedImage
      // img = new BufferedImage(cm, raster, false, null);
      // return img;

      TiledImage ti = new TiledImage(0,0,width,
                                      height,0,0,smTile,cm);
      ti.setData(raster);

      // Use the following if img is of type RenderedImage
      // RenderedImageAdapter img = new RenderedImageAdapter ((RenderedImage)ti);
      // return img;

      // Do this if you want to save memory. Might be a bit slow because of
      // garbage collection
      // Destroy the extra copy (bpix) and do garbage collection
      // bpix = null;
      // Runtime.getRuntime().gc();
      // Runtime.getRuntime().gc();
      // Runtime.getRuntime().gc();
      return ti;
   }

   // used in Pixel and other operations where the actual file data is needed.
   // fills numbers in [] fpix
   public void readRawData() throws IOException {

      din = new DataInputStream
                     (new BufferedInputStream(new FileInputStream(fileName)));
      din.skipBytes(labbyt);
      int numPix = nsam*nrow*nslice;
      fpix = new float[numPix];

      // big-endian
      if (fileFormat == "bigEndian") {
         byte [] pixBytes = new byte [numPix*4];
         // read all image pixel data in byte and store them in array pixBytes
         // all the data is read at a time OR ELSE it is extremely slow in SGI
         din.read(pixBytes);
         int arrayIndex = 0;

         for (int i = 0; i < numPix; i++) {
            //find each pixel values in float
            fpix[i] = java.lang.Float.intBitsToFloat
                           ((pixBytes[arrayIndex]&0xff)<<24 |
                                   (pixBytes[arrayIndex+1]&0xff)<<16 |
                                        (pixBytes[arrayIndex+2]&0xff)<<8 |
                                                (pixBytes[arrayIndex+3]&0xff));
            arrayIndex = (i+1)*4;
         }
         // release resources
         pixBytes = null;
         Runtime.getRuntime().gc();
         Runtime.getRuntime().gc();
         Runtime.getRuntime().gc();
      }
      // little-endian
      else {
         for (int i = 0; i < numPix; i++) {
            int b1 = din.read();
            int b2 = din.read();
            int b3 = din.read();
            int b4 = din.read();
            float pixelFloat = java.lang.Float.intBitsToFloat((b4 << 24)+(b3 << 16)
                                                                   +(b2 << 8)+ b1);
            // save actual pixel value in float array
            fpix[i] = pixelFloat;
            din.close();
         }
      }
   }
}
