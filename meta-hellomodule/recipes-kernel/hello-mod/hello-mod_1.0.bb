DESCRIPTION = "hello-world-mod tests the module.bbclass mechanism"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=cf540fc7d35b5777e36051280b3a911c"

inherit module

SRC_URI = "file://Makefile \
	   file://hello.c \
	   file://COPYING \
	   "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-hello"

KERNEL_MODULE_AUTOLOAD += "hello"
