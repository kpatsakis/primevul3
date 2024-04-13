static CURLcode smtp_state_servergreet_resp(struct connectdata *conn,
                                            int smtpcode,
                                            smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  (void)instate; /* no use for this yet */

  if(smtpcode/100 != 2) {
    failf(data, "Got unexpected smtp-server response: %d", smtpcode);
    result = CURLE_WEIRD_SERVER_REPLY;
  }
  else
    result = smtp_perform_ehlo(conn);

  return result;
}
