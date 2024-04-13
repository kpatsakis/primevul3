connection_ap_process_end_not_open(
    relay_header_t *rh, cell_t *cell, origin_circuit_t *circ,
    entry_connection_t *conn, crypt_path_t *layer_hint)
{
  node_t *exitrouter;
  int reason = *(cell->payload+RELAY_HEADER_SIZE);
  int control_reason;
  edge_connection_t *edge_conn = ENTRY_TO_EDGE_CONN(conn);
  (void) layer_hint; /* unused */

  if (rh->length > 0) {
    if (reason == END_STREAM_REASON_TORPROTOCOL ||
        reason == END_STREAM_REASON_DESTROY) {
      /* Both of these reasons could mean a failed tag
       * hit the exit and it complained. Do not probe.
       * Fail the circuit. */
      circ->path_state = PATH_STATE_USE_FAILED;
      return -END_CIRC_REASON_TORPROTOCOL;
    } else if (reason == END_STREAM_REASON_INTERNAL) {
      /* We can't infer success or failure, since older Tors report
       * ENETUNREACH as END_STREAM_REASON_INTERNAL. */
    } else {
      /* Path bias: If we get a valid reason code from the exit,
       * it wasn't due to tagging.
       *
       * We rely on recognized+digest being strong enough to make
       * tags unlikely to allow us to get tagged, yet 'recognized'
       * reason codes here. */
      pathbias_mark_use_success(circ);
    }
  }

  if (rh->length == 0) {
    reason = END_STREAM_REASON_MISC;
  }

  control_reason = reason | END_STREAM_REASON_FLAG_REMOTE;

  if (edge_reason_is_retriable(reason) &&
      /* avoid retry if rend */
      !connection_edge_is_rendezvous_stream(edge_conn)) {
    const char *chosen_exit_digest =
      circ->build_state->chosen_exit->identity_digest;
    log_info(LD_APP,"Address '%s' refused due to '%s'. Considering retrying.",
             safe_str(conn->socks_request->address),
             stream_end_reason_to_string(reason));
    exitrouter = node_get_mutable_by_id(chosen_exit_digest);
    switch (reason) {
      case END_STREAM_REASON_EXITPOLICY: {
        tor_addr_t addr;
        tor_addr_make_unspec(&addr);
        if (rh->length >= 5) {
          int ttl = -1;
          tor_addr_make_unspec(&addr);
          if (rh->length == 5 || rh->length == 9) {
            tor_addr_from_ipv4n(&addr,
                                get_uint32(cell->payload+RELAY_HEADER_SIZE+1));
            if (rh->length == 9)
              ttl = (int)ntohl(get_uint32(cell->payload+RELAY_HEADER_SIZE+5));
          } else if (rh->length == 17 || rh->length == 21) {
            tor_addr_from_ipv6_bytes(&addr,
                                (char*)(cell->payload+RELAY_HEADER_SIZE+1));
            if (rh->length == 21)
              ttl = (int)ntohl(get_uint32(cell->payload+RELAY_HEADER_SIZE+17));
          }
          if (tor_addr_is_null(&addr)) {
            log_info(LD_APP,"Address '%s' resolved to 0.0.0.0. Closing,",
                     safe_str(conn->socks_request->address));
            connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
            return 0;
          }

          if ((tor_addr_family(&addr) == AF_INET && !conn->ipv4_traffic_ok) ||
              (tor_addr_family(&addr) == AF_INET6 && !conn->ipv6_traffic_ok)) {
            log_fn(LOG_PROTOCOL_WARN, LD_APP,
                   "Got an EXITPOLICY failure on a connection with a "
                   "mismatched family. Closing.");
            connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
            return 0;
          }
          if (get_options()->ClientDNSRejectInternalAddresses &&
              tor_addr_is_internal(&addr, 0)) {
            log_info(LD_APP,"Address '%s' resolved to internal. Closing,",
                     safe_str(conn->socks_request->address));
            connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
            return 0;
          }

          client_dns_set_addressmap(conn,
                                    conn->socks_request->address, &addr,
                                    conn->chosen_exit_name, ttl);

          {
            char new_addr[TOR_ADDR_BUF_LEN];
            tor_addr_to_str(new_addr, &addr, sizeof(new_addr), 1);
            if (strcmp(conn->socks_request->address, new_addr)) {
              strlcpy(conn->socks_request->address, new_addr,
                      sizeof(conn->socks_request->address));
              control_event_stream_status(conn, STREAM_EVENT_REMAP, 0);
            }
          }
        }
        /* check if he *ought* to have allowed it */

        adjust_exit_policy_from_exitpolicy_failure(circ,
                                                   conn,
                                                   exitrouter,
                                                   &addr);

        if (conn->chosen_exit_optional ||
            conn->chosen_exit_retries) {
          /* stop wanting a specific exit */
          conn->chosen_exit_optional = 0;
          /* A non-zero chosen_exit_retries can happen if we set a
           * TrackHostExits for this address under a port that the exit
           * relay allows, but then try the same address with a different
           * port that it doesn't allow to exit. We shouldn't unregister
           * the mapping, since it is probably still wanted on the
           * original port. But now we give away to the exit relay that
           * we probably have a TrackHostExits on it. So be it. */
          conn->chosen_exit_retries = 0;
          tor_free(conn->chosen_exit_name); /* clears it */
        }
        if (connection_ap_detach_retriable(conn, circ, control_reason) >= 0)
          return 0;
        /* else, conn will get closed below */
        break;
      }
      case END_STREAM_REASON_CONNECTREFUSED:
        if (!conn->chosen_exit_optional)
          break; /* break means it'll close, below */
        /* Else fall through: expire this circuit, clear the
         * chosen_exit_name field, and try again. */
      case END_STREAM_REASON_RESOLVEFAILED:
      case END_STREAM_REASON_TIMEOUT:
      case END_STREAM_REASON_MISC:
      case END_STREAM_REASON_NOROUTE:
        if (client_dns_incr_failures(conn->socks_request->address)
            < MAX_RESOLVE_FAILURES) {
          /* We haven't retried too many times; reattach the connection. */
          circuit_log_path(LOG_INFO,LD_APP,circ);
          /* Mark this circuit "unusable for new streams". */
          mark_circuit_unusable_for_new_conns(circ);

          if (conn->chosen_exit_optional) {
            /* stop wanting a specific exit */
            conn->chosen_exit_optional = 0;
            tor_free(conn->chosen_exit_name); /* clears it */
          }
          if (connection_ap_detach_retriable(conn, circ, control_reason) >= 0)
            return 0;
          /* else, conn will get closed below */
        } else {
          log_notice(LD_APP,
                     "Have tried resolving or connecting to address '%s' "
                     "at %d different places. Giving up.",
                     safe_str(conn->socks_request->address),
                     MAX_RESOLVE_FAILURES);
          /* clear the failures, so it will have a full try next time */
          client_dns_clear_failures(conn->socks_request->address);
        }
        break;
      case END_STREAM_REASON_HIBERNATING:
      case END_STREAM_REASON_RESOURCELIMIT:
        if (exitrouter) {
          policies_set_node_exitpolicy_to_reject_all(exitrouter);
        }
        if (conn->chosen_exit_optional) {
          /* stop wanting a specific exit */
          conn->chosen_exit_optional = 0;
          tor_free(conn->chosen_exit_name); /* clears it */
        }
        if (connection_ap_detach_retriable(conn, circ, control_reason) >= 0)
          return 0;
        /* else, will close below */
        break;
    } /* end switch */
    log_info(LD_APP,"Giving up on retrying; conn can't be handled.");
  }

  log_info(LD_APP,
           "Edge got end (%s) before we're connected. Marking for close.",
       stream_end_reason_to_string(rh->length > 0 ? reason : -1));
  circuit_log_path(LOG_INFO,LD_APP,circ);
  /* need to test because of detach_retriable */
  if (!ENTRY_TO_CONN(conn)->marked_for_close)
    connection_mark_unattached_ap(conn, control_reason);
  return 0;
}
