all: CheckHardware

CheckHardware: verify.o common.o
	g++ -Wall -O2 -o CheckHardware verify.o common.o \
		 -lqt-mt -L/usr/lib/qt3/lib

common.o: common.c common.h
	g++ -Wall -O2 -c common.c -I/usr/lib/qt3/include

verify.o: verify.c
	g++ -Wall -O2 -c verify.c -I/usr/lib/qt3/include

clean:
	rm -f *.o
	rm -f CheckHardware

pot:
	./CheckHardware --makePot | xgettext -L C - -c -o - | \
		grep -v "^#:" | sed -e s"@^#.@#:@"
