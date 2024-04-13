node_get_verbose_nickname_by_id(const char *id_digest,
                                char *verbose_name_out)
{
  const node_t *node = node_get_by_id(id_digest);
  if (!node) {
    verbose_name_out[0] = '$';
    base16_encode(verbose_name_out+1, HEX_DIGEST_LEN+1, id_digest, DIGEST_LEN);
  } else {
    node_get_verbose_nickname(node, verbose_name_out);
  }
}
