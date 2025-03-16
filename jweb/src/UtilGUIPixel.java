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

public class UtilGUIPixel extends JFrame implements ActionListener,
                                      MouseMotionListener, MouseListener{

   private JCheckBox  docFileSelection;
   private JLabel     docFile, keyNum, XRegNum, YRegNum, ScreenValueRegNum,
                      FileValueRegNum;
   private JTextField docFileInput, keyNumInput, XRegNumInput, YRegNumInput,
                      ScreenValueRegNumInput, FileValueRegNumInput;
   private JRadioButton screenValue, fileValue, leaveMarker;
   private ButtonGroup buttonGroup;
   private WebDisplayPanel wdp;
   private boolean bool;
   private UtilImageStatistics uis;
   private ImageFrame imageFrame;
   private float[] pixelScreen = new float[3];
   private Color colorScreen;
   private SpiderImage spiderImage;
   private float actualPixel;
   private RenderedOp ct;
   private UtilImageManipulate utilImageManipulate;
   private UtilParameters utilParameters;
   private String docName, docNameSave= "";
   private int keyRegValue, xRegValue, yRegValue, screenRegValue, fileRegValue;
   private float [] regValArray = new float [9];
   private int numberOfRegisters, posX, posY, keyRegValueSave, keyRegVal;
   private int lineCountSave = -7;


   public UtilGUIPixel(ImageFrame imgFr, WebDisplayPanel panel, SpiderImage spiImage,
         UtilImageManipulate uim, boolean montage, UtilParameters utilParam) {

      utilParameters = utilParam;
      imageFrame = imgFr;
      wdp = panel;
      spiderImage = spiImage;
      imageFrame.editMenu.setEnabled(false);
      if (!montage) {
         // if single image read Spider raw data and save in float [] spiderImage.fpix.
         try {
            spiderImage.readRawData();
         }
         catch (IOException e) {
            String str = "Error reading SpiderImage raw data: UtilGUIPixel " + e + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
      }
      utilImageManipulate = uim;
      uis = new UtilImageStatistics(wdp,utilImageManipulate);
      setTitle("Pixel Popup Menu");
      setSize(400,500);

      Container contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      docFileSelection = new JCheckBox("Save Selection in Document File: ");
      docFileSelection.setSelected(false);
      addComponent(contentPane,docFileSelection,gridBagConstraints,0,0,3,1);
      docFileSelection.addActionListener(this);

      docFile = new JLabel("Doc. File : ");
      addComponent(contentPane,docFile,gridBagConstraints,1,1,1,1);

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);

      docFileInput = new JTextField(fileNameWOExtension+".pix",15);
      addComponent(contentPane,docFileInput,gridBagConstraints,2,1,2,1);

      keyNum = new JLabel("Key No. : ");
      addComponent(contentPane,keyNum,gridBagConstraints,1,2,1,1);

      keyNumInput = new JTextField("1",5);
      addComponent(contentPane,keyNumInput,gridBagConstraints,2,2,1,1);

      XRegNum = new JLabel("X Reg. : ");
      addComponent(contentPane,XRegNum,gridBagConstraints,3,2,1,1);

      XRegNumInput = new JTextField("1",5);
      addComponent(contentPane,XRegNumInput,gridBagConstraints,4,2,1,1);

      YRegNum = new JLabel("Y Reg. : ");
      addComponent(contentPane,YRegNum,gridBagConstraints,5,2,1,1);

      YRegNumInput = new JTextField("2",5);
      addComponent(contentPane,YRegNumInput,gridBagConstraints,6,2,1,1);


      ScreenValueRegNum = new JLabel("Screen Value Reg. : ");
      addComponent(contentPane,ScreenValueRegNum,gridBagConstraints,1,3,1,1);

      ScreenValueRegNumInput = new JTextField("3",5);
      addComponent(contentPane,ScreenValueRegNumInput,gridBagConstraints,2,3,1,1);

      FileValueRegNum = new JLabel("File Value Reg. : ");
      addComponent(contentPane,FileValueRegNum,gridBagConstraints,3,3,1,1);

      FileValueRegNumInput = new JTextField("4",5);
      addComponent(contentPane,FileValueRegNumInput,gridBagConstraints,4,3,1,1);


      screenValue = new JRadioButton("Screen Value");
      screenValue.setSelected(true);
      addComponent(contentPane,screenValue,gridBagConstraints,1,4,1,1);
      screenValue.addActionListener(this);

      fileValue = new JRadioButton("File Value");
      fileValue.setSelected(false);
      addComponent(contentPane,fileValue,gridBagConstraints,1,5,1,1);
      fileValue.addActionListener(this);
      if (montage) fileValue.setEnabled(false);

      leaveMarker = new JRadioButton("Leave Marker");
      leaveMarker.setSelected(false);
      addComponent(contentPane,leaveMarker,gridBagConstraints,1,6,1,1);
      leaveMarker.addActionListener(this);

      setFirstSet(false);
      setSecondSet(false);
      setThirdSet(false);
      wdp.addMouseMotionListener(this);
      wdp.addMouseListener(this);

      bool = false;
      pack();
      setVisible(true);
  }

   public void setFirstSet(boolean bool) {
      docFile.setEnabled(bool);
      docFileInput.setEnabled(bool);
      keyNum.setEnabled(bool);
      keyNumInput.setEnabled(bool);
      XRegNum.setEnabled(bool);
      XRegNumInput.setEnabled(bool);
      YRegNum.setEnabled(bool);
      YRegNumInput.setEnabled(bool);
   }

   public void setSecondSet(boolean bool) {
      ScreenValueRegNum.setEnabled(bool);
      ScreenValueRegNumInput.setEnabled(bool);
   }

   public void setThirdSet(boolean bool) {
      FileValueRegNum.setEnabled(bool);
      FileValueRegNumInput.setEnabled(bool);
   }

   public void addComponent(Container contentPane, Component comp,
                               GridBagConstraints gridBagConstraints, int gridX,
                               int gridY, int gridWidth, int gridHeight) {

      gridBagConstraints.gridx = gridX;
      gridBagConstraints.gridy = gridY;
      gridBagConstraints.gridwidth = gridWidth;
      gridBagConstraints.gridheight = gridHeight;
      // will grow to fill the space if the window size is changed
      gridBagConstraints.weightx = 100;
      gridBagConstraints.weighty = 100;
      // make it aligned
      gridBagConstraints.fill = GridBagConstraints.BOTH;
      contentPane.add(comp,gridBagConstraints);
   }

   public void actionPerformed(ActionEvent e){
      if (docFileSelection.isSelected()){
         setFirstSet(true);
         if (screenValue.isSelected()){
            setSecondSet(true);
            bool = true;
         }
         else {
            setSecondSet(false);
         }
         if (fileValue.isSelected()){
            setThirdSet(true);
         }
         else {
            setThirdSet(false);
         }
      }
      else {
         setFirstSet(false);
         setSecondSet(false);
         setThirdSet(false);
      }
   }

 // stop the mouse listener and reinstall the default cursor if the pixel pop up
 // menu is closed
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      wdp.removeMouseMotionListener(this);
      wdp.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
      wdp.drawMarker = false;
      wdp.drawString = false;
      wdp.repaint();
      imageFrame.editMenu.setEnabled(true);
      imageFrame.pixelFrameOpen = false;
      }
   }

   public void mouseMoved(MouseEvent e) {
      //wdp.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));

      Point point =e.getPoint();
      int x = (int)point.getX();
      int y = (int)point.getY();
      // if mouse is inside the image
      if ((x < wdp.outImage.getWidth()) && (y < wdp.outImage.getHeight())) {

         if (screenValue.isSelected() || fileValue.isSelected()) {
            colorScreen    = uis.getPixel(x,y);
            pixelScreen[0] = colorScreen.getRed();
            pixelScreen[1] = colorScreen.getGreen();
            pixelScreen[2] = colorScreen.getBlue();

            if (fileValue.isSelected()) {

               actualPixel = spiderImage.fpix[y*wdp.outImage.getWidth()+x];
            }

            if (screenValue.isSelected() && fileValue.isSelected()) {
               String str = "(X,Y) = " + x + "," + y + "  Scr.Val. = " +
                       pixelScreen[0] + "  File Val. = "+ actualPixel + "\n";
               showOutput(str);
            }
            else if (screenValue.isSelected()) {
               String str = "(X,Y) = " + x + "," + y +
                                       "  Scr.Val. = " + pixelScreen[0] +"\n";
               showOutput(str);
            }
            else {
               String str = "(X,Y) = " + x + "," + y +
                                        "  File Val. = "+ actualPixel + "\n";
              showOutput(str);
            }
         }

         // do nothing
         else if (leaveMarker.isSelected()){
         }

         else {
           String str = "(X,Y) = " + x + "," + y +"\n";
           showOutput(str);
         }
      }
   }
   public void mousePressed(MouseEvent e) {
      if(!this.isShowing()) return;

      posX = e.getX();
      posY = e.getY();
      // if mouse is inside the image
      if ((posX < wdp.outImage.getWidth()) && (posY < wdp.outImage.getHeight())) {
         if (leaveMarker.isSelected()) {
            wdp.drawMarker = true;

            if (!docFileSelection.isSelected()) {
             //String str = "(X,Y) = " + posX + "," + posY + "\n";
             // rollover the preset line because it adds a line and lineCountSave
             // is no longer same as lineCount. The info might be a bit off  if
             // there is discrepancy between the position where the mouse is clicked
             // and where the mouse was moved to before clicking. I surely think
             // they are the same position. Or else one may take these X and Y
             // positions that are found after clicking mouse and find out the
             // screen and file values etc. if it need to be displayed after
             // mouse is clicked.
               String str = " ";
               utilParameters.outputMessageTextArea.append(str);
            }
            wdp.markers[wdp.markerNumber] = new Point(posX, posY);
            wdp.markerNumber++;
            wdp.repaint();
         }

         if (docFileSelection.isSelected()) {
            inputUpdate();
            colorScreen = uis.getPixel(posX, posY);
            pixelScreen[0] = colorScreen.getRed();
            pixelScreen[1] = colorScreen.getGreen();
            pixelScreen[2] = colorScreen.getBlue();

            actualPixel = spiderImage.fpix[posY * wdp.outImage.getWidth() + posX];

            Arrays.fill(regValArray, 0);

            if (xRegValue <= 9 && yRegValue <= 9 && screenRegValue <= 9
                 && fileRegValue <= 9) {
               for (int i = 1; i <= 9; i++) {
                  if (xRegValue == i) regValArray[i - 1] = posX;
                  if (yRegValue == i) regValArray[i - 1] = posY;
                  if (screenValue.isSelected() && screenRegValue == i)
                   regValArray[i - 1] = pixelScreen[0];
                  if (fileValue.isSelected() && fileRegValue == i)
                   regValArray[i - 1] = actualPixel;
               }
               numberOfRegisters = xRegValue;
               if (numberOfRegisters < yRegValue) numberOfRegisters = yRegValue;
               if (screenValue.isSelected() && (numberOfRegisters < screenRegValue))
                 numberOfRegisters = screenRegValue;
               if (fileValue.isSelected() && (numberOfRegisters < fileRegValue))
                 numberOfRegisters = fileRegValue;
               if (!docName.equalsIgnoreCase(docNameSave)) {
                // true for the first time and also when the document name is changed
                  keyRegValueSave = 10000;
               }
               if (keyRegValue == keyRegValueSave) {
                // false for the first mouse click and also when key reg is changed
                  keyRegVal++;
               }
               else {
                  keyRegVal = keyRegValue;
               }
               String op = "w";
               UtilDoc docFile = new UtilDoc(docName, keyRegVal,
                                          numberOfRegisters, regValArray, op,
                                          utilParameters);
               docFile.writeDoc();
               docFile.closeWriteDoc();
               keyRegValueSave = keyRegValue;
               docNameSave = docName;
            }
            else {
               String str = "Register values should be less than 9 \n ";
               utilParameters.outputMessageTextArea.append(str);
            }
         }
      }
   }

   public void inputUpdate() {
      docName  = docFileInput.getText().trim();
      keyRegValue  = Integer.parseInt(keyNumInput.getText().trim());
      xRegValue = Integer.parseInt(XRegNumInput.getText().trim());
      yRegValue = Integer.parseInt(YRegNumInput.getText().trim());
      screenRegValue = Integer.parseInt(ScreenValueRegNumInput.getText().trim());
      fileRegValue = Integer.parseInt(FileValueRegNumInput.getText().trim());
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

