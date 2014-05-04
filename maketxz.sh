#!/bin/sh

cd $(dirname $0)
mkdir -p pkg
export DESTDIR=$PWD/pkg
VER=1.2
ARCH=${ARCH:-i486}
RLZ=1dj

	cmake -DCMAKE_INSTALL_PREFIX=/usr .
		
	make || return 1
	make DESTDIR=pkg install || return 1

	cd pkg

	mkdir install

cat <<EOF > install/slack-desc
slackware-live: "Slackware Live (live DVD/USB/NFS build and install tool)"
slackware-live: "It is written for Slackware / Slackware64 / Slackware-ARM Linux:"
slackware-live: "support GPT and legacy MBR partitionned disks, UEFI and CSM "
slackware-live: "(legacy / BIOS) booting, contains all the necessary to convert an"
slackware-live: "installed system to a live system and vice-versa;"
slackware-live: "it doesn’t need any kernel recompile, but it support seamlessly"
slackware-live: "AUFS if your kernel support them; no changes are needed on the system"
slackware-live: "to make live; the live system boots like an installed one"
slackware-live: "(100% compatible with stock Slackware); support persistent changes"
EOF

	/sbin/makepkg -l y -c n ../sli-$VER-$RLZ-$ARCH.txz
	cd ..
rm -rf pkg
