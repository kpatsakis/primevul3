obtain_nonce(unsigned char* nonce)
{
  NTSTATUS status;

  status = BCryptGenRandom(NULL, nonce, 8,
			   BCRYPT_USE_SYSTEM_PREFERRED_RNG);

  if (!NT_SUCCESS(status))
    return (-1);

  return (0);
}
