package web;

import java.awt.*;
import javax.swing.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilParameters {
   public JTextArea outputMessageTextArea;
   public Dimension screenDimension;
   public String currDirectory, dataFileExtension;
   public int contrastLowRamp, contrastHighRamp;
   public float resizeFactor;
   public boolean montageXYZShown, sliceXShown, sliceYShown, sliceZShown;
   public boolean movieXShown, movieYShown, movieZShown;

   public boolean ImageMarker, resizeLarge, resizeSmall;

   public boolean showHeaderInfo;

   public UtilParameters() {
      screenDimension = Toolkit.getDefaultToolkit().getScreenSize();
      outputMessageTextArea = new JTextArea(8,70);
      currDirectory = ".";
      dataFileExtension = "*";
      resizeFactor = 1;
      contrastLowRamp = 0;
      contrastHighRamp = 255;
      resizeLarge = true;
      resizeSmall = false;

      // used to check if montage, slice and movie windows have ever been opened
      montageXYZShown = false;
      sliceXShown = false;
      sliceYShown = false;
      sliceZShown = false;
      movieXShown = false;
      movieYShown = false;
      movieZShown = false;

      showHeaderInfo = true;

      ImageMarker=false;
   }
}
