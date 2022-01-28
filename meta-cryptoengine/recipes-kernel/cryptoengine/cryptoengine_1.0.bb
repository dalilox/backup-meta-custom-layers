SUMMARY = "Crypto engine layer recipe"
DESCRIPTION = "A crypto engine driver"
LICENSE = "CLOSED"

inherit module 

SRC_URI = " file://cryptoengine.c \
            file://cryptoengine.h \
            file://cryptotest.c \
            file://Makefile \
            file://COPYING \
"

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-cryptoengine"

RPROVIDES_${PN} += "kernel-module-cryptotest"

KERNEL_MODULE_AUTOLOAD += "cryptoengine"