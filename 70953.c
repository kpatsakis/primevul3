static void nfc_llcp_timeout_work(struct work_struct *work)
{
	struct nfc_llcp_local *local = container_of(work, struct nfc_llcp_local,
						    timeout_work);

	nfc_dep_link_down(local->dev);
}
