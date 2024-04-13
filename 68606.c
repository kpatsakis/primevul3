static CURLcode smtp_perform(struct connectdata *conn, bool *connected,
                             bool *dophase_done)
{
  /* This is SMTP and no proxy */
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;

  DEBUGF(infof(conn->data, "DO phase starts\n"));

  if(data->set.opt_no_body) {
    /* Requested no body means no transfer */
    smtp->transfer = FTPTRANSFER_INFO;
  }

  *dophase_done = FALSE; /* not done yet */

  /* Store the first recipient (or NULL if not specified) */
  smtp->rcpt = data->set.mail_rcpt;

  /* Initial data character is the first character in line: it is implicitly
     preceded by a virtual CRLF. */
  smtp->trailing_crlf = TRUE;
  smtp->eob = 2;

  /* Start the first command in the DO phase */
  if((data->set.upload || data->set.mimepost.kind) && data->set.mail_rcpt)
    /* MAIL transfer */
    result = smtp_perform_mail(conn);
  else
    /* SMTP based command (VRFY, EXPN, NOOP, RSET or HELP) */
    result = smtp_perform_command(conn);

  if(result)
    return result;

  /* Run the state-machine */
  result = smtp_multi_statemach(conn, dophase_done);

  *connected = conn->bits.tcpconnect[FIRSTSOCKET];

  if(*dophase_done)
    DEBUGF(infof(conn->data, "DO phase is complete\n"));

  return result;
}
