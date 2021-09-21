#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/scos.kernel isodir/boot/scos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "scos" {
	multiboot /boot/scos.kernel
    	set gfxpayload=text    
}
menuentry "System shutdown" {
	echo "System shutting down..."
	halt
}
menuentry "System restart" {
	echo "System rebooting..."
	reboot
}
menuentry "Firmware setup" {
	fwsetup

menuentry "first drive boot" {
    set root=(hd0,1)
    chainloader +1
    boot
}
EOF
grub-mkrescue -o scos.iso isodir

