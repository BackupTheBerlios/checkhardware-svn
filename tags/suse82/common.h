/**************
FILE          : common.h
***************
PROJECT       : gameChecker
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : Validation program for running 
              : games under Linux
              :
DESCRIPTION   : header file for common.c
              :
STATUS        : Development
**************/
#ifndef COMMON_H
#define COMMON_H 1

#include <stdio.h>
#include <qdict.h>

//====================================
// Defines...
//------------------------------------
#define TDOMAIN  "/usr/share/locale"

//====================================
// Prototypes...
//------------------------------------
bool setWarning ( const QString & text, const QDict<char> & mText );
void setIntro ( const QDict<char> & mText );
QDict<char> loadText ( bool = false );
void child ( int  );
void usage ( void );

#endif
