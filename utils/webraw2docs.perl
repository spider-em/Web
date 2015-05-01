#!/usr/bin/perl
#
# SOURCE FILE: webraw2docs.perl
#
# PURPOSE: Puts buttons, & header on WEB html documents
#          Copy files from: $rawdocs_dir into:  $docs_dir adding headers, etc.

$web_root     = "/usr8/web";
$spider_root  = "/usr8/spider";


$rawdocs_dir  = "$web_root/rawdocs";
$docs_dir     = "$web_root/docs/";
$butt_path    = "$spider_root/docs/buttons";

#print "web_root:  $web_root \n";
#print "butt_path: $butt_path \n";
#print "rawdocs_dir: $rawdocs_dir \n \n \n \n";

# --------------------------- Main routine ---------------------------

#Special temp output dir for external www site
if (@ARGV[0] =~ /wadsworth/)
   {
   # Retarget $docs_dir
   $docs_dir     = "tmpwww";
   }

$rel_path=".";
while (<$rawdocs_dir/*.html>)
    {
    $TITLE = "";

    s/$rawdocs_dir\///e;
    $fn = $_;
    open(fro, "< $rawdocs_dir/$fn");
    open(too, "> $docs_dir/$fn")|| 
       die "Can't open: $docs_dir/$fn \n";

    if ($fn=~/^[A-Z]/) 
       { # File with capital letters??
       while ($linn=<fro>) 
          {print too $linn; print $linn}
       }# html.html stuff
    else 
       {&installit;}
       
    close(fro);
    close(too);
    
    print "Created: $docs_dir/$fn\n";
    }
print " ------------------  \n";


# ------------------- print_wadsbuttons ---------------------------------------

# Leave no blanks between the </a><a> otherwise the images will be
# connected with underscore in the html page.

sub print_wadsbuttons
   {  # Prints the Wadsworth lab buttons 	
   return qq(\
   <hr> <h1>\
   <img align="left" width="103" height="78" alt="Web Icon" src="web_icon.gif"></a> \
   <a href="http://www.wadsworth.org"><img width="125" height="40" alt="WADSWORTH"\
       align="right" src="$rel_path/$butt_path/wadsworth.gif"></a>\
   &nbsp;&nbsp;$TITLE </h1> <p><hr> <p>\n);
   }

# ------------------- print_web_icon ---------------------------------------

sub print_web_icon
   { # 
   return qq(<hr><h1>\
   <img align="left" width="103" height="78" alt="Web Icon" src="web_icon.gif"></a> \
   &nbsp;&nbsp;$TITLE </h1> <p><hr> <p>\n);
   }


sub print_trailer
   {   # -------------------- UNUSED!! ---------------------------------
   return qq(<hr> <small><address>&copy; <a href="$rel_path/copyright.html"> \
   Copyright Notice</a> /  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  \
   Enquiries: <a href= "mailto:spider@wadsworth.org">spider@wadsworth.org</a> </address></small></a>\n);
   }

# ------------------- installit ---------------------------------------
# Install subroutine

sub installit
   {
   print too "<html>";

   $doicon = 1;    # Want Web Icon at top

   while(<fro>)
      {
      if (/<noicon/) 
         { $doicon = 0;  }  # Do not want Web-icon

      if (/<BODY/ || /<body/) 
         {		#Insert Buttons at top
         print too $_;
         if ($doicon)
            {
            print too qq(<!-- Begin Wadsworth Buttons -->\n);
            $buttons=&print_wadsbuttons;
            print too $buttons;
            print too qq(<!-- End Wadsworth Buttons -->\n);
            }
         }

      elsif (/<\/BODY>/ || /<\/body>/)
         {  # Insert trailer 
         ##$trailer=&print_trailer;
         ##print too qq(<!-- Begin Trailer -->\n);
         ##print too $trailer;
         ##print too qq(<!-- End Trailer -->\n);
         print too $_;
         }
      elsif (/<TITLE/ || /<title/)
        { # Title tag
        # Echo to file then extract title for header
        print too $_;
        s/< *TITLE *>//i;
        s/< *title *>//i;
        s/<\/TITLE>//i;
        s/<\/title>//i;
        $TITLE =$_;
        #print "Set title: ". $TITLE ."\n";
        }
      else 
         {			#Otherwise..
         print too $_;
         }
      }
     print too "</html>";
   } # End of: installit
