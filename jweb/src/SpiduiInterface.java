package web;

import java.lang.*;
import java.io.*;
import java.net.*;


/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class SpiduiInterface extends Thread {

   private Thread serverThread;
   private ServerSocket srvr;
   private BufferedReader in;
   private WebFrame frame;
   private String strInterfaceType = new String ("SPIDUI");

   public SpiduiInterface(WebFrame webFrame) {
      frame = webFrame;
      serverThread = new Thread(this) ;
      serverThread.start();
   }

   public void run() {

      // thread runs here
      Thread currentThread = Thread.currentThread();

      try {
         // open a port
         srvr = new ServerSocket(50008);
         // start an infinite loop
         while(currentThread == serverThread) {
            // make a new socket for each request of the client
            // works for one client at a time. This set-up cann't handle
            // more than one client at the same time.
            Socket skt = srvr.accept();
            in = new BufferedReader(new
                             InputStreamReader(skt.getInputStream()));

            // writes back to the client through the same socket mechanism
            // read and write both are done through the same port.
            /*PrintWriter out = new PrintWriter(new BufferedWriter
                 (new OutputStreamWriter(skt.getOutputStream())), true);  */
            // reads the line (one line only) sent by the client
            String imgFile = in.readLine();
            frame.openAnImage(imgFile, strInterfaceType);
            in.close();
            skt.close();
            // resources related to srvr  should be released when JWEB main
            // GUI is closed, i.e. the JWEB application stops running.
            // if this is not the case then srvr can be closed by using
            // srvr.close(); in the WebFrame window closing listener.

            // writes back to the client
            //out.println("Echo: " + imgFile);
         }
      }
      catch (Exception exception){
         System.err.println(exception.toString());
      }
   }
}
