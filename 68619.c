static CURLcode smtp_state_auth_resp(struct connectdata *conn,
                                     int smtpcode,
                                     smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  saslprogress progress;

  (void)instate; /* no use for this yet */

  result = Curl_sasl_continue(&smtpc->sasl, conn, smtpcode, &progress);
  if(!result)
    switch(progress) {
    case SASL_DONE:
      state(conn, SMTP_STOP);  /* Authenticated */
      break;
    case SASL_IDLE:            /* No mechanism left after cancellation */
      failf(data, "Authentication cancelled");
      result = CURLE_LOGIN_DENIED;
      break;
    default:
      break;
    }

  return result;
}
