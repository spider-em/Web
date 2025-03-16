package web;


import java.awt.*;
import java.io.*;
import java.text.*;
import java.util.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilDoc {
   private PrintWriter out;
   private BufferedReader in;
   private int keyVal, numOfReg;
   private float [] regValArray = new float[9];
   private int maxKey = 0;
   private int maxReg = 0;
   private int numOfInputKeys = 0;
   private int lastKeyNum;
   private int extraOccurance = 0;
   private int arrayLength = 0;
   private String readFileName;
   private String commentsToWrite;
   private UtilParameters utilParameters;

   // used for reading a document file. sets values of maxKey, maxReg,
   // numOfInputKeys and lastKeyNum
   public UtilDoc(String fileName, String operation, UtilParameters utilParam) {

      readFileName = fileName;
      String read = "r";
      utilParameters = utilParam;
      if (operation == read) {
         in = openReadDoc(readFileName);
         int keyNum = 0;
         int NoOfReg = 0;
         try {
            String s = new String();
            while ((s = in.readLine()) != null) {
               // ignore if a blank line is encountered
               if (s.trim().compareTo("") !=0) {
                  // if not commented (trim the white space before checking)
                  // due to some reason if (s.trim().substring(0,1) != ";")
                  // doesn't work
                  if (s.trim().substring(0,1).compareTo(";") != 0)  {
                     keyNum = Integer.parseInt(s.substring(1,5).trim());
                     if (keyNum > maxKey) maxKey = keyNum;
                     numOfInputKeys = numOfInputKeys + 1;
                     NoOfReg = Integer.parseInt(s.substring(6,7).trim());
                     if (NoOfReg > maxReg) maxReg = NoOfReg;
                  }
               }
            }
            lastKeyNum = keyNum;
         }
         catch(IOException er) {
            System.err.println(er.getMessage());
         }
         closeReadDoc(in);
      }
   }

   public UtilDoc(String fileName, int key, int numReg, float [] regArray,
                      String operation, UtilParameters utilParam) {
      keyVal = key;
      regValArray = regArray;
      numOfReg = numReg;
      String write = "w";
      utilParameters = utilParam;
      if (operation == write) {
         try {
            out = new PrintWriter(new BufferedWriter
                                 (new FileWriter(fileName,true)));
         }

         catch(IOException er) {
         System.err.println(er.getMessage());
         }
      }
   }

   // writes a comments line to the document file
   public UtilDoc(String fileName,  String commentsIn,
                         String operation, UtilParameters utilParam) {

         String write = "w";
         utilParameters = utilParam;
         commentsToWrite = commentsIn;
         if (operation == write) {
            try {
               out = new PrintWriter(new BufferedWriter
                                    (new FileWriter(fileName,true)));
            }

            catch(IOException er) {
            System.err.println(er.getMessage());
            }
         }
      }

      public void writeCommentsDoc() {
             String str1 = new String();
             str1 = "; " + commentsToWrite;
             str1 = str1.concat("\n");
             utilParameters.outputMessageTextArea.append(str1);
             out.print(str1);
       }




   public void writeDoc() {

      if (keyVal <= 9999) {
         DecimalFormat df1 = new DecimalFormat("0000");
         DecimalFormat df2 = new DecimalFormat("######0.000000");
         String str1, str2 = new String();
         String spaceString = new String();
         spaceString = " ";
         str1 = df1.format(keyVal);
         str1 = " " + str1 + " ";
         str1 = str1 + Integer.toString(numOfReg);

         for (int i = 0; i < numOfReg; i++) {
            str2 = df2.format(regValArray[i]);
            int index = str2.indexOf(".");
            int space = 8 - index;
            for (int j = 1; j < space; j++) {
               str2 = " "+str2;
            }
            str1 = str1 + str2;
         }
         str1 = str1.concat("\n");
         utilParameters.outputMessageTextArea.append(str1);
         out.print(str1);
      }
      else {
         String str = "Key number should be less than 10000 \n";
         utilParameters.outputMessageTextArea.append(str);
      }
   }

   // reads a register and returns the values in a float array
   // if registerNumber = 0, then the array of key numbers is returned
   public float [] readDoc(int registerNumber, boolean KeyDuplicationAllowed) {

      float [] registerValueArray;
      if (KeyDuplicationAllowed) {
         findArrayLength(registerNumber);
         if (arrayLength != 0) {
            registerValueArray =
                     makeRegValArrayWithKeyRepeat(registerNumber);
            return registerValueArray;
         }
         else {
            String str = "register "+Integer.toString(registerNumber)
                                               +" doesn't exist ! \n";
            utilParameters.outputMessageTextArea.append(str);
            return null;
         }
      }
      // if key duplication is not allowed
      else {
         int [][] keyDuplicationRefArray;
         keyDuplicationRefArray = findKeyDuplicationRefArray(registerNumber);
         registerValueArray = makeRegValArrayWithoutKeyRepeat
                                     (keyDuplicationRefArray, registerNumber);
         return registerValueArray;
      }
   }

   // finds array length required to hold a particular register column that is
   // read from a doc file
   private void findArrayLength(int registerNumber) {
      //initialize arrayLength
      arrayLength = 0;
      int noOfReg = 0;
      in = openReadDoc(readFileName);
      String s = new String() ;
      try {
         while ((s = in.readLine()) != null) {
            // ignore if a blank line is encountered
            if (s.trim().compareTo("") !=0) {
               // if not commented (trim the white space before checking)
               // due to some reason if (s.trim().substring(0,1) != ";")
               // doesn't work
               if (s.trim().substring(0,1).compareTo(";") != 0) {
                  noOfReg = Integer.parseInt(s.substring(6,7).trim());
                  // count only the lines that contain the requested register
                  if (noOfReg >= registerNumber) {
                     arrayLength = arrayLength + 1;
                  }
               }
            }
         }
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
      }
      closeReadDoc(in);
   }

   // return a float array of the requested register values
   private float [] makeRegValArrayWithKeyRepeat(int registerNumber) {

      float [] registerValueArray = new float [arrayLength];
      int noOfReg = 0;
      int index = 0;

      in = openReadDoc(readFileName);
      String s = new String();
      try {
         while ((s = in.readLine()) != null) {
            // ignore if a blank line is encountered
            if (s.trim().compareTo("") !=0) {
               // if not commented (trim the white space before checking)
               // due to some reason if (s.trim().substring(0,1) != ";")
               // doesn't work
               if (s.trim().substring(0,1).compareTo(";") != 0) {
                  noOfReg = Integer.parseInt(s.substring(6,7).trim());
                  // find register value
                  if (noOfReg >= registerNumber) {
                     // registerNumber = 0 means the array with key numbers
                     // will be returned
                     if (registerNumber == 0) {
                        registerValueArray[index] =
                               Integer.parseInt(s.substring(1,5).trim());
                     }
                     else {
                        registerValueArray[index] =
                           Float.parseFloat(s.substring
                        ((7+(registerNumber-1)*12),(7+(registerNumber*12))));
                     }
                     index = index + 1;
                  }
               }
            }
         }
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
      }
      closeReadDoc(in);
      return registerValueArray;
   }

   // returns a 2D array with first column with key numbers and 2nd column
   // with how many times the key is repeated in the doc file (with the
   // requested register)
   private int [][] findKeyDuplicationRefArray(int registerNumber){

      // initialize arrayLength and extraOccurance
      arrayLength = 0;
      extraOccurance =0;
      int keyNum = 0;
      int noOfReg = 0;
      int [] keyArray = new int [numOfInputKeys];
      in = openReadDoc(readFileName);
      try {
         String s = new String();
         while ((s = in.readLine()) != null) {
            // ignore if a blank line is encountered
            if (s.trim().compareTo("") !=0) {
               // if not commented (trim the white space before checking)
               // due to some reason if (s.trim().substring(0,1) != ";")
               // doesn't work
               if (s.trim().substring(0,1).compareTo(";") != 0) {
                  keyNum = Integer.parseInt(s.substring(1,5).trim());
                  noOfReg = Integer.parseInt(s.substring(6,7).trim());
                  if (noOfReg >= registerNumber) {
                     // find array length
                     arrayLength = arrayLength + 1;
                     // make an arry of keys that have the requested register
                     keyArray[arrayLength - 1] = keyNum;
                  }
               }
            }
         }
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
      }
      closeReadDoc(in);
      int [][] keyDuplicationRefArray = new int [arrayLength][2];
      if (arrayLength != 0) {
         // sort the key array
         int [] keyArrayValidSorted = new int [arrayLength];
         for (int i = 0; i < arrayLength; i++) {
            keyArrayValidSorted[i] = keyArray[i];
         }
         Arrays.sort(keyArrayValidSorted);
         // fill  keyDuplicationRefArray[i][1] with the keyArrayValidSorted[]
         // fill  keyDuplicationRefArray[i][2] with 1 (one's)
         for (int i = 0; i < arrayLength; i++ ) {
            keyDuplicationRefArray[i][0] = keyArrayValidSorted [i];
         }
         for (int i = 0; i < arrayLength; i++ ) {
            keyDuplicationRefArray[i][1] = 1;
         }
         int noOfOccurance = 1;
         // arrayLength-1 is used since it tests for arrayLength too
         for (int i = 0; i < arrayLength-1; i++) {
            // find if the key is repeated
            if (keyArrayValidSorted[i] == keyArrayValidSorted[i+1]) {
               noOfOccurance = noOfOccurance + 1;
               // update the 2nd column of  keyDuplicationRefArray with
               // how many times the key is repeated in the list
               for (int j = 0 ; j <  noOfOccurance; j++) {
                  keyDuplicationRefArray[i+1-j][1] = noOfOccurance;
               }
            }
            else {
               // reset noOfOccurance and leave keyDuplicationRefArray
               // unchanged
               // these statements will not be executed if the last
               // two keys are repeated. That is why there is an if statement
               // below
               extraOccurance = extraOccurance + noOfOccurance - 1;
               noOfOccurance = 1;
            }
         }
         // if the last 2 keys are repeated then reset the noOfOccurance
         // and update the extraOccurance
         if ( (keyArrayValidSorted.length > 1) && (keyArrayValidSorted[arrayLength-2] ==
                                     keyArrayValidSorted[arrayLength-1]) ) {
            extraOccurance = extraOccurance + noOfOccurance - 1;
            noOfOccurance = 1;
         }
      }
      else {
         String str = "register "+Integer.toString(registerNumber)
                                               +" doesn't exist ! \n";
         utilParameters.outputMessageTextArea.append(str);
      }
      return keyDuplicationRefArray;
   }

   // returns a float array of the register values. ignores the earlier keys
   // those are repeated. only the key that appears later is taken
   private float [] makeRegValArrayWithoutKeyRepeat(int [][] keyDuplicationRefArray,
                                         int registerNumber) {

      float [] registerValueArray = new float [arrayLength-extraOccurance];
      int index = 0;
      int keyNum = 0;
      int noOfReg = 0;
      String temp;
      in = openReadDoc(readFileName);
      try {
         String s = new String();
         boolean alreadyCounted;
         while ((s = in.readLine()) != null) {
            // ignores empty spaces
            if (s.trim().compareTo("") !=0) {
               // if not commented (trim the white space before checking)
               // due to some reason if (s.trim().substring(0,1) != ";")
               // doesn't work
               if (s.trim().substring(0,1).compareTo(";") != 0) {
                 StringTokenizer st =  new StringTokenizer(s);
                 temp = st.nextToken();
                 keyNum = Integer.parseInt(temp);
                  //keyNum = Integer.parseInt(s.substring(1,5).trim());
                  alreadyCounted = false;
                  for (int i = 0; i < arrayLength; i++) {
                     if (keyDuplicationRefArray[i][0] == keyNum) {
                        keyDuplicationRefArray[i][1] =
                                      keyDuplicationRefArray[i][1]-1;
                        // store the register value
                        if (( keyDuplicationRefArray[i][1] == 0)
                                                && (alreadyCounted == false)) {

                           if (registerNumber == 0) {
                              registerValueArray[index] = keyNum;
                           }
                           else {
                             temp = st.nextToken();
                             for (int k = 0; k < registerNumber; k++ ){
                               temp = st.nextToken();
                             }
                             registerValueArray[index] = Float.parseFloat(temp);
/*                             float temp =  Float.parseFloat
                              (s.substring((7+(registerNumber-1)*12),
                                                     (7+(registerNumber*12))));

                             registerValueArray[index] = Float.parseFloat
                              (s.substring((7+(registerNumber-1)*12),
                                                     (7+(registerNumber*12))));*/
                           }
                           index = index + 1;
                           // necessary or else there will be repeatation entries
                           // for the same key
                           alreadyCounted = true;
                        }
                     }
                  }
               }
            }
         }
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
      }
      closeReadDoc(in);
      return registerValueArray;
   }

   public void closeWriteDoc() {
      out.close();
   }

   public void closeWriteCommentsDoc() {
      out.close();
   }

   public BufferedReader openReadDoc(String fileName) {
      BufferedReader bufferedReader;
      try {
         bufferedReader = new BufferedReader(new FileReader(fileName));
         return bufferedReader;
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
         return null;
      }
   }

   public void closeReadDoc(BufferedReader bufferedReader) {
      try {
         bufferedReader.close();
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
      }
   }

   public int getMaxKey() {
      return maxKey;
   }

   public int getMaxReg() {
      return maxReg;
   }

   public int getLastKeyNum() {
      return lastKeyNum;
   }

   public int getNumOfInputKeys() {
      return numOfInputKeys;
   }

   public float getTiltAngle() {
     in = openReadDoc(readFileName);
     String temp1, temp2, temp;
     float angle = 0;
     try {
       String s = new String();
       while ( (s = in.readLine()) != null) {
         s.trim();
         StringTokenizer st =  new StringTokenizer(s);
         temp1 = st.nextToken();
         temp2 = st.nextToken();

         if ( temp1.equals(";") && temp2.equals("1") ) {
           temp = st.nextToken();
           temp = st.nextToken();
           temp = st.nextToken();
           angle = Float.parseFloat(temp);
           break;
         }
       }
     }
     catch(IOException er) {
       System.err.println(er.getMessage());
     }
     closeReadDoc(in);
     return angle;
   }

}
