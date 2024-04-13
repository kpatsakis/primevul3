static CURLcode smtp_done(struct connectdata *conn, CURLcode status,
                          bool premature)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;
  struct pingpong *pp = &conn->proto.smtpc.pp;
  char *eob;
  ssize_t len;
  ssize_t bytes_written;

  (void)premature;

  if(!smtp || !pp->conn)
    return CURLE_OK;

  /* Cleanup our per-request based variables */
  Curl_safefree(smtp->custom);

  if(status) {
    connclose(conn, "SMTP done with bad status"); /* marked for closure */
    result = status;         /* use the already set error code */
  }
  else if(!data->set.connect_only && data->set.mail_rcpt &&
          (data->set.upload || data->set.mimepost.kind)) {
    /* Calculate the EOB taking into account any terminating CRLF from the
       previous line of the email or the CRLF of the DATA command when there
       is "no mail data". RFC-5321, sect. 4.1.1.4.

       Note: As some SSL backends, such as OpenSSL, will cause Curl_write() to
       fail when using a different pointer following a previous write, that
       returned CURLE_AGAIN, we duplicate the EOB now rather than when the
       bytes written doesn't equal len. */
    if(smtp->trailing_crlf || !conn->data->state.infilesize) {
      eob = strdup(SMTP_EOB + 2);
      len = SMTP_EOB_LEN - 2;
    }
    else {
      eob = strdup(SMTP_EOB);
      len = SMTP_EOB_LEN;
    }

    if(!eob)
      return CURLE_OUT_OF_MEMORY;

    /* Send the end of block data */
    result = Curl_write(conn, conn->writesockfd, eob, len, &bytes_written);
    if(result) {
      free(eob);
      return result;
    }

    if(bytes_written != len) {
      /* The whole chunk was not sent so keep it around and adjust the
         pingpong structure accordingly */
      pp->sendthis = eob;
      pp->sendsize = len;
      pp->sendleft = len - bytes_written;
    }
    else {
      /* Successfully sent so adjust the response timeout relative to now */
      pp->response = Curl_now();

      free(eob);
    }

    state(conn, SMTP_POSTDATA);

    /* Run the state-machine

       TODO: when the multi interface is used, this _really_ should be using
       the smtp_multi_statemach function but we have no general support for
       non-blocking DONE operations!
    */
    result = smtp_block_statemach(conn);
  }

  /* Clear the transfer mode for the next request */
  smtp->transfer = FTPTRANSFER_BODY;

  return result;
}
