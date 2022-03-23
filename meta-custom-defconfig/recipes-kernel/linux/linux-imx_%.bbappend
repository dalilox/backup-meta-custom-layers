SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"

FILESEXTRAPATHS_prepend :="${THISDIR}/${PN}:"

SRC_URI += "file://custom_defconfig \
	     file://.config \
"


#DELTA_KERNEL_DEFCONFIG = "pci.cfg"


do_copy_defconfig_append () {
        cp ${WORKDIR}/custom_defconfig ${WORKDIR}/defconfig         
        cp ${WORKDIR}/.config ${WORKDIR}/build/.config
}

#do_configure_append() {

#	cat ../*.cfg >> ${B}/.config

#}
