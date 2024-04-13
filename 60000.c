guards_choose_guard(cpath_build_state_t *state,
                   circuit_guard_state_t **guard_state_out)
{
  const node_t *r = NULL;
  const uint8_t *exit_id = NULL;
  entry_guard_restriction_t *rst = NULL;
  if (state && (exit_id = build_state_get_exit_rsa_id(state))) {
    /* We're building to a targeted exit node, so that node can't be
     * chosen as our guard for this circuit.  Remember that fact in a
     * restriction. */
    rst = tor_malloc_zero(sizeof(entry_guard_restriction_t));
    memcpy(rst->exclude_id, exit_id, DIGEST_LEN);
  }
  if (entry_guard_pick_for_circuit(get_guard_selection_info(),
                                   GUARD_USAGE_TRAFFIC,
                                   rst,
                                   &r,
                                   guard_state_out) < 0) {
    tor_assert(r == NULL);
  }
  return r;
}
