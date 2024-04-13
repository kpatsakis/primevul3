static int msg_fetch_header(struct Context *ctx, struct ImapHeader *h, char *buf, FILE *fp)
{
  unsigned int bytes;
  int rc = -1; /* default now is that string isn't FETCH response */
  int parse_rc;

  struct ImapData *idata = ctx->data;

  if (buf[0] != '*')
    return rc;

  /* skip to message number */
  buf = imap_next_word(buf);
  if (mutt_str_atoui(buf, &h->data->msn) < 0)
    return rc;

  /* find FETCH tag */
  buf = imap_next_word(buf);
  if (mutt_str_strncasecmp("FETCH", buf, 5) != 0)
    return rc;

  rc = -2; /* we've got a FETCH response, for better or worse */
  buf = strchr(buf, '(');
  if (!buf)
    return rc;
  buf++;

  /* FIXME: current implementation - call msg_parse_fetch - if it returns -2,
   *   read header lines and call it again. Silly. */
  parse_rc = msg_parse_fetch(h, buf);
  if (!parse_rc)
    return 0;
  if (parse_rc != -2 || !fp)
    return rc;

  if (imap_get_literal_count(buf, &bytes) == 0)
  {
    imap_read_literal(fp, idata, bytes, NULL);

    /* we may have other fields of the FETCH _after_ the literal
     * (eg Domino puts FLAGS here). Nothing wrong with that, either.
     * This all has to go - we should accept literals and nonliterals
     * interchangeably at any time. */
    if (imap_cmd_step(idata) != IMAP_CMD_CONTINUE)
      return rc;

    if (msg_parse_fetch(h, idata->buf) == -1)
      return rc;
  }

  rc = 0; /* success */

  /* subtract headers from message size - unfortunately only the subset of
   * headers we've requested. */
  h->content_length -= bytes;

  return rc;
}
