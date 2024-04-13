cupsdSendCommand(
    cupsd_client_t *con,		/* I - Client connection */
    char           *command,		/* I - Command to run */
    char           *options,		/* I - Command-line options */
    int            root)		/* I - Run as root? */
{
  int	fd;				/* Standard input file descriptor */


  if (con->filename)
  {
    fd = open(con->filename, O_RDONLY);

    if (fd < 0)
    {
      cupsdLogClient(con, CUPSD_LOG_ERROR,
                     "Unable to open \"%s\" for reading: %s",
                     con->filename ? con->filename : "/dev/null",
	             strerror(errno));
      return (0);
    }

    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
  }
  else
    fd = -1;

  con->pipe_pid    = pipe_command(con, fd, &(con->file), command, options, root);
  con->pipe_status = HTTP_STATUS_OK;

  httpClearFields(con->http);

  if (fd >= 0)
    close(fd);

  cupsdLogClient(con, CUPSD_LOG_INFO, "Started \"%s\" (pid=%d, file=%d)",
                 command, con->pipe_pid, con->file);

  if (con->pipe_pid == 0)
    return (0);

  fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);

  cupsdAddSelect(con->file, (cupsd_selfunc_t)write_pipe, NULL, con);

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Waiting for CGI data.");

  con->sent_header = 0;
  con->file_ready  = 0;
  con->got_fields  = 0;
  con->header_used = 0;

  return (1);
}
