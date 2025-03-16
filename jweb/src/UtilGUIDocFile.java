package web;

import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

/**
 * <p>Title: WEB: A GUI for viewing and analyzing SPIDER images</p>
 * <p>Description: WEB is a graphical user interface for viewing and analyzing SPIDER images.</p>
 * <p>Copyright: Copyright (c) 2001</p>
 * <p>Company: Health Research Inc.</p>
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIDocFile extends JFrame implements ChangeListener,
                                                           ActionListener{
   UtilParameters utilParameters;
   UtilGUIDendrogram utilGUIDendrogram;
   protected  JTabbedPane jTabbedPane;


   public UtilGUIDocFile(UtilDoc utilDoc, JButton closeAll, UtilParameters utilParam) {

     utilParameters = utilParam;
     jTabbedPane = new JTabbedPane(SwingConstants.TOP);

     utilGUIDendrogram =  new UtilGUIDendrogram(utilDoc, utilParameters);
     jTabbedPane.addTab("Dendrogram", utilGUIDendrogram);

     getContentPane().add(jTabbedPane, "Center");

     setTitle("Operations Based On SPIDER Document File");
     setSize(300,400);

     //set the window position properly, so that windows will not be overlayed.
     setLocation(200,5);
     pack();
     setVisible(true);
   }

   public void stateChanged(ChangeEvent event) {

   }

   public void actionPerformed(ActionEvent evt) {
      // close window when closeAll button selected from the main WEB menu
      this.dispose();
   }
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {

      }
   }
}
