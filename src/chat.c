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

#include <chat.h>

volatile sig_atomic_t toRefresh;
bool debugOn=false;
int logDescriptor;
FILE *logStream;

int main(int argc, char **argv){
	struct chatContext context=CONTEXT_ZERO;
	struct termios backupTerm;

	bool quit=false;
	int current=0;
	char version[]=VERSION; 

	pthread_t readerId=0;
	pthread_t listenerId=0;
	pthread_t refresherId=0;
	pthread_attr_t readerAttr;
	pthread_attr_t listenerAttr;
	pthread_attr_t refresherAttr;

	if(!checkParameters(argc,argv)){
		exit(EXIT_FAILURE);
	}

	if (tcgetattr(STDIN_FILENO, &backupTerm) == -1){
		perror("Can't save terminal settings.");
		exit(EXIT_FAILURE);
	}

	if(initscr() == NULL){
		perror("Init screen");
		exit(EXIT_FAILURE);
	} 	

	if(LINES<MINLINES || COLS<MINCOLUMNS){
		perror(WRONG_SIZE);
		(void)endwin();			
		exit(EXIT_FAILURE);
	}

	setDebug();

	(void)signal(SIGWINCH, resizeHandler);

	initContext(&context);

	setTerm();

	SSL_load_error_strings();
	ERR_load_BIO_strings();
	ERR_load_SSL_strings();
	OpenSSL_add_all_algorithms();
	(void)SSL_library_init();

	printMenu(UPPER_MENU);
	changeStatus(context.statusDisconnectp, context.statusDisconnectSize);
	printVersion(version,strlen(version));

	setWindows(&context);

	(void)(void)pthread_attr_init(&readerAttr);
	(void)pthread_attr_setdetachstate(&readerAttr, PTHREAD_CREATE_DETACHED);
	(void)pthread_create(&readerId, &readerAttr, &readIncoming, &context);

	(void)pthread_attr_init(&listenerAttr);
	(void)pthread_attr_setdetachstate(&listenerAttr, PTHREAD_CREATE_DETACHED);

	(void)pthread_attr_init(&refresherAttr);
	(void)pthread_attr_setdetachstate(&refresherAttr, PTHREAD_CREATE_DETACHED);
	(void)pthread_create(&refresherId, &refresherAttr, &refreshAll, &context);

	while(!quit){
		current=wgetch(context.down_winp);
		switch(current){
			case KEY_F(5):
				quit=true;
			break;
			case KEY_F(6):
				manageConfigurationModule(&context, &listenerId, &listenerAttr);
			break;
			case C_NEWLINE: 
				sendMessage(&context);
			break;
			case C_BACKSPACE:
			case KEY_BACKSPACE:
				doBackspace(&context);
			break;
			case KEY_LEFT:
				moveBackward(&context);
			break;
			case KEY_RIGHT:
				moveForward(&context);
			break;
			case KEY_UP:
				moveBackward(&context);
			break;
			case KEY_DOWN:
				moveForward(&context);
			break;
			default: 
				parseInput(&context, current);

		}
	}

	(void)pthread_cancel(readerId);
	(void)pthread_cancel(refresherId);
	(void)pthread_cancel(listenerId);

	(void)clear();	
	printMenu("BYE!");	
	(void)sleep(1);

	freeFormContext(&context);

	(void)endwin();			

	disconnectNotify(&context);

	cleanContext(&context);

	(void)tcsetattr(STDIN_FILENO, TCSANOW, &backupTerm);

	endDebug();

	return 0;
}
