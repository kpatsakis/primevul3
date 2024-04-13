static void local_release(struct kref *ref)
{
	struct nfc_llcp_local *local;

	local = container_of(ref, struct nfc_llcp_local, ref);

	list_del(&local->list);
	local_cleanup(local);
	kfree(local);
}
