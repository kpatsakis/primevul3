static CURLcode smtp_state_starttls_resp(struct connectdata *conn,
                                         int smtpcode,
                                         smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  (void)instate; /* no use for this yet */

  if(smtpcode != 220) {
    if(data->set.use_ssl != CURLUSESSL_TRY) {
      failf(data, "STARTTLS denied, code %d", smtpcode);
      result = CURLE_USE_SSL_FAILED;
    }
    else
      result = smtp_perform_authentication(conn);
  }
  else
    result = smtp_perform_upgrade_tls(conn);

  return result;
}
