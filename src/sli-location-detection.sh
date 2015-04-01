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

disks=`cat /proc/partitions | sed 's/  */:/g' | cut -f5 -d: | sed -e /^$/d -e /[1-9]/d -e /^sr0/d -e /loop/d -e /$bootdevice/d`

for disk in $disks; do
	if [ "$1" == "copy" ]; then
		size=`cat /proc/partitions | grep "$disk$" | sed 's/  */:/g' |cut -f4 -d:`
		if (( $size > $requiredsize )); then
			echo "/dev/$disk"
		fi
	fi
	partitions=`cat /proc/partitions | sed -n /$disk[0-9]/p | sed 's/  */:/g' | cut -f5 -d:`
	for partition in $partitions; do
		type=`blkid -pi /dev/$partition | grep "^TYPE=" | cut -f2 -d=`
		if [ "$type" != "ntfs" ] && [ "$type" != "vfat" ] && [ "$type" != "swap" ]; then
			size=`cat /proc/partitions | grep "$partition$" | sed 's/  */:/g'| cut -f4 -d:`
			if (( $size > $requiredsize )); then
				echo "/dev/$partition"
			fi
		fi
	done
done
