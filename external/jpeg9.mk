all: cdjpeg.o jaricom.o jcapimin.o jcapistd.o jcarith.o jccoefct.o jccolor.o jcdctmgr.o jchuff.o jcinit.o jcmainct.o jcmarker.o jcmaster.o jcomapi.o jcparam.o jcprepct.o jcsample.o jctrans.o jdapimin.o jdapistd.o jdarith.o jdatadst.o jdatasrc.o jdcoefct.o jdcolor.o jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o jdmaster.o jdmerge.o jdpostct.o jdsample.o jdtrans.o jerror.o jfdctflt.o jfdctfst.o jfdctint.o jidctflt.o jidctfst.o jidctint.o jmemansi.o jmemmgr.o jmemnobs.o jquant1.o jquant2.o jutils.o transupp.o
	$(AR) rcs $(PREFIX)/lib/libjpeg.a cdjpeg.o jaricom.o jcapimin.o jcapistd.o jcarith.o jccoefct.o jccolor.o jcdctmgr.o jchuff.o jcinit.o jcmainct.o jcmarker.o jcmaster.o jcomapi.o jcparam.o jcprepct.o jcsample.o jctrans.o jdapimin.o jdapistd.o jdarith.o jdatadst.o jdatasrc.o jdcoefct.o jdcolor.o jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o jdmaster.o jdmerge.o jdpostct.o jdsample.o jdtrans.o jerror.o jfdctflt.o jfdctfst.o jfdctint.o jidctflt.o jidctfst.o jidctint.o jmemansi.o jmemmgr.o jmemnobs.o jquant1.o jquant2.o jutils.o transupp.o

cdjpeg.o: cdjpeg.c
	$(CC) -fPIC $(CFLAGS) -c $<

jaricom.o: jaricom.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcapimin.o: jcapimin.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcapistd.o: jcapistd.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcarith.o: jcarith.c
	$(CC) -fPIC $(CFLAGS) -c $<

jccoefct.o: jccoefct.c
	$(CC) -fPIC $(CFLAGS) -c $<

jccolor.o: jccolor.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcdctmgr.o: jcdctmgr.c
	$(CC) -fPIC $(CFLAGS) -c $<

jchuff.o: jchuff.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcinit.o: jcinit.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcmainct.o: jcmainct.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcmarker.o: jcmarker.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcmaster.o: jcmaster.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcomapi.o: jcomapi.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcparam.o: jcparam.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcprepct.o: jcprepct.c
	$(CC) -fPIC $(CFLAGS) -c $<

jcsample.o: jcsample.c
	$(CC) -fPIC $(CFLAGS) -c $<

jctrans.o: jctrans.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdapimin.o: jdapimin.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdapistd.o: jdapistd.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdarith.o: jdarith.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdatadst.o: jdatadst.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdatasrc.o: jdatasrc.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdcoefct.o: jdcoefct.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdcolor.o: jdcolor.c
	$(CC) -fPIC $(CFLAGS) -c $<

jddctmgr.o: jddctmgr.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdhuff.o: jdhuff.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdinput.o: jdinput.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdmainct.o: jdmainct.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdmarker.o: jdmarker.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdmaster.o: jdmaster.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdmerge.o: jdmerge.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdpostct.o: jdpostct.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdsample.o: jdsample.c
	$(CC) -fPIC $(CFLAGS) -c $<

jdtrans.o: jdtrans.c
	$(CC) -fPIC $(CFLAGS) -c $<

jerror.o: jerror.c
	$(CC) -fPIC $(CFLAGS) -c $<

jfdctflt.o: jfdctflt.c
	$(CC) -fPIC $(CFLAGS) -c $<

jfdctfst.o: jfdctfst.c
	$(CC) -fPIC $(CFLAGS) -c $<

jfdctint.o: jfdctint.c
	$(CC) -fPIC $(CFLAGS) -c $<

jidctflt.o: jidctflt.c
	$(CC) -fPIC $(CFLAGS) -c $<

jidctfst.o: jidctfst.c
	$(CC) -fPIC $(CFLAGS) -c $<

jidctint.o: jidctint.c
	$(CC) -fPIC $(CFLAGS) -c $<

jmemansi.o: jmemansi.c
	$(CC) -fPIC $(CFLAGS) -c $<

jmemmgr.o: jmemmgr.c
	$(CC) -fPIC $(CFLAGS) -c $<

jmemnobs.o: jmemnobs.c
	$(CC) -fPIC $(CFLAGS) -c $<

jquant1.o: jquant1.c
	$(CC) -fPIC $(CFLAGS) -c $<

jquant2.o: jquant2.c
	$(CC) -fPIC $(CFLAGS) -c $<

jutils.o: jutils.c
	$(CC) -fPIC $(CFLAGS) -c $<

transupp.o: transupp.c
	$(CC) -fPIC $(CFLAGS) -c $<
