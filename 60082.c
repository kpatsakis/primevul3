nodelist_refresh_countries(void)
{
  smartlist_t *nodes = nodelist_get_list();
  SMARTLIST_FOREACH(nodes, node_t *, node,
                    node_set_country(node));
}
