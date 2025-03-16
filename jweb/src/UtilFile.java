package web;

import java.io.*;
import javax.swing.*;
import java.util.*;
import javax.swing.filechooser.FileFilter;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilFile extends JFrame {
  UtilParameters utilParameters;
  String fileType, fileTemplate;

   public UtilFile(UtilParameters utilParam) {
      utilParameters = utilParam;

   }
   // This method returns a string array with incremental file names when the seed name
   // and the last file number is given. Array contains only the file names that exist
   // in the directory.
   public String[] makeFileNameArray(String file, int endFileNumber){
      // Copy file name and last file number
      String fileName = file;
      int lastFileNum = endFileNumber;

      // initialize number of files
      int numFiles = 1;

      // find file extension and rest of the file name
      int index_1 = fileName.lastIndexOf(".");
      String fileExtension = fileName.substring(index_1 + 1);
      String fileNameWOExtension = fileName.substring(0,index_1);
      char [] fileNameWOECharArray = fileNameWOExtension.toCharArray();
      int indexCounter = 0;
      // Loop from the character right before "." to the end of the file name
      for (int i = index_1-1; i >= 0 ; i--) {
         if (Character.isDigit(fileNameWOECharArray[i])) {
            // save the index if no character is encountered in the file name
            // i.e. if file name consists of numbers only
            indexCounter = i;
         }
         else {
            // Encountered Character. Exit if the first one is not a digit.
            if (i == index_1-1){
               // If no numbers at the end of the file name
               System.out.println("Invalid File Names for Montage");
               System.exit(0);
            }
            // save the index where encountered character
            indexCounter = i;
            // stop the loop
            i = -1;
         }
      }

      String stringBeforeNumeric = fileName.substring(0,indexCounter+1);
      String numericString = fileName.substring(indexCounter+1,index_1);
      // find # of characters in the number string
      int numericStringLength = numericString.length();
      int valueOfString = Integer.parseInt(numericString);
      int maxNumFiles = lastFileNum - valueOfString + 1;
      // make an array with maximum possible elements within the start and end value
      // already known from user inputs
      String [] fileNames = new String[maxNumFiles];
      fileNames[0] = fileName;

      // Construct the new file names and check if they exist in the directory
      for (int i = valueOfString; i < lastFileNum; i++) {
         valueOfString = valueOfString + 1;
         String incrementedNumericString = Integer.toString(valueOfString);
         // find # of characters in the new string
         int incrementedNumericStringLength = incrementedNumericString.length();

         // add zeros before the number as per compatibality of SPIDER file names
         for (int j = 0; j < numericStringLength -
                                       incrementedNumericStringLength; j++) {
          incrementedNumericString = "0" + incrementedNumericString;
         }
         String newFileName = stringBeforeNumeric.concat(incrementedNumericString).
                                    concat(".").concat(fileExtension);
         File newFile = new File(newFileName);
         if (newFile.exists()) {
            fileNames[numFiles] = newFileName;
            numFiles = numFiles + 1;
         }
      }
      // make a string array of size equal to the available number of files in the
      // series (excluding the missing ones) and return it.
      String [] fileNamesArray = new String [numFiles];
      for (int i = 0; i < numFiles; i++){
         fileNamesArray [i] = fileNames[i];
      }
      return fileNamesArray;
   }

  // return an array of file names, given a template and file number array
  // usually retrieved from a document file
   public String [] makeFileNameArrayFromNumbers
                                        (String template, int [] fileNumArray){
      fileTemplate = template;
      int [] fileNumArraySorted;
      String [] fileNames = new String[fileNumArray.length];
      String fileNameStr = "";
      String fileExtension = "";
      // check template validity
      int indexDot = fileTemplate.indexOf(".");
      String checkStar = "";
      // if . present implying file extension present then check if * is before . .
      if (indexDot != -1) {
         checkStar = fileTemplate.substring(indexDot-1,indexDot);
         fileExtension = fileTemplate.substring(indexDot,fileTemplate.length());
      }
      if ((indexDot == -1) || ( checkStar.compareTo("*") != 0   )) {
         String str = "invalid template, no file extension or no * before . !\n";
         utilParameters.outputMessageTextArea.append(str);
      }
      // template is valid
      else {
         // find how many *'s are supplied
         int endIndex = fileTemplate.lastIndexOf("*");
         String testStr = "*";
         int numOfStar = 0;
         while ( testStr.compareTo("*") == 0) {
            testStr = fileTemplate.substring
                                    (endIndex-numOfStar, endIndex-numOfStar+1);
            numOfStar++;
         }
         numOfStar = numOfStar - 1;
         // find the file name string without the *'s
         fileNameStr = fileTemplate.substring
                                     (0,(fileTemplate.indexOf("*")-numOfStar+3));
         fileNumArraySorted = new int [fileNumArray.length];
         // copy fileNumArray to fileNumArraySorted
         for (int i = 0; i < fileNumArray.length; i++) {
            fileNumArraySorted[i] = fileNumArray[i];
         }
         Arrays.sort(fileNumArraySorted);
         // find what is the max character in the max number in the fileNumArray
         int maxNumOfChar = (Integer.toString
                 (fileNumArraySorted[fileNumArraySorted.length - 1])).length();
         // check validity
         if (maxNumOfChar > numOfStar) {
            String str = "invalid template, less no. of *'s supplied !\n";
            utilParameters.outputMessageTextArea.append(str);
         }
         // template is valid
         else {
            for (int i = 1; i <= fileNumArray.length; i++) {
               String  fileNameStrPlusZero = fileNameStr;
               int numOfChar = (Integer.toString(fileNumArray[i - 1])).length();
               int numOfZero =  numOfStar - numOfChar;
               for (int j = 1; j <= numOfZero; j++) {
                  // add appropriate # of 0's before the file number
                  fileNameStrPlusZero = fileNameStrPlusZero + "0";
               }
               // construct the file name
               fileNames[i-1] = fileNameStrPlusZero +
                         Integer.toString(fileNumArray[i - 1]) + fileExtension;
            }
         }
      }
      // return file name array
      return fileNames;
   }

   // returns an array of file names, given an array of filename strings, a file number
   // and a fileExtension. for example, if [c:\abc, c:\def, c:\ghi], 7 and dat are given then
   // the the filenames array that will be created will be, c:\abc007.dat, c:\def007.dat and
   // c:\ghi007.dat.

   public String [] makeFileNameArrayFromStrings
                    (String [] templateArray, int fileNum, String fileExtStr){

      String [] fileNames = new String[templateArray.length];
      int fileNumLen = Integer.toString(fileNum).length();
      String fileNumStr;
      fileNumStr = Integer.toString(fileNum);
      if (fileNumLen < 3) {
         int fileNumLess = 3 - fileNumLen;
         for (int i = 1; i <= fileNumLess ; i++) {
            fileNumStr = "0" + fileNumStr;
         }
      }
      for (int i = 1; i <= templateArray.length ; i++) {
         // construct the file name
         fileNames[i - 1] = templateArray[i - 1] + fileNumStr + "." + fileExtStr;
      }
      // return file name array
      return fileNames;
   }

   // returns the file number, given the file name string
   public int findFileNumber(String file){

      // Copy file name
      String fileName = file;

      // find file extension and rest of the file name
      int index_1 = fileName.lastIndexOf(".");
      String fileExtension = fileName.substring(index_1 + 1);
      String fileNameWOExtension = fileName.substring(0,index_1);
      char [] fileNameWOECharArray = fileNameWOExtension.toCharArray();
      int indexCounter = 0;
      // Loop from the character right before "." to the end of the file name
      for (int i = index_1-1; i >= 0 ; i--) {
         if (Character.isDigit(fileNameWOECharArray[i])) {
            // save the index if no character is encountered in the file name
            // i.e. if file name consists of numbers only
            indexCounter = i;
         }
         else {
            // Encountered Character. Exit if the first one is not a digit.
            if (i == index_1-1){
               // If no numbers at the end of the file name
               System.out.println("Invalid File Names for Montage");
               System.exit(0);
            }
            // save the index where encountered character
            indexCounter = i;
            // stop the loop
            i = -1;
         }
      }

      String numericString = fileName.substring(indexCounter+1,index_1);
      int fileNumber = Integer.parseInt(numericString);
      return fileNumber;
   }

   // selects multiple files and returns the fileNames array
   public String [] selectMultipleFiles () {

      String [] fileNames;
      JFileChooser jFileChooser1 = new JFileChooser();
      jFileChooser1.setCurrentDirectory(new File(utilParameters.currDirectory));
      jFileChooser1.setMultiSelectionEnabled(true);

      if ( !utilParameters.dataFileExtension.equals("*") ) {
      // filters only the files with extension as set in utilParameters
      // and the directories
      jFileChooser1.setFileFilter(new FileFilter(){
         public String getDescription() {
            return utilParameters.dataFileExtension;
         }
         public boolean accept(File file) {
            if (file.isDirectory()) {
               return true;
            }
            String name = file.getName().toLowerCase();
            if(name.endsWith(utilParameters.dataFileExtension)) {
               return true;
            }
               return false;
         } // accept()
      });

      }
      // get multiple files selected in an array of file objects "files"
      if (JFileChooser.APPROVE_OPTION == jFileChooser1.showOpenDialog(this)) {
         File [] files = jFileChooser1.getSelectedFiles();
         utilParameters.currDirectory = files[0].getPath();
         // get # of files selected
         int numFiles = files.length;
         // if only one file is selected then solicit last file number
         if(numFiles == 1) {
            String fileName = files[0].getPath();
            utilParameters.showHeaderInfo = false;
            // make a test spider image to check what type os file it is
            SpiderImage spiImg = new SpiderImage(fileName, utilParameters);
            utilParameters.showHeaderInfo = true;
            fileType = spiImg.getFileType();
            // if an image file
            if  (fileType == "imageFile") {
               String lastFileNumString = JOptionPane.showInputDialog(
                                         "Enter Last File Number for Montage");
               if (lastFileNumString != null) {
                  int lastFileNum = Integer.parseInt(lastFileNumString);
                  fileNames = makeFileNameArray(fileName,lastFileNum);
                  // Update numFiles
                  numFiles = fileNames.length;
               }
               else {
                  fileNames = null;
               }
            }
            // construct file name array from document file
            else if (fileType == "documentFile") {
               String op = "r";
               int registerNumber = 1;
               boolean KeyDuplicationAllowed = false;
               UtilDoc utilDoc = new UtilDoc(fileName,op,utilParameters);
               float [] regVal = utilDoc.readDoc
                                       (registerNumber, KeyDuplicationAllowed);
               int [] regValInt = new int[regVal.length];
               for (int i = 0; i < regVal.length; i++) {
                  regValInt[i] = (int)regVal[i];
               }
               String template = JOptionPane.showInputDialog(
                                   "Enter File Name Template e.g. AVG * * *.DAT");
               if (template != null) {
                  String filePath = files[0].getPath();
                  // unix
                  int lastFound = filePath.lastIndexOf("//");
                  if (lastFound == -1) {
                     // windows
                     lastFound = filePath.lastIndexOf("\\");
                  }
                  String dirStr = filePath.substring(0,lastFound+1);
                  template = dirStr + template;
                  fileNames = makeFileNameArrayFromNumbers(template, regValInt);
               }
               else {
                  fileNames = null;
               }
               return fileNames;
            }
            // image Stack
            else {
               fileNames = new String[1];
               fileNames[0] = fileName;
            }
         }
         // more than one file selected by highlighting with a mouse
         else {
            fileNames = new String[numFiles];
            // construct the fileNames array
            for (int i =0; i < numFiles; i++) {
               // construct the fileNames array
               fileNames[i] = files[i].getPath();
            }
         }
      }
      else {
      // returns null, catch it in caller to verify if files got selected or
      // user closed the window.
         fileNames = null;
      }
      return fileNames;
   }


   public String selectAFile () {

      String s;
      JFileChooser jFileChooser1 = new JFileChooser();
      jFileChooser1.setCurrentDirectory(new File(utilParameters.currDirectory));

      if ( !utilParameters.dataFileExtension.equals("*") ) {
         // filters only the files with extension as set in utilParameters
         // and the directories
         jFileChooser1.setFileFilter(new FileFilter(){
            public String getDescription() {
               return utilParameters.dataFileExtension;
            }
            public boolean accept(File file) {
               if (file.isDirectory()) {
                  return true;
               }
               String name = file.getName().toLowerCase();
               if(name.endsWith(utilParameters.dataFileExtension)) {
                  return true;
               }
                  return false;
            } // accept()
         });
      }
      if (JFileChooser.APPROVE_OPTION == jFileChooser1.showOpenDialog(this)) {
         s = jFileChooser1.getSelectedFile().getPath();
         utilParameters.currDirectory = s;
      }
      else {
         // returns null if file is not selected. can be caught in caller to
         // show error message
         s = null;
      }
      return s;
   }

   public boolean check2DOr3D (String fileName) {
      // checks if the image is a single image (2D) or a volume (3D)

      boolean twoDImage = true;
      int iform;
      SpiderImage spiImage = new SpiderImage(fileName,utilParameters);
      iform = spiImage.getiform();
      if (iform == 3) {
         // 3D image
         twoDImage = false;
      }
      else if (iform != 1) {
        // print error message
        String str = "wrong value for the header variable iform !\n";
        utilParameters.outputMessageTextArea.append(str);
      }

   return  twoDImage;
   }




   // tests if the images are of same size. returns true if the images are
   // of same size OR ELSE returns false
   public boolean areSameSizeImages (String [] fileNames) {

      boolean sameSizeImages = true;
      int numFiles = fileNames.length;

      // Don't show header info of the spider images. Only the montage
      // image width and height is calculated. Header info will be shown when the
      // image is actually read and montage image is constructed.
      utilParameters.showHeaderInfo = false;
      int imgWidth = 0;
      int imgHeight = 0;
      // read the image headers. find out if the images are of same size.
      // find the size of the larger array that is going to hold the sum of
      // pixels (in bytes) of individual images. pixel values from all
      // images are going to be stored as a spider volume. each image starting
      // from the first one is treated as a slice of the volume.
      for (int i =0; i < numFiles; i++) {
         String fileName = fileNames[i];
         SpiderImage spiImage = new SpiderImage(fileName,utilParameters);
         if ( i == 0) {
            imgWidth = spiImage.nsam;
            imgHeight = spiImage.nrow;
         }
         if  ((imgWidth != spiImage.nsam) || (imgHeight != spiImage.nrow)) {
            sameSizeImages = false;
         }
      }
      // show header info ("reset to default") of the spider images.
      utilParameters.showHeaderInfo = true;
      return sameSizeImages;
   }

   // returns how many images (all of them having same dimension) can
   // fit into a row in the host computer's screen
   public int findNoOfImgPerRow (String [] fileNames) {

      int numFiles = fileNames.length;
      SpiderImage spiImage = new SpiderImage(fileNames[0],utilParameters);
      int imgWidth  = spiImage.getnsam();
      // find scaling of the image
      float scale;
      if (utilParameters.resizeLarge) {
         scale = utilParameters.resizeFactor;
      }
      else {
         scale = (1/utilParameters.resizeFactor);
      }
      int scrnWid = utilParameters.screenDimension.width - 50;
      // reset the screen width as per the image scaling
      // the image could be scaled in ImageFrame.java
      scrnWid = (int)(scrnWid/scale) ;

      int noOfImgPerRow = (scrnWid)/imgWidth;
      if (noOfImgPerRow == 0) noOfImgPerRow =1;
      if ( numFiles <= noOfImgPerRow) noOfImgPerRow = numFiles;
      return noOfImgPerRow;
   }

   // returns the montaged image width for the image series
   // (all of them having same dimension) given the no. of images per row
   public int findMontagedImageWidth (String [] fileNames, int noOfImgPerRow) {
      SpiderImage spiImage = new SpiderImage(fileNames[0],utilParameters);
      int imgWidth  = spiImage.getnsam();
      // add noOfImgPerRow -1 pixels for a blank pixel after each image
      int montagedImageWidth = noOfImgPerRow * imgWidth + noOfImgPerRow -1;
      return montagedImageWidth;
   }

   // returns the montaged image height for the image series
   // (all of them having same dimension) given the no. of images per row
   public int findMontagedImageHeight (String [] fileNames, int noOfImgPerRow) {

      int numFiles = fileNames.length;
      SpiderImage spiImage = new SpiderImage(fileNames[0],utilParameters);
      int imgHeight  = spiImage.getnrow();
      int k = numFiles/noOfImgPerRow;
      int l = numFiles % noOfImgPerRow;

      if (l != 0)  k = k + 1;
      // add k-1 pixels for a blank pixel after each image
      int montagedImageHeight = k*imgHeight + k-1;
      return montagedImageHeight;
   }

  // makes and returns a byte array from a series of images. reads individual
  // image data and writes them to the array that is to be returned.
  // data is stored like a spider volume data.
   public byte [] makeVolumeFromImageSeries (String [] fileNames) {

      int numFiles = fileNames.length;
      utilParameters.showHeaderInfo = false;
      SpiderImage spiTempImage = new SpiderImage(fileNames[0],utilParameters);
      int imgWidth = spiTempImage.getnsam();
      int imgHeight = spiTempImage.getnrow();
      int stackedImageSize = imgWidth * imgHeight * numFiles;
      // show header info ("reset to default") of the spider images.
      utilParameters.showHeaderInfo = true;

      byte [] stackedImagePixels = new byte[stackedImageSize];

      int startIndex = 0;
      for (int i =0; i < numFiles; i++) {
         String fileName = fileNames[i];
         // make a spider image
         SpiderImage spiImage = new SpiderImage(fileName,utilParameters);
         try {
            // read float data and convert to normalized byte data
            spiImage.readImage();
         }
         catch (IOException er) {
            String str = "Error reading SpiderImage data: " + er + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
         for (int j = 0; j < imgWidth * imgHeight; j++) {
            // copy individual image byte data into the array.
            // this large array holds data the same way a spider
            // volume data is stored
            stackedImagePixels [startIndex + j] = spiImage.bpix[j];
         }
         //the array index starts with 0. do necessary increment to the index
         startIndex = startIndex + (imgWidth * imgHeight) ;
      }
       return stackedImagePixels;
   }

   // returns a byte array created from the average of a series of images. reads individual
   // image data (normalized, 0-255, byte)and finds the average pixel values, writes
   // them to the array that is to be returned.
   public byte [] makeAnAverageFromImageSeries (String [] fileNames) {

      int numFiles = fileNames.length;
      utilParameters.showHeaderInfo = false;
      SpiderImage spiTempImage = new SpiderImage(fileNames[0],utilParameters);
      int imgWidth = spiTempImage.getnsam();
      int imgHeight = spiTempImage.getnrow();
      int averageImageSize = imgWidth * imgHeight ;
      // show header info ("reset to default") of the spider images.
      utilParameters.showHeaderInfo = true;

      int [] averageImagePixels = new int[averageImageSize];
      byte [] averageImagePixelsByte = new byte[averageImageSize];

      for (int i =0; i < numFiles; i++) {
         String fileName = fileNames[i];
         // make a spider image
         SpiderImage spiImage = new SpiderImage(fileName,utilParameters);
         try {
            // read float data and convert to normalized byte data
            spiImage.readImage();
         }
         catch (IOException er) {
            String str = "Error reading SpiderImage data: " + er + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
         for (int j = 0; j < imgWidth * imgHeight; j++) {
            // copy individual image byte data into the array.
            // this large array holds data the same way a spider
            // volume data is stored
            averageImagePixels [j] = averageImagePixels [j] + (int)spiImage.bpix[j];
/*
            // if single image read Spider raw data and save in float [] spiderImage.fpix.
            try {
               spiImage.readRawData();
            }
            catch (IOException e) {
               String str = "Error reading SpiderImage raw data: UtilGUIPixel " + e + "\n";
               utilParameters.outputMessageTextArea.append(str);
            }
            averageImagePixels [j] = averageImagePixels [j] + (int)spiImage.fpix[j];
*/
         }
      }
      for (int j = 0; j < imgWidth * imgHeight; j++) {
         // find the average and convert it to byte data
         averageImagePixels [j] = averageImagePixels [j] / numFiles ;
         averageImagePixelsByte[j] = (byte)averageImagePixels[j];
      }

       return averageImagePixelsByte;
    }

   // reads a no of sets of images finds average of each set, creates a montage
   // of these average images and returns a byte array created from the series
   // of average images. data is stored like a spider volume data.
   // classInfo array of size (no. of classes  X 5) with the following information
   // ( Begining key #, End key #, class #, # of Particles in the class, Height)
   /*
   public byte [] makeVolumeFromClassAvgsForDendrogram (int [][] classInfo,
                                        int noOfClasses, String templateStr) {


      for (int k =0; k < noOfClasses; k++) {
        for (int j = 0; j < classInfo[k][3]; j++) {

        }
        makeFileNameArrayFromNumbers (templateStr, int [] fileNumArray)




        int numFiles = fileNames.length;
        utilParameters.showHeaderInfo = false;
        SpiderImage spiTempImage = new SpiderImage(fileNames[0], utilParameters);
        int imgWidth = spiTempImage.getnsam();
        int imgHeight = spiTempImage.getnrow();
        int stackedImageSize = imgWidth * imgHeight * numFiles;
        // show header info ("reset to default") of the spider images.
        utilParameters.showHeaderInfo = true;

        byte[] stackedImagePixels = new byte[stackedImageSize];
        int startIndex = 0;
        for (int i = 0; i < numFiles; i++) {
          String fileName = fileNames[i];
          // make a spider image
          SpiderImage spiImage = new SpiderImage(fileName, utilParameters);
          try {
            // read float data and convert to normalized byte data
            spiImage.readImage();
          }
          catch (IOException er) {
            String str = "Error reading SpiderImage data: " + er + "\n";
            utilParameters.outputMessageTextArea.append(str);
          }
          for (int j = 0; j < imgWidth * imgHeight; j++) {
            // copy individual image byte data into the array.
            // this large array holds data the same way a spider
            // volume data is stored
            stackedImagePixels[startIndex + j] = spiImage.bpix[j];
          }
          //the array index starts with 0. do necessary increment to the index
          startIndex = startIndex + (imgWidth * imgHeight);
        }
      }
      return stackedImagePixels;
   }
*/
   public void writeFileHeader (RandomAccessFile dout, int nsam, int nrow,
                  int nslice, int imami, float avg, float sig, int iform,
                     float fmax, float fmin, String format) throws IOException{

      int lenbyt = nsam*4;
      int labrec = 1024/lenbyt;
      if ((1024 % lenbyt) != 0) labrec = labrec + 1;
      int labbyt = labrec * lenbyt;

      float [] headerArrayOne = new float [211];

      for (int i = 0; i < 211; i++) {
         headerArrayOne[i] = 0;
      }
      headerArrayOne[0] = (float)nslice;
      headerArrayOne[1] = (float)nrow;
      headerArrayOne[4] = (float)iform;
      headerArrayOne[5] = (float)imami;
      headerArrayOne[6] = fmax;
      headerArrayOne[7] = fmin;
      headerArrayOne[8] = avg;
      headerArrayOne[9] = sig;
      headerArrayOne[11] = (float)nsam;
      headerArrayOne[12] = (float)labrec;
      headerArrayOne[21] = (float)labbyt;
      headerArrayOne[22] = (float)lenbyt;

      // if image is created using windows machine
      if ( format == "little-endian") {
         byte  [] headerArrayByte = new byte [211*4];
         int j = 0;
         for (int i = 0; i < 211; i++) {
            int k = java.lang.Float.floatToIntBits(headerArrayOne[i]);
            // by casting to byte we take only the last 8 bits of information
            // other values are lost
            headerArrayByte[j]   = (byte) k;
            // right shift 8 bits and cast byte
            headerArrayByte[j+1] = (byte) (k >> 8);
            // right shift 16 bits and cast byte
            headerArrayByte[j+2] = (byte) (k >> 16);
            // right shift 24 bits and cast byte
            headerArrayByte[j+3] = (byte) (k >> 24);
            j = j + 4;
         }
         for (int i = 0; i < 211*4; i++) {
            dout.writeByte(headerArrayByte[i]);
         }
      }
      // if image is created using unix machine
      else {
         for (int i = 0; i < 211; i++) {
            dout.writeFloat(headerArrayOne[i]);
         }
      }
      Date date = new Date();
      String str = new String();
      str = date.toString() ;

      // added a space at the end just to make it 12 bytes i.e. divisible
      // by 4. may not confirm to spider format

      String dateString = str.substring(8,10) + "-" + str.substring(4,7)
                       + "-" + str.substring(str.length()-4,str.length())+ " ";
      String timeString = str.substring(11,19);
      //char [] dateCharArray = dateString.toCharArray();
      //char [] timeCharArray = timeString.toCharArray();
      dout.writeBytes(dateString);
      dout.writeBytes(timeString);

      //for big or little endian the code remains same as the values are zeros.
      for (int i = 217; i <= labbyt/4; i++) {
         dout.writeFloat(0);
      }
   }

   // writes a spider image file. Always written in big-endian (UNIX) format.
   public void writeSpiderFile (String spiderFileName,float [] floatData,
          int nsam, int nrow, int nslice, int imami, float avg, float sig,
                       int iform, float fmax, float fmin) throws IOException{

      String format = "big-endian";
      RandomAccessFile  dout;
      dout = new RandomAccessFile(spiderFileName, "rw");
      writeFileHeader(dout,nsam,nrow,nslice,imami,avg,sig,iform,fmax,fmin,format);
      try {
         int length = floatData.length;
         for (int i = 0; i < length; i++) {
            dout.writeFloat(floatData[i]);
         }
         dout.close();
      }
      catch (Exception b) {
         System.err.println(b.getMessage());
         utilParameters.outputMessageTextArea.append
                                   ("Error writting body of the SPIDER file");
      }
   }
}
