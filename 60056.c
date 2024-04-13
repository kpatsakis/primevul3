node_get_verbose_nickname(const node_t *node,
                          char *verbose_name_out)
{
  const char *nickname = node_get_nickname(node);
  int is_named = node_is_named(node);
  verbose_name_out[0] = '$';
  base16_encode(verbose_name_out+1, HEX_DIGEST_LEN+1, node->identity,
                DIGEST_LEN);
  if (!nickname)
    return;
  verbose_name_out[1+HEX_DIGEST_LEN] = is_named ? '=' : '~';
  strlcpy(verbose_name_out+1+HEX_DIGEST_LEN+1, nickname, MAX_NICKNAME_LEN+1);
}
