void nntp_acache_free(struct NntpData *nntp_data)
{
  for (int i = 0; i < NNTP_ACACHE_LEN; i++)
  {
    if (nntp_data->acache[i].path)
    {
      unlink(nntp_data->acache[i].path);
      FREE(&nntp_data->acache[i].path);
    }
  }
}
