static CURLcode smtp_perform_command(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;

  /* Send the command */
  if(smtp->rcpt)
    result = Curl_pp_sendf(&conn->proto.smtpc.pp, "%s %s",
                           smtp->custom && smtp->custom[0] != '\0' ?
                           smtp->custom : "VRFY",
                           smtp->rcpt->data);
  else
    result = Curl_pp_sendf(&conn->proto.smtpc.pp, "%s",
                           smtp->custom && smtp->custom[0] != '\0' ?
                           smtp->custom : "HELP");

  if(!result)
    state(conn, SMTP_COMMAND);

  return result;
}
