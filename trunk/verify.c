/**************
FILE          : CheckHardware.c
***************
PROJECT       : gameChecker
              :
AUTHOR        : Marcus Sch?r <ms@suse.de>
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
#include <qregexp.h>
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
extern "C" {
#include <GL/glx.h>
}

#include "common.h"

//====================================
// Prototypes ...
//------------------------------------
bool hasDirectRendering (int);

//====================================
// Just do the job :-) ...
//------------------------------------
int main (int argc,char* argv[],char* envp[]) {
	int fd = 0;
	QString device ("/dev/dsp");
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
	}

	// ...
	// init main application object
	// ---
	QApplication app (argc,argv);
	bool needWarning = false;

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
	QRegExp option ("^-");
	for ( int i=1; i<qApp->argc(); i++ ) {
	int position = option.search (qApp->argv()[i],0);
	if (position < 0) { 
		if (program.isNull()) {
		program.sprintf("%s",qApp->argv()[i]);
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
	// ...
	// only take other options than "--sound" and "--3D" as command
	// option if it follows the command
	// CheckHardware --sound /bin/ls -l                     --> ok
	// CheckHardware --sound -l /bin/ls                     --> invalid
	// ---
	if (! program.isNull()) {
		pArgv[count] = qApp->argv()[i];
		count++;
	} else {
		usage();
	}
	}

	// ...
	// Only try to fork command if command is set
	// ---
	if (program.isNull()) {
		usage();
	}
	baseName = (char*) malloc (sizeof(char) * program.length() + 1);
	sprintf(baseName,"%s",program.latin1());
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
		needWarning = true;
		baseCheck = setWarning ( "SoundText",mText );
		if (! baseCheck) {
			exit (1);
		}
	} else {
		needWarning = false;
		close (fd);
	}
	}

	// ...
	// next check for 3D using the GLX extension
	// to ensure direct rendering is supported
	// ---
	if (check3D) {
	if (! hasDirectRendering(0)) {
		needWarning = true;
		baseCheck = setWarning ( "3DText",mText );
		if (! baseCheck) {
			exit (1);
		}
	} else {
		needWarning = false;
	}
	}

	// ...
	// all checks are done, what does 
	// baseCheck tell us ?
	// --
	if (! baseCheck) {
		exit (1);
	}
	if (! needWarning) {
		setIntro ( mText );
	}
	app.exit();

	// ...
	// ok the checks were successfull and we can
	// call the program now. At the time we will
	// replace the current process with the new
	// one without preforking the current process
	// ---
	setsid();
	execvp ( pArgv[0],pArgv );
	fprintf ( stderr,"execvp() failed: %s\n",
		strerror(errno)
	);
	exit (1);
}

//=====================================
// check for render extension
//-------------------------------------
bool hasDirectRendering (int screen) {
	Display *dpy = QApplication::desktop()->x11Display();
	int attribSingle[] = {
		GLX_RGBA,
		GLX_RED_SIZE,   1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE,  1,
		None
	};
	XVisualInfo* visinfo = glXChooseVisual (
		dpy, screen, attribSingle
	);
	if (visinfo) {
		GLXContext ctx = glXCreateContext ( dpy, visinfo, NULL, True );
		if (glXIsDirect(dpy, ctx)) {
			glXDestroyContext (dpy,ctx);
			return true;
		}
		glXDestroyContext (dpy,ctx);
		return false;
	} else {
		return false;
	}
}

