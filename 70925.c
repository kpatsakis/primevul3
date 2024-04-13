void nfc_llcp_put_ssap(struct nfc_llcp_local *local, u8 ssap)
{
	u8 local_ssap;
	unsigned long *sdp;

	if (ssap < LLCP_WKS_NUM_SAP) {
		local_ssap = ssap;
		sdp = &local->local_wks;
	} else if (ssap < LLCP_LOCAL_NUM_SAP) {
		atomic_t *client_cnt;

		local_ssap = ssap - LLCP_WKS_NUM_SAP;
		sdp = &local->local_sdp;
		client_cnt = &local->local_sdp_cnt[local_ssap];

		pr_debug("%d clients\n", atomic_read(client_cnt));

		mutex_lock(&local->sdp_lock);

		if (atomic_dec_and_test(client_cnt)) {
			struct nfc_llcp_sock *l_sock;

			pr_debug("No more clients for SAP %d\n", ssap);

			clear_bit(local_ssap, sdp);

			/* Find the listening sock and set it back to UNBOUND */
			l_sock = nfc_llcp_sock_get(local, ssap, LLCP_SAP_SDP);
			if (l_sock) {
				l_sock->ssap = LLCP_SDP_UNBOUND;
				nfc_llcp_sock_put(l_sock);
			}
		}

		mutex_unlock(&local->sdp_lock);

		return;
	} else if (ssap < LLCP_MAX_SAP) {
		local_ssap = ssap - LLCP_LOCAL_NUM_SAP;
		sdp = &local->local_sap;
	} else {
		return;
	}

	mutex_lock(&local->sdp_lock);

	clear_bit(local_ssap, sdp);

	mutex_unlock(&local->sdp_lock);
}
