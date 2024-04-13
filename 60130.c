remap_event_helper(entry_connection_t *conn, const tor_addr_t *new_addr)
{
  tor_addr_to_str(conn->socks_request->address, new_addr,
                  sizeof(conn->socks_request->address),
                  1);
  control_event_stream_status(conn, STREAM_EVENT_REMAP,
                              REMAP_STREAM_SOURCE_EXIT);
}
