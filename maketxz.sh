#!/bin/sh

cd $(dirname $0)
mkdir -p pkg
export DESTDIR=$PWD/pkg
VER=1.2.5
ARCH=${ARCH:-x86_64}
RLZ=2dj

	cmake -DCMAKE_INSTALL_PREFIX=/usr .
		
	make || return 1
	make DESTDIR=pkg install || return 1

	cd pkg

	mkdir install

cat <<EOF > install/slack-desc
sli: "Slackware Live (live DVD/USB/NFS build and install tool)"
sli: "It is written for Slackware / Slackware64 / Slackware-ARM Linux:"
sli: "support GPT and legacy MBR partitionned disks, UEFI and CSM "
sli: "(legacy / BIOS) booting, contains all the necessary to convert an"
sli: "installed system to a live system and vice-versa;"
sli: "it doesn’t need any kernel recompile, but it support seamlessly"
sli: "AUFS if your kernel support them; no changes are needed on the system"
sli: "to make live; the live system boots like an installed one"
sli: "(100% compatible with stock Slackware); support persistent changes"
EOF

	/sbin/makepkg -l y -c n ../sli-$VER-$ARCH-$RLZ.txz
	cd ..
rm -rf pkg
md5sum sli-$VER-$ARCH-$RLZ.txz > sli-$VER-$ARCH-$RLZ.md5
echo -e "aaa_elflibs|libpng,aaa_elflibs|pcre,aaa_elflibs|zlib,atk,bzip2,cairo,expat,fontconfig,freetype,fribidi,gdk-pixbuf2,glib2,graphite2,gtk+2,harfbuzz,libX11,libXau,libXcomposite,libXcursor,libXdamage,libXdmcp,libXext,libXfixes,libXi,libXinerama,libXrandr,libXrender,libffi,libglvnd,libxcb,pango,pixman,util-linux" > sli-$VER-$ARCH-$RLZ.dep
