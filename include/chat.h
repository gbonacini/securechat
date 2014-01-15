#ifndef _CHAT_H_
#define _CHAT_H_

// -----------------------------------------------------------------
// Securechat - A simple chat program over SSL/TLS.
// Copyright (C) 2013  Gabriele Bonacini
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// -----------------------------------------------------------------

#include <config.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <form.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <term.h>
#include <termios.h>
#include <stdarg.h>

//AIX

#ifdef HAVE_STDBOOL_H 
#include <stdbool.h>
#endif

#define C_BACKSPACE 127
#define C_NEWLINE '\n'
#define C_TAB '\t'

#define UP_HEIGHT ( abs(LINES-(LINES / 4 )) - 2)
#define UP_WIDTH (COLS - 2)
#define DOWN_HEIGHT ( abs((LINES / 4 )) - 3 )
#define DOWN_WIDTH (COLS - 2)
#define DOWN_Y (abs(LINES-(LINES / 4 )) + 1)
#define DOWN_X 1
#define UP_Y 1
#define UP_X 1

#define MINLINES 35
#define MINCOLUMNS 95
#define WRONG_SIZE "Screen too small, minimum dimensions are 35 lines and 95 columns."

#define PADDING 1
#define STATUS_OFFSET 5
#define VERSION_OFFSET 15

#define INSERT_FORM_FIELDS 2
#define HISTORY_FORM_FIELDS 2
#define CONFIGURATION_FORM_FIELDS 24
#define SMALL_BUFFER 64
#define MEDIUM_BUFFER 256
#define BIG_BUFFER 2048
#define HUGE_BUFFER 10240

#define STATUS_CONNECTED "STATUS: CONNECTED          "
#define STATUS_DISCONNECTED "STATUS: DISCONNECT/INACTIVE"
#define STATUS_LISTENING "STATUS: LISTENING          "

#define UPPER_MENU "  - F5 Exit - F6 Configure - <Arrows> Scroll the log "

#define ERROR_PROMPT "Error:---> "
#define INFO_PROMPT "Info:---> "
#define LOCAL_PROMPT "Local:---> "
#define REMOTE_PROMPT "Remote:---> "

#define LOGFILE "/tmp/securechat.log"

#define POLLING_INTERVAL 100000

#define BLACKLIST "SCBLACKLIST"
#define DEFAULT_BLACKLIST "!aNULL:!eNULL:!3DES:!SHA1:!EXPORT:!EXPORT56:MEDIUM"

enum status { inactive, connected, listening };

struct chatContext{
	bool serverMode;
	int wrows;
	int wcols;
	int up_height;
	int up_width;
	int localPort;
	int remotePort;
	char* baseDir;
	BIO *biop;
	BIO *abiop;
	BIO *mbiop;
	SSL_CTX *ctxp;
	SSL *sslp;
	WINDOW *up_winp;
	WINDOW *down_winp;
	WINDOW *inner_up_winp;
	WINDOW *inner_down_winp;
	WINDOW *configurationp;
	WINDOW *inner_configurationp;
	FORM  *historyFormp;
	FORM  *configurationFormp;
	FORM  *insertFormp;
	FIELD *historyFieldp[HISTORY_FORM_FIELDS];
	FIELD *insertFieldp[INSERT_FORM_FIELDS];
	FIELD *configurationFieldp[CONFIGURATION_FORM_FIELDS];
	char incomingBufferp[MEDIUM_BUFFER];
	size_t incomingBufferSize;
	const char* statusConnectp;
	size_t statusConnectSize;
	const char* statusDisconnectp;
	size_t statusDisconnectSize;
	const char* statusListeningp;
	size_t statusListeningSize;
	char configIP[SMALL_BUFFER];
	size_t configIPSize;
	char sConfigPort[SMALL_BUFFER];
	size_t sConfigPortSize;
	char errBuffer[MEDIUM_BUFFER];
	size_t errBufferSize;
	char password[MEDIUM_BUFFER];
	size_t passwordSize;
	char handShakeSummary[MEDIUM_BUFFER];
	size_t handShakeSummarySize;
        int labelIpAddress;
        int labelPortAddress;
	int startIPAddress;
	int valuePortAddress;
	int passworHeaderAddress;
	int passwordAddress;
	int passworFooterAddress;
	int passworConfirmHeaderAddress;
	int passwordConfirmAddress;
	int passworConfirmFooterAddress;
	int serverModeAddress;
	int passwdErrorAddress;
	/* Status: Valid values: inactive, connected, listening. */
	enum status status;
	pthread_mutex_t historyFieldMtxp;
	char *blackList;
};

#define CONTEXT_ZERO {  false, /*bool serverMode */\
			0, /* int wrows */\
			0, /* int wcols */\
			0, /* int up_height */\
			0, /* int up_width */\
			0, /* int localPort */\
			0, /* int remotePort */\
			NULL, /* char* baseDir */\
			NULL, /* BIO *biop */\
			NULL, /* BIO *abiop */\
			NULL, /* BIO *mbiop */\
			NULL, /* SSL_CTX *ctxp */\
			NULL, /* SSL *sslp */\
			NULL, /* WINDOW *up_winp */\
			NULL, /* WINDOW *down_winp */\
			NULL, /* WINDOW *inner_up_winp */\
			NULL, /* WINDOW *inner_down_winp */\
			NULL, /* WINDOW *configurationp */\
			NULL, /* WINDOW *inner_configurationp */\
			NULL, /* FORM  *historyFormp */\
			NULL, /* FORM  *configurationFormp */\
			NULL, /* FORM  *insertFormp */\
			{0}, /* FIELD *historyFieldp[HISTORY_FORM_FIELDS] */\
			{0}, /* FIELD *insertFieldp[INSERT_FORM_FIELDS] */\
			{0}, /* FIELD *configurationFieldp[CONFIGURATION_FORM_FIELDS] */\
			{0}, /* char incomingBufferp[MEDIUM_BUFFER] */\
			(size_t)0, /* size_t incomingBufferSize */\
			STATUS_CONNECTED, /* const char* statusConnectp */\
			(size_t)0, /* size_t statusConnectSize */\
			STATUS_DISCONNECTED, /* const char* statusDisconnectp */\
			(size_t)0, /* size_t statusDisconnectSize */\
			STATUS_LISTENING, /* const char* statusListeningp */\
			(size_t)0, /* size_t statusListeningSize */\
			{0}, /* char configIP[SMALL_BUFFER] */\
			(size_t)0, /* size_t configIPSize */\
			{0}, /* char sConfigPort[SMALL_BUFFER] */\
			(size_t)0, /* size_t sConfigPortSize */\
			{0}, /* char errBuffer[MEDIUM_BUFFER] */\
			(size_t)0, /* size_t errBufferSize */\
			{0}, /* char password[MEDIUM_BUFFER] */\
			(size_t)0, /* size_t passwordSize */\
			{0}, /*char handShakeSummary[MEDIUM_BUFFER]*/\
			(size_t)0, /*size_t handShakeSummarySize*/\
			0, /* int labelIpAddress */\
			0, /* int labelPortAddress */\
			0, /* int startIPAddress */\
			0, /* int valuePortAddress */\
			0, /* int passworHeaderAddress */\
			0, /* int passwordAddress */\
			0, /* int passworFooterAddress */\
			0, /* int passworConfirmHeaderAddress */\
			0, /* int passwordConfirmAddress */\
			0, /* int passworConfirmFooterAddress */\
			0, /* int serverModeAddress */\
			0, /* int passwdErrorAddress */\
			inactive, /* enum status status */\
			PTHREAD_MUTEX_INITIALIZER, /* pthread_mutex_t historyFieldMtxp */\
			NULL /* char *blackList;*/\
}

WINDOW *new_win(int height, int width, int starty, int startx);
void free_win(WINDOW *local_win);
void setTerm(void);

bool checkParameters(int argc, char **argv);

void changeStatus(const char* string, size_t size);
void printMenu(const char* string);
void printVersion(const char* string, size_t size);
void printHistory(const char* prompt, const char* errorMessage, struct chatContext *context);

// Threads 
/*@null@*/
void* readIncoming(void* chatContext);
/*@null@*/
void* listenIncoming(void* chatContext);
int password_callback(char *buf, int size, int rwflag, void *chatContext);
/*@null@*/
void* refreshAll(void* chatContext);
void disconnectNotify(struct chatContext *context);

void sendMessage(struct chatContext *context);
void parseInput(struct chatContext *context, int current);
void doBackspace(struct chatContext *context);

void resizeHandler(int sig);

void initContext(struct chatContext *context);
bool setContext(struct chatContext *context);
void cleanContext(struct chatContext *context);
void freeFormContext(struct chatContext *context);

void setWindows(struct chatContext *context);
void showConfigWin(struct chatContext *context);
void setConfigurationForm(struct chatContext *context);
void manageConfigurationModule(struct chatContext *context, pthread_t *listenerId, pthread_attr_t *listenerAttr);
void extractConfiguration(struct chatContext *context, char *configReport);
bool passwordsMatch(struct chatContext *context);
void notifyPasswordError(struct chatContext *context);
void setServerMenu(bool activation, struct chatContext *context);

bool setClientMode(struct chatContext *context);
bool setServerMode(struct chatContext *context);
void moveForward(struct chatContext *context);
void moveBackward(struct chatContext *context);
void trimTrailingSpaces(char *buffer, size_t size);

void setDebug(void);
void writeSslError(const char *errString);
void writeErrorLog(const char *errString, ...);
void endDebug(void);

#endif
