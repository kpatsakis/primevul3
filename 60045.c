node_get_mutable_by_id(const char *identity_digest)
{
  node_t search, *node;
  if (PREDICT_UNLIKELY(the_nodelist == NULL))
    return NULL;

  memcpy(&search.identity, identity_digest, DIGEST_LEN);
  node = HT_FIND(nodelist_map, &the_nodelist->nodes_by_id, &search);
  return node;
}
