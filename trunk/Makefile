all: CheckHardware

CheckHardware: verify.o common.o
	g++ -Wall -O2 -o CheckHardware verify.o common.o \
		 -lqt-mt -L/usr/lib/qt3/lib

common.o: common.c common.h
	g++ -Wall -O2 -c common.c -I/usr/lib/qt3/include

verify.o: verify.c
	g++ -Wall -O2 -c verify.c -I/usr/lib/qt3/include

install: all
	mkdir -p $(DESTDIR)/usr/bin
	mkdir -p $(DESTDIR)/usr/share/locale/de/LC_MESSAGES/
	mkdir -p $(DESTDIR)/usr/share/locale/en/LC_MESSAGES/
	install -m 755 CheckHardware   $(DESTDIR)/usr/bin
	install -m 755 Check4SoundLibs $(DESTDIR)/usr/bin
	install -m 755 CheckFor3DLibs  $(DESTDIR)/usr/bin
	install -m 755 GameWrapper     $(DESTDIR)/usr/bin
	msgfmt locale/de/CheckHardware.po -o $(DESTDIR)/usr/share/locale/de/LC_MESSAGES/CheckHardware.mo
	msgfmt locale/en/CheckHardware.po -o $(DESTDIR)/usr/share/locale/en/LC_MESSAGES/CheckHardware.mo

clean:
	rm -f *.o
	rm -f CheckHardware

pot:
	./CheckHardware --makePot | xgettext -L C - -c -o - | \
		grep -v "^#:" | sed -e s"@^#.@#:@"
