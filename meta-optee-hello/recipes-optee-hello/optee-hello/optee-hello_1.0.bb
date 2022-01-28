SUMMARY = "OP-TEE examples"
DESCRIPTION = "Open Portable Trusted Execution Environment - Sample Applications"

LICENSE = "CLOSED"
# LICENSE = "BSD-2-Clause"
# LIC_FILES_CHKSUM = "file://${S}/LICENSE"


DEPENDS = "optee-client optee-os python3-pycryptodome-native"

inherit python3native

# SRC_URI = "git://github.com/dalilox/yocto-imx8qmmek;branch=${SRCBRANCH}"
# SRC_URI = "git://${BSPDIR}/sources/meta-optee-hello/recipes-optee-hello/optee-hello/files;protocol=file"
# SRCBRANCH = "master"
# SRCREV = "729cdc30e5447199ccc7be1ca34c48706d4ce967"

# SRC_URI = "file://Android.mk \
#            file://CMakeLists.txt \
#            file://CMakeToolchain.txt \
#            file://LICENSE \
#            file://Makefile \
#            file://README.md \
#            file://hello_world/ \
# "

SRC_URI = "file://crypto_engine/ \
            file://Makefile \
"

S = "${WORKDIR}"
B = "${WORKDIR}/build"

OPTEE_CLIENT_EXPORT = "${STAGING_DIR_HOST}${prefix}"
TEEC_EXPORT = "${STAGING_DIR_HOST}${prefix}"
TA_DEV_KIT_DIR = "${STAGING_INCDIR}/optee/export-user_ta_arm64"

EXTRA_OEMAKE += "TA_DEV_KIT_DIR=${TA_DEV_KIT_DIR} \
                 OPTEE_CLIENT_EXPORT=${OPTEE_CLIENT_EXPORT} \
                 TEEC_EXPORT=${TEEC_EXPORT} \
                 HOST_CROSS_COMPILE=${HOST_PREFIX} \
                 TA_CROSS_COMPILE=${HOST_PREFIX} \
                 -C ${S} OUTPUT_DIR=${B} \
               "

CFLAGS += "--sysroot=${STAGING_DIR_HOST}"

do_compile() {
    oe_runmake
}

do_install () {
    mkdir -p ${D}${nonarch_base_libdir}/optee_armtz
    mkdir -p ${D}${bindir}
    install -D -p -m0755 ${B}/ca/* ${D}${bindir}
    install -D -p -m0444 ${B}/ta/* ${D}${nonarch_base_libdir}/optee_armtz
}

FILES_${PN} += "${nonarch_base_libdir}/optee_armtz/"

# Imports machine specific configs from staging to build
PACKAGE_ARCH = "${MACHINE_ARCH}"