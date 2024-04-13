microdesc_has_curve25519_onion_key(const microdesc_t *md)
{
  if (!md) {
    return 0;
  }

  if (!md->onion_curve25519_pkey) {
    return 0;
  }

  if (tor_mem_is_zero((const char*)md->onion_curve25519_pkey->public_key,
                      CURVE25519_PUBKEY_LEN)) {
    return 0;
  }

  return 1;
}
