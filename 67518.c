write_chalresp_state(FILE *f, CR_STATE *state)
{
  char challenge_hex[CR_CHALLENGE_SIZE * 2 + 1], response_hex[CR_RESPONSE_SIZE * 2 + 1];
  char salt_hex[CR_SALT_SIZE * 2 + 1], hashed_hex[CR_RESPONSE_SIZE * 2 + 1];
  unsigned char salt[CR_SALT_SIZE], hash[CR_RESPONSE_SIZE];
  YK_PRF_METHOD prf_method = {20, yk_hmac_sha1};
  unsigned int iterations = CR_DEFAULT_ITERATIONS;
  int fd;

  memset(challenge_hex, 0, sizeof(challenge_hex));
  memset(response_hex, 0, sizeof(response_hex));
  memset(salt_hex, 0, sizeof(salt_hex));
  memset(hashed_hex, 0, sizeof(hashed_hex));

  yubikey_hex_encode(challenge_hex, (char *)state->challenge, state->challenge_len);
  yubikey_hex_encode(response_hex, (char *)state->response, state->response_len);

  if(state->iterations > 0) {
    iterations = state->iterations;
  }

  generate_random(salt, CR_SALT_SIZE);
  yk_pbkdf2(response_hex, salt, CR_SALT_SIZE, iterations,
      hash, CR_RESPONSE_SIZE, &prf_method);

  yubikey_hex_encode(hashed_hex, (char *)hash, CR_RESPONSE_SIZE);
  yubikey_hex_encode(salt_hex, (char *)salt, CR_SALT_SIZE);

  rewind(f);

  fd = fileno(f);
  if (fd == -1)
    goto out;

  if (ftruncate(fd, 0))
    goto out;

  fprintf(f, "v2:%s:%s:%s:%u:%d\n", challenge_hex, hashed_hex, salt_hex, iterations, state->slot);

  if (fflush(f) < 0)
    goto out;

  if (fsync(fd) < 0)
    goto out;

  return 1;
 out:
  return 0;
}
