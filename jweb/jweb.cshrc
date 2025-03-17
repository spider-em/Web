# For use with JWeb, edit following line for actual path of your 'JWeb" 
# directory then place all lines in user's 'C' shell startup file (.cshrc)

 setenv JWEB_DIR    SUBSTITUTE_PATH_OF_YOUR_JWEB_DIRECTORY_HERE

 setenv JAIHOME        $JWEB_DIR/j2re1.4.2_06/lib
 setenv JWEBCLASSPATH  $JWEB_DIR/linux.WEB.jar:$JAIHOME/mlibwrapper_jai.jar:$JAIHOME/jai_core.jar:$JAIHOME/jai_codec.jar

 # Add JWeb and JAI libraries to your $CLASSPATH
 if  ( $?CLASSPATH ) then
   setenv CLASSPATH  ${JWEBCLASSPATH}:$CLASSPATH
 else
   setenv CLASSPATH  $JWEBCLASSPATH
 endif

 # Create alias for JWeb location and set maximum memory usage
 alias  jweb  '$JWEB_DIR/j2re1.4.2_06/bin/java -Xmx512m -Djava.util.prefs.syncInterval=2000000 web/StartWeb'


