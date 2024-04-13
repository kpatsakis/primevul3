pipe_command(cupsd_client_t *con,	/* I - Client connection */
             int            infile,	/* I - Standard input for command */
             int            *outfile,	/* O - Standard output for command */
	     char           *command,	/* I - Command to run */
	     char           *options,	/* I - Options for command */
	     int            root)	/* I - Run as root? */
{
  int		i;			/* Looping var */
  int		pid;			/* Process ID */
  char		*commptr,		/* Command string pointer */
		commch;			/* Command string character */
  char		*uriptr;		/* URI string pointer */
  int		fds[2];			/* Pipe FDs */
  int		argc;			/* Number of arguments */
  int		envc;			/* Number of environment variables */
  char		argbuf[10240],		/* Argument buffer */
		*argv[100],		/* Argument strings */
		*envp[MAX_ENV + 20];	/* Environment variables */
  char		auth_type[256],		/* AUTH_TYPE environment variable */
		content_length[1024],	/* CONTENT_LENGTH environment variable */
		content_type[1024],	/* CONTENT_TYPE environment variable */
		http_cookie[32768],	/* HTTP_COOKIE environment variable */
		http_referer[1024],	/* HTTP_REFERER environment variable */
		http_user_agent[1024],	/* HTTP_USER_AGENT environment variable */
		lang[1024],		/* LANG environment variable */
		path_info[1024],	/* PATH_INFO environment variable */
		remote_addr[1024],	/* REMOTE_ADDR environment variable */
		remote_host[1024],	/* REMOTE_HOST environment variable */
		remote_user[1024],	/* REMOTE_USER environment variable */
		script_filename[1024],	/* SCRIPT_FILENAME environment variable */
		script_name[1024],	/* SCRIPT_NAME environment variable */
		server_name[1024],	/* SERVER_NAME environment variable */
		server_port[1024];	/* SERVER_PORT environment variable */
  ipp_attribute_t *attr;		/* attributes-natural-language attribute */


 /*
  * Parse a copy of the options string, which is of the form:
  *
  *     argument+argument+argument
  *     ?argument+argument+argument
  *     param=value&param=value
  *     ?param=value&param=value
  *     /name?argument+argument+argument
  *     /name?param=value&param=value
  *
  * If the string contains an "=" character after the initial name,
  * then we treat it as a HTTP GET form request and make a copy of
  * the remaining string for the environment variable.
  *
  * The string is always parsed out as command-line arguments, to
  * be consistent with Apache...
  */

  cupsdLogClient(con, CUPSD_LOG_DEBUG2, "pipe_command: infile=%d, outfile=%p, command=\"%s\", options=\"%s\", root=%d", infile, outfile, command, options ? options : "(null)", root);

  argv[0] = command;

  if (options)
    strlcpy(argbuf, options, sizeof(argbuf));
  else
    argbuf[0] = '\0';

  if (argbuf[0] == '/')
  {
   /*
    * Found some trailing path information, set PATH_INFO...
    */

    if ((commptr = strchr(argbuf, '?')) == NULL)
      commptr = argbuf + strlen(argbuf);

    commch   = *commptr;
    *commptr = '\0';
    snprintf(path_info, sizeof(path_info), "PATH_INFO=%s", argbuf);
    *commptr = commch;
  }
  else
  {
    commptr      = argbuf;
    path_info[0] = '\0';

    if (*commptr == ' ')
      commptr ++;
  }

  if (*commptr == '?' && con->operation == HTTP_STATE_GET && !con->query_string)
  {
    commptr ++;
    cupsdSetStringf(&(con->query_string), "QUERY_STRING=%s", commptr);
  }

  argc = 1;

  if (*commptr)
  {
    argv[argc ++] = commptr;

    for (; *commptr && argc < 99; commptr ++)
    {
     /*
      * Break arguments whenever we see a + or space...
      */

      if (*commptr == ' ' || *commptr == '+')
      {
	while (*commptr == ' ' || *commptr == '+')
	  *commptr++ = '\0';

       /*
	* If we don't have a blank string, save it as another argument...
	*/

	if (*commptr)
	{
	  argv[argc] = commptr;
	  argc ++;
	}
	else
	  break;
      }
      else if (*commptr == '%' && isxdigit(commptr[1] & 255) &&
               isxdigit(commptr[2] & 255))
      {
       /*
	* Convert the %xx notation to the individual character.
	*/

	if (commptr[1] >= '0' && commptr[1] <= '9')
          *commptr = (char)((commptr[1] - '0') << 4);
	else
          *commptr = (char)((tolower(commptr[1]) - 'a' + 10) << 4);

	if (commptr[2] >= '0' && commptr[2] <= '9')
          *commptr |= commptr[2] - '0';
	else
          *commptr |= tolower(commptr[2]) - 'a' + 10;

	_cups_strcpy(commptr + 1, commptr + 3);

       /*
	* Check for a %00 and break if that is the case...
	*/

	if (!*commptr)
          break;
      }
    }
  }

  argv[argc] = NULL;

 /*
  * Setup the environment variables as needed...
  */

  if (con->username[0])
  {
    snprintf(auth_type, sizeof(auth_type), "AUTH_TYPE=%s",
             httpGetField(con->http, HTTP_FIELD_AUTHORIZATION));

    if ((uriptr = strchr(auth_type + 10, ' ')) != NULL)
      *uriptr = '\0';
  }
  else
    auth_type[0] = '\0';

  if (con->request &&
      (attr = ippFindAttribute(con->request, "attributes-natural-language",
                               IPP_TAG_LANGUAGE)) != NULL)
  {
    switch (strlen(attr->values[0].string.text))
    {
      default :
	 /*
	  * This is an unknown or badly formatted language code; use
	  * the POSIX locale...
	  */

	  strlcpy(lang, "LANG=C", sizeof(lang));
	  break;

      case 2 :
	 /*
	  * Just the language code (ll)...
	  */

	  snprintf(lang, sizeof(lang), "LANG=%s.UTF8",
		   attr->values[0].string.text);
	  break;

      case 5 :
	 /*
	  * Language and country code (ll-cc)...
	  */

	  snprintf(lang, sizeof(lang), "LANG=%c%c_%c%c.UTF8",
		   attr->values[0].string.text[0],
		   attr->values[0].string.text[1],
		   toupper(attr->values[0].string.text[3] & 255),
		   toupper(attr->values[0].string.text[4] & 255));
	  break;
    }
  }
  else if (con->language)
    snprintf(lang, sizeof(lang), "LANG=%s.UTF8", con->language->language);
  else
    strlcpy(lang, "LANG=C", sizeof(lang));

  strlcpy(remote_addr, "REMOTE_ADDR=", sizeof(remote_addr));
  httpAddrString(httpGetAddress(con->http), remote_addr + 12,
                 sizeof(remote_addr) - 12);

  snprintf(remote_host, sizeof(remote_host), "REMOTE_HOST=%s",
           httpGetHostname(con->http, NULL, 0));

  snprintf(script_name, sizeof(script_name), "SCRIPT_NAME=%s", con->uri);
  if ((uriptr = strchr(script_name, '?')) != NULL)
    *uriptr = '\0';

  snprintf(script_filename, sizeof(script_filename), "SCRIPT_FILENAME=%s%s",
           DocumentRoot, script_name + 12);

  snprintf(server_port, sizeof(server_port), "SERVER_PORT=%d", con->serverport);

  if (httpGetField(con->http, HTTP_FIELD_HOST)[0])
  {
    char *nameptr;			/* Pointer to ":port" */

    snprintf(server_name, sizeof(server_name), "SERVER_NAME=%s",
	     httpGetField(con->http, HTTP_FIELD_HOST));
    if ((nameptr = strrchr(server_name, ':')) != NULL && !strchr(nameptr, ']'))
      *nameptr = '\0';			/* Strip trailing ":port" */
  }
  else
    snprintf(server_name, sizeof(server_name), "SERVER_NAME=%s",
	     con->servername);

  envc = cupsdLoadEnv(envp, (int)(sizeof(envp) / sizeof(envp[0])));

  if (auth_type[0])
    envp[envc ++] = auth_type;

  envp[envc ++] = lang;
  envp[envc ++] = "REDIRECT_STATUS=1";
  envp[envc ++] = "GATEWAY_INTERFACE=CGI/1.1";
  envp[envc ++] = server_name;
  envp[envc ++] = server_port;
  envp[envc ++] = remote_addr;
  envp[envc ++] = remote_host;
  envp[envc ++] = script_name;
  envp[envc ++] = script_filename;

  if (path_info[0])
    envp[envc ++] = path_info;

  if (con->username[0])
  {
    snprintf(remote_user, sizeof(remote_user), "REMOTE_USER=%s", con->username);

    envp[envc ++] = remote_user;
  }

  if (httpGetVersion(con->http) == HTTP_VERSION_1_1)
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/1.1";
  else if (httpGetVersion(con->http) == HTTP_VERSION_1_0)
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/1.0";
  else
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/0.9";

  if (httpGetCookie(con->http))
  {
    snprintf(http_cookie, sizeof(http_cookie), "HTTP_COOKIE=%s",
             httpGetCookie(con->http));
    envp[envc ++] = http_cookie;
  }

  if (httpGetField(con->http, HTTP_FIELD_USER_AGENT)[0])
  {
    snprintf(http_user_agent, sizeof(http_user_agent), "HTTP_USER_AGENT=%s",
             httpGetField(con->http, HTTP_FIELD_USER_AGENT));
    envp[envc ++] = http_user_agent;
  }

  if (httpGetField(con->http, HTTP_FIELD_REFERER)[0])
  {
    snprintf(http_referer, sizeof(http_referer), "HTTP_REFERER=%s",
             httpGetField(con->http, HTTP_FIELD_REFERER));
    envp[envc ++] = http_referer;
  }

  if (con->operation == HTTP_STATE_GET)
  {
    envp[envc ++] = "REQUEST_METHOD=GET";

    if (con->query_string)
    {
     /*
      * Add GET form variables after ?...
      */

      envp[envc ++] = con->query_string;
    }
    else
      envp[envc ++] = "QUERY_STRING=";
  }
  else
  {
    sprintf(content_length, "CONTENT_LENGTH=" CUPS_LLFMT,
            CUPS_LLCAST con->bytes);
    snprintf(content_type, sizeof(content_type), "CONTENT_TYPE=%s",
             httpGetField(con->http, HTTP_FIELD_CONTENT_TYPE));

    envp[envc ++] = "REQUEST_METHOD=POST";
    envp[envc ++] = content_length;
    envp[envc ++] = content_type;
  }

 /*
  * Tell the CGI if we are using encryption...
  */

  if (httpIsEncrypted(con->http))
    envp[envc ++] = "HTTPS=ON";

 /*
  * Terminate the environment array...
  */

  envp[envc] = NULL;

  if (LogLevel >= CUPSD_LOG_DEBUG)
  {
    for (i = 0; i < argc; i ++)
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "[CGI] argv[%d] = \"%s\"", i, argv[i]);
    for (i = 0; i < envc; i ++)
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "[CGI] envp[%d] = \"%s\"", i, envp[i]);
  }

 /*
  * Create a pipe for the output...
  */

  if (cupsdOpenPipe(fds))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "[CGI] Unable to create pipe for %s - %s",
                    argv[0], strerror(errno));
    return (0);
  }

 /*
  * Then execute the command...
  */

  if (cupsdStartProcess(command, argv, envp, infile, fds[1], CGIPipes[1],
			-1, -1, root, DefaultProfile, NULL, &pid) < 0)
  {
   /*
    * Error - can't fork!
    */

    cupsdLogMessage(CUPSD_LOG_ERROR, "[CGI] Unable to start %s - %s", argv[0],
                    strerror(errno));

    cupsdClosePipe(fds);
    pid = 0;
  }
  else
  {
   /*
    * Fork successful - return the PID...
    */

    if (con->username[0])
      cupsdAddCert(pid, con->username, con->type);

    cupsdLogMessage(CUPSD_LOG_DEBUG, "[CGI] Started %s (PID %d)", command, pid);

    *outfile = fds[0];
    close(fds[1]);
  }

  return (pid);
}
