connection_edge_update_circuit_isolation(const entry_connection_t *conn,
                                         origin_circuit_t *circ,
                                         int dry_run)
{
  const socks_request_t *sr = conn->socks_request;
  if (! conn->original_dest_address) {
    log_warn(LD_BUG, "Reached connection_update_circuit_isolation without "
             "having set conn->original_dest_address");
    ((entry_connection_t*)conn)->original_dest_address =
      tor_strdup(conn->socks_request->address);
  }

  if (!circ->isolation_values_set) {
    if (dry_run)
      return -1;
    circ->associated_isolated_stream_global_id =
      ENTRY_TO_CONN(conn)->global_identifier;
    circ->dest_port = conn->socks_request->port;
    circ->dest_address = tor_strdup(conn->original_dest_address);
    circ->client_proto_type = conn->socks_request->listener_type;
    circ->client_proto_socksver = conn->socks_request->socks_version;
    tor_addr_copy(&circ->client_addr, &ENTRY_TO_CONN(conn)->addr);
    circ->session_group = conn->entry_cfg.session_group;
    circ->nym_epoch = conn->nym_epoch;
    circ->socks_username = sr->username ?
      tor_memdup(sr->username, sr->usernamelen) : NULL;
    circ->socks_password = sr->password ?
      tor_memdup(sr->password, sr->passwordlen) : NULL;
    circ->socks_username_len = sr->usernamelen;
    circ->socks_password_len = sr->passwordlen;

    circ->isolation_values_set = 1;
    return 0;
  } else {
    uint8_t mixed = 0;
    if (conn->socks_request->port != circ->dest_port)
      mixed |= ISO_DESTPORT;
    if (strcasecmp(conn->original_dest_address, circ->dest_address))
      mixed |= ISO_DESTADDR;
    if (!memeq_opt(sr->username, sr->usernamelen,
                   circ->socks_username, circ->socks_username_len) ||
        !memeq_opt(sr->password, sr->passwordlen,
                   circ->socks_password, circ->socks_password_len))
      mixed |= ISO_SOCKSAUTH;
    if ((conn->socks_request->listener_type != circ->client_proto_type ||
         conn->socks_request->socks_version != circ->client_proto_socksver))
      mixed |= ISO_CLIENTPROTO;
    if (!tor_addr_eq(&ENTRY_TO_CONN(conn)->addr, &circ->client_addr))
      mixed |= ISO_CLIENTADDR;
    if (conn->entry_cfg.session_group != circ->session_group)
      mixed |= ISO_SESSIONGRP;
    if (conn->nym_epoch != circ->nym_epoch)
      mixed |= ISO_NYM_EPOCH;

    if (dry_run)
      return mixed;

    if ((mixed & conn->entry_cfg.isolation_flags) != 0) {
      log_warn(LD_BUG, "Updating a circuit with seemingly incompatible "
               "isolation flags.");
    }
    circ->isolation_flags_mixed |= mixed;
    return 0;
  }
}
