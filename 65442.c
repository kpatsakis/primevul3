static struct BodyCache *msg_cache_open(struct ImapData *idata)
{
  char mailbox[PATH_MAX];

  if (idata->bcache)
    return idata->bcache;

  imap_cachepath(idata, idata->mailbox, mailbox, sizeof(mailbox));

  return mutt_bcache_open(&idata->conn->account, mailbox);
}
