bin_PROGRAMS = manifold_node

AM_CPPFLAGS = -Iinterfaces -I../../kernel/include
MPICXX=mpic++
CXX=g++
CXXFLAGS= -g -Wall
CXX_FLAGS = -D_FILE_OFFSET_BITS=64 -I src -ansi -g -gstabs+ -O0 -static-libgcc -Wall -DNO_MPI
MLIBS  = -lm
CXXLD = $(MPICXX) 
LDFLAGS = -lgsl -lgslcblas
DEBUG_FLAGS = -g -pg -D_STLP_DEBUG 
PROFILE_FLAGS = -fprofile-arcs -ftest-coverage -lgcov -g -pg
FE_SOURCES = \
	     examples/manifold_node_mpi.cc 


SUBDIRS = iris_srcs .
MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure Makefile.in config.log config.status depcomp iris_srcs/Makefile.in

## manifold_node
manifold_node_CXXFLAGS = $(CXX_FLAGS)
manifold_node_CXXLD= $(CXXLD)

manifold_node_SOURCES = $(FE_SOURCES)

manifold_node_LDADD = iris_srcs/libiris.a ../../kernel/lib/manifold/libmanifold.a

