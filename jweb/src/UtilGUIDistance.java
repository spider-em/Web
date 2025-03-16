package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.text.*;
import java.io.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIDistance extends JFrame implements
                                          MouseMotionListener, MouseListener{

   private JCheckBox  saveDocFile;
   private JLabel     docuFile, keyNum, leftClick, rightClick;
   private JTextField docuFileInput, keyNumInput;
   private JRadioButton singleDistRadioButton, multiDistRadioButton;
   public final Vector singleDistXY = new Vector(10,100);
   public final Vector multiDistXY  = new Vector(10,100);
   private ButtonGroup windowButtonGroup;
   private WebDisplayPanel wdp;
   private UtilParameters utilParameters;
   private String docName, strOne;
   private int keyNumValue;
   private int posX, posY;
   private int numOfPoints = 0;
   private Point point, firstPoint;
   private int mouseClickNum = 0, singleDistXYSize = 0, multiDistXYSize = 0;
   private float [] regValArray;
   private double distance = 0, lastDistance =0;
   private Point prevPoint, pointOne, pointTwo;
   private double posX1, posY1, posX2, posY2;
   private UtilDoc docFile;
   private int lineCountSave = -7, lastKeyNum = 0;
   private DecimalFormat df1 = new DecimalFormat("####0.00");
   private File docFileObj;

   public UtilGUIDistance(WebDisplayPanel panel, UtilParameters utilParam) {

      utilParameters = utilParam;
      wdp = panel;

      setTitle("Distance Popup Menu");
      setSize(400,500);

      Container contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      saveDocFile = new JCheckBox("Save Distances in Document File: ");
      saveDocFile.setSelected(false);
      addComponent(contentPane,saveDocFile,gridBagConstraints,0,0,3,1);
      saveDocFile.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            saveDocFile_actionPerformed(e);
         }
      });

      docuFile = new JLabel("Document File Name: ");
      addComponent(contentPane,docuFile,gridBagConstraints,1,1,1,1);
      docuFile.setEnabled(false);

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);

      docuFileInput = new JTextField(fileNameWOExtension+".dis",15);
      addComponent(contentPane,docuFileInput,gridBagConstraints,2,1,4,1);
      docuFileInput.setEditable(false);
      docuFileInput.setEnabled(false);

      keyNum = new JLabel("Key No. : ");
      addComponent(contentPane,keyNum,gridBagConstraints,1,2,1,1);
      keyNum.setEnabled(false);



      keyNumInput = new JTextField("1",5);
      addComponent(contentPane,keyNumInput,gridBagConstraints,2,2,1,1);
      keyNumInput.setEnabled(false);
      keyNumInput.setEditable(false);

      singleDistRadioButton = new JRadioButton("Set of Distances Between Connecting Points");
      singleDistRadioButton.setSelected(true);
      addComponent(contentPane,singleDistRadioButton,gridBagConstraints,1,4,3,1);

      multiDistRadioButton = new JRadioButton("Set of Multiple Distances From One Point");
      multiDistRadioButton.setSelected(false);
      addComponent(contentPane,multiDistRadioButton,gridBagConstraints,1,5,3,1);

      windowButtonGroup = new ButtonGroup();
      windowButtonGroup.add(singleDistRadioButton);
      windowButtonGroup.add(multiDistRadioButton);

      wdp.addMouseMotionListener(this);
      wdp.addMouseListener(this);

      leftClick = new JLabel("Click left mouse button to select a point.");
      addComponent(contentPane,leftClick,gridBagConstraints,1,6,4,1);
      leftClick.setEnabled(false);

      rightClick = new JLabel("Click right mouse button to (save) and stop selecting the set.");
      addComponent(contentPane,rightClick,gridBagConstraints,1,7,4,1);
      rightClick.setEnabled(false);

      pack();
      setVisible(true);
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

   public void saveDocFile_actionPerformed(ActionEvent e){

      if (saveDocFile.isSelected()) {
         docuFile.setEnabled(true);
         docuFileInput.setEditable(true);
         docuFileInput.setEnabled(true);
         keyNum.setEnabled(true);
         keyNumInput.setEnabled(true);
         keyNumInput.setEditable(true);
         docName  = docuFileInput.getText().trim();
         docFileObj = new File(docName);
         if (docFileObj.exists()) {
            resetKeyNumber();
         }
         else {
            keyNumValue = 1;
            keyNumInput.setText(Integer.toString(keyNumValue));
         }
      }
      else {
         docuFile.setEnabled(false);
         docuFileInput.setEditable(false);
         docuFileInput.setEnabled(false);
         keyNum.setEnabled(false);
         keyNumInput.setEnabled(false);
         keyNumInput.setEditable(false);
      }
   }

   // stop the mouse listener
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
         wdp.drawRubberBandLine = false;
         wdp.removeMouseMotionListener(this);
         wdp.removeMouseListener(this);
         wdp.repaint();
      }
   }

   public void mouseMoved(MouseEvent e) {

      if ((wdp.drawRubberBandLine == true) && (mouseClickNum !=0)) {
         point = e.getPoint();
         posX = (int) point.getX();
         posY = (int) point.getY();
         // if mouse is inside the image
         if ( (posX < wdp.outImage.getWidth()) && (posY < wdp.outImage.getHeight())) {
            if(singleDistRadioButton.isSelected()){
               singleDistXY.set(singleDistXYSize,point);
               // draw lines
               drawPolygonLines();

               posX1 = prevPoint.x;
               posY1 = prevPoint.y;
               posX2 = point.x;
               posY2 = point.y;

               distance = Math.sqrt((posX2-posX1)*(posX2-posX1) +
                                        (posY2-posY1)*(posY2-posY1));

               // show distance in the output text area
               strOne = df1.format(distance);
               strOne  = "( Distance = " + strOne + " )" + "\n";
               showOutput(strOne);

            }
            else if (multiDistRadioButton.isSelected()) {
               multiDistXY.set(multiDistXYSize-1,point);
               // draw lines
               drawPolygonLines();

               posX1 = firstPoint.x;
               posY1 = firstPoint.y;
               posX2 = point.x;
               posY2 = point.y;

               distance = Math.sqrt((posX2-posX1)*(posX2-posX1) +
                                                  (posY2-posY1)*(posY2-posY1));
               // show distance in the output text area
               strOne = df1.format(distance);
               strOne  = "( Distance = " + strOne + " )" + "\n";
               showOutput(strOne);
            }
         }
      }
   }

   public void mousePressed(MouseEvent e) {
      //if(!this.isShowing()) return;
      point = e.getPoint();
      posX = (int) point.getX();
      posY = (int) point.getY();

      // if mouse is inside the image
      if ( (posX < wdp.outImage.getWidth()) && (posY < wdp.outImage.getHeight())) {

         // left mouse button is pressed
         if((e.getModifiers() & InputEvent.BUTTON1_MASK) !=0 ) {
            if (singleDistRadioButton.isSelected()) {
               multiDistRadioButton.setEnabled(false);
               singleDistXYSize = singleDistXYSize +1 ;
               mouseClickNum = mouseClickNum + 1;
               if (mouseClickNum == 1) {
                  singleDistXY.clear();
                  wdp.drawRubberBandLine = true;
                  // needs to be done so that Vector.set will work.
                  singleDistXY.add(point);
               }
               else {
                  singleDistXY.set(singleDistXYSize - 1, point);
                  posX1 = prevPoint.x;
                  posY1 = prevPoint.y;
                  posX2 = point.x;
                  posY2 = point.y;

                  lastDistance = Math.sqrt((posX2-posX1)*(posX2-posX1) +
                                                  (posY2-posY1)*(posY2-posY1));

               }
               // copy the same point to the next element of the vector
               singleDistXY.add(point);


               if ( saveDocFile.isSelected() && mouseClickNum != 1) {
                  inputUpdate();
                  String op = "w";
                  int numberOfRegisters = 5;
                  regValArray = new float [numberOfRegisters];

                  posX1 = prevPoint.x;
                  posY1 = prevPoint.y;
                  posX2 = point.x;
                  posY2 = point.y;

                  distance = Math.sqrt((posX2-posX1)*(posX2-posX1) +
                                                   (posY2-posY1)*(posY2-posY1));


                  regValArray[0] = (float)distance;
                  regValArray[1] = (float)posX1;
                  regValArray[2] = (float)posY1;
                  regValArray[3] = (float)posX2;
                  regValArray[4] = (float)posY2;


                  // write to document file
                  docFile = new UtilDoc(docName, keyNumValue, numberOfRegisters,
                                               regValArray, op, utilParameters);
                  docFile.writeDoc();
                  docFile.closeWriteDoc();
                  keyNumValue = keyNumValue + 1;
                  keyNumInput.setText(Integer.toString(keyNumValue));
               }
               prevPoint = point;
            }
            // multi distance radio button is selected
            else {
               singleDistRadioButton.setEnabled(false);
               multiDistXYSize = multiDistXYSize +2;
               mouseClickNum = mouseClickNum + 1;
               if(mouseClickNum == 1) {
                  multiDistXY.clear();
                  firstPoint = e.getPoint();
                  wdp.drawRubberBandLine = true;
                  // needs to be done so that Vector.set will work.
                  multiDistXY.add(firstPoint);
                  multiDistXY.add(firstPoint);
               }
               else {
                  point = e.getPoint();
                  multiDistXY.set(multiDistXYSize -3, point);
                  multiDistXY.add(multiDistXYSize -2, firstPoint);
                  multiDistXY.add(multiDistXYSize -1, point);

                  posX1 = firstPoint.x;
                  posY1 = firstPoint.y;
                  posX2 = point.x;
                  posY2 = point.y;

                  lastDistance = Math.sqrt((posX2-posX1)*(posX2-posX1) +
                                                  (posY2-posY1)*(posY2-posY1));
               }

            }

            if( mouseClickNum != 0) {
               // draw lines
               drawPolygonLines();
            }
         }
         // right mouse button is pressed
         if((e.getModifiers() & InputEvent.BUTTON3_MASK) !=0 ) {
            if( mouseClickNum != 0) {
               if (singleDistRadioButton.isSelected()) {
                  multiDistRadioButton.setEnabled(true);
                  mouseClickNum = 0;
                  // remove the last point
                  singleDistXY.remove(singleDistXYSize);
                  drawPolygonLines();

                  if ( saveDocFile.isSelected() && mouseClickNum != 1) {
                     strOne  = " ";
                     showOutput(strOne);
                  }
                  else {
                     // show the last selected distance in the output text area
                     strOne = df1.format(lastDistance);
                     strOne  = "( Distance = " + strOne + " )" + "\n";
                     showOutput(strOne);
                  }
                  singleDistXYSize = 0;
               }
               // multi distance radio button is selected
               else {
                  singleDistRadioButton.setEnabled(true);
                  mouseClickNum = 0;
                 // remove the last 2 points
                  multiDistXY.remove(multiDistXYSize-1);
                  multiDistXY.remove(multiDistXYSize-2);
                  drawPolygonLines();

                  if ( saveDocFile.isSelected() && mouseClickNum != 1) {
                     inputUpdate();
                     String op = "w";
                     int numberOfRegisters =  multiDistXY.size()/2;

                     regValArray = new float [numberOfRegisters];
                     for (int i = 0; i < numberOfRegisters ; i++) {
                        pointOne = (Point)multiDistXY.elementAt(i*2);
                        pointTwo = (Point)multiDistXY.elementAt((i*2)+1);
                        posX1 = pointOne.x;
                        posY1 = pointOne.y;
                        posX2 = pointTwo.x;
                        posY2 = pointTwo.y;

                        distance = Math.sqrt((posX2-posX1)*(posX2-posX1) +
                                                  (posY2-posY1)*(posY2-posY1));
                        regValArray[i] = (float)distance;
                     }
                     // show last selected distance in the output text area
                     strOne = df1.format(distance);
                     strOne  = "( Distance = " + strOne + " )" + "\n";
                     showOutput(strOne);

                     // write to document file
                     docFile = new UtilDoc(docName, keyNumValue,
                           numberOfRegisters, regValArray, op, utilParameters);
                     docFile.writeDoc();
                     docFile.closeWriteDoc();
                     keyNumValue = keyNumValue + 1;
                     keyNumInput.setText(Integer.toString(keyNumValue));
                  }
                  else if (!saveDocFile.isSelected() && mouseClickNum != 1){
                     strOne = df1.format(lastDistance);
                     strOne  = "( Distance = " + strOne + " )" + "\n";
                     showOutput(strOne);
                  }
                  multiDistXYSize = 0;
               }
            }
         }
      }
   }

   public void inputUpdate() {
      docName  = docuFileInput.getText().trim();
      keyNumValue  = Integer.parseInt(keyNumInput.getText().trim());

   }
   public void resetKeyNumber() {
      // do it if the doc  name has changed
      String op = "r";
      UtilDoc utilDocu = new UtilDoc(docName,op,utilParameters);
      lastKeyNum = utilDocu.getLastKeyNum();
      keyNumInput.setText(Integer.toString(lastKeyNum +1));
      keyNumValue = Integer.parseInt(keyNumInput.getText().trim());
   }

   public void drawPolygonLines () {

      if (singleDistRadioButton.isSelected()) {
         numOfPoints = singleDistXY.size();
         wdp.numPolyPoints = numOfPoints;
         for (int i = 0; i < numOfPoints; i++) {
            point = (Point)singleDistXY.elementAt(i);
            wdp.polyX[i] = point.x;
            wdp.polyY[i] = point.y;
         }

      }
      // multiDistRadioButton is selected
      else {
         numOfPoints = multiDistXY.size();
         wdp.numPolyPoints = numOfPoints;
         for (int i = 0; i < numOfPoints; i++) {
            point = (Point)multiDistXY.elementAt(i);
            wdp.polyX[i] = point.x;
            wdp.polyY[i] = point.y;
         }
      }
   wdp.repaint() ;
   }


   public void showOutput(String string) {
     // needs modification
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

