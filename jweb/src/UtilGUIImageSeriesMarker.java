package web;

import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.media.jai.*;
import java.io.*;
import java.util.*;
import java.text.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Wenji Zhang: Revised by Bimal Rath
 * Acknowledgement: Few methods in this class have been imported and adapted from the
 * ImageJ package developed at NIH.
 * @version 1.0
 */

public class UtilGUIImageSeriesMarker extends JPanel
                             implements MouseMotionListener, MouseListener  {

   private SpiderImage spiderVolume;
   private UtilParameters utilParameters;
   private UtilParametersImageSeries utilParametersImageSeries;

   private int nsam, nrow, nslice;

   private JLabel slicesLabel, numOfImagesLabel, dummyLabel;
   private JTextField slicesLowEndJTextField, slicesHighEndJTextField;

   private JButton editMarkerJButton;
   private JButton saveMarkerJButton;
   private int lineCountSave = -7;
   private int markerSideValue = 10;
   private float resizeFactor;

   private JSlider slicesLowEndSlider;
   private int columnsLVal, columnsHVal, rowsLVal, rowsHVal;
   private int slicesLVal, slicesHVal;
   private JButton jButtonCloseAll;
   private ImageFrame imageFrameZ;
   private String axisSelected;
   private boolean imageFrameZOpen = false;
   private boolean showSliceZactivatedForFirstTime = true;
   private JLabel markerDocFile;
   private JTextField markerDocFileInput;
   private JCheckBox reNumberMarkerSelection;
   private JCheckBox angleFileSelection;
   private JLabel angleDocFile, tiltAngleLabel;
   private JTextField tiltAngleJTextField, angleDocFileInput;
   private JLabel addMarker;
   private JLabel editMarker;
   private JLabel deleteMarker;
   public int nSlices;
   public String []markerFileNames;
   public final Vector tiltAngles = new Vector(0,250);
   public float tiltAngle, tiltAngVal;
   public final Vector markersXY = new Vector(0,15);
   public int currentMarkerNum = -1;
   public boolean reNumberMarker = false;
   private String markerDocName, markerDocNameSave;
   private String angleDocName;
   private int xRegValue, yRegValue, keyRegValue;

   public UtilGUIImageSeriesMarker(SpiderImage spiVol, String imageType,
                     JButton closeAll,UtilParameters utilParam,
                                UtilParametersImageSeries utilParamImgSeries) {
      spiderVolume = spiVol;
      utilParameters = utilParam;
      jButtonCloseAll = closeAll;
      utilParametersImageSeries = utilParamImgSeries;
      nsam = spiderVolume.getnsam();
      nrow = spiderVolume.getnrow();
      nslice = spiderVolume.getnslice();
      axisSelected = "z";
      keyRegValue = 0;
      xRegValue = 1;
      yRegValue = 2;
      markerSideValue = 10;

      GridBagLayout gridBagLayout = new GridBagLayout();
      this.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();
      markerDocFile = new JLabel(" Marker File: ");
      addComponent(markerDocFile,gridBagConstraints,1,1,2,1,"LEFT");

      // set a default marker file name
      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);
      markerDocName = fileNameWOExtension+".mrk";
      markerDocNameSave = markerDocName;

      markerDocFileInput = new JTextField(markerDocName,30);
      addComponent(markerDocFileInput,gridBagConstraints,3,1,4,1,"RIGHT");

      reNumberMarkerSelection = new JCheckBox("Renumber Markers After Delete");
      reNumberMarkerSelection.setSelected(false);
      addComponent(reNumberMarkerSelection,gridBagConstraints,1,2,4,1,"LEFT");
      reNumberMarkerSelection.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
                 reNumberMarkerSelection_actionPerformed(e);
         }
      });


      angleFileSelection = new JCheckBox("Use Tilt Angle File");
      angleFileSelection.setSelected(false);
      addComponent(angleFileSelection,gridBagConstraints,1,3,2,1,"LEFT");
      angleFileSelection.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
                 angleFileSelection_actionPerformed(e);
         }
      });
      tiltAngleLabel = new JLabel(" Tilt Angle:");
      addComponent(tiltAngleLabel,gridBagConstraints,7,3,1,1,"RIGHT");

      tiltAngleJTextField = new JTextField(4);
      tiltAngleJTextField.setEditable(false);
      addComponent(tiltAngleJTextField,gridBagConstraints,8,3,1,1,"LEFT");
      tiltAngleJTextField.setText(Integer.toString(-360));

      angleDocFile = new JLabel(" Tilt Angle File: ");
      angleDocFile.setEnabled(false);
      addComponent(angleDocFile,gridBagConstraints,1,4,2,1,"LEFT");

      // set a default angle file name
      angleDocName = fileNameWOExtension+".ang";
      angleDocFileInput = new JTextField(angleDocName,30);
      angleDocFileInput.setEnabled(false);
      addComponent(angleDocFileInput,gridBagConstraints,3,4,4,1,"RIGHT");
      // clear tiltAngles vector
      tiltAngles.clear();

      addMarker = new JLabel(" To Add a New Marker              : Click Left Mouse Button  ");
      addMarker.setEnabled(false);
      addComponent(addMarker,gridBagConstraints,1,7,5,1,"LEFT");

      editMarker = new JLabel(" To Change an Existing Marker  : Drag Right Mouse Button");
      editMarker.setEnabled(false);
      addComponent(editMarker,gridBagConstraints,1,8,5,1,"LEFT");

      deleteMarker = new JLabel(" To Delete a Marker                   : Click Middle Mouse Button ");
      deleteMarker.setEnabled(false);
      addComponent(deleteMarker,gridBagConstraints,1,9,5,1,"LEFT");

      slicesLabel = new JLabel(" Images:");
      addComponent(slicesLabel,gridBagConstraints,1,6,1,1,"LEFT");

      slicesLowEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nslice,1);
      addComponent(slicesLowEndSlider,gridBagConstraints,2,6,3,1,"LEFT");
      slicesLowEndJTextField = new JTextField(4);
      slicesLowEndJTextField.setEditable(false);
      addComponent(slicesLowEndJTextField,gridBagConstraints,5,6,1,1,"LEFT");
      slicesLowEndSlider.setEnabled(false);

      numOfImagesLabel = new JLabel(" Number of Images:");
      addComponent(numOfImagesLabel,gridBagConstraints,6,6,2,1,"LEFT");

      slicesHighEndJTextField = new JTextField(4);
      slicesHighEndJTextField.setEditable(false);
      addComponent(slicesHighEndJTextField,gridBagConstraints,8,6,1,1,"LEFT");
      slicesHighEndJTextField.setText(Integer.toString(nslice));

      editMarkerJButton = new JButton("Show/Edit Marker");
      addComponent(editMarkerJButton,gridBagConstraints,2,12,2,1,"LEFT");
      if (imageType == "ImageSeries") {
        editMarkerJButton.setEnabled(true);
      }
      else {
        editMarkerJButton.setEnabled(false);
      }

      saveMarkerJButton = new JButton("Save Marker File");
      addComponent(saveMarkerJButton,gridBagConstraints,5,12,2,1,"CENTER");
      saveMarkerJButton.setEnabled(false);

      slicesLowEndJTextField.setText("1");
      slicesLowEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            slicesLowEndJTextField.setText(""+slicesLowEndSlider.getValue());
            if (axisSelected == "z" && imageFrameZOpen ) {
               imageFrameZ.wdp.img = getImage();

               //If the contrast or scale is changed either in global settings
               //or from the image frame menu
               if (imageFrameZ.utilImageManipulate.lowContrast != 0 ||
                        imageFrameZ.utilImageManipulate.highContrast != 255
                            || imageFrameZ.utilImageManipulate.scale != 1  ) {

                  imageFrameZ.utilImageManipulate.setLowContrast(
                                 imageFrameZ.utilImageManipulate.lowContrast);
                  imageFrameZ.utilImageManipulate.setHighContrast(
                                 imageFrameZ.utilImageManipulate.highContrast);
               }
               else {
                  imageFrameZ.wdp.outImage = imageFrameZ.wdp.img;
               }
               //first clean the previous image display
               imageFrameZ.wdp.drawMarker = false;
               imageFrameZ.wdp.drawString = false;
               imageFrameZ.wdp.repaint();

               //If the marker series file with co-ordinates exists
               //display the new image with markers
               String markFileName = markerFileNames
                                             [slicesLowEndSlider.getValue()-1];
               showMarkers ( markFileName);
               markerDocFileInput.setText(markFileName);
               saveMarkerJButton.setEnabled(false);
               tiltAngleJTextField.setEditable(false);
               if(angleFileSelection.isSelected()){
                  Float f = (Float)(tiltAngles.elementAt(
                                            slicesLowEndSlider.getValue()-1));
                  tiltAngVal =  f.floatValue();
               }
               else {
                  File mf = new File( markFileName);
                  //if marker file does not exist set tilt angle to -360
                  if (!mf.exists())  {
                     tiltAngVal = -360;
                  }
                  else {
                     String op = "r";
                     UtilDoc utilDoc = new UtilDoc( markFileName,op,utilParameters);
                     tiltAngVal = utilDoc.getTiltAngle();
                  }
               }
               tiltAngleJTextField.setText(Float.toString(tiltAngVal));
            }
         }
      });

      editMarkerJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            editMarkerJButton_actionPerformed(e);
         }
      });

      saveMarkerJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            saveMarkerJButton_actionPerformed(e);
         }
      });
      setVisible(true);
   }

   public void angleFileSelection_actionPerformed(ActionEvent e){
      if(angleFileSelection.isSelected()){
         angleDocFile.setEnabled(true);
         angleDocFileInput.setEnabled(true);
         tiltAngleJTextField.setEditable(false);
      }
      else{
         angleDocFile.setEnabled(false);
         angleDocFileInput.setEnabled(false);
         tiltAngleJTextField.setEditable(true);
      }
   }


   public void reNumberMarkerSelection_actionPerformed(ActionEvent e){
      if(reNumberMarkerSelection.isSelected()){
         reNumberMarker = true;
      }
      else{
         reNumberMarker = false;
      }
   }


   public void addComponent(Component comp,
               GridBagConstraints gridBagConstraints, int gridX, int gridY,
                              int gridWidth, int gridHeight, String align) {
      gridBagConstraints.gridx = gridX;
      gridBagConstraints.gridy = gridY;
      gridBagConstraints.gridwidth = gridWidth;
      gridBagConstraints.gridheight = gridHeight;
      // will grow to fill the space if the window size is changed
      gridBagConstraints.weightx = 100;
      gridBagConstraints.weighty = 100;

      if (align == "LEFT") {
      // align to the left
         gridBagConstraints.anchor = GridBagConstraints.WEST;
      }
      else if (align == "CENTER"){
         // align to the center
         gridBagConstraints.anchor = GridBagConstraints.CENTER;
      }
      else {
         // align to the right
         gridBagConstraints.anchor = GridBagConstraints.EAST;
      }
      this.add(comp,gridBagConstraints);
   }

   public TiledImage getImage() {
      columnsLVal = 1;
      columnsHVal = nsam;
      rowsLVal = 1;
      rowsHVal = nrow;

      slicesLVal = slicesLowEndSlider.getValue();
      slicesHVal = nslice;

      int noOfSlices = 1;
      int sliceWidth = 0;
      int sliceHeight = 0;
      int noOfImgInRow =1;
      if (axisSelected == "z") {
         slicesHVal  = slicesLVal;
         sliceWidth   = columnsHVal - columnsLVal + 1;
         sliceHeight  = rowsHVal    - rowsLVal    + 1;
      }
      int montagedSlicesWidth = sliceWidth;
      int montagedSlicesHeight = sliceHeight;

      UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
      TiledImage montagedSlices;
      montagedSlices = utilImageCreate.getMontagedSlices(spiderVolume,
             axisSelected, columnsLVal, columnsHVal, rowsLVal, rowsHVal,
                  slicesLVal, slicesHVal, noOfImgInRow, noOfSlices,sliceWidth,
                           sliceHeight,montagedSlicesWidth, montagedSlicesHeight);

      slicesLowEndSlider.setEnabled(true);
      return montagedSlices;
   }

   public void resetSliders() {
      slicesLowEndSlider.setValue(1);
      slicesLowEndJTextField.setText(""+slicesLowEndSlider.getValue());
      slicesHighEndJTextField.setText(""+nslice);
   }

   public ImageFrame getImageFrameZ() {
      return imageFrameZ;
   }


   void editMarkerJButton_actionPerformed(ActionEvent e) {


      // if not activated for the first time, check if the window is closed
      if  (!showSliceZactivatedForFirstTime)  {

         imageFrameZOpen = imageFrameZ.isShowing();
         if (!imageFrameZOpen) markerDocFileInput.setText(markerDocNameSave);
      }

      //if the image was opened
      if ( imageFrameZOpen) {
        return;
      }
         // if activated for the first time OR if the window is closed
      else{
         markerDocName = markerDocFileInput.getText().trim();
         makeFileNameArray(markerDocName,nslice);

         slicesLowEndSlider.setValue(1);
         slicesLowEndJTextField.setText("" + slicesLowEndSlider.getValue());
         TiledImage montagedSlices = getImage();
         imageFrameZ = new ImageFrame(montagedSlices, utilParameters);

         imageFrameZ.wdp.addMouseMotionListener(this);
         imageFrameZ.wdp.addMouseListener(this);

         jButtonCloseAll.addActionListener(imageFrameZ);
         imageFrameZ.setTitle("Marker Editing Slide");

         utilParametersImageSeries.markerSeriesShown = true;

         imageFrameZ.setLocation(70, 70);
         imageFrameZ.setVisible(true);
         imageFrameZOpen = true;
         showSliceZactivatedForFirstTime = false;
      }
      showMarkers(markerDocName);


      if(angleFileSelection.isSelected()){

         // read only once for the whole series
         if(!tiltAngles.isEmpty()){
            angleDocName = angleDocFileInput.getText().trim();
            readAngleFile(angleDocName);
            Float f= (Float)(tiltAngles.elementAt(
                                            slicesLowEndSlider.getValue()-1));
            tiltAngVal = f.floatValue();
         }
      }
      else {

         File mf = new File(markerDocName);
         //if marker file does not exist, remove marker coordinates from previous
         //marker file
         if (!mf.exists())  {
            tiltAngVal = -360;
         }
         else {
            String op = "r";
            UtilDoc utilDoc = new UtilDoc(markerDocName,op,utilParameters);
            tiltAngVal = utilDoc.getTiltAngle();
         }
      }
      if(reNumberMarkerSelection.isSelected()){
         reNumberMarker = true;
      }
      else{
         reNumberMarker = false;
      }
       markerDocFileInput.setEditable(false);
       angleDocFileInput.setEditable(false);
       angleFileSelection.setEnabled(false);
       tiltAngleJTextField.setText(Float.toString(tiltAngVal));
   }

   void saveMarkerJButton_actionPerformed(ActionEvent e) {
      writeMarkerFile();
      saveMarkerJButton.setEnabled(false);
      tiltAngleJTextField.setEditable(false);
   }

   public void showOutput(String string) {
      int lineCount = utilParameters.outputMessageTextArea.getLineCount();
      String str1 = string;
      String str2 = utilParameters.outputMessageTextArea.getText();
      int index = str2.lastIndexOf('(');
      // No additional line has been printed and existing text contains "("
      // Remember "(" is necessary to check the validity and overwritting on
      // on the same line. It is done this way because I couldn't find any way to
      // overwrite the last line on a TextArea object.
      if ((lineCountSave == lineCount) && (index != -1)){
         String str3 = str2.substring(0,index);
         String str = str3+str1;
         utilParameters.outputMessageTextArea.setText(str);
       }
       else {
          // print to the next line. No overwritting
          utilParameters.outputMessageTextArea.append(str1);
          lineCountSave = utilParameters.outputMessageTextArea.getLineCount();
       }
   }

   public void writeMarkerFile() {

      // get value of resizeFactor
      resizeFactor = imageFrameZ.utilImageManipulate.scale;

      PrintWriter out;
      String fileName = markerFileNames[slicesLowEndSlider.getValue()-1];
      try{
         out = new PrintWriter(new BufferedWriter(new FileWriter(
                                                            fileName,false)));

         //Write the tilt angle comment line
         String str2 = " ;  1 3";
         String temp;
         DecimalFormat df2 = new DecimalFormat("####0.000000");
         temp = df2.format(0);
         str2 = str2 + "    " + temp;
         tiltAngle = (float) Float.parseFloat
                                      (tiltAngleJTextField.getText().trim());

         temp = df2.format(tiltAngle);
         int index = temp.indexOf(".");
         int space = 6 - index;
         String str3 = "";
         for (int j = 1; j < space; j++) {
            str3 = " " + str3;
         }
         str2 = str2 + str3 + temp + "  ";
         temp = df2.format(0);
         str2 = str2 + "  " + temp;
         str2 = str2.concat("\n");
         out.print(str2);

         //Record the new marker
         DecimalFormat df1 = new DecimalFormat("0000");
         String str1 = new String();
         String spaceString = new String();
         spaceString = " ";
         Point p = new Point(0, 0);
         for (int j = 0; j < markersXY.size(); j++) {
            p = (Point) markersXY.elementAt(j);
            //Only record the real markers. Markers with XY coordinate less than
            //0 is actually deleted.
            //take care if the image has been resized. write the co-ordinate
            //as per preresized image
            int markerXpos = (int)(p.getX()/resizeFactor);
            int markerYPos = (int)(p.getY()/resizeFactor);
            p.setLocation( markerXpos, markerYPos);

            if(p.getX() >= 0 && p.getY() >= 0){
               str1 = df1.format(j + 1);
               str1 = " " + str1 + " ";
               str1 = str1 + "2";
               //x co-ordinate
               str2 = df2.format(p.getX());
               index = str2.indexOf(".");
               space = 6 - index;
               // adds spaces
               for (int k = 1; k < space; k++) str2 = " " + str2;
               str1 = str1 + str2;
               //y co-ordinate
               str2 = df2.format(p.getY());
               index = str2.indexOf(".");
               space = 6 - index;
               // adds spaces
               for (int k = 1; k < space; k++) str2 = " " + str2;
               str1 = str1 + str2;
               //change line
               str1 = str1.concat("\n");

               out.print(str1);
               utilParameters.outputMessageTextArea.append(str1);
            }
         }
         out.close();
      }
      catch(IOException er){
         System.err.println(er.getMessage());
      }
   }

   public void drawMarkers(){
      int vectorSize = markersXY.size();
      int numOfmarkers = 0;
      for (int i = 0; i < vectorSize; i++) {
         Point p = (Point) markersXY.elementAt(i);
         //make sure this is a real marker. When the option of "Renumber
         //maker when delete" is NOT checked, deleted marker operation
         //will set the marker's coordinates to be (-200,-200), and draw marker
         //operation will not draw these markers.
         if(p.getX() >= 0 && p.getY() >=0 ){
            imageFrameZ.wdp.markers[numOfmarkers] = p;
            imageFrameZ.wdp.strings[numOfmarkers] = Integer.toString(i + 1);
            imageFrameZ.wdp.stringPos[numOfmarkers] = new Point(
                          (int) (p.getX() - (markerSideValue / 2) + 10),
                                    (int) (p.getY() + (markerSideValue / 2)));
            numOfmarkers++;
         }
      }

      imageFrameZ.wdp.markerLength = markerSideValue;
      imageFrameZ.wdp.drawMarker = true;
      imageFrameZ.wdp.markerNumber = numOfmarkers;
      imageFrameZ.wdp.drawString = true;
      imageFrameZ.wdp.stringNumber = numOfmarkers;
      imageFrameZ.wdp.repaint();
   }

   public void findClosest (int x, int y){
      int numOfMarkers = markersXY.size();
      //Maximum distance from a marker
      float maxDistance = 50;

      currentMarkerNum = -1;

      if (numOfMarkers == 0) return;

      Point p = (Point)markersXY.elementAt(0);
      float distance = (float)(x - p.x) * (float)(x - p.x)
                            + (float)(y - p.y) * (float)(y - p.y);
      if (distance < maxDistance) currentMarkerNum = 0;

      for (int k = 1; k < numOfMarkers ; k++) {
         p = (Point)markersXY.elementAt(k);
         final float candidate = (float)(x - p.x) * (float)(x - p.x)
                                        + (float)(y - p.y) * (float)(y - p.y);
         if (candidate < maxDistance && candidate < distance) {
            distance = candidate;
            currentMarkerNum = k;
         }
      }
   }

   public void moveMarker (int x, int y){
      int temp = currentMarkerNum;
      if (temp >= 0) {
         //update the marker coordinate
         markersXY.removeElementAt(temp);
         final Point p = new Point(x, y);
         markersXY.insertElementAt(p, temp);

         //draw and label the marker
         drawMarkers();
      }
   }

   public void removeMarker (){
      int temp =currentMarkerNum;
      if (temp ==0 || (temp > 0 && reNumberMarker) ) {
         //remove the marker coordinate from the markersXY vector
         markersXY.removeElementAt(temp);
      }
      else if (temp > 0 && !reNumberMarker) {
        final Point p = new Point(-200,-200);
        //When the option "Renumber markers when delete" is NOT checked
        //Set the marker coordinates to be (-200,-200).
        markersXY.setElementAt(p,temp);
      }

     //draw and label the marker
     drawMarkers();
   }

   void showMarkers(String fileName) {
      File mf = new File(fileName);
      //if marker file does not exist, remove marker coordinates from previous
      //marker file
      if (!mf.exists())  {
         markersXY.clear();
         return;
      }
      // update resizeFactor value
      resizeFactor = imageFrameZ.utilImageManipulate.scale;

      boolean KeyDuplicationAllowed = false;

      String op = "r";
      UtilDoc utilDoc = new UtilDoc(fileName,op,utilParameters);

      tiltAngleJTextField.setText(Float.toString(tiltAngVal));

      float [] xRegVal = utilDoc.readDoc(xRegValue, KeyDuplicationAllowed);
      int   [] xRegValInt = new int[xRegVal.length];
      float [] yRegVal = utilDoc.readDoc(yRegValue, KeyDuplicationAllowed);
      int   [] yRegValInt = new int[yRegVal.length];
      int      maxKeyValue = 0;
      float [] keyRegVal = utilDoc.readDoc(keyRegValue, KeyDuplicationAllowed);
      int   [] keyRegValInt = new int[keyRegVal.length];
      int keyNumbers, xRegNumbers, yRegNumbers;
      xRegNumbers = xRegVal.length;
      yRegNumbers = yRegVal.length;
      keyNumbers  = keyRegVal.length;
      //The x,y co-ordinates of default point
      final Point p = new Point(-200,-200);

      //remove old marker coordinates
      markersXY.clear();

      if (( keyNumbers == xRegNumbers) && (keyNumbers == yRegNumbers )) {
         for (int i = 0; i < xRegNumbers; i++) {
            markersXY.add(p);
         }

         for (int i = 0; i < xRegNumbers; i++) {
            xRegValInt[i] = (int)xRegVal[i];
            yRegValInt[i] = (int)yRegVal[i];
            keyRegValInt[i] = (int)keyRegVal[i];

            //Set the value of Maximum Marker Number
            if (keyRegValInt[i] > maxKeyValue) maxKeyValue = keyRegValInt[i];

            //If the maximum marker number is greater than the number of markers,
            //ie, marker numbers are not continuous, add default point for
            //markerXY vector
            if(maxKeyValue > markersXY.size()){
               for (int j = markersXY.size(); j < maxKeyValue; j++) {
                  markersXY.add(p);
               }
            }
            // take care if the scale is changed
            int markerXpos = (int)(xRegValInt[i]*resizeFactor);
            int markerYpos = (int)(yRegValInt[i]*resizeFactor);

            markersXY.setElementAt(new Point(markerXpos,markerYpos),
                                                            keyRegValInt[i]-1);

            imageFrameZ.wdp.markers[i] = new Point(markerXpos,markerYpos);
            imageFrameZ.wdp.strings[i] = Integer.toString(keyRegValInt[i]);
            imageFrameZ.wdp.stringPos[i] =
                        new Point((markerXpos-(markerSideValue/2 )+10),
                                             (markerYpos+(markerSideValue/2)));
         }
         imageFrameZ.wdp.markerLength = markerSideValue;
         imageFrameZ.wdp.drawMarker = true;
         imageFrameZ.wdp.markerNumber = xRegVal.length;
         imageFrameZ.wdp.drawString = true;
         imageFrameZ.wdp.stringNumber = keyRegVal.length;
         imageFrameZ.wdp.repaint();
      }
      else {
         String str = "No. of X, y registers and the key numbers are not same!\n ";
         utilParameters.outputMessageTextArea.append(str);
      }
  }

   void makeFileNameArray(String file, int numOfFiles){
      // Copy file name and number of files
      String fileName = file;
      int numFiles = numOfFiles;

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
      // make an array with maximum possible elements within the start and end value
      // already known from user inputs
      markerFileNames = new String[numFiles];
      markerFileNames[0] = fileName;

      // Construct the new file names
      for (int i = 1; i < numFiles; i++) {
         valueOfString = valueOfString + 1;
         String incrementedNumericString = Integer.toString(valueOfString);
         // find # of characters in the new string
         int incrementedNumericStringLength = incrementedNumericString.length();

         // add zeros before the number as per compatibality of SPIDER file names
         for (int j = 0; j < numericStringLength -
                incrementedNumericStringLength; j++) {
            incrementedNumericString = "0" + incrementedNumericString;
         }
         String newFileName = stringBeforeNumeric.concat
                  (incrementedNumericString).concat(".").concat(fileExtension);
         markerFileNames[i] = newFileName;
      }
    }

   void readAngleFile(String fileName){
      BufferedReader in;
      //empty the tiltAngles Vector
      tiltAngles.clear();
      try {
         in = new BufferedReader(new FileReader(fileName));
         String s = new String();
         String temp,temp1;
         while ((s = in.readLine()) != null )  {
            temp = s.trim();
            // ignore blank line and comment line
            if ( (!temp.equals("")) && (!temp.substring(0,1).equals(";")) ){
               //get the tilt angle value
               StringTokenizer st =  new StringTokenizer(temp);
               temp1 = st.nextToken();
               temp1 = st.nextToken();
               temp1 = st.nextToken();
               temp1 = st.nextToken();
               //add the Float object to vector
               tiltAngles.add(new Float(temp1));
            }
         }
      }
      catch(IOException er) {
         System.err.println(er.getMessage());
      }
   }

   public void mouseMoved(MouseEvent e) {
      //change the cursor type
      imageFrameZ.wdp.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
      String str = "(X,Y) = " + e.getX() + "," + e.getY() + "\n";
      showOutput(str);
   }

   public void mousePressed(MouseEvent e) {
      if( SwingUtilities.isLeftMouseButton(e) ){
         //add the marker coordinate to the marker vector markersXY
         markersXY.add(e.getPoint());
         //draw and label the marker
         drawMarkers();
      }
      else if( SwingUtilities.isRightMouseButton(e) ){
         findClosest(e.getX(), e.getY());
      }
      else {
         findClosest(e.getX(), e.getY());
         removeMarker();
      }
         saveMarkerJButton.setEnabled(true);
         tiltAngleJTextField.setEditable(true);
   }

   public void mouseDragged(MouseEvent e) {
      if( SwingUtilities.isRightMouseButton(e) ){
         moveMarker(e.getX(), e.getY());
      }
      saveMarkerJButton.setEnabled(true);
      tiltAngleJTextField.setEditable(true);
   }

   public void mouseClicked(MouseEvent e) {

   }

   public void mouseReleased(MouseEvent e) {

   }

   public void mouseEntered(MouseEvent e) {

   }

   public void mouseExited(MouseEvent e) {

   }
}
