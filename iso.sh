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
}
EOF
grub-mkrescue -o scos.iso isodir

