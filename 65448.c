static void update_context(struct ImapData *idata, int oldmsgcount)
{
  struct Header *h = NULL;

  struct Context *ctx = idata->ctx;
  if (!idata->uid_hash)
    idata->uid_hash = mutt_hash_int_create(MAX(6 * ctx->msgcount / 5, 30), 0);

  for (int msgno = oldmsgcount; msgno < ctx->msgcount; msgno++)
  {
    h = ctx->hdrs[msgno];
    mutt_hash_int_insert(idata->uid_hash, HEADER_DATA(h)->uid, h);
  }
}
