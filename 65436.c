int imap_msg_commit(struct Context *ctx, struct Message *msg)
{
  int r = mutt_file_fclose(&msg->fp);
  if (r != 0)
    return r;

  return imap_append_message(ctx, msg);
}
