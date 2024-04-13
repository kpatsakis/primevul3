install_cupsd_conf(cupsd_client_t *con)	/* I - Connection */
{
  char		filename[1024];		/* Configuration filename */
  cups_file_t	*in,			/* Input file */
		*out;			/* Output file */
  char		buffer[16384];		/* Copy buffer */
  ssize_t	bytes;			/* Number of bytes */


 /*
  * Open the request file...
  */

  if ((in = cupsFileOpen(con->filename, "rb")) == NULL)
  {
    cupsdLogClient(con, CUPSD_LOG_ERROR, "Unable to open request file \"%s\": %s",
                    con->filename, strerror(errno));
    goto server_error;
  }

 /*
  * Open the new config file...
  */

  if ((out = cupsdCreateConfFile(ConfigurationFile, ConfigFilePerm)) == NULL)
  {
    cupsFileClose(in);
    goto server_error;
  }

  cupsdLogClient(con, CUPSD_LOG_INFO, "Installing config file \"%s\"...",
                  ConfigurationFile);

 /*
  * Copy from the request to the new config file...
  */

  while ((bytes = cupsFileRead(in, buffer, sizeof(buffer))) > 0)
    if (cupsFileWrite(out, buffer, (size_t)bytes) < bytes)
    {
      cupsdLogClient(con, CUPSD_LOG_ERROR,
                      "Unable to copy to config file \"%s\": %s",
        	      ConfigurationFile, strerror(errno));

      cupsFileClose(in);
      cupsFileClose(out);

      snprintf(filename, sizeof(filename), "%s.N", ConfigurationFile);
      cupsdUnlinkOrRemoveFile(filename);

      goto server_error;
    }

 /*
  * Close the files...
  */

  cupsFileClose(in);

  if (cupsdCloseCreatedConfFile(out, ConfigurationFile))
    goto server_error;

 /*
  * Remove the request file...
  */

  cupsdUnlinkOrRemoveFile(con->filename);
  cupsdClearString(&con->filename);

 /*
  * Set the NeedReload flag...
  */

  NeedReload = RELOAD_CUPSD;
  ReloadTime = time(NULL);

 /*
  * Return that the file was created successfully...
  */

  return (HTTP_STATUS_CREATED);

 /*
  * Common exit for errors...
  */

  server_error:

  cupsdUnlinkOrRemoveFile(con->filename);
  cupsdClearString(&con->filename);

  return (HTTP_STATUS_SERVER_ERROR);
}
