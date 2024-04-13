connected_cell_parse(const relay_header_t *rh, const cell_t *cell,
                     tor_addr_t *addr_out, int *ttl_out)
{
  uint32_t bytes;
  const uint8_t *payload = cell->payload + RELAY_HEADER_SIZE;

  tor_addr_make_unspec(addr_out);
  *ttl_out = -1;
  if (rh->length == 0)
    return 0;
  if (rh->length < 4)
    return -1;
  bytes = ntohl(get_uint32(payload));

  /* If bytes is 0, this is maybe a v6 address. Otherwise it's a v4 address */
  if (bytes != 0) {
    /* v4 address */
    tor_addr_from_ipv4h(addr_out, bytes);
    if (rh->length >= 8) {
      bytes = ntohl(get_uint32(payload + 4));
      if (bytes <= INT32_MAX)
        *ttl_out = bytes;
    }
  } else {
    if (rh->length < 25) /* 4 bytes of 0s, 1 addr, 16 ipv4, 4 ttl. */
      return -1;
    if (get_uint8(payload + 4) != 6)
      return -1;
    tor_addr_from_ipv6_bytes(addr_out, (char*)(payload + 5));
    bytes = ntohl(get_uint32(payload + 21));
    if (bytes <= INT32_MAX)
      *ttl_out = (int) bytes;
  }
  return 0;
}
