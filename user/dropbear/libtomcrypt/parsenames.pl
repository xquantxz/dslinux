#!/usr/bin/perl
#
# Splits the list of files and outputs for makefile type files 
# wrapped at 80 chars 
# 
# Tom St Denis
@a = split(" ", $ARGV[1]);
$b = "$ARGV[0]=";
$len = length($b);
print $b;
foreach my $obj (@a) {
   $len = $len + length($obj);
   $obj =~ s/\*/\$/;
   if ($len > 100) {
      printf "\\\n";
      $len = length($obj);
   }
   print "$obj ";
}
if ($ARGV[0] eq "HEADERS") { print "testprof/tomcrypt_test.h"; }

print "\n\n";

# $Source$   
# $Revision: 1054 $   
# $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ 
