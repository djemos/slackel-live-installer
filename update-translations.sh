#!/bin/sh

intltool-extract --type="gettext/ini" src/sli.desktop.in
intltool-extract --type="gettext/ini" src/sli-kde.desktop.in

xgettext --from-code=utf-8 \
	-L Glade \
	-o po/sli.pot \
	src/sli.ui

xgettext --from-code=utf-8 \
	-j \
	-L Python \
	-o po/sli.pot \
	src/sli.c
xgettext --from-code=utf-8 -j -L C -kN_ -o po/sli.pot src/sli.desktop.in.h
xgettext --from-code=utf-8 -j -L C -kN_ -o po/sli.pot src/sli-kde.desktop.in.h

rm src/sli.desktop.in.h src/sli-kde.desktop.in.h

cd po
for i in `ls *.po`; do
	msgmerge -U $i sli.pot
done
rm -f ./*~
