#!/bin/sh
if [ "$1" != "install" ] && [ "$1" != "copy" ] && [ "$1" != "home" ]; then
	echo "usage: $0 copy|install|home"
	exit 1
fi

if [ -d /live/media ]
then if [ "`cat /proc/mounts | grep /live/media | cut -f3 -d' '`" == "nfs" ]
	then bootdevice="nfs"
	else bootdevice=`cat /proc/mounts | grep /live/media | cut -c6-8`
	fi
	livesize=`du -s /live/media/boot | sed 's/\t.*//'`
else bootdevice="notlive" #to list all partitions
	livesize=0
fi

case "$1" in
"install") let requiredsize=livesize*4 ;;
"copy") requiredsize=$livesize ;;
"home") requiredsize=0 ;;
esac

disks=`fdisk -l 2>/dev/null | grep "^/dev/sd" | cut -c6-8 | sort -u | grep -v "$bootdevice"`
for disk in $disks; do
	if [ "$1" == "copy" ]; then
		size=`cat /proc/partitions | grep "$disk$" | sed 's/  */:/g' |cut -f4 -d:`
		if (( $size > $requiredsize )); then
			echo "/dev/$disk"
		fi
	fi
	if fdisk -l -o Type-UUID /dev/$disk 2>/dev/null | grep -q ": dos$\|21686148-6449-6E6F-744E-656564454649$\|C12A7328-F81F-11D2-BA4B-00A0C93EC93B$"; then #if partition scheme support GRUB
		partitions=`{ fdisk -l -o Device,Type-UUID /dev/$disk; fdisk -l -o Device,Id /dev/$disk; } 2>/dev/null | grep " 0FC63DAF-8483-4772-8E79-3D69D8477DE4$\| 83$" | cut -f1 -d' ' | cut -f3 -d'/'` #linux partitions
		for partition in $partitions; do
			size=`cat /proc/partitions | grep "$partition$" | sed 's/  */:/g'| cut -f4 -d:`
			if (( $size > $requiredsize )); then
				echo "/dev/$partition"
			fi
		done
	fi
done
