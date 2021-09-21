#!/bin/sh
set -e
. ./headers.sh
clear 
for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done

