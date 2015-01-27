#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([securechat], m4_esyscmd([ tr -d '\n' < version ]))
AC_CONFIG_SRCDIR([src/chat.c])
AC_CONFIG_HEADER([include/config.h])


AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([disable])
AC_CONFIG_FILES([Makefile
		src/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_C_VOLATILE

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([memset])

# Libs list autmatically generated prom dependecy script
AC_CHECK_LIB([crypto],[BIO_ctrl],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[BIO_free_all],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[BIO_new_accept],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[BIO_pop],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[BIO_read],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[BIO_write],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[ERR_load_BIO_strings],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[ERR_print_errors_fp],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([crypto],[OPENSSL_add_all_algorithms_noconf],[],[AC_MSG_FAILURE([could not find lib crypto])])
AC_CHECK_LIB([form],[field_buffer])
AC_CHECK_LIB([form],[field_opts],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[field_opts_off],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[field_opts_on],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[form_driver],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[free_field],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[free_form],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[new_field],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[new_form],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[post_form],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_current_field],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_field_buffer],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_field_fore],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_field_just],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_field_opts],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_field_type],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_field_userptr],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_form_sub],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[set_form_win],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([form],[unpost_form],[],[AC_MSG_FAILURE([could not find lib form])])
AC_CHECK_LIB([ncurses],[delwin],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[derwin],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[cbreak],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[endwin],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[init_pair],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[initscr],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[mvprintw],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[newwin],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[noecho],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[start_color],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[tputs],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[wattr_off],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[wattr_on],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[wborder],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[wclear],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[wgetch],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([ncurses],[wrefresh],[],[AC_MSG_FAILURE([could not find lib ncurses])])
AC_CHECK_LIB([pthread],[pthread_attr_init],[],[AC_MSG_FAILURE([could not find lib pthread])])
AC_CHECK_LIB([pthread],[pthread_attr_setdetachstate],[],[AC_MSG_FAILURE([could not find lib pthread])])
AC_CHECK_LIB([pthread],[pthread_cancel],[],[AC_MSG_FAILURE([could not find lib pthread])])
AC_CHECK_LIB([pthread],[pthread_create],[],[AC_MSG_FAILURE([could not find lib pthread])])
AC_CHECK_LIB([pthread],[pthread_mutex_lock],[],[AC_MSG_FAILURE([could not find lib pthread])])
AC_CHECK_LIB([pthread],[pthread_mutex_unlock],[],[AC_MSG_FAILURE([could not find lib pthread])])
AC_CHECK_LIB([ssl],[BIO_new_ssl],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[BIO_new_ssl_connect],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[ERR_load_SSL_strings],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CIPHER_description],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CIPHER_get_bits],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CIPHER_get_name],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_ctrl],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_free],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_load_verify_locations],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_new],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_set_cipher_list],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_set_default_passwd_cb],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_set_default_passwd_cb_userdata],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_use_certificate_file],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_CTX_use_PrivateKey_file],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_get_current_cipher],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_get_verify_result],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_get_version],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_library_init],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_load_error_strings],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSL_state_string_long],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSLv23_client_method],[],[AC_MSG_FAILURE([could not find lib ssl])])
AC_CHECK_LIB([ssl],[SSLv23_server_method],[],[AC_MSG_FAILURE([could not find lib ssl])])


AC_CANONICAL_HOST
case $host_os in
  aix* )
	      if test "x$CC" = xgcc; then
		    CFLAGS=" -g -Wextra -Wall \
                             -D_REENTRANT -D_FORTIFY_SOURCE=2 -O2 \
                             -Wformat -Werror=format-security -L/usr/lib \
                             -I/opt/freeware/include -I/opt/freeware/include/ncurses"
              else
                    CFLAGS=" -O2 -g -Wall "
              fi

        ;;
  darwin* )
	      if test "x$CC" = xgcc; then
		    CFLAGS=" -g -Wextra -Wall \
                             -D_REENTRANT -D_FORTIFY_SOURCE=2 -O2 -fstack-protector \
                             --param=ssp-buffer-size=4 -Wformat -Werror=format-security -L/usr/lib "
              else
                    CFLAGS=" -O2 -g -Wall "
              fi

        ;;
  linux*)
	      if test "x$CC" = xgcc; then
                    CFLAGS=" -g -Wextra -Wall \
                             -D_REENTRANT -D_FORTIFY_SOURCE=2 -O2 -fstack-protector \
                             --param=ssp-buffer-size=4 -Wformat -Werror=format-security "
                    LDFLAGS="$LDFLAGS -Wl,-z,relro"
              else
                    CFLAGS=" -O2 -g -Wall "
              fi
        ;;
    *)
            CFLAGS=" -O2 -g -Wall "
            #AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac


AC_OUTPUT
