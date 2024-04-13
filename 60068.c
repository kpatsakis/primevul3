node_is_dir(const node_t *node)
{
  if (node->rs) {
    routerstatus_t * rs = node->rs;
    /* This is true if supports_tunnelled_dir_requests is true which
     * indicates that we support directory request tunnelled or through the
     * DirPort. */
    return rs->is_v2_dir;
  } else if (node->ri) {
    routerinfo_t * ri = node->ri;
    /* Both tunnelled request is supported or DirPort is set. */
    return ri->supports_tunnelled_dir_requests;
  } else {
    return 0;
  }
}
