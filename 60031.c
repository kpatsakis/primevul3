init_nodelist(void)
{
  if (PREDICT_UNLIKELY(the_nodelist == NULL)) {
    the_nodelist = tor_malloc_zero(sizeof(nodelist_t));
    HT_INIT(nodelist_map, &the_nodelist->nodes_by_id);
    the_nodelist->nodes = smartlist_new();
  }
}
