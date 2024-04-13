relay_header_unpack(relay_header_t *dest, const uint8_t *src)
{
  dest->command = get_uint8(src);
  dest->recognized = ntohs(get_uint16(src+1));
  dest->stream_id = ntohs(get_uint16(src+3));
  memcpy(dest->integrity, src+5, 4);
  dest->length = ntohs(get_uint16(src+9));
}
