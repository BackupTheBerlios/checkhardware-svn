/**************
FILE          : common.c
***************
PROJECT       : gameChecker
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : Validation program for running 
              : games under Linux
              :
DESCRIPTION   : common used function used in Check3D
              : and CheckSound
              :
STATUS        : Development
**************/
#include <qapplication.h>
#include <qmessagebox.h>
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

#include "common.h"

//====================================
// setWarning [ QT message box ]
//------------------------------------
bool setWarning ( const QString & text, const QDict<char> & mText ) {
	QMessageBox mb (
		mText["Warning"],text,QMessageBox::Warning,
		QMessageBox::Ok | QMessageBox::Default,
		QMessageBox::Cancel,
		0,0
	);
	mb.setButtonText (
		QMessageBox::Ok, mText["Ok"]
	);
	mb.setButtonText (
		QMessageBox::Cancel,mText["Cancel"]
	);
	switch (mb.exec()) {
	case QMessageBox::Ok:
		return (true);
	break;
	case QMessageBox::Cancel:
		return (false);
	break;
	}
	return (true);
}

//=====================================
// text loader...
//-------------------------------------
QDict<char> loadText ( bool translate ) {
	setlocale (LC_MESSAGES, "");
	setlocale (LC_CTYPE,"");
	bindtextdomain ("CheckHardware", TDOMAIN);
	textdomain ("CheckHardware");
	QDict<char> mText;
	QDict<char> keyList;

	// ...
	// insert gettext keys as symbolic hash keys
	// ---
	keyList.insert ("Warning",
		"Warning..."
	);
	keyList.insert ("Ok",
		"Ok"
	);
	keyList.insert ("Cancel",
		"Cancel"
	);
	keyList.insert ("SoundText",
		"<b>Failed to init sound device:</b><br><hr><br>"
	);
	keyList.insert ("3DText",
		"<b>Missing Requirements:</b><br><hr><br><nobr>3D hardware acceleration not available</nobr>"
	);

	QDictIterator<char> it (keyList);
	for (; it.current(); ++it) {
	mText.insert (
		it.currentKey(),gettext((char*)it.current())
	);
	//printf("%s\n",mText[it.currentKey()]);
	if (translate) {
		printf ("// internal key used: %s\n",it.currentKey().ascii());
		printf ("printf(\"%cs\",gettext(\"%s\"))\n",'%',it.current());
	}
	}
	if (translate) {
		exit (0);
	}
	return(mText);
}

//====================================
// wait for process termination ...
//------------------------------------
void child (int s) {
	union wait wstatus;
	while(wait3(&wstatus,WNOHANG,NULL) > 0);
}

//====================================
// usage [ help message ]
//------------------------------------
void usage (void) {
	printf("Usage: CheckHardware [OPTION] <command> [command options]\n\n");
	printf("Available options:\n\n");
	printf("  --sound    check sound\n");
	printf("  --3D       check 3D\n");
	exit(1);
}
