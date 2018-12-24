#!/usr/bin/env perl

use strict;
use warnings;

my $file = $ARGV[0];
open(my $fh, '<', $file);
my $count = 0;

while (<$fh>) {
  chomp;
  my $first = substr($_, 0, 1);
  if ($first eq 'I') {
    ++$count;
  } elsif ($first eq 'M' && $count > 0) {
    --$count;
  }
  print "$count\n";
}

close($fh);

