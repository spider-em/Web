package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.io.*;
import java.util.*;
import java.text.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Wenji Zhang : Revised by Bimal Rath
 * @version 1.0
 */

public class UtilGUIMarker extends JFrame implements ActionListener,
                          MouseMotionListener, MouseListener, DocumentListener{

   private JLabel     nextMarkerNum;
   private JTextField nextMarkerNumInput;
   private JCheckBox showMarkerSelection;
   private JLabel docFile, tiltAngle;
   private JTextField docFileInput, tiltAngleInput;
   private WebDisplayPanel wdp;
   private UtilParameters utilParameters;
   private UtilImageManipulate utilImgManipulate;
   private String docName;
   private float tiltAngleValue, resizeFactor;
   private int nextMarkerNumValue;

   private int lineCountSave = -7;
   private int xRegValue, yRegValue, keyRegValue, markerSideValue;
   private boolean newMarkerFile, firstMarkerPicking;
   private Container contentPane;


   public UtilGUIMarker(WebDisplayPanel panel,
           UtilImageManipulate utilImageManipulate, UtilParameters utilParam) {

      wdp = panel;
      utilParameters = utilParam;
      utilImgManipulate = utilImageManipulate;

      keyRegValue = 0;
      xRegValue = 1;
      yRegValue = 2;
      markerSideValue = 10;
      nextMarkerNumValue = 1;
      tiltAngleValue = 0;
      firstMarkerPicking =  true;

      resizeFactor = utilImageManipulate.scale;

      contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      docFile = new JLabel("Marker File : ");
      addComponent(docFile,gridBagConstraints,1,1,3,1,"LEFT");

      // set a default marker file name
      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);
      docName = fileNameWOExtension+".mrk";
      // docName = utilParameters.currDirectory;
      docFileInput = new JTextField(docName,30);
      addComponent(docFileInput,gridBagConstraints,5,1,3,1,"LEFT");
      docFileInput.getDocument().addDocumentListener(this);

      // add dummy label for spacing
      JLabel dummyJLabel = new JLabel("     ");
      addComponent(dummyJLabel,gridBagConstraints,1,2,1,1,"CENTER");

      tiltAngle = new JLabel("Tilt Angle : ");
      addComponent(tiltAngle,gridBagConstraints,1,3,3,1,"LEFT");
      //  tiltAngleInput = new JTextField(""+tiltAngleValue,10);
      // default tilt angle = 0.0
      tiltAngleInput = new JTextField("0.0",10);
      addComponent(tiltAngleInput,gridBagConstraints,5,3,3,1,"LEFT");

      // add dummy label for spacing
      JLabel dummyJLabel2 = new JLabel("     ");
      addComponent(dummyJLabel2,gridBagConstraints,1,4,1,1,"CENTER");

      nextMarkerNum = new JLabel("Next Marker Number : ");
      addComponent(nextMarkerNum,gridBagConstraints,1,5,5,1,"LEFT");

      // default nextMarkerNumInput = 1
      //nextMarkerNumInput = new JTextField(Integer.toString(nextMarkerNumValue),4);
      nextMarkerNumInput = new JTextField("1",4);
      addComponent(nextMarkerNumInput,gridBagConstraints,5,5,3,1,"LEFT");

      // add dummy label for spacing
      JLabel dummyJLabel3 = new JLabel("     ");
      addComponent(dummyJLabel3,gridBagConstraints,1,6,1,1,"CENTER");

      showMarkerSelection = new JCheckBox("Show Markers");
      showMarkerSelection.setSelected(true);
      addComponent(showMarkerSelection,gridBagConstraints,1,7,3,1,"LEFT");
      showMarkerSelection.addActionListener(this);

      wdp.addMouseMotionListener(this);
      wdp.addMouseListener(this);

      File docFileObj = new File(docName);
      // get initial resizeFactor value

      if (docFileObj.exists()) {
         // show markers (by default settings), read last marker number
         // and the tilt angle from the marker file
         showMarkers();
         newMarkerFile = false;
         // next MarkerNumValue is read in showMarkers
         nextMarkerNumInput.setText(Integer.toString(nextMarkerNumValue));
         tiltAngleInput.setText(""+tiltAngleValue);
         // shouldn't be able to write tilt angle in the middle of the
         // marker file. Tilt angle is written at the begining of the
         // marker file when it is created.
         tiltAngleInput.setEditable(false);
      }
      else{
         newMarkerFile = true;
      }
      setTitle("MarkerPicking Popup Menu");
      setSize(400,500);
      pack();
      setVisible(true);
   }

   public void actionPerformed(ActionEvent e){

      showMarkers();
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
      }
   }

   public void mouseMoved(MouseEvent e) {

      Point point =e.getPoint();
      int x = (int)point.getX();
      int y = (int)point.getY();
      String str = "(X,Y) = " + x + "," + y + "\n";
      showOutput(str);
   }

   public void mousePressed(MouseEvent e) {

      if (!this.isShowing()) return;
      // update resizeFactor value
      resizeFactor = utilImgManipulate.scale;
      inputUpdate();
      PrintWriter out;

      //Write today's date comment line
      if (firstMarkerPicking){
         String str1 = " ;dat/dat   ";
         Date today;
         String dateOut;
         DateFormat dateFormatter;
         Locale currentLocale = new Locale("en", "US"); ;
         dateFormatter = DateFormat.getDateTimeInstance(DateFormat.DEFAULT,
                                             DateFormat.DEFAULT,currentLocale);
         today = new Date();
         dateOut = dateFormatter.format(today);
         str1 = str1 + dateOut; // + "   " + docName;
         str1 = str1.concat("\n");

         try {
            out = new PrintWriter(new BufferedWriter
                          (new FileWriter(docName, true)));
            out.print(str1);
            out.close();
         }
         catch (IOException er){
            System.err.println(er.getMessage());
         }
         firstMarkerPicking = false;
      }

      //Write the tilt angle comment line
      if (newMarkerFile){
         String str2=" ;  1 3";
         String temp;
         DecimalFormat df2 = new DecimalFormat("####0.000000");
         temp = df2.format(0);
         str2 = str2 + "    " + temp;
         temp = df2.format(tiltAngleValue);
         int index = temp.indexOf(".");
         int space =  6 - index;
         String str3="";
         for (int j = 1; j < space; j++){
            str3 = " " + str3;
         }
         str2 = str2 + str3 + temp + "  ";
         temp = df2.format(0);
         str2 = str2 + "  " + temp;
         str2 = str2.concat("\n");
         try {
            out = new PrintWriter(new BufferedWriter
                               (new FileWriter(docName, true)));
            out.print(str2);
            out.close();
         }
         catch (IOException er){
            System.err.println(er.getMessage());
         }
         newMarkerFile = false;
      }

      //Record the new marker
      float [] regValArray = new float [2];
      float [] regValArrayResize = new float [2];

      int numberOfRegisters = 2;
      String op = "w";

      Arrays.fill(regValArray, 0);
      regValArray[0] = e.getX();
      regValArray[1] = e.getY();
      regValArrayResize[0] = (int)(regValArray[0]/resizeFactor);
      regValArrayResize[1] = (int)(regValArray[1]/resizeFactor);

      UtilDoc docFile = new UtilDoc(docName,nextMarkerNumValue,
      //                                   numberOfRegisters,regValArray,op,
                                     numberOfRegisters,regValArrayResize,op,
                                                             utilParameters);
      docFile.writeDoc();
      docFile.closeWriteDoc();
      nextMarkerNumValue++;
      nextMarkerNumInput.setText(Integer.toString(nextMarkerNumValue));

      //Draw markers if the Show Marker Selection is checked
      if(showMarkerSelection.isSelected()) showMarkers();
   }

   public void inputUpdate() {
      docName = docFileInput.getText().trim();
      File docFileObj = new File(docName);
      // get initial resizeFactor value

      if (docFileObj.exists()) {
         // show marker and read last marker number from the marker file
         newMarkerFile = false;
         showMarkers();
         // next MarkerNumValue and tiltAngleValue are read in showMarkers
         tiltAngleInput.setText(""+tiltAngleValue);
         tiltAngleInput.setEditable(false);
      }
      else{
         newMarkerFile = true;
         tiltAngleInput.setEditable(true);
         tiltAngleValue = 0;
         tiltAngleInput.setText(""+tiltAngleValue);
         nextMarkerNumValue = 1;
         wdp.drawMarker = false;
         wdp.drawString = false;
         wdp.repaint();
      }
      nextMarkerNumValue  = Integer.parseInt(nextMarkerNumInput.getText().trim());
      nextMarkerNumInput.setText(Integer.toString(nextMarkerNumValue));
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

   void showMarkers() {
      //Do nothing if the marker file does not exist.
      if (newMarkerFile) return;

      // update resizeFactor value
      resizeFactor = utilImgManipulate.scale;

      boolean KeyDuplicationAllowed = false;
      String op = "r";
      UtilDoc utilDoc = new UtilDoc(docName,op,utilParameters);
      tiltAngleValue = utilDoc.getTiltAngle();
      float [] xRegVal = utilDoc.readDoc(xRegValue, KeyDuplicationAllowed);
      int [] xRegValInt = new int[xRegVal.length];
      float [] yRegVal = utilDoc.readDoc(yRegValue, KeyDuplicationAllowed);
      int [] yRegValInt = new int[yRegVal.length];
      int maxKeyValue = 0;
      float [] keyRegVal = utilDoc.readDoc(keyRegValue, KeyDuplicationAllowed);
      int [] keyRegValInt = new int[keyRegVal.length];
      int keyNumbers, xRegNumbers, yRegNumbers;
      xRegNumbers = xRegVal.length;
      yRegNumbers = yRegVal.length;
      keyNumbers  = keyRegVal.length;

      if (( keyNumbers == xRegNumbers) && (keyNumbers == yRegNumbers )) {
         for (int i = 0; i < xRegVal.length; i++) {
            /*
            xRegValInt[i] = (int)xRegVal[i];
            yRegValInt[i] = (int)yRegVal[i];
            keyRegValInt[i] = (int)keyRegVal[i];
            */
            xRegValInt[i]   = (int)(xRegVal[i]*resizeFactor);
            yRegValInt[i]   = (int)(yRegVal[i]*resizeFactor);
            keyRegValInt[i] = (int)keyRegVal[i];
            if (keyRegValInt[i] > maxKeyValue) maxKeyValue = keyRegValInt[i];
            wdp.markers[i] = new Point(xRegValInt[i],yRegValInt[i]);
            wdp.strings[i] = Integer.toString(keyRegValInt[i]);
            wdp.stringPos[i] =
               new Point((xRegValInt[i]-(markerSideValue/2 )+10),
                                         (yRegValInt[i]+(markerSideValue/2)));
         }

         nextMarkerNumValue = maxKeyValue + 1;
         if(showMarkerSelection.isSelected()) {
            wdp.markerLength = markerSideValue;
            wdp.drawMarker = true;
            wdp.markerNumber = xRegVal.length;
            wdp.drawString = true;
            wdp.stringNumber = keyRegVal.length;
            wdp.repaint();
         }
         else{
            wdp.drawMarker = false;
            wdp.drawString = false;
            wdp.repaint();
         }
      }
      else {
         String str = "No. of X, y registers and the key numbers are not same!\n ";
         utilParameters.outputMessageTextArea.append(str);
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
      contentPane.add(comp,gridBagConstraints);
   }

   public void insertUpdate(DocumentEvent e){
      //System.out.println("text inserted !!");
      inputUpdate();
   }
   public void removeUpdate(DocumentEvent e){
      //System.out.println("text deleted !!");
      inputUpdate();
   }
   public void changedUpdate(DocumentEvent e){
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
