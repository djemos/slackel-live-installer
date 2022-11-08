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

#disks=`fdisk -l 2>/dev/null | grep "^/dev/sd" | cut -c6-8 | sort -u | grep -v "$bootdevice"`
for disks in sda sdb sdc sdd sde sdf sdg sdh sdi sdj sdk sdl sdm sdn sdo sdp sd \
     nvme0n1 nvme1n1 nvme2n1 nvme3n1 nvme4n1 nvme5n1 nvme6n1 nvme7n1 nvme8n1 nvme9n1 \
     nvme0n2 nvme1n2 nvme2n2 nvme3n2 nvme4n2 nvme5n2 nvme6n2 nvme7n2 nvme8n2 nvme9n2 \
     nvme0n3 nvme1n3 nvme2n3 nvme3n3 nvme4n3 nvme5n3 nvme6n3 nvme7n3 nvme8n3 nvme9n3 \
     nvme0n4 nvme1n4 nvme2n4 nvme3n4 nvme4n4 nvme5n4 nvme6n4 nvme7n4 nvme8n4 nvme9n4  ; do
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
done
