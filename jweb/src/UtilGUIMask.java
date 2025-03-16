package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.io.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIMask extends JFrame implements
                                Runnable, MouseMotionListener, MouseListener{

   private JCheckBox  saveMaskFile, fillInside;
   private JLabel     imgFile, leftClick, rightClick, dummy;
   private JTextField fileInput ;
   private JButton jButtonClearMask;

   public final Vector polyPointsXY = new Vector(10,100);
   private WebDisplayPanel wdp;
   private UtilParameters utilParameters;
   private String fileName;
   private int posX, posY;
   private int numOfPoints = 0;
   private Point point;
   private int mouseClickNum = 0, polyPointsXYSize = 0;

   private Point prevPoint;
   private double posX1, posY1, posX2, posY2;
   private File imgFileObj;
   private boolean polygonClosed = false;
   private Thread saveMaskThread;
   private float [] imgArray;
   private DataOutputStream  dout;

   public UtilGUIMask(WebDisplayPanel panel, UtilParameters utilParam) {

      utilParameters = utilParam;
      wdp = panel;

      setTitle("Mask Popup Menu");
      setSize(400,500);

      Container contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      saveMaskFile = new JCheckBox("Save the Mask File: ");
      saveMaskFile.setSelected(false);
      addComponent(contentPane,saveMaskFile,gridBagConstraints,0,0,3,1,"LEFT");
      saveMaskFile.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            saveMaskFile_actionPerformed(e);
         }
      });

      imgFile = new JLabel("Mask File Name: ");
      addComponent(contentPane,imgFile,gridBagConstraints,1,1,1,1,"LEFT");
      imgFile.setEnabled(false);

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);

      fileInput = new JTextField(fileNameWOExtension+".msk",15);
      addComponent(contentPane,fileInput,gridBagConstraints,2,1,2,1,"LEFT");
      fileInput.setEditable(false);
      fileInput.setEnabled(false);

      fillInside = new JCheckBox("Fill inside the polygon: ");
      fillInside.setSelected(false);
      addComponent(contentPane,fillInside,gridBagConstraints,1,4,2,1,"LEFT");
      fillInside.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            fillInside_actionPerformed(e);
         }
      });

      wdp.addMouseMotionListener(this);
      wdp.addMouseListener(this);

      leftClick = new JLabel("Click left mouse button to select a point.");
      addComponent(contentPane,leftClick,gridBagConstraints,1,5,4,1,"LEFT");
      leftClick.setEnabled(false);

      rightClick = new JLabel("Click right mouse button to close polygon and save the mask(if checked !). ");
      addComponent(contentPane,rightClick,gridBagConstraints,1,6,6,1,"LEFT");
      rightClick.setEnabled(false);

      dummy = new JLabel (" ");
      addComponent(contentPane,dummy,gridBagConstraints,2,7,1,1,"LEFT");

      jButtonClearMask = new JButton("       Clear the Mask       ");
      addComponent(contentPane,jButtonClearMask,gridBagConstraints,1,8,2,1,"CENTER");
      jButtonClearMask.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
              jButtonClearMask_actionPerformed(e);
           }
        });

      pack();
      setVisible(true);
  }

  public void addComponent(Container contentPane, Component comp,
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


   public void  fillInside_actionPerformed (ActionEvent e){
      if (polygonClosed) {
         if (fillInside.isSelected()) {
            wdp.drawPolygon = false;
            wdp.drawFilledPolygon = true;
            drawClosedPolygon();
         }
         else {
            wdp.drawPolygon = true;
            wdp.drawFilledPolygon = false;
            drawClosedPolygon();
         }
      }
   }
   public void jButtonClearMask_actionPerformed(ActionEvent e) {
      if (polygonClosed) {
         polygonClosed = false;
         polyPointsXY.clear();
         wdp.drawFilledPolygon = false;
         wdp.drawRubberBandLine = false;
         wdp.drawPolygon = false;
         wdp.repaint();
      }
   }
   public void saveMaskFile_actionPerformed(ActionEvent e){

      if (saveMaskFile.isSelected()) {
         imgFile.setEnabled(true);
         fileInput.setEditable(true);
         fileInput.setEnabled(true);
      }
      else {
         imgFile.setEnabled(false);
         fileInput.setEditable(false);
         fileInput.setEnabled(false);
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
         wdp.drawFilledPolygon = false;
         wdp.drawPolygon = false;
         point = e.getPoint();
         posX = (int) point.getX();
         posY = (int) point.getY();
         // if mouse is inside the image
         if ( (posX < wdp.outImage.getWidth()) && (posY < wdp.outImage.getHeight())) {
            polyPointsXY.set(polyPointsXYSize,point);
            // draw lines
            drawPolygonLines();
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
         if ( (e.getModifiers() & InputEvent.BUTTON1_MASK) != 0) {
            polyPointsXYSize = polyPointsXYSize +1 ;
            mouseClickNum = mouseClickNum + 1;
            if (mouseClickNum == 1) {
               polygonClosed = false;
               polyPointsXY.clear();
               wdp.drawRubberBandLine = true;
               wdp.drawPolygon = false;
               // needs to be done so that Vector.set will work.
               polyPointsXY.add(point);
            }
            else {
               polyPointsXY.set(polyPointsXYSize - 1, point);
            }
            // copy the same point to the next element of the vector
            polyPointsXY.add(point);
         }
         // right mouse button is pressed
         if((e.getModifiers() & InputEvent.BUTTON3_MASK) !=0 ) {
            polygonClosed = true;
            mouseClickNum = 0;
            polyPointsXYSize = 0;
            inputUpdate();
            wdp.drawRubberBandLine = false;

            if (fillInside.isSelected()) {
               wdp.drawPolygon = false;
               wdp.drawFilledPolygon = true;
               drawClosedPolygon();
            }
            else {
               wdp.drawPolygon = true;
               wdp.drawFilledPolygon = false;
               drawClosedPolygon();
            }
            if (saveMaskFile.isSelected()) {
               //fileName  = fileInput.getText().trim();
               inputUpdate();

               // used a thread to save the mask. Otherwise wdp.polyObj is
               // NULL when wdp.polyObj.contains method is accessed. There is
               // a time lag.
               saveMaskThread = new Thread(this) ;
               saveMaskThread.start();
            }
         }
      }
   }

   public void run() {
      // thread runs here
      long sleepTime = 1000;
      try {
         Thread.sleep(sleepTime);
      }
      catch (InterruptedException ie) {
         String str = "Error in sleep: " + ie + "\n";
         utilParameters.outputMessageTextArea.append(str);
      }
      int imgNsam = wdp.outImage.getWidth();
      int imgNrow = wdp.outImage.getHeight();
      int arrayIndex = 0;
      imgArray = new float [imgNsam*imgNrow] ;
      for (int j = 0; j < imgNrow; j++) {
         for (int i = 0; i < imgNsam; i++){
            arrayIndex = j*imgNsam + i;
            if (wdp.polyObj.contains(i,j)) {
               imgArray[arrayIndex] = 1;
            }
            else {
               imgArray[arrayIndex] = 0;
            }
         }
      }
      try {
         dout = new DataOutputStream
             (new BufferedOutputStream(new FileOutputStream(fileName)));

         int wid = imgNsam;
         int ht =   imgNrow;
         int nslice = 1;
         int nsam = wid;
         int nrow = ht;
         int iform = 1;
         int imami = 0;
         float fmax = 0;
         float fmin = 0;
         float avg = 0;
         float sig = 0;

         UtilFile utilFile = new UtilFile(utilParameters);
         try {
            utilFile.writeSpiderFile(fileName, imgArray,
                   nsam, nrow, nslice, imami, avg, sig, iform, fmax, fmin);
         }
         catch (IOException eoe) {
            String str = "Error writting SpiderImage: UtilGUIMask " + eoe + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
      }
      catch (Exception b) {
         System.err.println(b.getMessage());
         utilParameters.outputMessageTextArea.append("Error writting to file");
      }
   }

   public void inputUpdate() {
      fileName  = fileInput.getText().trim();
   }

   public void drawPolygonLines () {
      numOfPoints = polyPointsXY.size();
      wdp.numPolyPoints = numOfPoints;
      for (int i = 0; i < numOfPoints; i++) {
         point = (Point)polyPointsXY.elementAt(i);
         wdp.polyX[i] = point.x;
         wdp.polyY[i] = point.y;
      }
      wdp.repaint() ;
   }

   public void drawClosedPolygon () {
      numOfPoints = polyPointsXY.size();
      wdp.numPolyPoints = numOfPoints;
      for (int i = 0; i < numOfPoints; i++) {
         point = (Point)polyPointsXY.elementAt(i);
         wdp.polyX[i] = point.x;
         wdp.polyY[i] = point.y;
      }
      wdp.repaint() ;
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

