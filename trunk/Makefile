LOCALEVZ   = $(DESTDIR)/usr/share/locale
LC         = LC_MESSAGES
LD_FLAGS   = -L/usr/lib/qt3/lib


all: CheckHardware Translate

CheckHardware: verify.o common.o
	g++ -Wall -O2 -o CheckHardware verify.o common.o \
		 -lqt-mt -lutil $(LD_FLAGS)

common.o: common.c common.h
	g++ -Wall -O2 -c common.c -I/usr/lib/qt3/include

verify.o: verify.c
	g++ -Wall -O2 -c verify.c -I/usr/lib/qt3/include

Translate: ./locale/CheckHardware.pot

./locale/CheckHardware.pot:
	${MAKE} -C ./locale all

install: all
	mkdir -p $(DESTDIR)/usr/bin
	install -m 755 CheckHardware   $(DESTDIR)/usr/bin
	install -m 755 Check4SoundLibs $(DESTDIR)/usr/bin
	install -m 755 CheckFor3DLibs  $(DESTDIR)/usr/bin
	install -m 755 GameWrapper     $(DESTDIR)/usr/bin

	# install NLS support...
	# -----------------------
	for i in `ls -l1 ./locale | grep -v CVS`; do \
		if [ -d ./locale/$$i ];then \
			install -d -m 755 ${LOCALEVZ}/$$i/${LC} ;\
			install -m 644 ./locale/$$i/${LC}/CheckHardware.mo \
			${LOCALEVZ}/$$i/${LC} ;\
		fi \
	done

clean:
	rm -f *.o
	rm -f CheckHardware
	${MAKE} -C ./locale clean
