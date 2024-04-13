connection_edge_flushed_some(edge_connection_t *conn)
{
  switch (conn->base_.state) {
    case AP_CONN_STATE_OPEN:
    case EXIT_CONN_STATE_OPEN:
      connection_edge_consider_sending_sendme(conn);
      break;
  }
  return 0;
}
