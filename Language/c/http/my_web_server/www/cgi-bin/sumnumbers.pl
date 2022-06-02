#!/usr/bin/perl

use CGI;
my $cgi = CGI->new();

my $num1 = $cgi->param( "num1" );
my $num2 = $cgi->param( "num2" );

print "HTTP/1.0 200 OK\r\n";
printf "Content-type: text/html\r\n\r\n";

if($num1 && $num2)
{
  $sum = $num1 + $num2;
  print "<h2>Number1: ".$num1.", Number2: ".$num2.", Sum: ".$sum."</h2>";  
}
else
{
  print "<h2>Invalid Numbers passed: ".$ENV{'QUERY_STRING'}."</h2>";
}
