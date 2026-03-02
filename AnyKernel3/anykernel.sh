# AnyKernel3 Ramdisk Mod Script
# osm0sis @ xda-developers

## AnyKernel setup
# begin properties
properties() { '
kernel.string=Sparxie kernel for MIUI, HyperOS 2+, and AOSP 13-15
do.devicecheck=0
do.modules=0
do.systemless=1
do.cleanup=1
do.cleanuponabort=1
device.name1=moonstone
device.name2=sunstone
device.name3=stone
device.name4=gemstone
device.name5=miholi
supported.versions=12-17
supported.patchlevels=
'; } # end properties

### AnyKernel install
# boot shell variables
block=boot;
is_slot_device=auto;

# import functions/variables and setup patching - see for reference (DO NOT REMOVE)
. tools/ak3-core.sh;

# boot install
split_boot;
flash_boot;
## end boot install
