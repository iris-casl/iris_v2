AM_CPPFLAGS = -I$(top_srcdir)/include/kernel

manifoldlib_LIBRARIES = libmanifold.a
manifoldlibdir = $(libdir)/manifold
libmanifold_a_SOURCES = \
	clock.cc \
	$(top_srcdir)/include/kernel/clock.h \
	$(top_srcdir)/include/kernel/common-defs.h \
	component.cc \
	$(top_srcdir)/include/kernel/component-decl.h \
	$(top_srcdir)/include/kernel/component.h \
	link.cc \
	$(top_srcdir)/include/kernel/link-decl.h \
	$(top_srcdir)/include/kernel/link.h \
	manifold.cc \
	$(top_srcdir)/include/kernel/manifold-decl.h \
	$(top_srcdir)/include/kernel/manifold.h \
	$(top_srcdir)/include/kernel/message.h \
	messenger.cc \
	$(top_srcdir)/include/kernel/messenger.h
manifoldkernelincdir = $(includedir)/manifold/kernel
manifoldkernelinc_HEADERS = \
	$(top_srcdir)/include/kernel/clock.h \
	$(top_srcdir)/include/kernel/common-defs.h \
	$(top_srcdir)/include/kernel/component-decl.h \
	$(top_srcdir)/include/kernel/component.h \
	$(top_srcdir)/include/kernel/link-decl.h \
	$(top_srcdir)/include/kernel/link.h \
	$(top_srcdir)/include/kernel/manifold-decl.h \
	$(top_srcdir)/include/kernel/manifold.h \
	$(top_srcdir)/include/kernel/message.h \
	$(top_srcdir)/include/kernel/messenger.h
