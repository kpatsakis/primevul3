live_consensus_is_missing(const guard_selection_t *gs)
{
  tor_assert(gs);
  if (gs->type == GS_TYPE_BRIDGE) {
    /* We don't update bridges from the consensus; they aren't there. */
    return 0;
  }
  return networkstatus_get_live_consensus(approx_time()) == NULL;
}
