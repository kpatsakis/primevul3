static u8 *nfc_llcp_connect_sn(struct sk_buff *skb, size_t *sn_len)
{
	u8 *tlv = &skb->data[2], type, length;
	size_t tlv_array_len = skb->len - LLCP_HEADER_SIZE, offset = 0;

	while (offset < tlv_array_len) {
		type = tlv[0];
		length = tlv[1];

		pr_debug("type 0x%x length %d\n", type, length);

		if (type == LLCP_TLV_SN) {
			*sn_len = length;
			return &tlv[2];
		}

		offset += length + 2;
		tlv += length + 2;
	}

	return NULL;
}
