#!/bin/csh
#
# SOURCE:  /usr8/web/utils/localwwwupdate.csh
#
# PURPOSE: Copies all relevant WEB html documents into docs 
#          location and adds header
#
# USAGE:   localwwwupdate.csh 

# Set location for (non-headerized) Web raw html files (input)
set web_root      = /usr8/web
set webrawdocsdir = $web_root/rawdocs/

# Add html headers, trailers etc. to stuff in: $webrawdocsdir 
$web_root/utils/webraw2docs.perl 

echo " DONE adding html headers, trailers, etc. " 
exit 1

