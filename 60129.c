relay_set_digest(crypto_digest_t *digest, cell_t *cell)
{
  char integrity[4];
  relay_header_t rh;

  crypto_digest_add_bytes(digest, (char*)cell->payload, CELL_PAYLOAD_SIZE);
  crypto_digest_get_digest(digest, integrity, 4);
  relay_header_unpack(&rh, cell->payload);
  memcpy(rh.integrity, integrity, 4);
  relay_header_pack(cell->payload, &rh);
}
