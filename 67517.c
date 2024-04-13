load_chalresp_state(FILE *f, CR_STATE *state, bool verbose, FILE *debug_file)
{
  /*
   * Load the current challenge and expected response information from a file handle.
   *
   * Format is hex(challenge):hex(response):slot num
   */
  char challenge_hex[CR_CHALLENGE_SIZE * 2 + 1], response_hex[CR_RESPONSE_SIZE * 2 + 1];
  char salt_hex[CR_SALT_SIZE * 2 + 1];
  unsigned int iterations;
  int slot;
  int r;

  if (! f)
    goto out;

  /* XXX not ideal with hard coded lengths in this scan string.
   * 126 corresponds to twice the size of CR_CHALLENGE_SIZE,
   * 40 is twice the size of CR_RESPONSE_SIZE
   * (twice because we hex encode the challenge and response)
   */
  r = fscanf(f, "v2:%126[0-9a-z]:%40[0-9a-z]:%64[0-9a-z]:%d:%d", challenge_hex, response_hex, salt_hex, &iterations, &slot);
  if(r == 5) {
    if (! yubikey_hex_p(salt_hex)) {
      D(debug_file, "Invalid salt hex input : %s", salt_hex);
      goto out;
    }

    if(verbose) {
      D(debug_file, "Challenge: %s, hashed response: %s, salt: %s, iterations: %d, slot: %d",
            challenge_hex, response_hex, salt_hex, iterations, slot);
    }

    yubikey_hex_decode(state->salt, salt_hex, sizeof(state->salt));
    state->salt_len = strlen(salt_hex) / 2;
  } else {
    rewind(f);
    r = fscanf(f, "v1:%126[0-9a-z]:%40[0-9a-z]:%d", challenge_hex, response_hex, &slot);
    if (r != 3) {
      D(debug_file, "Could not parse contents of chalresp_state file (%i)", r);
      goto out;
    }

    if (verbose) {
      D(debug_file, "Challenge: %s, expected response: %s, slot: %d", challenge_hex, response_hex, slot);
    }

    iterations = CR_DEFAULT_ITERATIONS;
  }

  state->iterations = iterations;


  if (! yubikey_hex_p(challenge_hex)) {
    D(debug_file, "Invalid challenge hex input : %s", challenge_hex);
    goto out;
  }

  if (! yubikey_hex_p(response_hex)) {
    D(debug_file, "Invalid expected response hex input : %s", response_hex);
    goto out;
  }

  if (slot != 1 && slot != 2) {
    D(debug_file, "Invalid slot input : %i", slot);
    goto out;
  }

  yubikey_hex_decode(state->challenge, challenge_hex, sizeof(state->challenge));
  state->challenge_len = strlen(challenge_hex) / 2;

  yubikey_hex_decode(state->response, response_hex, sizeof(state->response));
  state->response_len = strlen(response_hex) / 2;

  state->slot = slot;

  return 1;

 out:
  return 0;
}
