connection_edge_compatible_with_circuit(const entry_connection_t *conn,
                                        const origin_circuit_t *circ)
{
  const uint8_t iso = conn->entry_cfg.isolation_flags;
  const socks_request_t *sr = conn->socks_request;

  /* If circ has never been used for an isolated connection, we can
   * totally use it for this one. */
  if (!circ->isolation_values_set)
    return 1;

  /* If circ has been used for connections having more than one value
   * for some field f, it will have the corresponding bit set in
   * isolation_flags_mixed.  If isolation_flags_mixed has any bits
   * in common with iso, then conn must be isolated from at least
   * one stream that has been attached to circ. */
  if ((iso & circ->isolation_flags_mixed) != 0) {
    /* For at least one field where conn is isolated, the circuit
     * already has mixed streams. */
    return 0;
  }

  if (! conn->original_dest_address) {
    log_warn(LD_BUG, "Reached connection_edge_compatible_with_circuit without "
             "having set conn->original_dest_address");
    ((entry_connection_t*)conn)->original_dest_address =
      tor_strdup(conn->socks_request->address);
  }

  if ((iso & ISO_STREAM) &&
      (circ->associated_isolated_stream_global_id !=
       ENTRY_TO_CONN(conn)->global_identifier))
    return 0;

  if ((iso & ISO_DESTPORT) && conn->socks_request->port != circ->dest_port)
    return 0;
  if ((iso & ISO_DESTADDR) &&
      strcasecmp(conn->original_dest_address, circ->dest_address))
    return 0;
  if ((iso & ISO_SOCKSAUTH) &&
      (! memeq_opt(sr->username, sr->usernamelen,
                   circ->socks_username, circ->socks_username_len) ||
       ! memeq_opt(sr->password, sr->passwordlen,
                   circ->socks_password, circ->socks_password_len)))
    return 0;
  if ((iso & ISO_CLIENTPROTO) &&
      (conn->socks_request->listener_type != circ->client_proto_type ||
       conn->socks_request->socks_version != circ->client_proto_socksver))
    return 0;
  if ((iso & ISO_CLIENTADDR) &&
      !tor_addr_eq(&ENTRY_TO_CONN(conn)->addr, &circ->client_addr))
    return 0;
  if ((iso & ISO_SESSIONGRP) &&
      conn->entry_cfg.session_group != circ->session_group)
    return 0;
  if ((iso & ISO_NYM_EPOCH) && conn->nym_epoch != circ->nym_epoch)
    return 0;

  return 1;
}
