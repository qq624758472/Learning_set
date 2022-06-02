#!/usr/bin/perl
##
##  printenv.demo CGI program which just prints its environment
##
#
print "HTTP/1.0 200 OK\r\n";
print "Content-type: text/plain\r\n\r\n";
 foreach $var (sort(keys(%ENV))) {
   $val = $ENV{$var};
   $val =~ s|\n|\\n|g;
   $val =~ s|"|\\"|g;
   print "${var}=\"${val}\"\n";
 }
