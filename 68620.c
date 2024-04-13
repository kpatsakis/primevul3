static CURLcode smtp_state_data_resp(struct connectdata *conn, int smtpcode,
                                     smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  (void)instate; /* no use for this yet */

  if(smtpcode != 354) {
    failf(data, "DATA failed: %d", smtpcode);
    result = CURLE_SEND_ERROR;
  }
  else {
    /* Set the progress upload size */
    Curl_pgrsSetUploadSize(data, data->state.infilesize);

    /* SMTP upload */
    Curl_setup_transfer(conn, -1, -1, FALSE, NULL, FIRSTSOCKET, NULL);

    /* End of DO phase */
    state(conn, SMTP_STOP);
  }

  return result;
}
