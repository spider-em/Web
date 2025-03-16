package web;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilParametersImageSeries {

   public boolean categorizeShown;
   public boolean markerSeriesShown, tiltPairShown;


   public UtilParametersImageSeries() {

      // used to check if categorize window is ever been opened
      categorizeShown = false;

      // used to check if marker series window is ever been opened
      markerSeriesShown = false;
      // used to check if tilt pair window is ever been opened
      tiltPairShown = false;
  }
}
