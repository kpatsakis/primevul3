static CURLcode smtp_state_helo_resp(struct connectdata *conn, int smtpcode,
                                     smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  (void)instate; /* no use for this yet */

  if(smtpcode/100 != 2) {
    failf(data, "Remote access denied: %d", smtpcode);
    result = CURLE_REMOTE_ACCESS_DENIED;
  }
  else
    /* End of connect phase */
    state(conn, SMTP_STOP);

  return result;
}
