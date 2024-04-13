int nfc_llcp_local_put(struct nfc_llcp_local *local)
{
	if (local == NULL)
		return 0;

	return kref_put(&local->ref, local_release);
}
