package web;

import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.media.jai.*;
import java.io.*;
import java.util.*;


/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIImageSeriesCategorize  extends JPanel implements
                                        MouseMotionListener, MouseListener{

   private SpiderImage spiderVolume;
   private UtilParameters utilParameters;
   private UtilParametersImageSeries utilParametersImageSeries;
   private int nsam, nrow, nslice;
   private JButton jButtonCloseAll;
   private ImageFrame imageFrame;
   private String [] files;
   private int imageNumberFromFile, imageNumberFromFilePrev, noOfRepeat;
   private UtilFile utilFile;

   private int noOfSlices = 0;
   private int noOfImgInRow = 0;
   private int noOfImgInColumn = 0;
   private int posX = 0;
   private int posY = 0;

   private JLabel docFile, keyNum, currCategory, numOfCategories;
   private JLabel displayString;
   private JTextField docFileInput, keyNumInput;
   private JButton startJButton, stopJButton;
   private JComboBox numOfCategoriesComboBox;
   private JRadioButton pickASetOfParticlesRB, pickIndividualParticlesRB;
   private ButtonGroup buttonGroup;

   private String docName, docNameSave= "", currCategoryString;
   private int keyRegValue, keyRegVal, numOfCategoriesValue, currCategoryValue;
   private int keyRegValueSave;
   private int  numberOfRegisters;
   private float [] regValArray = new float [2];
   private float scale;
   private boolean secondClick = false;
   private boolean alternateMethod = true;

   public UtilGUIImageSeriesCategorize(SpiderImage spiVol, String [] fileNames,
                        JButton closeAll, UtilParameters utilParam,
                                UtilParametersImageSeries utilParamImgSeries) {

      spiderVolume = spiVol;
      files = fileNames;
      utilParameters = utilParam;
      jButtonCloseAll = closeAll;
      utilParametersImageSeries = utilParamImgSeries;
      nsam = spiderVolume.getnsam();
      nrow = spiderVolume.getnrow();
      // nslice is set to # of images in SpiderImage class
      nslice = spiderVolume.getnslice();
      // default values
      currCategoryValue = 1;
      numOfCategoriesValue = 1;

      GridBagLayout gridBagLayout = new GridBagLayout();
      this.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      docFile = new JLabel("  Doc. File Name: ");
      addComponent(docFile,gridBagConstraints,2,1,2,1,"LEFT");

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);

      docFileInput = new JTextField(fileNameWOExtension+".ctg",15);
      addComponent(docFileInput,gridBagConstraints,5,1,3,1,"LEFT");

      keyNum = new JLabel("  Starting Key No. : ");
      addComponent(keyNum,gridBagConstraints,2,3,2,1,"LEFT");

      keyNumInput = new JTextField("1",4);
      addComponent(keyNumInput,gridBagConstraints,5,3,1,1,"LEFT");

      numOfCategories = new JLabel("  No. of Categories: ");
      addComponent(numOfCategories,gridBagConstraints,2,5,2,1,"LEFT");

      numOfCategoriesComboBox = new JComboBox();
      for (int i =1; i < 10; i++) {
         String str = Integer.toString(i);
         numOfCategoriesComboBox.addItem(str);
      }
      addComponent(numOfCategoriesComboBox,gridBagConstraints,5,5,1,1,"LEFT");

      numOfCategoriesComboBox.addActionListener(
                                          new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            numOfCategoriesComboBox_actionPerformed(e);
         }
      });

      pickASetOfParticlesRB = new JRadioButton("Pick A Set of Particles");
      addComponent(pickASetOfParticlesRB,gridBagConstraints,8,1,1,1,"LEFT");
      pickASetOfParticlesRB.setSelected(true);
      pickASetOfParticlesRB.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            alternateMethod = true;
         }
      });

      pickIndividualParticlesRB = new JRadioButton("Pick Individual particles");
      addComponent(pickIndividualParticlesRB,gridBagConstraints,8,2,1,1,"LEFT");
      pickIndividualParticlesRB.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            alternateMethod = false;
         }
      });

      buttonGroup = new ButtonGroup();
      buttonGroup.add(pickASetOfParticlesRB);
      buttonGroup.add(pickIndividualParticlesRB);

      startJButton = new JButton("     START     ");
      addComponent(startJButton,gridBagConstraints,6,9,2,1,"LEFT");
      startJButton.addActionListener (new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
               startJButton_actionPerformed(e);
            }
      });
      setVisible(true);
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
      // make it aligned; didn't put the showMontage button in the middle,
      // so commented
      //gridBagConstraints.fill = GridBagConstraints.BOTH;
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

   public void numOfCategoriesComboBox_actionPerformed(ActionEvent e) {

      JComboBox source = (JComboBox)e.getSource();
      String keyNumString = (String)source.getSelectedItem();
      numOfCategoriesValue = Integer.parseInt(keyNumString);
   }

   public void startJButton_actionPerformed(ActionEvent e) {

      // if docfile exists then read the last key number and show it
      // this is done only after the Start button is pressed.
      utilFile = new UtilFile(utilParameters);
      inputUpdate();
      File docFileObj = new File(docName);
      if (docFileObj.exists()) {
         String op = "r";
         UtilDoc docFile = new UtilDoc(docName, op, utilParameters);
         int lastKeyNum = docFile.getLastKeyNum();
         keyNumInput.setText(Integer.toString(lastKeyNum+1));
      }

      int columnsLVal = 1;
      int columnsHVal = nsam;

      int rowsLVal = 1;
      int rowsHVal = nrow;

      int slicesLVal = 1;
      int slicesHVal = nslice;

      noOfSlices = 0;
      int sliceWidth = 0;
      int sliceHeight = 0;
      noOfImgInRow =0;
      noOfImgInColumn = 0;
      int scrnWid = 0;

      String axisSelected = "z";

      noOfSlices   = slicesHVal  - slicesLVal  + 1;
      sliceWidth   = columnsHVal - columnsLVal + 1;
      sliceHeight  = rowsHVal    - rowsLVal    + 1;

      // find scaling of the image
      if (utilParameters.resizeLarge) {
         scale = utilParameters.resizeFactor;
      }
      else {
         scale = (1/utilParameters.resizeFactor);
      }

      scrnWid = utilParameters.screenDimension.width-50;
      // reset the screen width as per the image scaling
      // the image will be scaled in ImageFrame.java
      scrnWid = (int)(scrnWid/scale) ;

      noOfImgInRow = scrnWid/sliceWidth;
      if ( noOfImgInRow == 0) noOfImgInRow = 1;
      if ( noOfSlices <= noOfImgInRow) noOfImgInRow = noOfSlices;
      // add noOfImgInRow -1 pixels for a blank pixel after each image
      int montagedSlicesWidth = noOfImgInRow * sliceWidth + noOfImgInRow -1;
      int k = noOfSlices / noOfImgInRow;
      int l = noOfSlices % noOfImgInRow;

      if (l != 0) k= k + 1;
      // add k-1 pixels for a blank pixel after each image
      int montagedSlicesHeight = k*sliceHeight + k-1;

      UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
      TiledImage montagedSlices;
      montagedSlices = utilImageCreate.getMontagedSlices(spiderVolume,
             axisSelected, columnsLVal, columnsHVal, rowsLVal, rowsHVal,
                  slicesLVal, slicesHVal, noOfImgInRow, noOfSlices,sliceWidth,
                           sliceHeight,montagedSlicesWidth, montagedSlicesHeight);

      imageFrame = new ImageFrame(montagedSlices, utilParameters);
      jButtonCloseAll.addActionListener(imageFrame);
      currCategoryString = "Left Click to SELECT image  :"
         +"  Right Click to INCREMENT category  :  Current Selected Category is  "
             +Integer.toString(currCategoryValue) + "  :  No. of Categories are "
                           +Integer.toString(numOfCategoriesValue);

      imageFrame.setTitle(currCategoryString);
      imageFrame.setLocation (50,50);
      imageFrame.setVisible(true);
      utilParametersImageSeries.categorizeShown = true;

      imageFrame.pixelMenuItem.setEnabled(false);
      imageFrame.translateMenuItem.setEnabled(false);
      imageFrame.interpolateMenuItem.setEnabled(false);
      imageFrame.wdp.addMouseMotionListener(this);
      imageFrame.wdp.addMouseListener(this);
   }

   public void mousePressed(MouseEvent e) {

      // left mouse button is pressed
      if((e.getModifiers() & InputEvent.BUTTON1_MASK) !=0 ) {
         posX = e.getX();
         posY = e.getY();

         // reset posX and posY as per the scaling of the image
         posX = (int)(posX/scale);
         posY = (int)(posY/scale);

         // read input values
         inputUpdate();

         int imageNumber = 0;
         if (nslice % noOfImgInRow == 0) {
            noOfImgInColumn = nslice/noOfImgInRow;
         }
         else {
            noOfImgInColumn = nslice/noOfImgInRow + 1;
         }
         boolean cursorInBlankSpace = false;
         for (int i = 1; i <= noOfImgInRow; i++) {
            for (int j = 1; j <= noOfImgInColumn; j++) {
               if((((i*nsam +i-6) <= (posX+1)) && ((posX+1) <= (i*nsam +i+6)))||
                   (((j*nrow +j-6) <= (posY+1)) && ((posY+1) <= (j*nrow +j+6)))) {
                     cursorInBlankSpace = true;
               }
               // do it until image number is found, once image number is found
               // no need to do anything
               if (imageNumber == 0) {
                  if ( ((posX+1) <= ((i*nsam) + (i-1))) &&
                                       ((posY+1) <= ((j*nrow) + (j-1)))) {
                     imageNumber = i + (j-1)*noOfImgInRow;

                     if (imageNumber > nslice) {
                        cursorInBlankSpace = true;
                     }
                  }
               }
            }
         }
         imageNumberFromFile = utilFile.findFileNumber(files[imageNumber-1]);

         if (!cursorInBlankSpace){

            regValArray [0] = imageNumberFromFile;
            regValArray [1] = currCategoryValue;
            if (!docName.equalsIgnoreCase(docNameSave)) {
               // true for the first time and also when the document name is changed
               keyRegValueSave = -10000;
            }
            if (keyRegValue == keyRegValueSave ) {
               // false for the first mouse click and also when key reg is changed
               if (!alternateMethod) {
                  keyRegVal++;
               }
            }
            else {
               keyRegVal = keyRegValue;
            }
            numberOfRegisters = 2;
            String op = "w";

            if (alternateMethod) {
               // Pick series of particles. Click once to set the mark for
               // begining of a series of good particles and then click on the
               // particle which is the last particle in the series.

               if (secondClick){
                  noOfRepeat = imageNumberFromFile - imageNumberFromFilePrev + 1;

                  for (int repeat = 1; repeat <= noOfRepeat; repeat++) {

                     regValArray [0] = imageNumberFromFilePrev;
                     UtilDoc docFile = new UtilDoc(docName,keyRegVal,
                               numberOfRegisters,regValArray,op,utilParameters);
                     docFile.writeDoc();
                     docFile.closeWriteDoc();
                     imageNumberFromFilePrev ++;
                     if (repeat != noOfRepeat) keyRegVal ++;
                  }
                  pickIndividualParticlesRB.setEnabled(true);
                  secondClick = false;
               }
               else {
                  // first click marking the begining of a good particle series
                  imageNumberFromFilePrev = imageNumberFromFile;
                  pickIndividualParticlesRB.setEnabled(false);
                  if (keyRegValue == keyRegValueSave )  keyRegVal ++;
                  secondClick = true;
               }
            }
            else {
               UtilDoc docFile = new UtilDoc(docName,keyRegVal,
                              numberOfRegisters,regValArray,op,utilParameters);
               docFile.writeDoc();
               docFile.closeWriteDoc();
            }
            keyRegValueSave = keyRegValue;
            docNameSave = docName;

            // take care of the scaling so that string is drawn at the proper
            // place
            posX = (int)(posX*scale);
            posY = (int)(posY*scale);

            imageFrame.wdp.drawString = true;
            if (alternateMethod) {
               if (secondClick) {
                  // display begining of a good particle series
                  imageFrame.wdp.strings[imageFrame.wdp.stringNumber] =
                                   Integer.toString(currCategoryValue) +"-> ";
               }
               else {
                  // display end of the good particle series
                  imageFrame.wdp.strings[imageFrame.wdp.stringNumber] =
                                  " <-" + Integer.toString(currCategoryValue);
               }
            }
            else {
               // pick individual particles
               imageFrame.wdp.strings[imageFrame.wdp.stringNumber] =
                                           Integer.toString(currCategoryValue);
            }
            imageFrame.wdp.stringPos[imageFrame.wdp.stringNumber] =
                                                          new Point(posX,posY);
            imageFrame.wdp.stringNumber++;
            imageFrame.wdp.repaint();
         }
      }
      // right mouse button is pressed
      if((e.getModifiers() & InputEvent.BUTTON3_MASK) !=0 ) {
         // increment the category number; reset to 1 when max category number
         // is reached
         if (currCategoryValue < numOfCategoriesValue) {
            currCategoryValue = currCategoryValue + 1;
         }
         else {
            currCategoryValue = 1;
         }
         currCategoryString = "Left Click to SELECT image  :"
           +"  Right Click to INCREMENT category  :  Current Selected Category is  "
              +Integer.toString(currCategoryValue) + "  :  No. of Categories are "
                  +Integer.toString(numOfCategoriesValue);
         imageFrame.setTitle(currCategoryString);
      }
   }

   public void inputUpdate() {
      docName  = docFileInput.getText().trim();
      keyRegValue  = Integer.parseInt(keyNumInput.getText().trim());
   }

   public ImageFrame getImageFrame() {
      return imageFrame;
   }

   public void mouseMoved(MouseEvent e) {
      imageFrame.wdp.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
   }
   public void mouseDragged(MouseEvent e) {
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
