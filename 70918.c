u8 nfc_llcp_get_local_ssap(struct nfc_llcp_local *local)
{
	u8 local_ssap;

	mutex_lock(&local->sdp_lock);

	local_ssap = find_first_zero_bit(&local->local_sap, LLCP_LOCAL_NUM_SAP);
	if (local_ssap == LLCP_LOCAL_NUM_SAP) {
		mutex_unlock(&local->sdp_lock);
		return LLCP_SAP_MAX;
	}

	set_bit(local_ssap, &local->local_sap);

	mutex_unlock(&local->sdp_lock);

	return local_ssap + LLCP_LOCAL_SAP_OFFSET;
}
