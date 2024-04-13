static struct ImapHeaderData *new_header_data(void)
{
  struct ImapHeaderData *d = mutt_mem_calloc(1, sizeof(struct ImapHeaderData));
  return d;
}
