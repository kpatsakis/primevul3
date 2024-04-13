node_get_by_hex_id(const char *hex_id)
{
  char digest_buf[DIGEST_LEN];
  char nn_buf[MAX_NICKNAME_LEN+1];
  char nn_char='\0';

  if (hex_digest_nickname_decode(hex_id, digest_buf, &nn_char, nn_buf)==0) {
    const node_t *node = node_get_by_id(digest_buf);
    if (!node)
      return NULL;
    if (nn_char) {
      const char *real_name = node_get_nickname(node);
      if (!real_name || strcasecmp(real_name, nn_buf))
        return NULL;
      if (nn_char == '=') {
        const char *named_id =
          networkstatus_get_router_digest_by_nickname(nn_buf);
        if (!named_id || tor_memneq(named_id, digest_buf, DIGEST_LEN))
          return NULL;
      }
    }
    return node;
  }

  return NULL;
}
