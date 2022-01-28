SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "CLOSED"

SRC_URI = "file://0001-First-patch.patch;striplevel=3"

S = "${WORKDIR}"

do_compile() {
	${CC} ${CFLAGS} ${LDFLAGS} hellopatch.c -o hellopatch
	${CC} ${CFLAGS} ${LDFLAGS} test2.c -o test2
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 hellopatch ${D}${bindir}
	install -m 0755 test2 ${D}${bindir}
}
