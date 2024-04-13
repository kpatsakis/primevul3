static u8 nfc_llcp_reserve_sdp_ssap(struct nfc_llcp_local *local)
{
	u8 ssap;

	mutex_lock(&local->sdp_lock);

	ssap = find_first_zero_bit(&local->local_sdp, LLCP_SDP_NUM_SAP);
	if (ssap == LLCP_SDP_NUM_SAP) {
		mutex_unlock(&local->sdp_lock);

		return LLCP_SAP_MAX;
	}

	pr_debug("SDP ssap %d\n", LLCP_WKS_NUM_SAP + ssap);

	set_bit(ssap, &local->local_sdp);

	mutex_unlock(&local->sdp_lock);

	return LLCP_WKS_NUM_SAP + ssap;
}
