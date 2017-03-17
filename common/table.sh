#!/bin/sh
#
#  table.sh  --  Extract and format a -table from Bound-T output.
#
#  Usage: as a filter on a Bound-T output file, when Bound-T was run
#  with the -table option.
#
# This file is copyright (c) 2005 Tidorum Ltd.
# Permission is granted to use and distribute this file for any purposes.
#
#-----------------------------------------------------------------------

(
  echo -e "Total\tSelf\tNum\tTime Per Call"
  echo -e "Time\tTime\tCalls\tMin\tMax\tSubprogram"
  echo -e "-----\t----\t-----\t-------------\t----------"

  egrep '^Time_Table' |
  cut -d: -f6-11 |
  sort -t: -nr |
  tr ':' '\011'
) |
expand --tabs=10,20,30,40,50
