static int bzImage64_verify_sig(const char *kernel, unsigned long kernel_len)
{
	bool trusted;
	int ret;

	ret = verify_pefile_signature(kernel, kernel_len,
				      system_trusted_keyring,
				      VERIFYING_KEXEC_PE_SIGNATURE,
				      &trusted);
	if (ret < 0)
		return ret;
	if (!trusted)
		return -EKEYREJECTED;
	return 0;
}
