decode_address_from_payload(tor_addr_t *addr_out, const uint8_t *payload,
                            int payload_len)
{
  if (payload_len < 2)
    return NULL;
  if (payload_len < 2+payload[1])
    return NULL;

  switch (payload[0]) {
  case RESOLVED_TYPE_IPV4:
    if (payload[1] != 4)
      return NULL;
    tor_addr_from_ipv4n(addr_out, get_uint32(payload+2));
    break;
  case RESOLVED_TYPE_IPV6:
    if (payload[1] != 16)
      return NULL;
    tor_addr_from_ipv6_bytes(addr_out, (char*)(payload+2));
    break;
  default:
    tor_addr_make_unspec(addr_out);
    break;
  }
  return payload + 2 + payload[1];
}
