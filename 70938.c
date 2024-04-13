static void nfc_llcp_rx_skb(struct nfc_llcp_local *local, struct sk_buff *skb)
{
	u8 dsap, ssap, ptype;

	ptype = nfc_llcp_ptype(skb);
	dsap = nfc_llcp_dsap(skb);
	ssap = nfc_llcp_ssap(skb);

	pr_debug("ptype 0x%x dsap 0x%x ssap 0x%x\n", ptype, dsap, ssap);

	if (ptype != LLCP_PDU_SYMM)
		print_hex_dump_debug("LLCP Rx: ", DUMP_PREFIX_OFFSET, 16, 1,
				     skb->data, skb->len, true);

	switch (ptype) {
	case LLCP_PDU_SYMM:
		pr_debug("SYMM\n");
		break;

	case LLCP_PDU_UI:
		pr_debug("UI\n");
		nfc_llcp_recv_ui(local, skb);
		break;

	case LLCP_PDU_CONNECT:
		pr_debug("CONNECT\n");
		nfc_llcp_recv_connect(local, skb);
		break;

	case LLCP_PDU_DISC:
		pr_debug("DISC\n");
		nfc_llcp_recv_disc(local, skb);
		break;

	case LLCP_PDU_CC:
		pr_debug("CC\n");
		nfc_llcp_recv_cc(local, skb);
		break;

	case LLCP_PDU_DM:
		pr_debug("DM\n");
		nfc_llcp_recv_dm(local, skb);
		break;

	case LLCP_PDU_SNL:
		pr_debug("SNL\n");
		nfc_llcp_recv_snl(local, skb);
		break;

	case LLCP_PDU_I:
	case LLCP_PDU_RR:
	case LLCP_PDU_RNR:
		pr_debug("I frame\n");
		nfc_llcp_recv_hdlc(local, skb);
		break;

	case LLCP_PDU_AGF:
		pr_debug("AGF frame\n");
		nfc_llcp_recv_agf(local, skb);
		break;
	}
}
