# BSD Makefile for building ngpm service and client \
Copyright (C) 2026 Daniel Sierpiński and contributors

CC             ?= cc
AR             ?= ar

VERSION        != cat .version 2> /dev/null || echo "unknown"
COPYRIGHT      != cat .copy 2> /dev/null || echo "none"
SERVICE_EXEC   := ngpmd
SERVICE_SRCS   := src/[a-z]*.[ch]

CLIENT_EXEC    := ngpm
CLIENT_SRCS    := src/client/main.c

STYLE_SRC      ?= ${.CURDIR}/css/style.css

DESTDIR        ?=
SYSDIR         ?= /usr/local
XDGDIR         ?= /etc/xdg/autostart
DESKTOP_IN     ?= ${.CURDIR}/${SERVICE_EXEC}.desktop.in
DESKTOP_FILE   ?= ${.CURDIR}/${SERVICE_EXEC}.desktop

CFLAGS         := -std=c99 -Og -g -Wall -Wextra
CFLAGS         += -I${.CURDIR}/include

.ifdef WITH_GTK
  GTK_CFLAGS     != pkg-config --cflags gtk+-3.0
  GTK_LIB        != pkg-config --libs gtk+-3.0
  CFLAGS         += -DWITH_GTK
  CFLAGS_DL      := ${GTK_CFLAGS}
  LIBS_DL        := ${GTK_LIB}
.else
  GLIB_CFLAGS    != pkg-config --cflags glib-2.0
  CFLAGS_DL      := ${GLIB_CFLAGS}
  LIBS_DL        != pkg-config --libs glib-2.0
.endif

BATTERY        ?= 0
DELAY_IN_SEC   ?= 180
THRESHOLD      ?= 20
CRIT_THRESHOLD ?= 5
ETC            ?= /etc

CFLAGS         += -DBATTERY=${BATTERY}\
                  -DDELAY_IN_SEC=${DELAY_IN_SEC}\
                  -DTHRESHOLD=${THRESHOLD}\
	          -DCRIT_THRESHOLD=${CRIT_THRESHOLD}\
                  -DETC=${ETC}\
                  -DDESTDIR=${DESTDIR}\
                  -DVERSION=${VERSION}\
                  -DCOPYRIGHT="${COPYRIGHT}"

LIB := lib${CLIENT_EXEC}.a
LIB_SRCS := ngpm.c
LIB_OBJS := ${LIB_SRCS:.c=.o}

SYSD != command -v systemctl >/dev/null 2>&1 && echo 1 || echo 0
.if ${SYSD} == 1
  CFLAGS += -DSYSD
.endif

.PHONY: all dev ncpu run-service help clean install
.PATH.c: src/ngpm

all: ${LIB} ${SERVICE_EXEC} ${CLIENT_EXEC}

${LIB}: ${LIB_OBJS}
	${AR} rcs ${.TARGET} ${.ALLSRC}

${SERVICE_EXEC}: ${SERVICE_SRCS} ${LIB}
	mkdir -p ${.CURDIR}/bin
	${CC} ${CFLAGS} ${CFLAGS_DL} -o ${.CURDIR}/bin/${.TARGET} \
              ${SERVICE_SRCS} ${LIB} ${LIBS_DL}

${CLIENT_EXEC}: ${CLIENT_SRCS} ${LIB}
	mkdir -p ${.CURDIR}/bin
	${CC} ${CFLAGS} -o ${.CURDIR}/bin/${.TARGET} \
              ${CLIENT_SRCS} ${LIB}

${DESKTOP_FILE}: ${DESKTOP_IN}
	sed -e 's|@PREFIX@|${SYSDIR}|g'\
	    -e 's|@SERVICE@|${SERVICE_EXEC}|g'\
	    -e 's|@VERSION@|${VERSION}|g'\
	    ${.ALLSRC} > ${.TARGET}

install: ${DESKTOP_FILE}
	mkdir -p ${DESTDIR}${SYSDIR}/bin
	install -m 755 ${.CURDIR}/bin/${SERVICE_EXEC} ${DESTDIR}${SYSDIR}/bin/
	install -m 755 ${.CURDIR}/bin/${CLIENT_EXEC} ${DESTDIR}${SYSDIR}/bin/
	mkdir -p ${DESTDIR}${ETC}/${CLIENT_EXEC}
	install -m 644 ${STYLE_SRC} ${DESTDIR}${ETC}/${CLIENT_EXEC}/
	mkdir -p ${DESTDIR}${XDGDIR}
	install -m 644 ${DESKTOP_FILE} ${DESTDIR}${XDGDIR}/

clean:
	rm -rf bin obj ${LIB}

help:
	@echo "Build targets:"
	@echo "  make            - build"
	@echo "  make install    - install"
	@echo ""
	@echo "Tunable variables:"
	@echo "  WITH_GTK=1        Enable GTK support"
	@echo "  DELAY_IN_SEC=n    Delay between checks {sec}"
	@echo "  THRESHOLD=n       Warning threshold {%}"
	@echo "  CRIT_THRESHOLD=n  Critical threshold {%}"
	@echo "  DESTDIR=path      Installation root"
	@echo "  XDGDIR=path       .desktop file destination"
	@echo ""
	@echo "To get an idea about most flags: \`bmake -V CFLAGS\`"

# Used for testing and development
run-service:
	${DESTDIR}${SYSDIR}/bin/${SERVICE_EXEC}

NCPU != nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null
dev:
	THRESHOLD=100 DELAY_IN_SEC=5\
	bmake -j ${NCPU}
	DESTDIR=${.CURDIR}/build\
	bmake install
