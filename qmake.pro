TEMPLATE       = app
CONFIG         = release qt lex yacc uic resources warn_on precompile_header
MAKEFILE       = Makefile

OBJECTS_DIR    = obj
BINARY_DIR     = bin
DESKTOP_DIR    = src/dsk
DEBIAN_DIR     = debian

TARGET         = $$BINARY_DIR/kcheckhash
INCLUDEPATH    = .


kcheckhash.path  = /usr/bin
kcheckhash.files = $$TARGET

checksum_dsk.path  = /usr/share/kde4/services
checksum_dsk.files =  $$DESKTOP_DIR/checksum.desktop

kcheckhash_dsk.path  = /usr/share/applications/kde4
kcheckhash_dsk.files =  $$DESKTOP_DIR/kcheckhash.desktop

INSTALLS += kcheckhash checksum_dsk kcheckhash_dsk


QMAKE_CFLAGS_RELEASE   = -O2 -fdata-sections -ffunction-sections
QMAKE_CXXFLAGS_RELEASE = -O2 -fdata-sections -ffunction-sections
QMAKE_LFLAGS_RELEASE   = -s -Wl,--gc-sections


SOURCES     = src/main.cpp src/main-dlg.cpp src/preferences-dlg.cpp src/checksum.cpp
HEADERS     = src/main-dlg.h src/preferences-dlg.h src/checksum.h
FORMS       = resource/main-dlg.ui resource/preferences-dlg.ui
RESOURCES   = resource/resource.qrc

LIBS        = -lmhash

mkdir.depends  =
mkdir.commands = \
    mkdir -p $$OBJECTS_DIR;\
    mkdir -p $$BINARY_DIR

purge.depends += clean
purge.commands = \
    rm -rf $$OBJECTS_DIR;\
    rm -rf $$BINARY_DIR; \
    rm -rf Debug; \
    rm -rf Release; \
    rm -f  *.mk; \
    rm -f  *.session; \
    rm -f  *.tags; \
    rm -rf $$DEBIAN_DIR/tmp; \
    rm -f  $$DEBIAN_DIR/substvars; \
    rm -f  $$DEBIAN_DIR/debhelper.log; \
    rm -f  $$DEBIAN_DIR/files; \
    rm -f  build-stamp

QMAKE_EXTRA_TARGETS += mkdir purge
PRE_TARGETDEPS       = mkdir
