#! /bin/csh
#
# SOURCE: /usr8/web/utils/tosend.csh
#
# PURPOSE: Update local copy of WEB distribution        
#          The distribution copy is currently in:  /usr8/send/web/...
#
# NEXT:    The combined SPIDER/WB distribution can then be placed in a compressed
#            tar archive and put in the download directory on the
#            external SPIDER website.
# 
# AUTHOR:  April 1993   ArDean Leith 

echo 

# Set some variables
set webroot = '/usr8/web'
set src     = $webroot/src
set srchtml = $webroot/docs

set dest    = '/usr8/send/web'

mkdir -p $dest/src $dest/docs $dest/rgb $dest/bin  

# --------------------- source & other files -------------------
#ls -1 | grep -v / | grep -v '\.c'

echo " Copying: source files."
scp -pv $src/*.c  $src/*.h $src/Make*  $src/lib*.a $src/Nextversion $src/*.xbm $src/*.X* $src/*resources* $dest/src  
echo " "

# --------------------- html/files ----------------------------
echo " Copying: Html files."

scp -pv $srchtml/*.html  $srchtml/*gif  $srchtml/*.cmd $dest/docs  
echo " "

# --------------------- AppPlusS/files -------------------------
echo " Copying: AppPlusS files for OSX only."

scp -pv $webroot/AppPlusS_osx/* $dest/AppPlusS
echo " "

# --------------------- rgb files -----------------------------
echo " NOT Copying: rgb files."

##scp -pv $webroot/rgb/*         $dest/rgb
echo " "

# ---------------------- Makebody.inc -------------------------

# Replace Makebody.inc with sendaway version
echo ' Replace Makebody.inc with sendaway version'
scp -pv $src/Makebody.inc.send $dest/src/Makebody.inc
echo " "

# ---------------------- Linux binaries ----------------------
echo " NOT    Copying: Linux binary files."
#scp -pv  $webroot/bin/web_linux_dir.56   $dest/bin/web_linux_dir
#scp -pv  $webroot/bin/web_linux_dirs.56   $dest/bin/web_linux_dirs
#scp -pv  $webroot/bin/web_linux_tru.56   $dest/bin/web_linux_tru
#scp -pv  $webroot/bin/web_linux_trus.56   $dest/bin/web_linux_trus

# ---------------------- OSX binaries ------------------------
echo " Copying: OSX binary files."

scp -pv  $webroot/bin/web_osx_8      $dest/bin/ 
scp -pv  $webroot/bin/web_osx_8.sh   $dest/bin/ 
scp -pv  $webroot/bin/web_osx_8.zsh  $dest/bin/ 
scp -pv  $webroot/bin/web_osx_tru    $dest/bin/ 

# --------------------- JWeb files ----------------------------
echo " "
echo " Jweb files are updated by spider tosend."
echo " "

echo " Great: Linux Sendaway Web updated successfully."
exit 0



