relay_crypt_one_payload(crypto_cipher_t *cipher, uint8_t *in,
                        int encrypt_mode)
{
  int r;
  (void)encrypt_mode;
  r = crypto_cipher_crypt_inplace(cipher, (char*) in, CELL_PAYLOAD_SIZE);

  if (r) {
    log_warn(LD_BUG,"Error during relay encryption");
    return -1;
  }
  return 0;
}
