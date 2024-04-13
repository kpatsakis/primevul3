void nfc_llcp_recv(void *data, struct sk_buff *skb, int err)
{
	struct nfc_llcp_local *local = (struct nfc_llcp_local *) data;

	pr_debug("Received an LLCP PDU\n");
	if (err < 0) {
		pr_err("err %d\n", err);
		return;
	}

	__nfc_llcp_recv(local, skb);
}
