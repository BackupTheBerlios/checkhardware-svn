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
#include <qprogressdialog.h>
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
// setIntro [ QT message ]
//------------------------------------
void setIntro ( const QDict<char> & mText ) {
	sleep (1);
}

//====================================
// setWarning [ QT message box ]
//------------------------------------
bool setWarning ( const QString & text, QDict<char> & mTextPtr ) {
	XWrapPointer< QDict<char> > mText (&mTextPtr);
	QMessageBox mb (
		mText["Warning"],text,QMessageBox::Warning,
		QMessageBox::Ok,
		QMessageBox::Cancel | QMessageBox::Default,
		0,0
	);
	QString button;
	QTextOStream(&button) << "  " << mText["Ok"] << " ";
	mb.setButtonText (
		QMessageBox::Ok, button
	);
	button = "";
	QTextOStream(&button) << "  " << mText["Cancel"] << " ";
	mb.setButtonText (
		QMessageBox::Cancel, button
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
		"Continue"
	);
	keyList.insert ("Cancel",
		"Abort"
	);
	keyList.insert ("SoundText",
		"<b>Direct access to sound device not possible</b><br><hr><br>No soundcard available or sound support not configured yet. However application uses respectively requires sound support."
	);
	keyList.insert ("3DText",
		"<b>3D hardware support not available</b><br><hr><br>No 3D graphics card available or 3D support not configured yet. However application uses respectively requires 3D hardware support."
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
