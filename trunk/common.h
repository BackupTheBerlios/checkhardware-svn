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

//===================================
// Templates...
//-----------------------------------
template <class T>
class XWrapPointer {
    private:
    T* pM;

    public:
    XWrapPointer (T* typ) {
        pM = typ;
    }
    QString operator[] (const char* key) {
        return(QString::fromLocal8Bit(pM->operator[](key)));
    }
};

//====================================
// Prototypes...
//------------------------------------
bool setWarning ( const QString & text, QDict<char> & mText );
void setIntro ( const QDict<char> & mText );
QDict<char> loadText ( bool = false );
void child ( int  );
void usage ( void );

#endif
