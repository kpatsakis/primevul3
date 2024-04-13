MOCK_IMPL(smartlist_t *,
nodelist_get_list,(void))
{
  init_nodelist();
  return the_nodelist->nodes;
}
