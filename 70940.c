static void nfc_llcp_sdreq_timer(struct timer_list *t)
{
	struct nfc_llcp_local *local = from_timer(local, t, sdreq_timer);

	schedule_work(&local->sdreq_timeout_work);
}
