SUMMARY = "bitbake githello"
DESCRIPTION = "githello recipe to test a local git repository"
LICENSE = "CLOSED"

S = "${WORKDIR}"

# PR = "r1"
SRCBRANCH = "githello"
SRC_URI = "git://${BSPDIR}/sources/meta-githello;protocol=file;branch=${SRCBRANCH}"
SRCREV = "d3e627a30e6f85f45d6049231fee930cd433e95a"


do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} githello.c -o githello 
}

do_install() {
    install -d ${D}${bindir} 
    install -m 0755 githello ${D}${bindir} 
}
