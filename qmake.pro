TEMPLATE       = app
CONFIG         = release qt lex yacc uic resources warn_on precompile_header
MAKEFILE       = Makefile

OBJECTS_DIR    = obj
TARGET         = usr/bin/kcheckhash
INCLUDEPATH    = .

QMAKE_CFLAGS_RELEASE   = -O2 -fdata-sections -ffunction-sections
QMAKE_CXXFLAGS_RELEASE = -O2 -fdata-sections -ffunction-sections
QMAKE_LFLAGS_RELEASE   = -s -Wl,--gc-sections
LIBS        = -lmhash

SOURCES     = src/main.cpp src/main-dlg.cpp src/checksum.cpp
HEADERS     = src/main-dlg.h src/checksum.h
FORMS       = resource/main-dlg.ui
RESOURCES   = resource/resource.qrc

MOD   = kcheckhash
MODR  = checksum.desktop
MODL  = kcheckhash.desktop

DEBD  = debian
USRD  = usr/share
BIND  = usr/bin
SERVD = $$USRD/kde4/services
APPD  = $$USRD/applications/kde4

clean.depends += fullclean
fullclean.commands = \
    rm -rf $(OBJECTS_DIR);\
    rm -rf $(DESTDIR); \
    rm -f  *.mk; \
    rm -f  *.session; \
    rm -f  *.tags; \
    rm -rf $$DEBD/tmp; \
    rm -f  $$DEBD/substvars; \
    rm -f  $$DEBD/debhelper.log; \
    rm -f  $$DEBD/files; \
    rm -f  build-stamp

install.depends =
install.commands = \
    $(INSTALL_FILE) $$SERVD/$$MODR /$$SERVD; \
    $(INSTALL_FILE) $$APPD/$$MODL /$$APPD;   \
    $(INSTALL_PROGRAM) $$BIND/$$MOD /$(DESTDIR)

uninstall.depends =
uninstall.commands = \
    $(DEL_FILE) /$$SERVD/$$MODR; \
    $(DEL_FILE) /$$APPD/$$MODL;  \
    $(DEL_FILE) /$(DESTDIR)/$$MOD

QMAKE_EXTRA_TARGETS += clean fullclean install uninstall
