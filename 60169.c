connection_edge_is_rendezvous_stream(const edge_connection_t *conn)
{
  tor_assert(conn);
  if (conn->rend_data)
    return 1;
  return 0;
}
