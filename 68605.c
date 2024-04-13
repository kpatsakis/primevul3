static CURLcode smtp_parse_url_path(struct connectdata *conn)
{
  /* The SMTP struct is already initialised in smtp_connect() */
  struct Curl_easy *data = conn->data;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  const char *path = data->state.path;
  char localhost[HOSTNAME_MAX + 1];

  /* Calculate the path if necessary */
  if(!*path) {
    if(!Curl_gethostname(localhost, sizeof(localhost)))
      path = localhost;
    else
      path = "localhost";
  }

  /* URL decode the path and use it as the domain in our EHLO */
  return Curl_urldecode(conn->data, path, 0, &smtpc->domain, NULL, TRUE);
}
