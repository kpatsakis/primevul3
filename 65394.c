void nntp_newsrc_gen_entries(struct Context *ctx)
{
  struct NntpData *nntp_data = ctx->data;
  anum_t last = 0, first = 1;
  bool series;
  int save_sort = SORT_ORDER;
  unsigned int entries;

  if (Sort != SORT_ORDER)
  {
    save_sort = Sort;
    Sort = SORT_ORDER;
    mutt_sort_headers(ctx, 0);
  }

  entries = nntp_data->newsrc_len;
  if (!entries)
  {
    entries = 5;
    nntp_data->newsrc_ent = mutt_mem_calloc(entries, sizeof(struct NewsrcEntry));
  }

  /* Set up to fake initial sequence from 1 to the article before the
   * first article in our list */
  nntp_data->newsrc_len = 0;
  series = true;
  for (int i = 0; i < ctx->msgcount; i++)
  {
    /* search for first unread */
    if (series)
    {
      /* We don't actually check sequential order, since we mark
       * "missing" entries as read/deleted */
      last = NHDR(ctx->hdrs[i])->article_num;
      if (last >= nntp_data->first_message && !ctx->hdrs[i]->deleted &&
          !ctx->hdrs[i]->read)
      {
        if (nntp_data->newsrc_len >= entries)
        {
          entries *= 2;
          mutt_mem_realloc(&nntp_data->newsrc_ent, entries * sizeof(struct NewsrcEntry));
        }
        nntp_data->newsrc_ent[nntp_data->newsrc_len].first = first;
        nntp_data->newsrc_ent[nntp_data->newsrc_len].last = last - 1;
        nntp_data->newsrc_len++;
        series = false;
      }
    }

    /* search for first read */
    else
    {
      if (ctx->hdrs[i]->deleted || ctx->hdrs[i]->read)
      {
        first = last + 1;
        series = true;
      }
      last = NHDR(ctx->hdrs[i])->article_num;
    }
  }

  if (series && first <= nntp_data->last_loaded)
  {
    if (nntp_data->newsrc_len >= entries)
    {
      entries++;
      mutt_mem_realloc(&nntp_data->newsrc_ent, entries * sizeof(struct NewsrcEntry));
    }
    nntp_data->newsrc_ent[nntp_data->newsrc_len].first = first;
    nntp_data->newsrc_ent[nntp_data->newsrc_len].last = nntp_data->last_loaded;
    nntp_data->newsrc_len++;
  }
  mutt_mem_realloc(&nntp_data->newsrc_ent, nntp_data->newsrc_len * sizeof(struct NewsrcEntry));

  if (save_sort != Sort)
  {
    Sort = save_sort;
    mutt_sort_headers(ctx, 0);
  }
}
