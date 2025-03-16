package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIAnnotate extends JFrame {
   private JLabel     docFile, xRegNum, yRegNum, markerSide;

   private JTextField docFileInput, xRegNumInput, yRegNumInput,
                      markerSideInput;
   private JButton showMarkerJButton, eraseMarkerJButton;

   private UtilParameters utilParameters;
   private WebDisplayPanel webDisPanel;
   private UtilImageManipulate utilImageManipulate;

   private String docName;
   private int xRegValue,yRegValue,markerSideValue;

   public UtilGUIAnnotate(WebDisplayPanel panel, UtilImageManipulate utilImgManipulate,SpiderImage spiImage,
                                                 UtilParameters utilParam) {
      webDisPanel = panel;
      utilParameters = utilParam;
      utilImageManipulate = utilImgManipulate;

      setTitle("Annotate Popup Menu");
      setSize(200,300);

      Container contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      docFile = new JLabel("Doc. File : ");
      addComponent(contentPane,docFile,gridBagConstraints,1,1,1,1);

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);

      docFileInput = new JTextField(fileNameWOExtension+".ant",10);
      addComponent(contentPane,docFileInput,gridBagConstraints,5,1,1,1);

      xRegNum = new JLabel("X Reg. : ");
      addComponent(contentPane,xRegNum,gridBagConstraints,1,5,1,1);

      xRegNumInput = new JTextField("1",5);
      addComponent(contentPane,xRegNumInput,gridBagConstraints,5,5,1,1);

      yRegNum = new JLabel("Y Reg. : ");
      addComponent(contentPane,yRegNum,gridBagConstraints,1,10,1,1);

      yRegNumInput = new JTextField("2",5);
      addComponent(contentPane,yRegNumInput,gridBagConstraints,5,10,1,1);

      markerSide = new JLabel("Marker Size (in Pixel) : ");
      addComponent(contentPane,markerSide,gridBagConstraints,1,15,1,1);

      markerSideInput = new JTextField("10",5);
      addComponent(contentPane,markerSideInput,gridBagConstraints,5,15,1,1);

      showMarkerJButton = new JButton("Show Markers");
      addComponent(contentPane,showMarkerJButton,gridBagConstraints,1,18,1,1);

      eraseMarkerJButton = new JButton("Erase Markers");
      addComponent(contentPane,eraseMarkerJButton,gridBagConstraints,5,18,1,1);

      showMarkerJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            showMarkerJButton_actionPerformed(e);
         }
      });

      eraseMarkerJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            eraseMarkerJButton_actionPerformed(e);
         }
      });

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

   void showMarkerJButton_actionPerformed(ActionEvent e) {
      inputUpdate();
      //key is at register number = 0
      int keyRegValue = 0;
      boolean KeyDuplicationAllowed = false;
      String op = "r";
      UtilDoc utilDoc = new UtilDoc(docName,op,utilParameters);
      float [] xRegVal = utilDoc.readDoc
                                       (xRegValue, KeyDuplicationAllowed);
      int [] xRegValInt = new int[xRegVal.length];

      float [] yRegVal = utilDoc.readDoc
                                       (yRegValue, KeyDuplicationAllowed);
      int [] yRegValInt = new int[yRegVal.length];

      float [] keyRegVal = utilDoc.readDoc
                                       (keyRegValue, KeyDuplicationAllowed);
      int [] keyRegValInt = new int[keyRegVal.length];

      int keyNumbers, xRegNumbers, yRegNumbers;
      xRegNumbers = xRegVal.length;
      yRegNumbers = yRegVal.length;
      keyNumbers  = keyRegVal.length;
      float resizeFactor = utilImageManipulate.scale;
      if (( keyNumbers == xRegNumbers) && (keyNumbers == yRegNumbers )) {
         for (int i = 0; i < xRegVal.length; i++) {
            xRegValInt[i] = (int)(xRegVal[i]*resizeFactor);
            yRegValInt[i] = (int)(yRegVal[i]*resizeFactor);
            keyRegValInt[i] = (int)keyRegVal[i];

            webDisPanel.markers[i] = new Point(xRegValInt[i],yRegValInt[i]);
            webDisPanel.strings[i] = Integer.toString(keyRegValInt[i]);
            webDisPanel.stringPos[i] =
                  new Point((xRegValInt[i]-(markerSideValue/2 )+4),
                            (yRegValInt[i]+(markerSideValue/2)));
         }
         webDisPanel.markerLength = markerSideValue;
         webDisPanel.drawMarker = true;
         webDisPanel.markerNumber = xRegVal.length;
         webDisPanel.drawString = true;
         webDisPanel.stringNumber = keyRegVal.length;
         webDisPanel.repaint();
      }
      else {
         String str = "No. of X, y registers and the key numbers are not same!\n ";
         utilParameters.outputMessageTextArea.append(str);
      }
   }

   protected void processWindowEvent(WindowEvent e) {
     super.processWindowEvent(e);
     if (e.getID() == WindowEvent.WINDOW_CLOSING) {
       webDisPanel.drawMarker = false;
       webDisPanel.drawString = false;
       webDisPanel.repaint();
     }
   }


   void eraseMarkerJButton_actionPerformed(ActionEvent e) {
      webDisPanel.drawMarker = false;
      webDisPanel.drawString = false;
      webDisPanel.repaint();
   }

   public void inputUpdate() {
      docName  = docFileInput.getText().trim();
      xRegValue = Integer.parseInt(xRegNumInput.getText().trim());
      yRegValue = Integer.parseInt(yRegNumInput.getText().trim());
      markerSideValue = Integer.parseInt(markerSideInput.getText().trim());
   }
}
