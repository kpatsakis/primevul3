static CURLcode smtp_perform_mail(struct connectdata *conn)
{
  char *from = NULL;
  char *auth = NULL;
  char *size = NULL;
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  /* Calculate the FROM parameter */
  if(!data->set.str[STRING_MAIL_FROM])
    /* Null reverse-path, RFC-5321, sect. 3.6.3 */
    from = strdup("<>");
  else if(data->set.str[STRING_MAIL_FROM][0] == '<')
    from = aprintf("%s", data->set.str[STRING_MAIL_FROM]);
  else
    from = aprintf("<%s>", data->set.str[STRING_MAIL_FROM]);

  if(!from)
    return CURLE_OUT_OF_MEMORY;

  /* Calculate the optional AUTH parameter */
  if(data->set.str[STRING_MAIL_AUTH] && conn->proto.smtpc.sasl.authused) {
    if(data->set.str[STRING_MAIL_AUTH][0] != '\0')
      auth = aprintf("%s", data->set.str[STRING_MAIL_AUTH]);
    else
      /* Empty AUTH, RFC-2554, sect. 5 */
      auth = strdup("<>");

    if(!auth) {
      free(from);

      return CURLE_OUT_OF_MEMORY;
    }
  }

  /* Prepare the mime data if some. */
  if(data->set.mimepost.kind != MIMEKIND_NONE) {
    /* Use the whole structure as data. */
    data->set.mimepost.flags &= ~MIME_BODY_ONLY;

    /* Add external headers and mime version. */
    curl_mime_headers(&data->set.mimepost, data->set.headers, 0);
    result = Curl_mime_prepare_headers(&data->set.mimepost, NULL,
                                       NULL, MIMESTRATEGY_MAIL);

    if(!result)
      if(!Curl_checkheaders(conn, "Mime-Version"))
        result = Curl_mime_add_header(&data->set.mimepost.curlheaders,
                                      "Mime-Version: 1.0");

    /* Make sure we will read the entire mime structure. */
    if(!result)
      result = Curl_mime_rewind(&data->set.mimepost);

    if(result) {
      free(from);
      free(auth);
      return result;
    }

    data->state.infilesize = Curl_mime_size(&data->set.mimepost);

    /* Read from mime structure. */
    data->state.fread_func = (curl_read_callback) Curl_mime_read;
    data->state.in = (void *) &data->set.mimepost;
  }

  /* Calculate the optional SIZE parameter */
  if(conn->proto.smtpc.size_supported && data->state.infilesize > 0) {
    size = aprintf("%" CURL_FORMAT_CURL_OFF_T, data->state.infilesize);

    if(!size) {
      free(from);
      free(auth);

      return CURLE_OUT_OF_MEMORY;
    }
  }

  /* Send the MAIL command */
  if(!auth && !size)
    result = Curl_pp_sendf(&conn->proto.smtpc.pp,
                           "MAIL FROM:%s", from);
  else if(auth && !size)
    result = Curl_pp_sendf(&conn->proto.smtpc.pp,
                           "MAIL FROM:%s AUTH=%s", from, auth);
  else if(auth && size)
    result = Curl_pp_sendf(&conn->proto.smtpc.pp,
                           "MAIL FROM:%s AUTH=%s SIZE=%s", from, auth, size);
  else
    result = Curl_pp_sendf(&conn->proto.smtpc.pp,
                           "MAIL FROM:%s SIZE=%s", from, size);

  free(from);
  free(auth);
  free(size);

  if(!result)
    state(conn, SMTP_MAIL);

  return result;
}
