#! /bin/csh
#  Copyright (c) 1989 Lars Fredriksen, Bryan So, Barton Miller
#  All rights reserved
#   
#  This software is furnished under the condition that it may not
#  be provided or otherwise made available to, or used by, any
#  other person.  No title to or ownership of the software is
#  hereby transferred.
# 
#  Any use of this software must include the above copyright notice.

#  Fuzz tests.
#
#
# Testing utilites that require special file name extensions
#
# Example
#           run.cp t.c cc -g
#
# will use cp to copy the test cases to a file named t.c, then execute
# cc -g t.c
#
# a - (minus) can be given as the first argument to suppress appending
# the file name to the command
#

set TESTS=(t1 t2 t3 t4 t5 t6 t7 t8 t9 t10 t11 t12)
set DATE=`date | cut -d " " -f -3 | sed -e "s/ /_/g"`
set NOTES=notes-$DATE

set suppress=0
if ( $1 == - ) then
     set suppress=1
     shift
endif

set TARGET=$1
shift
if ( ! `./executable $1` ) then
     echo $*\: Command not found.
     echo $*\: Command not found. >> $NOTES
     exit 1
endif

echo Testing $* ... >> $NOTES

#
#  Erase some old core images
#
if ( -f core ) /bin/rm core

foreach test ($TESTS)
     onintr InfiniteLoop
     echo -n cp $test $TARGET\; 
     /bin/cp $test $TARGET
     if ( $suppress ) then
	  echo $* \>\& /dev/null
	  $* >& /dev/null
     else
	  echo $* $TARGET \>\& /dev/null
          $* $TARGET >& /dev/null
     endif
     goto CheckCore

InfiniteLoop:
     echo ''
     echo User aborted.
     echo $test infinite loop. >> $NOTES

CheckCore:
     if ( -f core ) then
	  echo core dumped.
	  echo $test core dumped. >> $NOTES
#	  /bin/rm core
     /bin/mv core core-$1-$test
     endif
end
if ( -f $TARGET ) then
     echo rm $TARGET
     /bin/rm $TARGET
endif
