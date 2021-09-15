#!/bin/sh
set -e
. ./iso.sh
 
qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -no-reboot -no-shutdown -d cpu_reset,in_asm,int,pcall,page,mmu,guest_errors -device virtio-serial -serial telnet:localhost:4321,server,nowait -cdrom scos.iso
