static void pop_clear_cache(struct PopData *pop_data)
{
  if (!pop_data->clear_cache)
    return;

  mutt_debug(1, "delete cached messages\n");

  for (int i = 0; i < POP_CACHE_LEN; i++)
  {
    if (pop_data->cache[i].path)
    {
      unlink(pop_data->cache[i].path);
      FREE(&pop_data->cache[i].path);
    }
  }
}
