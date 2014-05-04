#!/bin/sh
if [ -d /live/media ]
then if [ `cat /proc/mounts | grep /live/media | cut -f3 -d' '` == "nfs" ]
	then bootdevice="nfs"
	else bootdevice=`cat /proc/mounts | grep /live/media | cut -c6-8`
	fi
	livesize=`du -s /live/media/boot | sed 's/\t.*//'`
	if [ `uname -m` == "x86_64" ]; then
		efisize=`du -s /live/media/EFI | sed 's/\t.*//'`
		let livesize+=efisize
		efisize=`du -s /live/media/efi.img | sed 's/\t.*//'`
		let livesize+=efisize
	fi
	exitvalue=0
else bootdevice="notlive" #to list all partitions
	livesize=0
	exitvalue=1
fi
let installsize=livesize*4

disks=`cat /proc/partitions | sed 's/  */:/g' | cut -f5 -d: | sed -e /^$/d -e /[1-9]/d -e /^sr0/d -e /$bootdevice/d -e s@^@/dev/@`

if [ "$1" == "install" ]; then
	for disk in $disks; do
		if fdisk -l $disk 2>/dev/null | grep -q GPT; then
			linuxparts=`gdisk -l $disk | grep "^  *[1-9]" | sed -e s@^\ \ *@$disk@ -e 's/  */:/g' | grep ":8300:" | cut -f1 -d:`
		else
			linuxparts=`fdisk -l $disk 2>/dev/null | grep "^$disk" | sed -e 's/\*//' -e 's/  */:/g' | grep ":83:" | cut -f1 -d:`
		fi
		for linuxpart in $linuxparts; do
			part=`echo $linuxpart | cut -f3 -d/`
			size=`cat /proc/partitions | grep "$part$" | sed 's/  */:/g'| cut -f4 -d:`
			if (( $size > $installsize )); then
				echo $linuxpart
			fi
		done
	done
else
	if [ "$1" == "copy" ]; then
		for disk in $disks; do
			disk=`echo $disk | cut -f3 -d/`
			size=`cat /proc/partitions | grep "$disk$" | sed 's/  */:/g' |cut -f4 -d:`
			disk="/dev/$disk"
			if (( $size > $livesize )); then
				echo $disk
				if fdisk -l $disk 2>/dev/null | grep -q GPT; then
					liveparts=`gdisk -l $disk | grep "^  *[1-9]" | sed -e s@^\ \ *@$disk@ -e 's/  */:/g' | sed -n '/:\(EF00\|8300\):/p' | cut -f1 -d:`
					tmpliveparts=`gdisk -l $disk | grep "^  *[1-9]" | sed -e s@^\ \ *@$disk@ -e 's/  */:/g' | sed -n '/:0700:/p' | cut -f1 -d:`
					mkdir -p /mnt/tmp
					for livepart in $tmpliveparts; do #Microsoft partitions can be FAT or NTFS formated
						if mount $livepart /mnt/tmp 2>/dev/null; then
							sleep 1
							if ! mount | grep "^$livepart" | grep -q fuseblk; then #if filesystem is not NTFS
								liveparts="$liveparts $livepart"
							fi
							umount /mnt/tmp
						else
							liveparts="$liveparts $livepart"
						fi
					done
				else
					liveparts=`fdisk -l $disk 2>/dev/null | grep "^$disk" | sed -e 's/[*+]//g' -e 's/  */:/g' | sed -n '/:\(83\|6\|b\|c\|e\):/p' | cut -f1 -d:`
				fi
				for livepart in $liveparts; do
					part=`echo $livepart | cut -f3 -d/`
					size=`cat /proc/partitions | grep "$part$" | sed 's/  */:/g'| cut -f4 -d:`
					if (( $size > $livesize )); then
						echo $livepart
					fi
				done
			fi
		done
	else
		echo "usage: $0 copy|install"
	fi
fi

exit $exitvalue
