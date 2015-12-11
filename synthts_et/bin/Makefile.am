
#EXTRA_DIST = Makefile.mak

#INCLUDES = -I$(top_srcdir)/include

bin_PROGRAMS = synthts_et

synthts_et_SOURCES = synthts_et.cpp 

synthts_et_LDFLAGS = -pthread

synthts_et_LDADD = ../lib/etana/libEtAna.a ../lib/fsc/libFsc.a ../lib/engine/libHTSEngine.a ../lib/label/libEtLab.a

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in
