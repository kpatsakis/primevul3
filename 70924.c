static u8 nfc_llcp_ns(struct sk_buff *pdu)
{
	return pdu->data[2] >> 4;
}
