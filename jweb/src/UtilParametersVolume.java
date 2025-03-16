package web;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilParametersVolume {

   public boolean montageXYZShown, sliceXShown, sliceYShown, sliceZShown;
   public boolean movieXShown, movieYShown, movieZShown, surfaceShown,
                                                         montagedSurfaceShown ;

   public UtilParametersVolume() {

      // used to check if montage, slice and movie windows have ever been opened
      montageXYZShown = false;
      sliceXShown = false;
      sliceYShown = false;
      sliceZShown = false;
      movieXShown = false;
      movieYShown = false;
      movieZShown = false;
      surfaceShown = false;
      montagedSurfaceShown = false;
  }
}
