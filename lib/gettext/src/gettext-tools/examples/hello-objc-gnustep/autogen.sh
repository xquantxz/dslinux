#!/bin/sh
# Example for use of GNU gettext.
# Copyright (C) 2003-2006 Free Software Foundation, Inc.
# This file is in the public domain.
#
# Script for regenerating all autogenerated files.

if test -r ../Makefile.am; then
  # Inside the gettext source directory.
  GETTEXT_TOPSRCDIR=../../..
else
  if test -r ../Makefile; then
    # Inside a gettext build directory.
    GETTEXT_TOOLS_SRCDIR=`sed -n -e 's,^top_srcdir *= *\(.*\)$,\1,p' ../Makefile`
    # Adjust a relative top_srcdir.
    case $GETTEXT_TOOLS_SRCDIR in
      /*) ;;
      *) GETTEXT_TOOLS_SRCDIR=../$GETTEXT_TOOLS_SRCDIR ;;
    esac
    GETTEXT_TOPSRCDIR=$GETTEXT_TOOLS_SRCDIR/../..
  else
    # Installed under ${prefix}/share/doc/gettext/examples.
    . ../installpaths
  fi
fi

cp -p ${GETTEXTSRCPODIR-$GETTEXT_TOPSRCDIR/gettext-runtime/po}/remove-potcdate.sin po/remove-potcdate.sin
