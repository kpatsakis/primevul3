circuit_consider_sending_sendme(circuit_t *circ, crypt_path_t *layer_hint)
{
  while ((layer_hint ? layer_hint->deliver_window : circ->deliver_window) <=
          CIRCWINDOW_START - CIRCWINDOW_INCREMENT) {
    log_debug(LD_CIRC,"Queuing circuit sendme.");
    if (layer_hint)
      layer_hint->deliver_window += CIRCWINDOW_INCREMENT;
    else
      circ->deliver_window += CIRCWINDOW_INCREMENT;
    if (relay_send_command_from_edge(0, circ, RELAY_COMMAND_SENDME,
                                     NULL, 0, layer_hint) < 0) {
      log_warn(LD_CIRC,
               "relay_send_command_from_edge failed. Circuit's closed.");
      return; /* the circuit's closed, don't continue */
    }
  }
}
