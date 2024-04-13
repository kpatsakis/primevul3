node_get_platform(const node_t *node)
{
  /* If we wanted, we could record the version in the routerstatus_t, since
   * the consensus lists it.  We don't, though, so this function just won't
   * work with microdescriptors. */
  if (node->ri)
    return node->ri->platform;
  else
    return NULL;
}
