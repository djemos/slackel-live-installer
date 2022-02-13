#!/bin/sh

cd $(dirname $0)
mkdir -p pkg
export DESTDIR=$PWD/pkg
VER=1.2.5
ARCH=${ARCH:-x86_64}
RLZ=8dj

	cmake -DCMAKE_INSTALL_PREFIX=/usr .

	make || return 1
	make DESTDIR=pkg install || return 1

	cd pkg

	mkdir install

cat <<EOF > install/slack-desc
sli: Slackel Live Installer (gui application for installing live system)"
sli: "It is  written for Slackel Linux:"
sli: "support GPT and legacy MBR partitionned disks, UEFI and CSM"
sli: "(legacy / BIOS) booting"
EOF

	/sbin/makepkg -l y -c n ../sli-$VER-$ARCH-$RLZ.txz
	cd ..
rm -rf pkg
md5sum sli-$VER-$ARCH-$RLZ.txz > sli-$VER-$ARCH-$RLZ.md5
echo -e "aaa_elflibs|libpng,aaa_elflibs|pcre,aaa_elflibs|zlib,atk,bzip2,cairo,expat,fontconfig,freetype,fribidi,gdk-pixbuf2,glib2,graphite2,gtk+2,harfbuzz,libX11,libXau,libXcomposite,libXcursor,libXdamage,libXdmcp,libXext,libXfixes,libXi,libXinerama,libXrandr,libXrender,libffi,libglvnd,libxcb,pango,pixman,util-linux" > sli-$VER-$ARCH-$RLZ.dep
