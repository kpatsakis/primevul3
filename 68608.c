static CURLcode smtp_perform_authentication(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  saslprogress progress;

  /* Check we have enough data to authenticate with, and the
     server supports authentiation, and end the connect phase if not */
  if(!smtpc->auth_supported ||
     !Curl_sasl_can_authenticate(&smtpc->sasl, conn)) {
    state(conn, SMTP_STOP);
    return result;
  }

  /* Calculate the SASL login details */
  result = Curl_sasl_start(&smtpc->sasl, conn, FALSE, &progress);

  if(!result) {
    if(progress == SASL_INPROGRESS)
      state(conn, SMTP_AUTH);
    else {
      /* Other mechanisms not supported */
      infof(conn->data, "No known authentication mechanisms supported!\n");
      result = CURLE_LOGIN_DENIED;
    }
  }

  return result;
}
