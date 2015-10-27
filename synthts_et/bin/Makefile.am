
EXTRA_DIST = Makefile.mak

INCLUDES = -I$(top_srcdir)/include

bin_PROGRAMS = synthts_et

synthts_et_SOURCES = hts_engine.cpp 

synthts_et_LDFLAGS = -static -pthread

synthts_et_LDADD = ../etana/libEtAna.a ../fsc/libFsc.a ../lib/libHTSEngine.a ../lib_utt/libEtLab.a

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in
