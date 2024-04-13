void nntp_newsrc_close(struct NntpServer *nserv)
{
  if (!nserv->newsrc_fp)
    return;

  mutt_debug(1, "Unlocking %s\n", nserv->newsrc_file);
  mutt_file_unlock(fileno(nserv->newsrc_fp));
  mutt_file_fclose(&nserv->newsrc_fp);
}
