/**************
FILE          : CheckHardware.c
***************
PROJECT       : gameChecker
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : Validation program for running 
              : games under Linux
              :
DESCRIPTION   : Check if the sound device /dev/dsp
              : could be opend in NONBLOCK mode
              : if yes everything seems to be ok 
              :
STATUS        : Development
**************/
#include <qapplication.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qfile.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <qdict.h>
#include <locale.h>
#include <libintl.h>
#include <unistd.h>

#include "common.h"

//====================================
// Just do the job :-) ...
//------------------------------------
int main (int argc,char* argv[],char* envp[]) {
	int fd = 0;
	QString device ("/dev/dsp");
	QString test3D ("/usr/bin/3Ddiag-result");
	QString program;
	QDict<char> mText;

	// ...
	// check minimum required number of arguments
	// ---
	if (argc < 2) {
		usage();
	}

	// ...
	// set global base check result to true, may be 
	// overwritten from the check code initialized via
	// check3D / checkSound
	// ---
	bool baseCheck  = true;
	bool check3D    = false;
	bool checkSound = false;

	// ...
	// check for pot template generation
	// ---
	for (int i=0;i<argc;i++) {
	if (QString(argv[i]) == "--makePot") {
		loadText (true);
	}
	if (QString(argv[i]) == "--help") {
		usage();
	}
	}

	// ...
	// init main application object
	// ---
	QApplication app (argc,argv);

	// ...
	// check the commandline paramters. The first parameter
	// not containing the "-" sign is treated as the command
	// name for the later execv() call. All other parameters
	// except --sound and --3D are treated as parameters to this
	// command
	// ---
	int count = 1;
	char* pArgv [qApp->argc()];
	char* baseName = NULL;
	for ( int i=1; i<qApp->argc(); i++ ) {
	if (! QString(qApp->argv()[i]).contains ("-")) {
		if (program.isNull()) {
		program.sprintf("%s",qApp->argv()[i]);
		baseName = (char*) malloc (sizeof(char) * program.length());
		sprintf(baseName,"%s",program.ascii());
		continue;
		} 
	}
	if (QString(qApp->argv()[i]) == "--sound") {
		checkSound = true;
		continue;
	}
	if (QString(qApp->argv()[i]) == "--3D") {
		check3D = true;
		continue;
	}
	pArgv[count] = qApp->argv()[i];
	count++;
	}
	pArgv[0] = baseName;
	pArgv[count] = NULL;

	// ...
	// get the mText hash with the gettext translated
	// information data
	// ---
	mText = loadText();

	// ...
	// first check if we can use the sound device
	// if the device is bussy give a message
	// ---
	if (checkSound) {
	if ((fd=open (device.ascii(),O_RDONLY | O_NONBLOCK)) < 0) {
		QString convert = QString::fromLocal8Bit(mText["SoundText"]);
		QString message;
		QTextOStream (&message) 
			<< convert << "<nobr>" << strerror(errno) << " /dev/dsp</nobr>";
		baseCheck = setWarning ( message,mText );
		if (! baseCheck) {
			exit (1);
		}
	} else {
		close (fd);
	}
	}

	// ...
	// next check for 3D using the magic test3D script
	// which checks if the given command is linked
	// against the libGL and if the openGL system is
	// ready
	// ---
	if (check3D) {
		QString command;
		//freopen("/dev/null","w",stdout);
		bool needWarning = false;
		int code = system (test3D.ascii());
		switch ( WEXITSTATUS (code) ) {
		case 1:
			needWarning = true;
		break;
		default:
			fprintf (stderr,"system() failed: %s",
				strerror(errno)
			);
		break;
		}
		if (needWarning) {
		QString message;
		baseCheck = setWarning ( mText["3DText"],mText );
		if (! baseCheck) {
			exit (1);
		}
		}
	}

	// ...
	// all checks are done, what does 
	// baseCheck tell us ?
	// --
	if (! baseCheck) {
		exit (1);
	}

	// ...
	// ok the checks were successfull and we can
	// call the program now
	// ---
	#if 0
	signal ( SIGCHLD,child );
	switch (fork()) {
	case -1:
		fprintf(stderr,"fork() failed: %s\n",
			strerror(errno)
		);
	break;
	case 0:
		printf("%s %s\n",pArgv[0],pArgv[1]);
		execve(pArgv[0],pArgv,envp);
		fprintf(stderr,"execve() failed: %s\n",
			strerror(errno)
		);
		exit (1);
	}
	exit(0);
	#endif
	execvp(pArgv[0],pArgv);
	fprintf(stderr,"execvp() failed: %s\n",
		strerror(errno)
	);
	exit (1);
}
