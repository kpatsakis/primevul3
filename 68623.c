static CURLcode smtp_state_mail_resp(struct connectdata *conn, int smtpcode,
                                     smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  (void)instate; /* no use for this yet */

  if(smtpcode/100 != 2) {
    failf(data, "MAIL failed: %d", smtpcode);
    result = CURLE_SEND_ERROR;
  }
  else
    /* Start the RCPT TO command */
    result = smtp_perform_rcpt_to(conn);

  return result;
}
