connection_edge_free_all(void)
{
  untried_pending_connections = 0;
  smartlist_free(pending_entry_connections);
  pending_entry_connections = NULL;
}
