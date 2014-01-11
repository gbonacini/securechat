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

extern bool debugOn;
extern int logDescriptor;
extern FILE *logStream;
extern volatile sig_atomic_t toRefresh;

void resizeHandler(int sig){
	toRefresh=(sig_atomic_t)sig;
}

WINDOW *new_win(int height, int width, int starty, int startx){	
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	(void)box(local_win, 0 , 0);
	(void)wrefresh(local_win);

	return local_win;
}

void free_win(WINDOW *local_win){	
	(void)wborder(local_win, (chtype)' ', (chtype)' ', (chtype)' ', (chtype)' ', (chtype)' ', (chtype)' ', (chtype)' ', (chtype)' ');
	(void)wrefresh(local_win);
	(void)delwin(local_win);
}

void setTerm(void){
	bool keypadOn=true;
        (void)start_color();
        (void)cbreak();                 
        (void)noecho();                 
        (void)keypad(stdscr, keypadOn); 

        (void)init_pair(1, COLOR_CYAN, COLOR_BLACK);
        (void)init_pair(2, COLOR_GREEN, COLOR_BLACK);
        (void)init_pair(3, COLOR_BLACK, COLOR_CYAN);
        (void)init_pair(4, COLOR_BLACK, COLOR_RED);
}

void changeStatus(const char* string, size_t size){
	(void)attron(COLOR_PAIR(1) | A_BOLD | A_REVERSE);
	(void)mvprintw(0,(COLS-(int)size-STATUS_OFFSET),string);
	(void)refresh();
	(void)attroff(COLOR_PAIR(1) | A_BOLD | A_REVERSE);
}

void printMenu(const char* string){
	(void)attron(COLOR_PAIR(1) | A_BOLD);
	(void)mvprintw(0,0,string);
	(void)refresh();
	(void)attroff(COLOR_PAIR(1) | A_BOLD);
}

void printVersion(const char* string, size_t size){
	(void)attron(COLOR_PAIR(1) | A_BOLD);
	(void)mvprintw(LINES-1,(COLS-(int)size-VERSION_OFFSET),"Version: %s",string);
	(void)refresh();
	(void)attroff(COLOR_PAIR(1) | A_BOLD);
}

void printHistory(const char* prompt, const char* errorMessage, struct chatContext *context){
	int idxSent=0, ind=0;

	(void)pthread_mutex_lock(&(context->historyFieldMtxp));
	(void)form_driver(context->historyFormp, REQ_END_FIELD);
	(void)form_driver(context->historyFormp, REQ_NEW_LINE);
	(void)form_driver(context->historyFormp, REQ_NEW_LINE);

	while(prompt[ind]!=(char)0){
		(void)form_driver(context->historyFormp, (int)prompt[ind]);
		ind++;
	}

	while(errorMessage[idxSent]!=(char)0){
		(void)form_driver(context->historyFormp, (int)errorMessage[idxSent]);
		idxSent++;
	}

	(void)wrefresh(context->up_winp);
	(void)form_driver(context->historyFormp, REQ_END_FIELD);
	(void)pthread_mutex_unlock(&(context->historyFieldMtxp));
}

void setConfigurationForm(struct chatContext *context){
	int ind=0;

	context->configurationFieldp[ind] = new_field(1, 15, 2, 2, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "Remote IP   : [");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);
        context->labelIpAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 3, 2, 17, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_RIGHT); 
	(void)set_field_type(context->configurationFieldp[ind], TYPE_INTEGER, PADDING, 0, 255);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "192"); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)field_opts_off(context->configurationFieldp[ind], (Field_Options)O_AUTOSKIP);
        context->startIPAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 1, 2, 20, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, ".");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 3, 2, 21, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_RIGHT); 
	(void)set_field_type(context->configurationFieldp[ind], TYPE_INTEGER, PADDING, 0, 255);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "168"); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)field_opts_off(context->configurationFieldp[ind], (Field_Options)O_AUTOSKIP);

	context->configurationFieldp[++ind] = new_field(1, 1, 2, 24, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, ".");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 3, 2, 25, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_RIGHT); 
	(void)set_field_type(context->configurationFieldp[ind], TYPE_INTEGER, PADDING, 0, 255);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "0"); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)field_opts_off(context->configurationFieldp[ind], (Field_Options)O_AUTOSKIP);

	context->configurationFieldp[++ind] = new_field(1, 1, 2, 28, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, ".");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 3, 2, 29, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_RIGHT); 
	(void)set_field_type(context->configurationFieldp[ind], TYPE_INTEGER, PADDING, 0, 255);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "2"); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)field_opts_off(context->configurationFieldp[ind], (Field_Options)O_AUTOSKIP);

	context->configurationFieldp[++ind] = new_field(1, 1, 2, 32, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "]");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 15, 3, 2, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "Remote Port : [");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);
        context->labelPortAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 5, 3, 17, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_RIGHT); 
	(void)set_field_type(context->configurationFieldp[ind], TYPE_INTEGER, PADDING, 0, 65535);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "4422"); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)field_opts_off(context->configurationFieldp[ind], (Field_Options)O_AUTOSKIP);
        context->valuePortAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 1, 3, 22, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "]");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 15, 4, 2, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "Password    : [");
	(void)set_field_opts(context->configurationFieldp[ind],\
		 field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
        context->passworHeaderAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 26, 4, 17, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_LEFT); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)set_field_opts(context->configurationFieldp[ind],\
		field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE &\
		~(Field_Options)O_AUTOSKIP & ~(Field_Options)O_PUBLIC);
        context->passwordAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 1, 4, 43, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "]");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) &\
		~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
        context->passworFooterAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 15, 5, 2, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "Re-Password : [");
	(void)set_field_opts(context->configurationFieldp[ind],\
		 field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
        context->passworConfirmHeaderAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 26, 5, 17, 0, 0);
	(void)set_field_just(context->configurationFieldp[ind], JUSTIFY_LEFT); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)set_field_opts(context->configurationFieldp[ind], \
		field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE &\
		~(Field_Options)O_VISIBLE & ~(Field_Options)O_AUTOSKIP & ~(Field_Options)O_PUBLIC);
        context->passwordConfirmAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 1, 5, 43, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "]");
	(void)set_field_opts(context->configurationFieldp[ind],\
		field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
        context->passworConfirmFooterAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 15, 6, 2, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "Server Mode : [");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 1, 6, 17, 0, 0);
	(void)set_field_type(context->configurationFieldp[ind], TYPE_REGEXP, "^[xX ]{1,1}$"); 
	(void)set_field_buffer(context->configurationFieldp[ind], 0, " "); 
	(void)set_field_userptr(context->configurationFieldp[ind], NULL);
	(void)field_opts_off(context->configurationFieldp[ind], (Field_Options)O_AUTOSKIP);
        context->serverModeAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 1, 6, 18, 0, 0);
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "]");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[++ind] = new_field(1, 80, 8, 2, 0, 0);
	(void)set_field_fore(context->configurationFieldp[ind],(chtype)COLOR_PAIR(4));
	(void)set_field_buffer(context->configurationFieldp[ind], 0, "ERROR: The passwords does not match.");
	(void)set_field_opts(context->configurationFieldp[ind],\
		field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
	context->passwdErrorAddress=ind;

	context->configurationFieldp[++ind] = new_field(1, 80, ((context->up_height)-8), 2, 0, 0);
	(void)set_field_fore(context->configurationFieldp[ind],(chtype)COLOR_PAIR(2));
	(void)set_field_buffer(context->configurationFieldp[ind], 0, " <F6> Commit - <Arrows> Select/Confirm Field - <X|SPACE> Set/Unset Server Mode");
	(void)set_field_opts(context->configurationFieldp[ind], field_opts(context->configurationFieldp[ind]) & ~(Field_Options)O_ACTIVE);

	context->configurationFieldp[CONFIGURATION_FORM_FIELDS-1] = NULL;
}

void setServerMenu(bool activation, struct chatContext *context){
	if(activation){
		(void)set_field_buffer(context->configurationFieldp[context->serverModeAddress], 0, "X");
		(void)set_field_buffer(context->configurationFieldp[context->labelIpAddress], 0, "Local IP    : [");
		(void)set_field_buffer(context->configurationFieldp[context->labelPortAddress], 0, "Local Port  : [");
		(void)field_opts_on(context->configurationFieldp[context->passworHeaderAddress], (Field_Options)O_VISIBLE);
		(void)field_opts_on(context->configurationFieldp[context->passworFooterAddress], (Field_Options)O_VISIBLE);
		(void)set_field_opts(context->configurationFieldp[context->passwordAddress], \
			field_opts(context->configurationFieldp[context->passwordAddress]) \
			| (Field_Options)O_ACTIVE | (Field_Options)O_VISIBLE);
		(void)set_field_opts(context->configurationFieldp[context->passwordConfirmAddress], \
			field_opts(context->configurationFieldp[context->passwordConfirmAddress]) \
			| (Field_Options)O_ACTIVE | (Field_Options)O_VISIBLE);
		(void)field_opts_on(context->configurationFieldp[context->passworConfirmHeaderAddress], (Field_Options)O_VISIBLE);
		(void)field_opts_on(context->configurationFieldp[context->passworConfirmFooterAddress], (Field_Options)O_VISIBLE);
	}else{
		(void)set_field_buffer(context->configurationFieldp[context->serverModeAddress], 0, " ");
		(void)set_field_buffer(context->configurationFieldp[context->labelIpAddress], 0, "Remote IP   : [");
		(void)set_field_buffer(context->configurationFieldp[context->labelPortAddress], 0, "Remote Port : [");
		(void)field_opts_off(context->configurationFieldp[context->passworHeaderAddress], (Field_Options)O_VISIBLE);
		(void)field_opts_off(context->configurationFieldp[context->passworFooterAddress], (Field_Options)O_VISIBLE);
		(void)set_field_opts(context->configurationFieldp[context->passwordAddress], \
			field_opts(context->configurationFieldp[context->passwordAddress]) \
			& ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
		(void)set_field_opts(context->configurationFieldp[context->passwordConfirmAddress], \
			field_opts(context->configurationFieldp[context->passwordConfirmAddress]) \
			& ~(Field_Options)O_ACTIVE & ~(Field_Options)O_VISIBLE);
		(void)field_opts_off(context->configurationFieldp[context->passworConfirmHeaderAddress], (Field_Options)O_VISIBLE);
		(void)field_opts_off(context->configurationFieldp[context->passworConfirmFooterAddress], (Field_Options)O_VISIBLE);
	}
}

void extractConfiguration(struct chatContext *context, char *configReport){

	char tmpBuffer[SMALL_BUFFER];
	memset(tmpBuffer, 0,sizeof(tmpBuffer));

	(void)memset(context->configIP, 0, context->configIPSize);
	(void)memset(context->sConfigPort, 0, context->sConfigPortSize);
	(void)memset(configReport, 0, MEDIUM_BUFFER);

	(void)strncpy(tmpBuffer, (const char *)field_buffer(context->configurationFieldp[context->startIPAddress], 0), 3);	
	(void)strtok(tmpBuffer, " ");
	(void)strncat(context->configIP, (const char *)tmpBuffer, 3);
	(void)memset(tmpBuffer, 0, sizeof(tmpBuffer));
	(void)strcat(context->configIP, ".");

	(void)strncpy(tmpBuffer, (const char *)field_buffer(context->configurationFieldp[context->startIPAddress+2], 0), 3);
	(void)strtok(tmpBuffer, " ");
	(void)strncat(context->configIP, (const char *)tmpBuffer, 3);
	(void)memset(tmpBuffer, 0, sizeof(tmpBuffer));
	(void)strcat(context->configIP, ".");

	(void)strncpy(tmpBuffer, (const char *)field_buffer(context->configurationFieldp[context->startIPAddress+4], 0), 3);
	(void)strtok(tmpBuffer, " ");
	(void)strncat(context->configIP, (const char *)tmpBuffer, 3);
	(void)memset(tmpBuffer, 0, sizeof(tmpBuffer));
	(void)strcat(context->configIP, ".");

	(void)strncpy(tmpBuffer, (const char *)field_buffer(context->configurationFieldp[context->startIPAddress+6], 0), 3);
	(void)strtok(tmpBuffer, " ");
	(void)strncat(context->configIP, (const char *)tmpBuffer, 3);
	(void)memset(tmpBuffer, 0, sizeof(tmpBuffer));

	(void)strncat(tmpBuffer, (const char *)field_buffer(context->configurationFieldp[context->valuePortAddress], 0), 5);
	(void)strtok(tmpBuffer, " ");
	(void)strncat(context->sConfigPort, (const char *)tmpBuffer, 5);
	(void)memset(tmpBuffer, 0, sizeof(tmpBuffer));
	context->remotePort=atoi((const char *)field_buffer(context->configurationFieldp[context->valuePortAddress], 0));

	(void)strncpy(tmpBuffer, (const char *)field_buffer(context->configurationFieldp[context->passwordAddress], 0), 26);
	(void)strtok(tmpBuffer, " ");
	(void)strncat(context->password, (const char *)tmpBuffer, 26);
	context->passwordSize=strlen(context->password);
	(void)memset(tmpBuffer, 0, sizeof(tmpBuffer));

	context->blackList=getenv(BLACKLIST);
	if(context->blackList==NULL)
		context->blackList=DEFAULT_BLACKLIST;

	(void)snprintf(configReport, MEDIUM_BUFFER-1, "Config:-->  Address: %s:%s - Blacklist: %s",\
			context->configIP, context->sConfigPort, context->blackList);
}

void manageConfigurationModule(struct chatContext *context, pthread_t *listenerId, pthread_attr_t *listenerAttr){
	char configReport[MEDIUM_BUFFER];
	memset(configReport, 0, MEDIUM_BUFFER);

	if(context->configurationp==NULL){
		showConfigWin(context);
	}else{
		if(passwordsMatch(context)){
			extractConfiguration(context, configReport);
			if(context->status==inactive && context->serverMode==false){
				if(setClientMode(context)){
					changeStatus(context->statusConnectp,context->statusConnectSize);
				}
			}else if(context->status==inactive && context->serverMode){
				if(setServerMode(context)){
					changeStatus(context->statusListeningp,context->statusListeningSize);
					(void)pthread_create(listenerId, listenerAttr, &listenIncoming, context);
				}
			}else{
				printHistory(ERROR_PROMPT, "Already Connected.", context);
			}

			printHistory(INFO_PROMPT, configReport, context);

			(void)unpost_form(context->configurationFormp);
			(void)free_win(context->configurationp);
			(void)free_win(context->inner_configurationp);
			context->configurationp=NULL;
			context->inner_configurationp=NULL;
			(void)form_driver(context->historyFormp, REQ_INS_CHAR);
			(void)set_current_field(context->insertFormp, context->insertFieldp[0]);
			(void)wrefresh(context->up_winp);
		}else{
			notifyPasswordError(context);
		}
	}
}

bool passwordsMatch(struct chatContext *context){
	bool ret=true;

	if(context->serverMode){
		if(strcmp(field_buffer(context->configurationFieldp[context->passwordAddress],0),\
		   field_buffer(context->configurationFieldp[context->passwordConfirmAddress],0))!=0){
			
			ret=false;
		}
	}

	return ret;
}

void notifyPasswordError(struct chatContext *context){
	(void)field_opts_on(context->configurationFieldp[context->passwdErrorAddress], (Field_Options)O_VISIBLE);
	(void)wrefresh(context->configurationp);
	(void)wrefresh(context->inner_configurationp);
	(void)field_opts_off(context->configurationFieldp[context->passwdErrorAddress], (Field_Options)O_VISIBLE);
}

/*@null@*/ 
void* refreshAll(void* chatContext){
	struct chatContext *context=(struct chatContext *)chatContext;
	char buffer[MEDIUM_BUFFER];
	memset(buffer, 0, sizeof(buffer));
	# ifndef S_SPLINT_S
	struct winsize ts;
	# endif

	while(true){
		if(toRefresh!=0){
			(void)ioctl(0, TIOCGWINSZ, &ts);
			memset(buffer,0,sizeof(buffer));
			(void)snprintf(buffer, MEDIUM_BUFFER-1, " - signal %i - Lines: %i Columns: %i",(int)toRefresh, ts.ws_row, ts.ws_col);
			writeErrorLog("Refreshed Windows", buffer, NULL);
			if(ts.ws_row>=context->wrows && ts.ws_col>=context->wcols){
				// Clear Terminal 
				(void)setupterm((char *) NULL, STDOUT_FILENO, (int *) NULL);
				(void)tputs(clear_screen, ts.ws_row, putchar);

				(void)wclear(context->inner_down_winp);
				(void)wclear(context->inner_up_winp);
				(void)refresh();
				(void)wrefresh(context->up_winp);
				(void)wrefresh(context->down_winp);
				(void)wrefresh(context->inner_up_winp);
				(void)wrefresh(context->inner_down_winp);
				(void)form_driver(context->historyFormp, REQ_END_FIELD);
				toRefresh=0;
			}else{
				fprintf(stderr, WRONG_SIZE);
				fprintf(stderr, "\n");
			}
		}

		(void)usleep(POLLING_INTERVAL);
	}

	return NULL; 
}

void disconnectNotify(struct chatContext *context){
	if(context->biop!=NULL){
		(void)BIO_flush(context->biop);
		(void)BIO_reset(context->biop);
	}
}

void sendMessage(struct chatContext *context){
	char* sentMessage=NULL;

	if(context->configurationp==NULL && context->status==connected){
		(void)form_driver(context->insertFormp, REQ_END_FIELD);

		sentMessage=field_buffer(context->insertFieldp[0],0);

		trimTrailingSpaces(sentMessage, strlen(sentMessage));
		(void)BIO_write(context->biop, sentMessage, (int)strlen(sentMessage));
		(void)BIO_flush(context->biop);

		printHistory(LOCAL_PROMPT, sentMessage, context);

		(void)form_driver(context->insertFormp, REQ_CLR_FIELD);
	}else if(context->configurationp==NULL && context->status!=connected){
		printHistory(ERROR_PROMPT, "Unconnected !", context);
	}
}

/*@null@*/ 
void* readIncoming(void* chatContext){
	struct chatContext *context=(struct chatContext *)chatContext;
	size_t incomingSize=0;

	while (true){
		(void)usleep(POLLING_INTERVAL);
		if(context->status==connected){
			memset(context->incomingBufferp, 0, (context->incomingBufferSize));
			incomingSize = BIO_read(context->biop, context->incomingBufferp, ((int)context->incomingBufferSize-1));
			(context->incomingBufferp)[incomingSize] = (char)0;
			if(incomingSize > 0){
				printHistory(REMOTE_PROMPT, context->incomingBufferp, context);
			}else if(incomingSize==0){
				printHistory(ERROR_PROMPT, "Client sent eof.", context);

                                if(context->biop!=NULL)(void)BIO_reset(context->biop);
                                if(context->abiop!=NULL)(void)BIO_reset(context->abiop);
                                if(context->mbiop!=NULL)(void)BIO_reset(context->mbiop);
                                context->status=inactive;
                                (void)attron(COLOR_PAIR(1) | A_BOLD | A_REVERSE);
                                (void)mvprintw(0,(context->wcols)-((int)context->statusDisconnectSize)-5,context->statusDisconnectp);
                                (void)refresh();
                                (void)attroff(COLOR_PAIR(1) | A_BOLD | A_REVERSE);
                        } 
		}

	}

	return NULL; 
}

/*@null@*/ 
void* listenIncoming(void* chatContext){
	struct chatContext *context=(struct chatContext *)chatContext;
	SSL *tempSsl=NULL;
	SSL_CIPHER *cipher=NULL;
	char buffer[MEDIUM_BUFFER];
	int bits=0;

	memset(buffer, 0, sizeof(buffer));

	while(true){
		if(BIO_do_accept(context->abiop) <= 0){
			ERR_print_errors_fp(logStream);
			printHistory(ERROR_PROMPT, "Listener accept error !", context);
			cleanContext(context);
		}else{
			context->biop = BIO_pop(context->abiop);

			if(BIO_do_handshake(context->biop) <= 0){
				cleanContext(context);
				writeSslError("Handshake failed.");
			}else{
				context->status=connected;
				(void)attron(COLOR_PAIR(1) | A_BOLD | A_REVERSE);
				(void)mvprintw(0,(context->wcols)-((int)(context->statusConnectSize))-5,context->statusConnectp);
				(void)attroff(COLOR_PAIR(1) | A_BOLD | A_REVERSE);
				(void)refresh();

				(void)BIO_get_ssl(context->biop,&tempSsl);
				cipher=(SSL_CIPHER *)SSL_get_current_cipher(tempSsl);
				if(cipher!=NULL){
					SSL_CIPHER_description(cipher,buffer,sizeof(buffer)-1);
					(void)snprintf(context->handShakeSummary, MEDIUM_BUFFER-1,\
						"Info - Algorithms: %s - Algorithm bits %i - Cipher bits used %i - Connection Protocol Version - %s.",\
						cipher->name,\
						cipher->alg_bits,\
						cipher->strength_bits,\
						SSL_get_version((const SSL*)context->sslp)\
					);
					context->handShakeSummarySize=strlen(context->handShakeSummary);
					printHistory(INFO_PROMPT, context->handShakeSummary, context);
				}else{
					printHistory(INFO_PROMPT, "No cipher informations.", context);
					printHistory(INFO_PROMPT, SSL_get_cipher((const SSL*)context->sslp), context);
					printHistory(INFO_PROMPT, "Bits:", context);
					SSL_CIPHER_get_bits((SSL_get_current_cipher((const SSL*)context->sslp)), &bits);
					(void)snprintf(buffer, MEDIUM_BUFFER-1, "#%i\n", bits); 
					printHistory(INFO_PROMPT, buffer, context);
					
				}
				break;
			}
		}
	}
	return NULL; 
}

int password_callback(char *buf, int size, int rwflag, void *chatContext){
	struct chatContext *context=(struct chatContext *)chatContext;
	char buffer[MEDIUM_BUFFER];
	memset(buffer, 0, sizeof(buffer));

	(void)snprintf(buffer,MEDIUM_BUFFER-1, "cllb: %i-%i-%p",size,rwflag,chatContext);
	writeErrorLog((const char *)buffer, NULL);

	if(context->passwordSize!=(size_t)0){
		strcpy(buf, context->password);
	}else{
		strcpy(buf, "dummy");
	}

	return 1;
}

void parseInput(struct chatContext *context, int current){
	if(context->configurationp==NULL){
		(void)form_driver(context->insertFormp, current);
		(void)wrefresh(context->down_winp);
	}else{
		if(current==(int)'.'){
			(void)form_driver(context->configurationFormp, REQ_NEXT_FIELD);
		}else if(current==(int)'x' || current==(int)'X'){
			context->serverMode=true;
			setServerMenu(true, context);
		}else if(current==(int)' '){
			context->serverMode=false;
			setServerMenu(false, context);
		}else{
			(void)form_driver(context->configurationFormp, current);
		}
		(void)wrefresh(context->configurationp);
		(void)wrefresh(context->inner_configurationp);
	}
}

void doBackspace(struct chatContext *context){
	if(context->configurationp==NULL){
		if (form_driver(context->insertFormp, REQ_DEL_PREV)==E_REQUEST_DENIED){
			(void)form_driver(context->insertFormp, REQ_PREV_LINE);
			(void)form_driver(context->insertFormp, REQ_END_LINE);
			(void)form_driver(context->insertFormp, REQ_DEL_PREV);
		}
	}else{
		if(form_driver(context->configurationFormp, REQ_DEL_PREV)==E_REQUEST_DENIED){
			(void)form_driver(context->configurationFormp, REQ_PREV_LINE);
			(void)form_driver(context->configurationFormp, REQ_END_LINE);
			(void)form_driver(context->configurationFormp, REQ_DEL_PREV);
		}
		(void)wrefresh(context->inner_configurationp);
	}
}

void showConfigWin(struct chatContext *context){
	context->configurationp = new_win((context->up_height)-4, (context->up_width)-8, 4, 4);
	context->inner_configurationp=derwin(context->configurationp, (context->up_height)-7, (context->up_width)-11, 1, 1);
	(void)set_form_sub(context->configurationFormp, context->inner_configurationp);
	(void)box(context->configurationp, 0, 0);
	(void)post_form(context->configurationFormp);
	(void)set_current_field(context->configurationFormp, context->configurationFieldp[context->startIPAddress]);
	(void)refresh();
	(void)wrefresh(context->up_winp);
	(void)wrefresh(context->configurationp);
	(void)form_driver(context->configurationFormp, REQ_OVL_MODE);
	(void)form_driver(context->configurationFormp, REQ_FIRST_FIELD);
}

void setWindows(struct chatContext *context){
	bool keypadOn=true;
        context->up_winp = new_win(UP_HEIGHT, UP_WIDTH, UP_Y, UP_X);
        context->down_winp = new_win(DOWN_HEIGHT, DOWN_WIDTH, DOWN_Y, DOWN_X);

        (void)keypad(context->down_winp, keypadOn);

        (void)set_form_win(context->insertFormp, context->down_winp);
        context->inner_down_winp=derwin(context->down_winp, DOWN_HEIGHT-4, DOWN_WIDTH-4, 2, 2);
        (void)set_form_sub(context->insertFormp, context->inner_down_winp);
        (void)box(context->down_winp, 0, 0);
        (void)post_form(context->insertFormp);
        (void)set_form_win(context->historyFormp, context->up_winp);
        context->inner_up_winp=derwin(context->up_winp, UP_HEIGHT-3, UP_WIDTH-4, 2, 2);
        (void)set_form_sub(context->historyFormp, context->inner_up_winp);
        (void)box(context->up_winp, 0, 0);
        (void)post_form(context->historyFormp);

        (void)wrefresh(context->down_winp);
        (void)wrefresh(context->up_winp);

        (void)form_driver(context->insertFormp, REQ_OVL_MODE);
}


bool setClientMode(struct chatContext *context){
	bool status=true;
	int bits=0;

	if(!setContext(context)){
		(void)BIO_get_ssl(context->biop, &(context->sslp));
		(void)SSL_set_mode(context->sslp, SSL_MODE_AUTO_RETRY);

		(void)BIO_set_conn_hostname(context->biop, context->configIP);
		(void)BIO_set_conn_port(context->biop, context->sConfigPort);
		if(BIO_do_connect(context->biop) <= 0){ 
			writeErrorLog("Error attempting to connect - ", "IP: ", context->configIP, " Port: ", context->sConfigPort, NULL);
			printHistory(ERROR_PROMPT, "Error attempting to connect", context);
			status=false;
		}else{
			// Check the certificate 
			if(SSL_get_verify_result(context->sslp) != X509_V_OK){
				printHistory(ERROR_PROMPT, "Certificate verification error.", context);
				memset(context->errBuffer, 0, context->errBufferSize);
				(void)snprintf(context->errBuffer, context->errBufferSize-1, "%li", SSL_get_verify_result(context->sslp));
				writeErrorLog(" Certificate verification error:  ", context->errBuffer, NULL);
				status=false;
			}
		}   

		if(status){
			(void)snprintf(context->handShakeSummary, MEDIUM_BUFFER-1,\
				"Info - %s - Algorithms: %s - Algorithm bits %i - Connection Protocol Version - %s.",\
				SSL_state_string_long((const SSL*)context->sslp),\
				SSL_get_cipher_name((const SSL*)context->sslp),\
				SSL_get_cipher_bits((const SSL*)context->sslp, &bits),\
				SSL_get_version((const SSL*)context->sslp)\
			);
			context->handShakeSummarySize=strlen(context->handShakeSummary);
			printHistory(INFO_PROMPT, context->handShakeSummary, context);
			context->status=connected;
		}else{
			cleanContext(context);
		}
	}   
	return status;
}

bool setServerMode(struct chatContext *context){
	bool status=true;
	char connectionString[MEDIUM_BUFFER];

	memset(connectionString, 0, sizeof(connectionString));
	(void)snprintf(connectionString, MEDIUM_BUFFER-1, "%s:%s",context->configIP, context->sConfigPort);

	if(!setContext(context)){
		(void)BIO_get_ssl(context->mbiop, &(context->sslp));
		(void)SSL_set_mode(context->sslp, SSL_MODE_AUTO_RETRY);
		context->abiop = BIO_new_accept(connectionString);
		(void)BIO_set_accept_bios(context->abiop, context->mbiop);

		if(BIO_do_accept(context->abiop) <= 0){
			printHistory(ERROR_PROMPT, "Accept failed.", context);
			writeSslError("Accept failed.");
			status=false;
		}

		if(status){
			context->status=listening;
		}else{
			cleanContext(context);
		}
	}

	return status;
}

// Initialize the fields and other values in the context. 

void initContext(struct chatContext *context){
	context->wrows=LINES;
	context->wcols=COLS;
	context->up_height=UP_HEIGHT;
	context->up_width=UP_WIDTH;
        context->insertFieldp[0] = new_field(DOWN_HEIGHT-4, DOWN_WIDTH-6, 0, 0, 0, 0);
        context->insertFieldp[INSERT_FORM_FIELDS - 1] = NULL;
        context->historyFieldp[0] = new_field((context->up_height)-3, (context->up_width)-6, 0, 0, 0, 0);
        context->historyFieldp[HISTORY_FORM_FIELDS - 1] = NULL;

        (void)set_field_userptr(context->insertFieldp[0], NULL);
        (void)set_field_userptr(context->historyFieldp[0], NULL);

        setConfigurationForm(context);

        (void)field_opts_off(context->insertFieldp[0], (Field_Options)O_AUTOSKIP);
        (void)field_opts_off(context->historyFieldp[0], (Field_Options)O_STATIC);

	(void)set_field_fore(context->historyFieldp[0],(chtype)COLOR_PAIR(2));

	/* The history field grows without limits. To set a limit uncomment the following line:*/
        // (void)set_max_field(context->historyFieldp[0],5000);

        context->insertFormp = new_form(context->insertFieldp);
        context->historyFormp = new_form(context->historyFieldp);
        context->configurationFormp = new_form(context->configurationFieldp);

	context->statusConnectSize=strlen(context->statusConnectp);
	context->statusDisconnectSize=strlen(context->statusDisconnectp);
	context->statusListeningSize=strlen(context->statusListeningp);
	context->incomingBufferSize=MEDIUM_BUFFER;
	context->configIPSize=SMALL_BUFFER;
	context->sConfigPortSize=SMALL_BUFFER;
	context->errBufferSize=MEDIUM_BUFFER;
}

bool setContext(struct chatContext *context){
	bool error=false;
	char trustStore[BIG_BUFFER];
	char serverPem[BIG_BUFFER];
	char serverKey[BIG_BUFFER];

	memset(trustStore, 0, sizeof(trustStore));
	memset(serverPem, 0, sizeof(serverPem));
	memset(serverKey, 0, sizeof(serverKey));

	context->baseDir = calloc(BIG_BUFFER, sizeof(char));
	if(context->baseDir == NULL ){
		writeSslError("Calloc error on baseDir buffer.");
		error=true;
	}
	
	(void)snprintf(context->baseDir, BIG_BUFFER-1, "%s/.securechat/",getenv("HOME"));

	writeErrorLog("Basedir:", context->baseDir, NULL);

	if(context->serverMode==false){
		context->ctxp = SSL_CTX_new(SSLv23_client_method());
		if(context->ctxp == NULL ){
			writeSslError("SSL context failure.");
			error=true;
		}

		(void)snprintf(trustStore, BIG_BUFFER-1, "%sTrustStore.pem",context->baseDir);

		if(!error){
			if(SSL_CTX_load_verify_locations(context->ctxp, trustStore, NULL)==0){
				cleanContext(context);
				writeSslError("Error loading trust store");
				error=true;
			}
		}

		if (SSL_CTX_set_cipher_list(context->ctxp, context->blackList) <= 0) {
			printHistory(ERROR_PROMPT, "Error setting the cipher list from environment: setting default.", context);
		}

		context->biop = BIO_new_ssl_connect(context->ctxp);
	}else{
		context->ctxp = SSL_CTX_new(SSLv23_server_method());
		if(context->ctxp == NULL){
			writeSslError("Failed. Aborting.");
			error=true;
		}
		if(!error){
			int (*callback)(char *, int, int, void *) = &password_callback;
			SSL_CTX_set_default_passwd_cb(context->ctxp, callback);
			SSL_CTX_set_default_passwd_cb_userdata(context->ctxp, (void *)context);

			(void)snprintf(serverPem, BIG_BUFFER-1, "%sserver.pem",context->baseDir);

			if(SSL_CTX_use_certificate_file(context->ctxp, serverPem, SSL_FILETYPE_PEM)!=1){
				ERR_print_errors_fp(logStream);
				cleanContext(context);
				error=true;
			}
		}

		if(!error){
			(void)snprintf(serverKey, BIG_BUFFER-1, "%sserver.key",context->baseDir);

			if(SSL_CTX_use_PrivateKey_file(context->ctxp, serverKey, SSL_FILETYPE_PEM)!=1){
				ERR_print_errors_fp(logStream);
				cleanContext(context);
				error=true;
			}
		}

		if (SSL_CTX_set_cipher_list(context->ctxp, context->blackList) <= 0) {
			printHistory(ERROR_PROMPT, "Error setting the cipher list from environment: setting default.", context);
		}

		if(!error){
			context->mbiop = BIO_new_ssl(context->ctxp, 0);
			if(context->mbiop == NULL){
				fprintf(logStream,"Failed. Aborting.\n");
				ERR_print_errors_fp(logStream);
				cleanContext(context);
				error=true;
			}
		}
	}

	if(error) printHistory(ERROR_PROMPT, "Context creation error !", context);

	return error;
}

void cleanContext(struct chatContext *context){
	if(context->biop!=NULL){
		BIO_free_all(context->biop);
		context->biop=NULL;
	}
	if(context->mbiop!=NULL){
		BIO_free_all(context->mbiop);
		context->mbiop=NULL;
	}
	if(context->abiop!=NULL){
		free(context->abiop);
		context->abiop=NULL;
	}
	if(context->ctxp!=NULL){
		SSL_CTX_free(context->ctxp);
		context->ctxp=NULL;
	}
	if(context->baseDir!=NULL){
		free(context->baseDir);
		context->baseDir=NULL;
	}
}

void freeFormContext(struct chatContext *context){
	int count=0;
        (void)unpost_form(context->insertFormp);
        (void)free_form(context->insertFormp);
        (void)free_form(context->historyFormp);
        (void)free_form(context->configurationFormp);
        (void)free_field(context->insertFieldp[0]);
        (void)free_field(context->historyFieldp[0]);
        for(count=0;count<CONFIGURATION_FORM_FIELDS-1;count++){
                (void)free_field(context->configurationFieldp[count]);
        }
        free_win(context->inner_up_winp);
        free_win(context->inner_down_winp);
        free_win(context->up_winp);
        free_win(context->down_winp);
}

void trimTrailingSpaces(char *buffer, size_t size){
	long i;
	for(i=(long int)size-1;i>=0;i--){
		if(buffer[i]==' ')
			buffer[i]=(char)0;
		else
			break;
	}
}

void setDebug(void){
        if(debugOn){
                logDescriptor=open(LOGFILE, O_WRONLY|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR);
                if(logDescriptor!=-1){
                        char errMsg[]="Log start:\n";
			logStream=fdopen(logDescriptor, "a");
                        if(write(logDescriptor, errMsg, sizeof(errMsg))<=0){
                                (void)endwin();
                                perror("Can't write in the error log");
                                exit(EXIT_FAILURE);
                        }    
                }else{
                        perror("Can't open or create the error log");
			(void)endwin();
                        exit(EXIT_FAILURE);
                }   
        }   
}

void endDebug(void){
	char errMsg[]="Log stop.\n";
        if(debugOn){
		if(write(logDescriptor, errMsg, sizeof(errMsg))<=0){
			perror("Can't write in the error log");
		}
		(void)fclose(logStream);
		(void)close(logDescriptor);
	}
}
#ifdef __GNUC__
#if __GNUC__ > 3 && (__GNUC_MINOR__ > 3 )
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
#endif
void writeSslError(const char *errString){
	if(debugOn){
		(void)write(logDescriptor, errString, strlen(errString));
		(void)write(logDescriptor, "\n", 1);
		ERR_print_errors_fp(logStream);
	}
}

void writeErrorLog(const char *errString, ...){
	if(debugOn){
		const char *item=NULL;
		va_list listp;

		item=errString;

		va_start(listp, errString);
		do{
			(void)write(logDescriptor, item, strlen(item));
			item=va_arg(listp,char*);
				
		}while(item!=NULL);
		(void)write(logDescriptor, "\n", 1);
		va_end(listp);
	}
}
#ifdef __GNUC__
#if __GNUC__ > 3 && (__GNUC_MINOR__ > 3 )
#pragma GCC diagnostic warning "-Wunused-result"
#endif
#endif

bool checkParameters(int argc, char **argv){
	if(argc>2){
		fprintf(stderr, "Invalid parameter(s).\nSyntax: %s [-d]\n", argv[0]);
		return false;
	}

	if(argc==2 && strncmp("-d",argv[1],2)!=0){
		fprintf(stderr, "Invalid parameter(s).\nSyntax: %s [-d]\n", argv[0]);
		return false;
	}

	if(argc==2) debugOn=true;

	return true;
}

void moveForward(struct chatContext *context){
	if(context->configurationp!=NULL){
		(void)form_driver(context->configurationFormp, REQ_NEXT_FIELD);
		(void)wrefresh(context->configurationp);
		(void)wrefresh(context->inner_configurationp);
	}else{
		(void)form_driver(context->historyFormp, REQ_SCR_FPAGE);
		(void)wrefresh(context->up_winp);
		(void)wrefresh(context->inner_up_winp);
	}
}

void moveBackward(struct chatContext *context){
	if(context->configurationp!=NULL){
		(void)form_driver(context->configurationFormp, REQ_PREV_FIELD);
		(void)wrefresh(context->configurationp);
		(void)wrefresh(context->inner_configurationp);
	}else{
		(void)form_driver(context->historyFormp, REQ_SCR_BPAGE);
		(void)wrefresh(context->up_winp);
		(void)wrefresh(context->inner_up_winp);
	}
}

