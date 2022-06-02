PRO_NAME:=student
INSTALLDIR:=${DESTDIR}/usr/lib/license/
#PRO_MAK:=./.${PRO_NAME}.mak
ARCH = $(shell arch)

.PHONY: all install rpm clean

all:
	cd ./student && make

install:
	mkdir -p ${DESTDIR}/usr/lib/license
	install -Dm644 ./student/student  ${DESTDIR}/usr/lib/license/
	mkdir -p ${DESTDIR}/usr/include/license/
	install -Dm644 ./student/server.h ${DESTDIR}/usr/include/license/server.h

clean:
	cd ./student && make clean
