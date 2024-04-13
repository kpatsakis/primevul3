circuit_clear_isolation(origin_circuit_t *circ)
{
  if (circ->isolation_any_streams_attached) {
    log_warn(LD_BUG, "Tried to clear the isolation status of a dirty circuit");
    return;
  }
  if (TO_CIRCUIT(circ)->state != CIRCUIT_STATE_OPEN) {
    log_warn(LD_BUG, "Tried to clear the isolation status of a non-open "
             "circuit");
    return;
  }

  circ->isolation_values_set = 0;
  circ->isolation_flags_mixed = 0;
  circ->associated_isolated_stream_global_id = 0;
  circ->client_proto_type = 0;
  circ->client_proto_socksver = 0;
  circ->dest_port = 0;
  tor_addr_make_unspec(&circ->client_addr);
  tor_free(circ->dest_address);
  circ->session_group = -1;
  circ->nym_epoch = 0;
  if (circ->socks_username) {
    memwipe(circ->socks_username, 0x11, circ->socks_username_len);
    tor_free(circ->socks_username);
  }
  if (circ->socks_password) {
    memwipe(circ->socks_password, 0x05, circ->socks_password_len);
    tor_free(circ->socks_password);
  }
  circ->socks_username_len = circ->socks_password_len = 0;
}
