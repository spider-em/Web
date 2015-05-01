#!/bin/csh
#
# SOURCE:  /usr8/web/utils/webwwwupdate.csh
#
# PURPOSE: Copies all website WEB documents to: CSS's desired location
#          CSS will then copy from that location into external 
#          Wadsworth website once a day
#
# USAGE:   webwwwupdate.csh 

# Location for local raw web doc files (non-headerized)
set web_root      = /usr8/web
set webrawdocsdir = $web_root/rawdocs/

# Location for local headerized html web doc files 
set webdocsdir = $web_root/docs/

# Location for local web src code files  
set websrcdir  = $web_root/src/

# Location for local Jweb files  
set jwebsrcdir = $web_root/jweb/

# Location for Wadsworth WWWinternal Output files 
#set wwwdir   = 'nnewton:/usr3/WWW/wwwinternal/spider_doc'
set wwwhost    = 'spider-stage'    
set wwwdir     = '/export/apache/vhosts/spider.wadsworth.org/htdocs/spider_doc/web'    
set wwwhostdir = $wwwhost':'$wwwdir    

# For Wadsworth WWW. Set copy command verbose, compressed, 
#    update, preserve executability, preserve time,
#    preserve symlinks, follow links, copy dir.  (-n = dryrun)
#set sendit = 'rsync -vzuEtLd --exclude="RCS/" \   #Linux

set sendit = 'rsync -vzuptL  --exclude="RCS/" \
                             --exclude="Attic/" \
                             --exclude="osx/" '

set LOGFILE = $web_root/utils/wwwupdate.LOG


pushd .
cd $web_root/utils

# Create directories on target machine
ssh $wwwhost mkdir -p $wwwdir/docs

# Create a LOG for update. -------------------------------
\rm -f $LOGFILE
echo "  "                 > $LOGFILE
echo " Update date is :" >> $LOGFILE
date                     >> $LOGFILE
echo "  "                >> $LOGFILE

# Add headers, etc. to : rawdocs/*   ------------------
mkdir -p tmpwww
$web_root/utils/webraw2docs.perl -wadsworth
echo "Prepared docs with html headers, etc. " 
echo "Prepared docs with html headers, etc. " >> $LOGFILE
echo " " 

# Copy headerized WEB doc files from temp dir------------------
cd $webdocsdir 
echo $sendit $web_root/utils/tmpwww/*  $wwwhostdir/docs
$sendit      $web_root/utils/tmpwww/*  $wwwhostdir/docs
echo "Copied non-headerized WEB files: *.gif  README* *cmd" 
echo "Copied non-headerized WEB files " >> $LOGFILE
echo " " 

# Copy non-headerized WEB doc files ------------------
cd $webdocsdir 
echo $sendit *.gif  README* *cmd  $wwwhostdir/docs
$sendit      *.gif  README* *cmd  $wwwhostdir/docs
echo "Copied non-headerized WEB files: *.gif  README* *cmd" 
echo "Copied non-headerized WEB files " >> $LOGFILE
echo " " 

# Copy WEB src files ---------------------------------

cd  $websrcdir 
echo $sendit  *.c *.h *.txt *.dat Make* README $wwwhostdir/src
$sendit       *.c *.h *.txt *.dat Make* README $wwwhostdir/src
echo "Copied WEB src files " 
echo "Copied WEB src files " >> $LOGFILE
echo " " 

# Copy xlib, rgb, osx/AppPlusS_osx    files --------------------------------

cd $web_root
# Create directories on target machine
ssh $wwwhost mkdir -p $wwwdir/src/osx
$sendit src/xlib/*                 $wwwhostdir/src/xlib
$sendit src/osx/AppPlusS_osx/*     $wwwhostdir/src/osx/AppPlusS_osx
$sendit rgb/*                      $wwwhostdir/rgb
echo $sendit src/xlib, src/osx/AppPlusS_osx  rgb   $wwwhostdir...
echo "Copied xlib, osx/AppPlusS_osx & rgb files " 
echo "Copied xlib, osx/AppPlusS_osx & rgb files " >> $LOGFILE
echo " " 

# Copy JWEB src files --------------------------------
cd $jwebsrcdir/src
# Create directories on target machine
ssh $wwwhost mkdir -p $wwwdir/jweb/src $wwwdir/jweb/linux $wwwdir/jweb/win
echo $sendit * $wwwhostdir/jweb/src
$sendit      * $wwwhostdir/jweb/src

cd $jwebsrcdir/linux
echo $sendit jweb.cshrc jweb-inst* font*  $wwwhostdir/jweb/linux 
$sendit      jweb.cshrc jweb-inst* font*  $wwwhostdir/jweb/linux

cd $jwebsrcdir/win
echo $sendit jweb.cmd jweb-inst*          $wwwhostdir/jweb/win 
$sendit      jweb.cmd jweb-inst*          $wwwhostdir/jweb/win

echo "Copied JWeb src, linux & win files " 
echo "Copied JWeb src, linux & win files " >> $LOGFILE
echo " " 


echo "Finished"     
echo "Finished"   >> $LOGFILE

exit 1

#########
#  Sterecon stuff is in ~/leith/www/sterecon
