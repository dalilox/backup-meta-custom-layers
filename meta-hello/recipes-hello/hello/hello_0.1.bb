SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "CLOSED"

SRC_URI = "file://hello.c"

# SRCBRANCH = "helloYoussef"
# SRC_URI = "git:///home/dalil/yoctoProject/imx-yocto-bsp/sources/meta-hello;protocol=file;branch=${SRCBRANCH}"
# SRCREV = "2279f893f8c77dfc58cc99fc4e8a8948ce77d3bc"

S = "${WORKDIR}"

do_compile() {
	${CC} ${CFLAGS} ${LDFLAGS} hello.c -o hello
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 hello ${D}${bindir}
}

