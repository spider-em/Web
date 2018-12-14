#!/usr/bin/perl

# PURPOSE:  Perl script to edit email address in SPIDER source files 
# USAGE:    dorcsnew.perl < filelist.txt
# AUTHOR:   ArDean Leith   Dec 2018

# awk '{print "coi "$1" "$2" "$3}' junk1 > junk2

# ls -1 *.c > filelist.txt

while ($webfile = <>) 
   {
   #print ' Accessing: '. $webfile."\n";
   chop $webfile;
        
   system(" ci -mbollingers_email_and_cosmetic $webfile"."\n") ;
   
   system(" co -u $webfile  "."\n") ;
   system(" chmod ug+w $webfile"."\n") ;
   system(" chmod +r   $webfile"."\n") ;
   #exit;	
   }
