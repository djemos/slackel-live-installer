#!/bin/sh
system="$1"
home="$2"
login="$3"
password="$4"

if [ ! -b "$1" ] || [ ! -b "$2" ]; then
	echo "usage: $0 system_partition home_partition [login [password]]"
	#~ echo "usage: $0 system_partition home_partition auto_login [password]"
	echo "example: $0 /dev/sdx2 /dev/sdx3 wolf b0seMolv"
	exit 1
fi



mkdir -p /mnt/install
mount $system /mnt/install



#handles login
firstuserlogin=`cat /mnt/install/etc/passwd | grep ":1000:" | cut -f1 -d':'`
#~ if [ "$login" == "auto_login" ]; then
if [ -z "$login" ]; then
	if [ ! -z "$firstuserlogin" ]
	then login="$firstuserlogin" #distribution default choice
	else login="vador" #because of darkstar ;-)
	fi
fi

#~ if [ ! -z "$login" ]; then
	if [ "$login" != "$firstuserlogin" ]; then
		if [ ! -z "$firstuserlogin" ]; then
			chroot /mnt/install userdel -r $firstuserlogin
		fi
		chroot /mnt/install useradd -g users -G disk,lp,wheel,floppy,audio,video,cdrom,plugdev,power,netdev,scanner -m $login
	fi
	
	if [ -z "$password" ]; then
		sed -e 's/^root:[^:]*:/root::/' -e "s/^$login:[^:]*:/$login::/" -i /mnt/install/etc/shadow
	else
		echo -e "root:$password\n$login:$password" > /mnt/install/root/chpasswd.dat
		echo "chpasswd < /root/chpasswd.dat" > /mnt/install/root/chpasswd.sh
		chmod +x /mnt/install/root/chpasswd.sh
		chroot /mnt/install /root/chpasswd.sh
		rm -f /mnt/install/root/chpasswd.{sh,dat}
	fi
#~ fi



#handles home partition
if [ "$home" != "$system" ]; then
	mkdir -p /mnt/home
	if ! mount $home /mnt/home 2>/dev/null; then
		mkfs.ext4 $home
		mount $home /mnt/home
	fi
	fs=`mount | grep "$home" | cut -f5 -d' '`
	if echo $fs | grep -q "ext" || echo $fs | grep -q "brtfs" || echo $fs | grep -q "reiser"|| echo $fs | grep -q "xfs"; then
		echo "$home /home $fs defaults 1 1" >> /mnt/install/etc/fstab
		for login in `ls /mnt/home`; do #detect existing home directories and create users
			if ! grep -q "^$login:" /mnt/install/etc/passwd && [ -d /mnt/home/.config ]; then
				chroot /mnt/install useradd -g users -G disk,lp,wheel,floppy,audio,video,cdrom,plugdev,power,netdev,scanner $login
				sed "s/^$login:!:/$login::/" -i /mnt/install/etc/shadow
			fi
			uid=`cat /mnt/install/etc/passwd | grep "^$login" | cut -f3 -d:`
			chown -R $uid:100 /mnt/home/$login
		done
		for login in `ls /mnt/install/home`; do
			if [ -d /mnt/home/$login ]
			then rm -rf /mnt/install/home/$login
			else mv /mnt/install/home/$login /mnt/home/
			fi
		done
	fi
	umount /mnt/home
	rmdir /mnt/home
fi



umount /mnt/install
rmdir /mnt/install
