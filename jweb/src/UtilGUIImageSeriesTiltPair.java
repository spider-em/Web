package web;

import java.io.*;
import java.text.*;
import java.util.*;
import javax.media.jai.*;
import java.awt.geom.Point2D;


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal Rath
 * Acknowledgement: Few methods in this class have been imported and adapted from the
 * ImageJ package developed at NIH.
 * @version 1.0
 */

public class UtilGUIImageSeriesTiltPair extends JPanel
                             implements MouseMotionListener, MouseListener  {

   private SpiderImage spiderVolume;
   private UtilParameters utilParameters;
   private UtilParametersImageSeries utilParametersImageSeries;
   private UtilFile utilFile;
   private int nsam, nrow, nslice;
   private JButton editMarkerJButton, determineThetaJButton, fitAnglesJButton;
   private JButton drawFitLocJButton, saveMarkerJButton, saveAnglesJButton;
   private JButton hideMarkersJButton, unhideMarkersJButton;
   private int lineCountSave = -7;
   private int markerSideValue = 10;
   private float resizeFactor;

   private int columnsLVal, columnsHVal, rowsLVal, rowsHVal,  slicesHVal;
   private float   [] unTiltX, unTiltY, tiltX, tiltY, tiltFitX, tiltFitY;
   private float [] xPicked = new float [1], yPicked = new float [1];
   private float [] xPredicted = new float [1], yPredicted = new float [1];
   private float    originUnTiltX, originUnTiltY, originTiltX, originTiltY ;
   private int markersPicked;
   private float thetaG, phiG, gammaG, tiltAngleAreaG;

   private boolean thetaDetermined = false, markerFileNumChanged = false;
   private boolean anglesFitted = false;



   private JButton jButtonCloseAll;
   private ImageFrame imageFrameZ, imageFrameZ2;
   private String axisSelected;
   private boolean imageFrameZOpen = false;
   private boolean showSliceZactivatedForFirstTime = true;
   private boolean tiltImagePicked = false;
   private boolean unTiltImagePicked = false;
   private JLabel markerDocFileNo, keyNumInput, offsetLabel;
   private JLabel markFileInfo,keyNumInfo;

   private JLabel keyNumOrigin, phiLabel, gammaLabel, thetaLabel, tiltAngleArea;
   private JTextField markerDocFileNoInput, keyNumInputText, offsetInputText;
   private JTextField keyNumOriginText, phiInputText, gammaInputText, thetaInputText;
   private JTextField tiltAngleAreaText;

   private JCheckBox reNumberMarkerSelection;
   private JLabel addMarker;
   private JLabel editMarker;
   private JLabel deleteMarker;
   public int nSlices, docNumSave = 0;
   public String [] markerFileNames;

   public final Vector tiltAngles = new Vector(0,250);
   public float tiltAngle, tiltAngVal;
   public final Vector markersXYTilt = new Vector(0,15);
   public final Vector markersXYUnTilt = new Vector(0,15);
   public final Vector markersFitTilt = new Vector(0,15);
   public int currentMarkerNum = -1;
   public boolean reNumberMarker = false;
   private String  fileExtension, markerUnTiltDocName, markerTiltDocName ;
   private String []  docFileNameArray;
   private Robot rob;

   private int xRegValue, yRegValue, keyRegValue, offsetRegValue;
   private int keyNo, offsetInput, keyNumOriginVal;
   private DecimalFormat df3, df4;

   public UtilGUIImageSeriesTiltPair(SpiderImage spiVol, String imageType,
                     JButton closeAll,UtilParameters utilParam,
                           UtilParametersImageSeries utilParamImgSeries)
                                                          throws AWTException {

      spiderVolume = spiVol;
      utilParameters = utilParam;
      jButtonCloseAll = closeAll;
      utilParametersImageSeries = utilParamImgSeries;
      nsam = spiderVolume.getnsam();
      nrow = spiderVolume.getnrow();
      nslice = spiderVolume.getnslice();
      axisSelected = "z";
      keyRegValue = 0;
      offsetRegValue = 1;
      xRegValue = 2;
      yRegValue = 3;
      markerSideValue = 10;
      rob = new Robot();
      df3 = new DecimalFormat("###0.000");
      df4 = new DecimalFormat("###0.00");

      GridBagLayout gridBagLayout = new GridBagLayout();
      this.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();
      markerDocFileNo = new JLabel(" Marker File Number: ");
      addComponent(markerDocFileNo,gridBagConstraints,1,1,2,1,"LEFT");

      markerDocFileNoInput = new JTextField("0",5);
      addComponent(markerDocFileNoInput,gridBagConstraints,3,1,4,1,"LEFT");

      utilFile = new UtilFile(utilParam);

      markFileInfo = new JLabel(" Click on Show/Edit marker button if this input is edited ! ");
      addComponent(markFileInfo,gridBagConstraints,4,1,6,1,"LEFT");
      markFileInfo.setEnabled(false);

      keyNumInput = new JLabel(" Key Number: ");
      addComponent(keyNumInput,gridBagConstraints,1,2,1,1,"LEFT");
      keyNumInputText = new JTextField("1",5);
      addComponent(keyNumInputText,gridBagConstraints,2,2,1,1,"LEFT");
      keyNumInputText.setEditable(false);

      offsetLabel = new JLabel(" Offset: ");
      addComponent(offsetLabel,gridBagConstraints,3,2,1,1,"LEFT");
      offsetInputText = new JTextField("0",5);
      addComponent(offsetInputText,gridBagConstraints,4,2,1,1,"LEFT");
      offsetInputText.setEditable(false);

      keyNumInfo = new JLabel ("== > Used to Save Marker Files !");
      addComponent(keyNumInfo,gridBagConstraints,5,2,7,1,"LEFT");
      keyNumInfo.setEnabled(false);

      keyNumOrigin = new JLabel(" Key number for origin: ");
      addComponent(keyNumOrigin,gridBagConstraints,1,3,2,1,"LEFT");
      keyNumOriginText = new JTextField("1",5);
      addComponent(keyNumOriginText,gridBagConstraints,4,3,1,1,"LEFT");
              keyNumOriginText.setEditable(false);

      phiLabel = new JLabel(" Phi: ");
      addComponent(phiLabel,gridBagConstraints,1,4,1,1,"LEFT");
      phiInputText = new JTextField("-90",5);
      addComponent(phiInputText,gridBagConstraints,2,4,1,1,"LEFT");

      gammaLabel = new JLabel(" Gamma: ");
      addComponent(gammaLabel,gridBagConstraints,3,4,1,1,"LEFT");
      gammaInputText = new JTextField("-90",5);
      addComponent(gammaInputText,gridBagConstraints,4,4,1,1,"LEFT");

      thetaLabel = new JLabel(" Theta: ");
      addComponent(thetaLabel,gridBagConstraints,5,4,1,1,"LEFT");
      thetaInputText = new JTextField("0",5);
      addComponent(thetaInputText,gridBagConstraints,6,4,1,1,"LEFT");

      tiltAngleArea = new JLabel(" Tilt angle area: ");
      addComponent(tiltAngleArea,gridBagConstraints,1,5,1,1,"LEFT");
      tiltAngleAreaText = new JTextField("5000.00",5);
      addComponent(tiltAngleAreaText,gridBagConstraints,2,5,1,1,"LEFT");

      // clear tiltAngles vector
      tiltAngles.clear();

      addMarker = new JLabel(" To Add a New Marker              : Click Left Mouse Button  ");
      addMarker.setEnabled(false);
      addComponent(addMarker,gridBagConstraints,1,8,3,1,"LEFT");

      editMarker = new JLabel(" To Change an Existing Marker  : Drag Right Mouse Button");
      editMarker.setEnabled(false);
      addComponent(editMarker,gridBagConstraints,4,8,3,1,"LEFT");

      deleteMarker = new JLabel(" To Delete a Marker                   : Click Middle Mouse Button ");
      deleteMarker.setEnabled(false);
      addComponent(deleteMarker,gridBagConstraints,1,9,3,1,"LEFT");

      reNumberMarkerSelection = new JCheckBox("Renumber Markers After Delete");
      reNumberMarkerSelection.setSelected(true);
      addComponent(reNumberMarkerSelection,gridBagConstraints,4,9,2,1,"LEFT");
      reNumberMarkerSelection.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            reNumberMarkerSelection_actionPerformed(e);
         }
      });

      determineThetaJButton = new JButton("Determine Theta");
      addComponent(determineThetaJButton,gridBagConstraints,1,7,2,1,"LEFT");
      determineThetaJButton.setEnabled(false);

      fitAnglesJButton = new JButton("Fit Angles");
      addComponent(fitAnglesJButton,gridBagConstraints,2,7,2,1,"CENTER");
      fitAnglesJButton.setEnabled(false);

      drawFitLocJButton = new JButton("Draw Fitted Locations");
      addComponent(drawFitLocJButton,gridBagConstraints,3,7,2,1,"RIGHT");
      drawFitLocJButton.setEnabled(false);

      saveAnglesJButton = new JButton("Save Angles");
      addComponent(saveAnglesJButton,gridBagConstraints,5,7,2,1,"RIGHT");
      saveAnglesJButton.setEnabled(false);

      editMarkerJButton = new JButton("Show/Edit Marker");
      addComponent(editMarkerJButton,gridBagConstraints,1,12,2,1,"LEFT");
      if (imageType == "ImageSeries") {
         editMarkerJButton.setEnabled(true);
      }
      else {
        editMarkerJButton.setEnabled(false);
      }

      saveMarkerJButton = new JButton("Save Marker Files");
      addComponent(saveMarkerJButton,gridBagConstraints,2,12,2,1,"CENTER");
      saveMarkerJButton.setEnabled(false);

      hideMarkersJButton = new JButton("Hide Markers");
      addComponent(hideMarkersJButton,gridBagConstraints,3,12,2,1,"RIGHT");
      hideMarkersJButton.setEnabled(false);

      unhideMarkersJButton = new JButton("Unhide Markers");
      addComponent(unhideMarkersJButton,gridBagConstraints,5,12,2,1,"RIGHT");
      unhideMarkersJButton.setEnabled(false);

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

      hideMarkersJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            hideMarkersJButton_actionPerformed(e);
         }
      });

      unhideMarkersJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            unhideMarkersJButton_actionPerformed(e);
         }
      });

      determineThetaJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            determineThetaJButton_actionPerformed(e);
         }
      });

      fitAnglesJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            fitAnglesJButton_actionPerformed(e);
         }
      });

      drawFitLocJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            drawFitLocJButton_actionPerformed(e);
         }
      });

      saveAnglesJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            saveAnglesJButton_actionPerformed(e);
         }
      });

      setVisible(true);
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

   public TiledImage getImage(int slicesLVal) {
      columnsLVal = 1;
      columnsHVal = nsam;
      rowsLVal = 1;
      rowsHVal = nrow;

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

      return montagedSlices;
   }

   public ImageFrame getImageFrameZ() {
      return imageFrameZ;
   }

   public ImageFrame getImageFrameZ2() {
      return imageFrameZ2;
   }

   void editMarkerJButton_actionPerformed(ActionEvent e) {

      updateInput();
      setDefault();

      // if not activated for the first time, check if the window is closed
      if  (!showSliceZactivatedForFirstTime)  {
         imageFrameZOpen = imageFrameZ.isShowing();
      }

      //if the image was opened
      if ( imageFrameZOpen) {
         imageFrameZ.show();
         imageFrameZ2.show();
      }
      // if activated for the first time OR if the window is closed
      else{
         imageFrameZOpen = true;
         showSliceZactivatedForFirstTime = false;
         TiledImage montagedSlices1 = getImage(1);
         imageFrameZ = new ImageFrame(montagedSlices1, utilParameters);
         // if window is closed then exit

         imageFrameZ.addWindowListener(new WindowAdapter() {
            public void windowClosing (WindowEvent e) {
               imageFrameZ2.dispose();
               disableButtonGroup();
            }
         });

         TiledImage montagedSlices2 = getImage(2);
         imageFrameZ2 = new ImageFrame(montagedSlices2, utilParameters);

         imageFrameZ2.addWindowListener(new WindowAdapter() {
            public void windowClosing (WindowEvent e) {
               imageFrameZ.dispose();
               disableButtonGroup();
            }
         });

         int locX = 70;
         int locY = 70;
         makeImgFrame(imageFrameZ, locX, locY);
         imageFrameZ.setTitle("Untilted Image");

         // set window location
         // if width is such that locX and locY will be out of screen then
         // make locX = 90, locY = 90.

         Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
         int tiltImgX = locX + nsam + locX;

         if (tiltImgX > screenSize.width) {
            locX = 90;
            locY = 90;
         }
         else {
            locX = tiltImgX;
         }

         makeImgFrame(imageFrameZ2, locX, locY);
         imageFrameZ2.setTitle("Tilted Image");
      }

      showMarkers(markerUnTiltDocName, imageFrameZ,markersXYUnTilt );
      showMarkers(markerTiltDocName, imageFrameZ2,markersXYTilt );

      if(reNumberMarkerSelection.isSelected()){
         reNumberMarker = true;
      }
      else{
         reNumberMarker = false;
      }
   }

   void makeImgFrame(ImageFrame  imageFrameZ, int locX, int locY) {

      // if not activated for the first time, check if the window is closed
      if  (!showSliceZactivatedForFirstTime)  {
         imageFrameZOpen = imageFrameZ.isShowing();
      }

      //if the image was opened
      if ( imageFrameZOpen) {
         return;
      }
      // if activated for the first time OR if the window is closed
      else{
         imageFrameZ.wdp.addMouseMotionListener(this);
         imageFrameZ.wdp.addMouseListener(this);

         jButtonCloseAll.addActionListener(imageFrameZ);
         utilParametersImageSeries.tiltPairShown = true;

         imageFrameZ.setLocation(locX, locY);
         imageFrameZ.setVisible(true);
         imageFrameZOpen = true;
         showSliceZactivatedForFirstTime = false;
      }
   }

   void saveMarkerJButton_actionPerformed(ActionEvent e) {

      // if the vectors are not empty then write to output markerfile
      determineThetaJButton.setEnabled(true);
      if ((markersXYUnTilt.size() != 0) && (markersXYTilt.size() != 0) &&
                          (markersXYUnTilt.size() == markersXYTilt.size())) {
         writeMarkerFile(markersXYUnTilt, docFileNameArray[0], "integer");
         writeMarkerFile(markersXYTilt, docFileNameArray[1], "integer");
         saveMarkerJButton.setEnabled(false);
         keyNumInputText.setEditable(false);
         offsetInputText.setEditable(false);
      }
      else {
         String str = "either no marker is picked or unequal number of \n";
         str = str + " markers picked in the untilted and tilted images ! \n";
         utilParameters.outputMessageTextArea.append(str);
      }
   }

   void hideMarkersJButton_actionPerformed(ActionEvent e) {

      imageFrameZ.wdp.drawMarker = false;
      imageFrameZ.wdp.drawString = false;
      imageFrameZ2.wdp.drawMarker = false;
      imageFrameZ2.wdp.drawString = false;
      imageFrameZ2.wdp.drawRectMarker = false;
      imageFrameZ.wdp.repaint();
      imageFrameZ2.wdp.repaint();
   }

   void unhideMarkersJButton_actionPerformed(ActionEvent e) {

      imageFrameZ.wdp.drawMarker = true;
      imageFrameZ.wdp.drawString = true;
      imageFrameZ2.wdp.drawMarker = true;
      imageFrameZ2.wdp.drawString = true;
      imageFrameZ2.wdp.drawRectMarker = true;
      imageFrameZ.wdp.repaint();
      imageFrameZ2.wdp.repaint();
   }

   void determineThetaJButton_actionPerformed(ActionEvent e) {

      // reset anglesFitted variable since some input parameters might
      // have been edited.
      anglesFitted = false;
      fitAnglesJButton.setEnabled(true);
      keyNumOriginText.setEditable(true);

      updateAngles();
      markersPicked = markersXYUnTilt.size();
      unTiltX = new float [markersPicked];
      unTiltY = new float [markersPicked];
      tiltX = new float [markersPicked];
      tiltY = new float [markersPicked];
      tiltFitX = new float [markersPicked];
      tiltFitY = new float [markersPicked];

      float thetaSent = 0, arealim = tiltAngleAreaG;
      int iarea = 0, retVal= 0;
      thetaDetermined = true;

      if (markersXYUnTilt.size() == markersXYTilt.size()) {
         for (int i = 0; i < markersPicked; i++) {
            unTiltX[i] = (float)((Point) markersXYUnTilt.elementAt(i)).getX();
            unTiltY[i] = (float)((Point) markersXYUnTilt.elementAt(i)).getY();
            tiltX[i] = (float)((Point) markersXYTilt.elementAt(i)).getX();
            tiltY[i] = (float)((Point) markersXYTilt.elementAt(i)).getY();
         }
         retVal = tiltang(unTiltX, unTiltY, tiltX, tiltY, markersPicked, thetaSent, iarea, arealim);

         if (retVal == 0 || retVal == -1) {
            // update tiltangle display
            thetaInputText.setText(df3.format(thetaG));
            tiltAngleAreaText.setText(df4.format(tiltAngleAreaG));
         }
      }
      else {
      // error message that # of markers picked are not equal
      String str = "unequal number of markers picked in tilted and untilted images. \n";
      utilParameters.outputMessageTextArea.append(str);
      }
   }

   void fitAnglesJButton_actionPerformed(ActionEvent e) {

      updateOrigin();
      updateAngles();

      int retFlag = willsq(unTiltX, unTiltY,tiltX,tiltY,
                 markersPicked, thetaG, gammaG, phiG, originUnTiltX,
                                    originUnTiltY, originTiltX, originTiltY);
      if (retFlag == 0) {
         // update phi and gamma display
         phiInputText.setText(df3.format(phiG));
         gammaInputText.setText(df3.format(gammaG));

         anglesFitted = true;
         drawFitLocJButton.setEnabled(true);
      }
      else {
         Toolkit.getDefaultToolkit().beep();
      }

   }
   void drawFitLocJButton_actionPerformed(ActionEvent e) {

      drawFitLocJButton.setEnabled(false);
      saveAnglesJButton.setEnabled(true);

      witran(unTiltX, unTiltY, tiltX, tiltY, markersPicked, gammaG,
                                 thetaG, phiG, originUnTiltX, originUnTiltY,
                                                     originTiltX, originTiltY);

      imageFrameZ.wdp.drawString = true;
      imageFrameZ.wdp.drawMarker = true;
      imageFrameZ2.wdp.drawString = true;

      imageFrameZ2.wdp.drawRectMarker = true;
      imageFrameZ2.wdp.drawMarker = true;

      imageFrameZ2.wdp.rectMarkerNumber = markersPicked;
      imageFrameZ2.wdp.repaint();
      imageFrameZ.wdp.repaint();

      // should write float, not integer
      writeMarkerFile(markersFitTilt, docFileNameArray[3], "float");
   }

   void saveAnglesJButton_actionPerformed(ActionEvent e) {

      saveAnglesJButton.setEnabled(false);
      File fileDelete = new File(docFileNameArray[2]);
      fileDelete.delete();
      String angFileName = docFileNameArray[2];
      float[] regValues= new float [6] ;
      //Write today's date comment line
      String str2 = "dat/" + fileExtension + " ";
      Date today;
      String dateOut;
      DateFormat dateFormatter;
      Locale currentLocale = new Locale("en", "US"); ;
      dateFormatter = DateFormat.getDateTimeInstance(DateFormat.DEFAULT,
      DateFormat.DEFAULT, currentLocale);
      today = new Date();
      dateOut = dateFormatter.format(today);

      str2 = str2 + " " + docFileNameArray[2] + " " + dateOut;
      String op = "w";

      int key = 121;
      for (int i = 0; i <= 3; i++) {
         if (i == 0) {
            regValues[4] = markersXYTilt.size();
            UtilDoc utilDoc1 = new UtilDoc(docFileNameArray[2], str2, op, utilParameters);
            utilDoc1.writeCommentsDoc();
            utilDoc1.closeWriteCommentsDoc();
         }
         if (i == 1) {
            // reset register values
            regValues[4] = 0;
            // set register value regValues[0] = 1 if angles fitted , = 0 otherwise.
            // Angles already fitted. This is always true, otherwise,
            // draw-fitted-location button will not be enabled
            regValues[0] = 1;
            str2 = "FITTED flag";
            UtilDoc utilDoc2 = new UtilDoc(docFileNameArray[2], str2, op, utilParameters);
            utilDoc2.writeCommentsDoc();
            utilDoc2.closeWriteCommentsDoc();
         }
         if (i == 2) {
            str2 = "X0, Y0 ORIG. IN 0 DEG IM., XS0, YS0 ORIG. IN TILTED IM. REDUCTION FACTOR";
            UtilDoc utilDoc3 = new UtilDoc(docFileNameArray[2], str2, op, utilParameters);
            utilDoc3.writeCommentsDoc();
            utilDoc3.closeWriteCommentsDoc();

            regValues[0]  = originUnTiltX;
            regValues[1]  = originUnTiltY;
            regValues[2]  = originTiltX;
            regValues[3]  = originTiltY;
            regValues[4]  = resizeFactor;
         }
         if (i == 3) {
            str2 = "TILTANGLE, AXIS DIR. IN: 0 DEG IM. THETA GAMMA PHI";
            UtilDoc utilDoc4 = new UtilDoc(docFileNameArray[2], str2, op, utilParameters);
            utilDoc4.writeCommentsDoc();
            utilDoc4.closeWriteCommentsDoc();

            // reset register values
            regValues[3]  = 0;
            regValues[4]  = 0;
            // set register values
            regValues[0]  = thetaG;
            regValues[1]  = gammaG;
            regValues[2]  = phiG;
         }

         UtilDoc utilDoc = new UtilDoc(docFileNameArray[2], key, 6,
                                                   regValues,op, utilParameters);
         utilDoc.writeDoc();
         utilDoc.closeWriteDoc();

         key ++;
      }
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

   public void writeMarkerFile(Vector markersXY, String fileName, String dataType) {

      int offsetKeyNo;
      // get value of resizeFactor
      resizeFactor = imageFrameZ.utilImageManipulate.scale;

      PrintWriter out;
      try{
         out = new PrintWriter(new BufferedWriter(new FileWriter(
                                                            fileName,false)));
         utilParameters.outputMessageTextArea.append(fileName.concat("\n"));

         DecimalFormat df2 = new DecimalFormat("####0.000000");
         int index, space;
         //Write today's date comment line
         String str2 = " ;dat/" +fileExtension +  " ";
         Date today;
         String dateOut;
         DateFormat dateFormatter;
         Locale currentLocale = new Locale("en", "US"); ;
         dateFormatter = DateFormat.getDateTimeInstance(DateFormat.DEFAULT,
                                                 DateFormat.DEFAULT,currentLocale);
         today = new Date();
         dateOut = dateFormatter.format(today);
         str2 = str2 + " " + fileName + " " + dateOut;
         str2 = str2.concat("\n");
         out.print(str2);

         //Record the new marker
         DecimalFormat df1 = new DecimalFormat("0000");
         String str1 = new String();
         String str4 = new String();
         Point p = new Point();
         Point2D.Float p2d = new Point2D.Float();
         int keyNo = Integer.parseInt(keyNumInputText.getText().trim());

         int offsetInput = Integer.parseInt(offsetInputText.getText().trim());
         int strComp = dataType.compareTo("integer");
         int markerXpos = -99, markerYpos = -99;
         float markerXposFloat = -99, markerYposFloat = -99;
         int markX = -99, markY = -99;

         for (int j = 0; j < markersXY.size(); j++) {

            //Only record the real markers. Markers with XY coordinate less than
            //0 is actually deleted.  Take care if the image has been resized.
            //write the co-ordinate as per preresized image for integer values
            if (strComp == 0) {
               p = (Point) markersXY.elementAt(j);
               markerXpos = (int)(p.getX()/resizeFactor);
               markerYpos = (int)(p.getY()/resizeFactor);
               //System.out.println(" markerXpos.. = "+markerXpos);

               markX = markerXpos;
               markY = markerYpos;
            }
            // for float values
            else {
               p2d = (Point2D.Float) markersXY.elementAt(j);
               //System.out.println(" p2d.getx = "+p2d.getX());
               markerXposFloat = (float)(p2d.getX()/resizeFactor);
               markerYposFloat = (float)(p2d.getY()/resizeFactor);
               //System.out.println(" markerXposFloat = "+markerXposFloat);
               markX = (int)markerXposFloat;
               markY = (int)markerYposFloat;
            }

            if(markX >= 0 && markY >= 0){
               str1 = df1.format(keyNo);
               str1 = " " + str1 + " ";
               str1 = str1 + "6";

               offsetKeyNo = offsetInput + keyNo;
               // write offsetKeyno in 13 characters including spaces
               str4 = df1.format(offsetKeyNo);
               str1 =  str1 + "        " + str4;

               for (int i = 1; i <= 2; i++) {
                  //x co-ordinate
                  // for integer values
                  if (strComp == 0 && i == 2) {
                     str2 = df2.format(p.getX());
                  }
                  //float
                  else if(strComp != 0 && i == 2){
                     str2 = df2.format(p2d.getX());
                  }
                  if (strComp == 0 && i == 1) {
                     str2 = df2.format(markerXpos);
                  }
                  //float
                  else if(strComp != 0 && i == 1){
                     str2 = df2.format(markerXposFloat);
                  }

                  index = str2.indexOf(".");
                  space = 6 - index;
                  // adds spaces
                  for (int k = 1; k < space; k++) str2 = " " + str2;
                  str1 = str1 + str2;
                  //y co-ordinate
                  // for integer values
                  if (strComp == 0 && i == 2) {
                    str2 = df2.format(p.getY());
                  }
                  // for float values
                  else if (strComp != 0 && i == 2){
                  str2 = df2.format(p2d.getY());
                  //System.out.println("str2 = "+str2);
                  }


                  if (strComp == 0 && i == 1) {
                    str2 = df2.format(markerYpos);
                  }
                  // for float values
                  else if (strComp != 0 && i == 1){
                  str2 = df2.format(markerYposFloat);
                  //System.out.println("str2 = "+str2);
                  }
                  index = str2.indexOf(".");
                  space = 6 - index;
                  // adds spaces
                  for (int k = 1; k < space; k++) str2 = " " + str2;
                  str1 = str1 + str2;
               }
               for (int k = 1; k < 12; k++) str1 = str1 + " ";
               str1 = str1 + "1";
               //change line
               str1 = str1.concat("\n");

               out.print(str1);
               utilParameters.outputMessageTextArea.append(str1);
               keyNo = keyNo + 1;
            }
         }
         out.close();
      }
      catch(IOException er){
         System.err.println(er.getMessage());
      }
   }

   public void drawMarkers(Vector markersXY, ImageFrame imageFrameZ){

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

   public void findClosest (int x, int y, Vector markersXY){

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

   public void moveMarker (int x, int y, Vector markersXY, ImageFrame imageFrameZ){

      int temp = currentMarkerNum;
      if (temp >= 0) {
         //update the marker coordinate
         markersXY.removeElementAt(temp);
         final Point p = new Point(x, y);
         markersXY.insertElementAt(p, temp);

         //draw and label the marker
         drawMarkers(markersXY, imageFrameZ);
      }
   }

   public void removeMarker (ImageFrame imageFrameZ, Vector markersXY){
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
      drawMarkers(markersXY, imageFrameZ);
   }

   void showMarkers(String fileName, ImageFrame imageFrameZ, Vector markersXY) {
      File mf = new File(fileName);
      //if marker file does not exist, remove marker coordinates from previous
      //marker file
      if (!mf.exists())  {
         markersXY.clear();
         imageFrameZ.wdp.drawString = false;
         imageFrameZ.wdp.drawMarker = false;
         imageFrameZ2.wdp.drawRectMarker = false;
         imageFrameZ.wdp.repaint();
         imageFrameZ2.wdp.repaint();

         // reset parameters
         keyNumInputText.setText("1");
         offsetInputText.setText("0");
         saveMarkerJButton.setEnabled(false);
         keyNumInputText.setEditable(false);
         offsetInputText.setEditable(false);
         determineThetaJButton.setEnabled(false);
         fitAnglesJButton.setEnabled(false);
                 keyNumOriginText.setEditable(false);
         drawFitLocJButton.setEnabled(false);
         saveAnglesJButton.setEnabled(false);
         determineThetaJButton.setEnabled(false);
         hideMarkersJButton.setEnabled(false);
         unhideMarkersJButton.setEnabled(false);
         return;
      }
      // update resizeFactor value
      resizeFactor = imageFrameZ.utilImageManipulate.scale;

      boolean KeyDuplicationAllowed = false;

      String op = "r";
      UtilDoc utilDoc = new UtilDoc(fileName,op,utilParameters);

      float [] xRegVal = utilDoc.readDoc(xRegValue, KeyDuplicationAllowed);
      int   [] xRegValInt = new int[xRegVal.length];
      float [] yRegVal = utilDoc.readDoc(yRegValue, KeyDuplicationAllowed);
      int   [] yRegValInt = new int[yRegVal.length];
      int      maxKeyValue = 0;
      float [] keyRegVal = utilDoc.readDoc(keyRegValue, KeyDuplicationAllowed);
      keyNumInputText.setText(Integer.toString((int)keyRegVal[0]));
      float [] offsetRegVal = utilDoc.readDoc(offsetRegValue, KeyDuplicationAllowed);
      offsetInputText.setText(Integer.toString((int)offsetRegVal[0]));
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

         determineThetaJButton.setEnabled(true);
         hideMarkersJButton.setEnabled(true);
         unhideMarkersJButton.setEnabled(true);
         imageFrameZ2.wdp.drawRectMarker = false;
      }
      else {
         String str = "No. of X, Y registers and the key numbers are not same! \n ";
         utilParameters.outputMessageTextArea.append(str);
      }
  }

   public void mouseMoved(MouseEvent e) {

      //change the cursor type
      imageFrameZ.wdp.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
      imageFrameZ2.wdp.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
      String str = "(X,Y) = " + e.getX() + "," + e.getY() + "\n";
      showOutput(str);
   }

   public void mousePressed(MouseEvent e) {

      if( SwingUtilities.isLeftMouseButton(e) ){
         //add the marker coordinate to the marker vector markersXY
         if (imageFrameZ.isActive()) {
            if ((!tiltImagePicked && !unTiltImagePicked) || tiltImagePicked
                            || (markersXYUnTilt.size() == markersXYTilt.size()) ) {
               unTiltImagePicked = true;
               tiltImagePicked = false;
               markersXYUnTilt.add(e.getPoint());
               //draw and label the marker
               drawMarkers(markersXYUnTilt, imageFrameZ);

               Point p = imageFrameZ2.wdp.getLocationOnScreen();
               // if angles already found then move the cursor to the predicted
               // point on the tilted image
               if(anglesFitted) {
                  xPicked[0] = e.getPoint().x;
                  yPicked[0] = e.getPoint().y;
                  witran(xPicked, yPicked, xPredicted, yPredicted, 1, gammaG,
                           thetaG, phiG, originUnTiltX, originUnTiltY,
                                                     originTiltX, originTiltY);
                  rob.mouseMove(p.x + (int)xPredicted[0], p.y + (int)yPredicted[0]);
                }
                else {
                   rob.mouseMove(p.x + e.getPoint().x, p.y + e.getPoint().y);
                }
            }
            else {
               String str = "marker on tilted image is not picked yet !" + "\n";
                utilParameters.outputMessageTextArea.append(str);
            }
         }
         if (imageFrameZ2.isActive()) {
            if ((!tiltImagePicked && !unTiltImagePicked) || unTiltImagePicked
                          || (markersXYUnTilt.size() == markersXYTilt.size())) {
               tiltImagePicked = true;
               unTiltImagePicked = false;
               markersXYTilt.add(e.getPoint());
               //draw and label the marker
               drawMarkers(markersXYTilt,imageFrameZ2);

               Point p = imageFrameZ.wdp.getLocationOnScreen();
               rob.mouseMove(p.x + e.getPoint().x, p.y + e.getPoint().y);
            }
            else {
               String str = "marker on untilted image is not picked yet !" + "\n";
                utilParameters.outputMessageTextArea.append(str);
            }
         }
      }
      else if( SwingUtilities.isRightMouseButton(e) ){
         if (imageFrameZ.isActive()) {
            findClosest(e.getX(), e.getY(), markersXYUnTilt);
         }
         if (imageFrameZ2.isActive()) {
            findClosest(e.getX(), e.getY(), markersXYTilt);
         }
      }
      else {
         if (imageFrameZ.isActive()) {
            findClosest(e.getX(), e.getY(), markersXYUnTilt);
            removeMarker(imageFrameZ, markersXYUnTilt);
            if (markersXYTilt.size() != 0) {
               removeMarker(imageFrameZ2, markersXYTilt);
            }
         }
         if (imageFrameZ2.isActive()) {
            findClosest(e.getX(), e.getY(), markersXYTilt);
            removeMarker(imageFrameZ2, markersXYTilt);
            if (markersXYUnTilt.size() != 0) {
               removeMarker(imageFrameZ, markersXYUnTilt);
            }
         }
      }
      saveMarkerJButton.setEnabled(true);
      keyNumInputText.setEditable(true);
      offsetInputText.setEditable(true);
      determineThetaJButton.setEnabled(false);
      fitAnglesJButton.setEnabled(false);
      keyNumOriginText.setEditable(false);
      drawFitLocJButton.setEnabled(false);
      saveAnglesJButton.setEnabled(false);

   }

   public void mouseDragged(MouseEvent e) {
      if( SwingUtilities.isRightMouseButton(e) ){
         if (imageFrameZ.isActive()) {
            moveMarker(e.getX(), e.getY(), markersXYUnTilt, imageFrameZ);
         }
         if (imageFrameZ2.isActive()) {
            moveMarker(e.getX(), e.getY(), markersXYTilt, imageFrameZ2);
         }
      }
      saveMarkerJButton.setEnabled(true);
      keyNumInputText.setEditable(true);
      offsetInputText.setEditable(true);
      determineThetaJButton.setEnabled(false);
      fitAnglesJButton.setEnabled(false);
      keyNumOriginText.setEditable(false);
      drawFitLocJButton.setEnabled(false);
      saveAnglesJButton.setEnabled(false);
   }

   public void mouseClicked(MouseEvent e) {

   }

   public void mouseReleased(MouseEvent e) {

   }

   public void mouseEntered(MouseEvent e) {

   }

   public void mouseExited(MouseEvent e) {

   }


   int tiltang(float [] x0, float [] y0, float [] xt, float [] yt,
             int npoint, float thetaIn, int iarea, float arealim) {

      int	i, k, ntot, iflag;
      float	area0, areat, w, sum, temp;
      float   x01, x02, y01, y02, xt1, xt2, yt1, yt2;

      //const float pi = 3.14159;
      final float pi = (float)3.14159;

      sum    = (float) 0.0;
      iarea = 0;
      ntot   = 0;
      float theta = thetaIn;

      if (npoint < 3) {
         String str = " *** Unable to compute Theta; Need > 2 points! \n";
         utilParameters.outputMessageTextArea.append(str);
         Toolkit.getDefaultToolkit().beep();
         return 1;
      }

      iflag = 0;
      for (i = 0; i < npoint; i++) {
         for (k = i + 1; k < npoint-1; k++) {
            /* area in untilted image: */
            x01    = x0[k]   - x0[i];
            y01    = y0[k]   - y0[i];
            x02    = x0[k+1] - x0[i];
            y02    = y0[k+1] - y0[i];
            //       area0  = (float) fabs( (double)(x01 * y02 - x02 * y01));
            area0  = (float) Math.abs( (double)(x01 * y02 - x02 * y01));
            ntot++;

            /* default arealim is 5000 sq. pixels */
            if (area0 >= arealim) {
               /* only triangles > arealim are considered */
               /* area in tilted image: */
               xt1   = xt[k]   - xt[i];
               yt1   = yt[k]   - yt[i];
               xt2   = xt[k+1] - xt[i];
               yt2   = yt[k+1] - yt[i];
               areat =  (float) Math.abs( (double)(xt1 * yt2 - xt2 * yt1));

               if (areat >= arealim) {
                  /* area in tilted image should be <= area in untilted */
                  w = areat / area0;

                  if (w <= 1.0) {
                     theta    = (float)Math.acos(w);
                     sum       = sum + theta;
                     (iarea)++;
                  }
                  else {
                     /* set bad location return flag */
                     iflag = -1;
                     Toolkit.getDefaultToolkit().beep();

                     String str = "*** Check keys: " + Integer.toString(i+1)+" "
                                 + Integer.toString(k+1)+" " + Integer.toString(k+2)
                                                                 +" for a bad point \n";
                     utilParameters.outputMessageTextArea.append(str);
                  }
               }
            }
         }
      }

      if (iarea == 0) {
         String str = " *** Unable to compute tilt angle; Need 3 points with area > arealim! \n";
         utilParameters.outputMessageTextArea.append(str);
         Toolkit.getDefaultToolkit().beep();
         return 1;
      }

      theta = sum    / (iarea);
      theta = theta / pi * (float)180.0;

      String str = "Areas used for theta: " + iarea +" out of possible: " +ntot +"\n";
      utilParameters.outputMessageTextArea.append(str);
      thetaG = theta;

      return iflag;
   }

      /*$Header: /net/bali/usr1/web/jweb/src/RCS/UtilGUIImageSeriesTiltPair.java,v 1.1 2008/06/12 16:39:20 leith Exp $*/
      /*
      C   TRANSFERED FROM WILLSQ.FOR BY JING SU 8/31/93
      C
      C++****************************************************************************
      C   PROGRAM TO CALCULATE A LINEAR LEAST SQUARE FIT TO DETERMINE
      C   THE DIRECTION OF THE TILT-AXIS, THE TILT ANGLE AND THE RELATIVE
      C   POSITION OF A TILT PAIR (0 DEG VERSUS TILTED)
      C
      C   PARAMETERS:
      C   X         ARRAY  X-COORD. OF PARTICLES IN 0 DEG IMAGE     (SENT)
      C   Y         ARRAY  Y-COORD. OF PARTICLES IN 0 DEG IMAGE     (SENT)
      C   XS        ARRAY  X-COORD. OF PARTICLES IN TILTED IMAGE    (SENT)
      C   YS        ARRAY  Y-COORD. OF PARTICLES IN TILTED IMAGE    (SENT)
      C   XU0T,YU0T   LOCATION OF ORIGIN IN UNTILTED IMAGE          (SENT)
      C   XS0T,YS0T LOCATION OF ORIGIN IN TILTED IMAGE       (SENT & RETURNED)
      C   PHI DIRECTION OF TILT AXIS IN TILTED IMAGE (RELATIVE TO Y)(RETURNED)
      C   GAM DIRECTION OF TILT AXIS IN UNTILTED IMAGE        (RETURNED)
      C   THE TILTANGLE THETA                                 (SENT)
      C   N   NUMBER OF MESUREMENTS                           (SENT)
      C   AVAL, BVAL   ARE ALLOCATED BUT NOT USED ELSEWHERE   (ADDRESS SENT)
      C
      C   RETURNS:    0   OK
      C               1   LEAST SQUARE FIT IMPOSSIBLE OR FAILS
      C                   IF (FAILS GAMMA AND PHI NOT ALTERED)
      C
      C
      C   VARIABLES:
      C   EPS   ARRAY  ERROR E(1)->XS0, E(2)->YS0, E(3)->PHI, E(4)->THE
      C
      C   THE NORMAL EQUATIONS SOLVED ARE:
      C
      C      ( A   * A   + B  * B  ) D    =   A  (XS -FX ) +  B  (YS - FY )
      C         KI    IL    KI   IL   L        KI   I   I      KI   I    I
      C
      C   WITH D = delta XS0, D = delta YS0, D =delta PHI, D =delta THE
      c         1              2              3             4
      c
      c   Fx := (X cos(gam)-Y sin(gam))cos(THE)cos(PHI)
      c     i     i          i
      c
      c         +(X sin(gam)+Y cos(gam))sin(phi) + XS0
      c            i          i
      c
      c   Fy := -(X cos(gam) - Y sin(gam))cos(the)sin(phi)
      c            i            i
      c
      c        +(X sin(gam)+Y cos(gam))cos(phi) + YS0
      c           i          i
      c
      c   A  = dFx /dXS0, A  = dFx /dYS0, A  = dFx /dPHI, A  = dFx /dGAM
      c    i1     i        i2     i        i3     i        i4     i
      c
      c   B  = dFy /dXS0, B  = dFy /dYS0, B  = dFy /dPHI, B  = dFy /dGAM
      c    i1     i        i2     i        i3     i        i4     i
      c
      c-*****************************************************************************
      */



      int willsq(float [] x, float [] y, float [] xs, float []ys,
                  int n, float thetaw, float gammaw, float phiw, float xu0t,
                                     float yu0t, float xs0t, float ys0t)      {

       float [][] sqa = new float [4][5], sqb = new float [4][4];
       float [] r = new float[4];
       float            rthe,rphi,rgam,cthe,cphi,cgam,sphi,sgam;
       float            eps, check, qxsum,qysum, fx, fy;
       int              i,it,k,l;
       float            phi, gamma, theta;

       float   []  aval = new float [4*n], xsmf = new float [n], ysmf = new float [n];

      //       const float pid = (3.1415927 / 180.0);
      //       const float pud = (180.0 / 3.1415927);
      final  float pid =(float) (3.1415927 / 180.0);
      final  float pud = (float)(180.0 / 3.1415927);

      if (n < 3) {
         String str = " *** Unable to fit angles: 3 or more points needed \n";
         utilParameters.outputMessageTextArea.append(str);
         return 1;
      }

      phi   = phiw;
      gamma = gammaw;
      theta = thetaw;

      rthe = theta * pid;
      rphi = phi   * pid;
      rgam = gamma * pid;

      cthe = (float)Math.cos(rthe);
      cphi = (float)Math.cos(rphi);
      cgam = (float)Math.cos(rgam);
      sphi = (float)Math.sin(rphi);
      sgam = (float)Math.sin(rgam);
      it   = 0;

      do {
         it++;

         if (it > 100) {
            String str = "***Determination of fit angles failed at 100 iterations! ";
            utilParameters.outputMessageTextArea.append(str);
            return 1;
         }

         qxsum = 0;   qysum = 0;

         /* Build system of normal equations build matrice A, calculate XSMF */

         /* Build system of normal equations build matrice A, calculate XSMF */

         /* Ai1:  */
         for (i = 0; i < n; i++) {
            fx = ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * cphi
                + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

            xsmf[i]    = xs[i] - fx;
            qxsum      = qxsum + xsmf[i] * xsmf[i];

            aval[i*4] = (float)1.0;

            /* Ai2: */
            aval[i*4+1] = (float)0.0;

            /* Ai3: */
            aval[i*4+2] =
              - ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * sphi * cthe
                       + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi;

            /* Ai4: */
            aval[i*4+3] =
                (-(x[i] - xu0t) * sgam - (y[i] - yu0t) * cgam) * cthe * cphi
                        + ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * sphi;
         }

         /*  Calculate square matrice Aki * Ail	 */
         for (l = 0; l < 4; l++) {
            for (k = 0; k < 4; k++) {
               sqa[k][l] = (float)0.0;
               for (i = 0; i < n; i++) {
                  sqa[k][l] =  sqa[k][l] + aval[i*4+k] * aval[i*4+l];
               }
            }
         }

         /* Calculate first part of left side of normal equation */
         for (k = 0; k < 4; k++) {
            r[k] = (float)0.0;
            for (i = 0; i < n; i++) r[k] = r[k] + aval[i*4+k] * xsmf[i];
         }

         /*  Build matrice B, calculate YSMF ------------------------------ */

         /* Bil:   */

         for (i = 0; i < n; i++) {
            fy = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * sphi
                  +((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi + ys0t;
            ysmf[i]      = ys[i] - fy;
            qysum          = qysum + ysmf[i] * ysmf[i];
            aval[i*4+0] = (float)0.0;

            /*  Bi2   */
            aval[i*4+1] = (float)1.0;

            /*  Bi3   */
            aval[i*4+2] =
               -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cphi * cthe
               -((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi;

            /* Bi4:				*/
            aval[i*4+3] =
              -(-(x[i] - xu0t) * sgam - (y[i] - yu0t) * cgam) * cthe * sphi
              + ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cphi;
         }

         /* Calculate square matrice Bki * Bil: */

         for (l = 0; l < 4; l++) {
            for (k = 0; k < 4; k++) {
               sqb[k][l] = (float)0.0;
               for (i = 0; i < n; i++) sqb[k][l] = sqb[k][l] + aval[i*4+k] * aval[i*4+l];
            }
         }

         /*  Calculate second part of left side of normal equation:  */
         for (k = 0; k < 4; k++) {
           for (i = 0; i < n; i++) r[k] = r[k] + aval[i*4+k] * ysmf[i];
         }

         /*  Add SQA and SQB   */
         for (k = 0; k < 4; k++) {
            for (l = 0; l < 4; l++) sqa[k][l] = sqa[k][l] + sqb[k][l];
         }

         eps = (float)0.0;
         for (i = 0; i < 4; i++) sqa[i][4] = r[i];
         if (mircol(4,1,5, sqa, eps, r) != 0 ) {
            String str = "*** Least Square Fit failed!"+ "\n";
            utilParameters.outputMessageTextArea.append(str);
            str = "*** Give more coordinates or better start values."+ "\n";
            utilParameters.outputMessageTextArea.append(str);
            Toolkit.getDefaultToolkit().beep();
            return 1;
         }

         xs0t    = xs0t + r[0];
         ys0t    = ys0t + r[1];
         rphi    = rphi + r[2];
         rgam    = rgam + r[3];

         // update origin in the tilt image with new values
         originTiltX = xs0t;
         originTiltY = ys0t;

         phi    = rphi * pud;
         gamma  = rgam * pud;

         /* Determine accuracy of solution */
         cphi   = (float)Math.cos(rphi);
         sphi   = (float)Math.sin(rphi);
         cgam   = (float)Math.cos(rgam);
         sgam   = (float)Math.sin(rgam);

         for (i = 0; i < n; i++) {
            fx = ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * cphi
              + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

            xsmf[i] = xs[i] - fx;
            qxsum   = qxsum + xsmf[i] * xsmf[i];

            fy = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * sphi
                +((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi + ys0t;

            ysmf[i] = ys[i] - fy;
            qysum   = qysum + ysmf[i] * ysmf[i];
         }
         /*
         ---- outputting some info---- resfp is web (old web) results file
         ---- don't think needed to be implemented-------
         if (resfp) fprintf(resfp,
         "Itera: %4d  Phi: %7.2f, Gam: %7.2f, Orig:(%7.2f,%7.2f)",
                                                 it,phi,gamma, xs0t,ys0t);
         if (resfp) fprintf(resfp, "Qxsum: %f  Qysum: %f\n",qxsum,qysum);
         */

         check = Math.abs(r[0]) + Math.abs(r[1]) + Math.abs(r[2]) + Math.abs(r[3]);

      } while (check > 0.00005);

      phiw   = phi;
      gammaw = gamma;

      phiG = phi;
      gammaG = gamma;

      //System.out.println("phiw = "+phiw+ " "+"gammaw = "+gammaw+ " "+"thetaw = "+thetaw );
      return 0;
   }

   /*$Header: /net/bali/usr1/web/jweb/src/RCS/UtilGUIImageSeriesTiltPair.java,v 1.1 2008/06/12 16:39:20 leith Exp $*/
   /*************************************************************************
   *									 *
   * Transfered from mircol.for by Jing Su 8/30/93
   *
   * WARNING: THIS is not suitable for general matrix inversion due to
   *          assignment of array dimension 4x5 in parameters!!!!!!!!!!!
   *									 *
   *************************************************************************/



   /*** willsq.c: if ((ier = mircol(4,1,5,sqa,eps,r)) != NULL ) call!! */

   /******************************** mircol ******************************/

   int mircol(int n, int m, int mm, float [][] a, float eps, float [] x) {

      int   i,ii,iii,j,jjj,k,kkk;
      boolean   [] wurz = new boolean [6];
      float epsq,s;

      epsq = eps * eps;

      for (i = 1; i <= n; i++) {
         wurz[i-1] = true;
         s = a[i-1][i-1];
         if (i != 1) {
            iii = i -1;
            for (j = 1; j <= iii; j++) {
               if (!wurz[j-1]) s = s + a[j-1][i-1] * a[j-1][i-1];
               else s = s - a[j-1][i-1] * a[j-1][i-1];
            }
         }
         if (s <= 0) {
            s = -s;
            wurz[i-1] = false;
         }
         if (s < epsq) return -1;

         a[i-1][i-1] =(float) Math.sqrt(s);
         iii         = i+1;

         for (k = iii; k <= mm; k++) {
            s   = a[i-1][k-1];
            jjj = i-1;

            if (jjj >= 1) {
               for (j = 1; j <= jjj; j++) {
                  if (!wurz[j-1]) s = (s + a[j-1][i-1] * a[j-1][k-1]);
                  else s = (s - a[j-1][i-1] * a[j-1][k-1]);
               }
            }

            if (!wurz[i-1]) s = -s;

            a[i-1][k-1] = s / a[i-1][i-1];
         }
      }

      for (k = 1; k <= m; k++) {
         for ( ii = 1; ii <= n; ii++) {
            i   = n - ii + 1;
            s   = a[i-1][n+k-1];
            kkk = i+1;
            if ( kkk <= n) {
               for (j = kkk; j <= n; j++) s = s - x[j-1] * a[i-1][j-1];
            }
            x[i-1] = s / a[i-1][i-1];
         }
      }
      return 0;
   }

   /*$Header: /net/bali/usr1/web/jweb/src/RCS/UtilGUIImageSeriesTiltPair.java,v 1.1 2008/06/12 16:39:20 leith Exp $*/

   /*
   ***********************************************************************
   *
   * witran.c
   * ported from witran.for by Jing Su
   *
   ***********************************************************************
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
   *  PROGRAM TO APPLY TRANSFORMATION TO X,Y AND OUTPUT XS,YS
   *
   ***********************************************************************
   */
   /***************************** witran ****************************/


   void witran(float [] x, float [] y, float [] xs, float [] ys, int n,
                   float gam, float the, float phi, float xu0t, float yu0t,
                                                       float xs0t, float ys0t) {
      float        rgam, rthe, rphi, cgam, sgam, cphi, sphi, cthe;
      float        cthecphi, cthesphi;
      int          i;
      Point2D.Float p ;

      //       const float pid =  (3.1415927 / 180.0);
      final float pid =  (float)(3.1415927 / 180.0);
      rthe     = the * pid;
      rphi     = phi * pid;
      rgam     = gam * pid;

      cgam     = (float)Math.cos(rgam);
      sgam     = (float)Math.sin(rgam);
      cphi     = (float)Math.cos(rphi);
      sphi     = (float)Math.sin(rphi);
      cthe     = (float)Math.cos(rthe);

      cthecphi = cthe * cphi;
      cthesphi = cthe * sphi;

      for (i = 0; i < n; i++) {
         xs[i] =  ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthecphi
                  + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

         ys[i] = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthesphi
                  + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi +ys0t;
         // fitted marker co-ordinates in the tilted image
         p  =  new Point2D.Float ();
         p.setLocation(xs[i], ys[i]);
         //System.out.println("xfloat = "+ (float)xs[i]   +"yfloat = "+ (float)ys[i]);
         //System.out.println("x0 = "+ p.getX()   +" y0 = "+ p.getY());

         markersFitTilt.add(p);
         imageFrameZ2.wdp.rectMarkers[i] = new Point(Math.round(xs[i]), Math.round(ys[i]));

         tiltFitX[i] = xs[i];
         tiltFitY[i] = ys[i];
         //System.out.println("x = "+ xs[i]   +"y = "+ ys[i]);
      }

      return;
   }

   private void updateInput() {

      //  reset anglesFitted variable
      anglesFitted = false;
      // dcu -- marker file for untilted micrograph
      // dct -- marker file for tilted micrograph
      // dcb -- angle file, dft -- fitted positions of the markers
      // duc -- backgrounding operation output for untilted micrograph
      // dtc -- backgrounding operation output for tilted micrograph

      String nameStr[] = {"dcu","dct","dcb","dft","duc","dtc"};

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      int currDirLen = utilParameters.currDirectory.length();
      fileExtension = utilParameters.currDirectory.substring(index_1+1, currDirLen);

      int index_2 = utilParameters.currDirectory.lastIndexOf("\\");
      String dirPath = utilParameters.currDirectory.substring(0,index_2+1);
      for (int i = 1; i<=nameStr.length; i++){
         nameStr[i-1] = dirPath + nameStr[i-1] ;
      }

      //System.out.println("dirpath = " + dirPath );
      //System.out.println("nameStr = "+ nameStr[0]+ " "+nameStr[1]);

      //System.out.println("fileExtension = "+fileExtension);
      int docNum = Integer.parseInt(markerDocFileNoInput.getText().trim());
      //System.out.println("docNum = "+ docNum);
      docFileNameArray = utilFile.makeFileNameArrayFromStrings(nameStr,docNum,fileExtension);

      markerUnTiltDocName = docFileNameArray[0];
      markerTiltDocName   = docFileNameArray[1];

      if (docNum != docNumSave) {
         docNumSave = docNum;
         markerFileNumChanged = true;
         saveMarkerJButton.setEnabled(false);
         keyNumInputText.setEditable(false);
         offsetInputText.setEditable(false);

         determineThetaJButton.setEnabled(false);
         fitAnglesJButton.setEnabled(false);
         keyNumOriginText.setEditable(false);
         drawFitLocJButton.setEnabled(false);
         saveAnglesJButton.setEnabled(false);
         hideMarkersJButton.setEnabled(false);
         unhideMarkersJButton.setEnabled(false);
         thetaDetermined = false;
      }

      keyNo = Integer.parseInt(keyNumInputText.getText().trim());
      offsetInput = Integer.parseInt(offsetInputText.getText().trim());
      updateAngles();

      // if theta is determined and no new marker has been picked that has not
      // been saved. Do the following operation on the markers that has been
      // picked and saved.
      if (thetaDetermined && !saveMarkerJButton.isEnabled()) {
         updateOrigin();
      }
   }

   void updateOrigin() {

      keyNumOriginVal = Integer.parseInt(keyNumOriginText.getText().trim());
      originUnTiltX  = unTiltX[keyNumOriginVal-1];
      originUnTiltY  = unTiltY[keyNumOriginVal-1];
      originTiltX  = tiltX[keyNumOriginVal-1];
      originTiltY  = tiltY[keyNumOriginVal-1];
   }

   void updateAngles() {

      phiG =  Float.parseFloat(phiInputText.getText().trim());
      gammaG =  Float.parseFloat(gammaInputText.getText().trim());
      thetaG =  Float.parseFloat(thetaInputText.getText().trim());
      tiltAngleAreaG =  Float.parseFloat(tiltAngleAreaText.getText().trim());
   }


   void setDefault() {
      keyNumInputText.setText("1");
      offsetInputText.setText("0");
      keyNumOriginText.setText("1");
      phiInputText.setText("-90");
      gammaInputText.setText("-90");
      thetaInputText.setText("0");
      tiltAngleAreaText.setText("5000.00");
   }

   void disableButtonGroup(){

      saveMarkerJButton.setEnabled(false);
      determineThetaJButton.setEnabled(false);
      fitAnglesJButton.setEnabled(false);

      drawFitLocJButton.setEnabled(false);
      saveAnglesJButton.setEnabled(false);
      determineThetaJButton.setEnabled(false);
      hideMarkersJButton.setEnabled(false);
      unhideMarkersJButton.setEnabled(false);
   }
}
