package web;

import javax.swing.UIManager;
import java.awt.*;
import java.awt.event.*;
import web.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class StartWeb{

   boolean packFrame = true;
   web.WebFrame frame;

   /**Construct the application*/
   public StartWeb() {

      makeWebFrame();
   }

   // overloaded constructor to handle command line argument
   public StartWeb(String str) {

      String strInterfaceType = new String ("SPIDUI");
      // JWEB called from inside SPIDUI
      if(str.equals(strInterfaceType)){
         makeWebFrame();
         frame.setState(frame.ICONIFIED);
         // start a thread to run the server
         // when server receives a message (image file name with the path)
         // it displays the image by calling method openAnImage() of WebFrame class.
         web.SpiduiInterface spiduiInterface = new web.SpiduiInterface(frame);
      }

      // JWEB called from command line with a file name
      else{

         /*
         UtilParameters utilParameters = new UtilParameters();

         // note: the command line argument is the file name including the path
         // ImageFrame imageFrame = new ImageFrame("c:\\bimal\\DATA\\slice001.dat",utilParameters);
         ImageFrame imageFrame = new ImageFrame(str,utilParameters);
         imageFrame.analysisMenu.setEnabled(false);
         imageFrame.setLocation(100, 100);
         imageFrame.setVisible(true);

         // if window is closed then exit
         imageFrame.addWindowListener(new WindowAdapter() {
            public void windowClosing (WindowEvent e) {
               System.exit(0);
            }
         });
         */
         strInterfaceType = "inline";
         makeWebFrame();
         frame.dispose();
         frame.openAnImage(str,strInterfaceType);
      }
   }

   // starts jweb opening GUI
   void makeWebFrame() {

      try {
         UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
      }

      catch(Exception e) {
         e.printStackTrace();
      }
      frame = new web.WebFrame();

      // Validate frames that have preset sizes
      // Pack frames that have useful preferred size info, e.g. from their layout
      frame.pack();

      // set window location
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      Dimension frameSize = frame.getSize();

      if (frameSize.height > screenSize.height) {
         frameSize.height = screenSize.height;
      }

      if (frameSize.width > screenSize.width) {
         frameSize.width = screenSize.width;
      }

      // frame.setLocation((screenSize.width - frameSize.width), 0);
      frame.setLocation((screenSize.width - frameSize.width - 25), 25);
      frame.setVisible(true);
   }

   /**Main method*/
   public static void main(String[] args) {

      String commLineArg;

      // jweb activated with no command line argument
      if (args.length == 0) {
         new StartWeb();
      }

      // jweb activated with one command line argument
      else if (args.length == 1) {
         commLineArg = args[0];
         new StartWeb(commLineArg);
      }

      // jweb activated with more than one command line argument
      else {
         System.out.println(" Incorrect command-line argument !");
         System.exit(0);
      }
   }
}
