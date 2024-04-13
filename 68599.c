static bool smtp_endofresp(struct connectdata *conn, char *line, size_t len,
                           int *resp)
{
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  bool result = FALSE;

  /* Nothing for us */
  if(len < 4 || !ISDIGIT(line[0]) || !ISDIGIT(line[1]) || !ISDIGIT(line[2]))
    return FALSE;

  /* Do we have a command response? This should be the response code followed
     by a space and optionally some text as per RFC-5321 and as outlined in
     Section 4. Examples of RFC-4954 but some e-mail servers ignore this and
     only send the response code instead as per Section 4.2. */
  if(line[3] == ' ' || len == 5) {
    result = TRUE;
    *resp = curlx_sltosi(strtol(line, NULL, 10));

    /* Make sure real server never sends internal value */
    if(*resp == 1)
      *resp = 0;
  }
  /* Do we have a multiline (continuation) response? */
  else if(line[3] == '-' &&
          (smtpc->state == SMTP_EHLO || smtpc->state == SMTP_COMMAND)) {
    result = TRUE;
    *resp = 1;  /* Internal response code */
  }

  return result;
}
