static void send_hsr_supervision_frame(struct hsr_port *master,
		u8 type, u8 hsrVer)
{
	struct sk_buff *skb;
	int hlen, tlen;
	struct hsr_tag *hsr_tag;
	struct hsr_sup_tag *hsr_stag;
	struct hsr_sup_payload *hsr_sp;
	unsigned long irqflags;

	hlen = LL_RESERVED_SPACE(master->dev);
	tlen = master->dev->needed_tailroom;
	skb = dev_alloc_skb(
			sizeof(struct hsr_tag) +
			sizeof(struct hsr_sup_tag) +
			sizeof(struct hsr_sup_payload) + hlen + tlen);

	if (skb == NULL)
		return;

	skb_reserve(skb, hlen);

	skb->dev = master->dev;
	skb->protocol = htons(hsrVer ? ETH_P_HSR : ETH_P_PRP);
	skb->priority = TC_PRIO_CONTROL;

	if (dev_hard_header(skb, skb->dev, (hsrVer ? ETH_P_HSR : ETH_P_PRP),
			    master->hsr->sup_multicast_addr,
			    skb->dev->dev_addr, skb->len) <= 0)
		goto out;
	skb_reset_mac_header(skb);

	if (hsrVer > 0) {
		hsr_tag = skb_put(skb, sizeof(struct hsr_tag));
		hsr_tag->encap_proto = htons(ETH_P_PRP);
		set_hsr_tag_LSDU_size(hsr_tag, HSR_V1_SUP_LSDUSIZE);
	}

	hsr_stag = skb_put(skb, sizeof(struct hsr_sup_tag));
	set_hsr_stag_path(hsr_stag, (hsrVer ? 0x0 : 0xf));
	set_hsr_stag_HSR_Ver(hsr_stag, hsrVer);

	/* From HSRv1 on we have separate supervision sequence numbers. */
	spin_lock_irqsave(&master->hsr->seqnr_lock, irqflags);
	if (hsrVer > 0) {
		hsr_stag->sequence_nr = htons(master->hsr->sup_sequence_nr);
		hsr_tag->sequence_nr = htons(master->hsr->sequence_nr);
		master->hsr->sup_sequence_nr++;
		master->hsr->sequence_nr++;
	} else {
		hsr_stag->sequence_nr = htons(master->hsr->sequence_nr);
		master->hsr->sequence_nr++;
	}
	spin_unlock_irqrestore(&master->hsr->seqnr_lock, irqflags);

	hsr_stag->HSR_TLV_Type = type;
	/* TODO: Why 12 in HSRv0? */
	hsr_stag->HSR_TLV_Length = hsrVer ? sizeof(struct hsr_sup_payload) : 12;

	/* Payload: MacAddressA */
	hsr_sp = skb_put(skb, sizeof(struct hsr_sup_payload));
	ether_addr_copy(hsr_sp->MacAddressA, master->dev->dev_addr);

	if (skb_put_padto(skb, ETH_ZLEN + HSR_HLEN))
		return;

	hsr_forward_skb(skb, master);
	return;

out:
	WARN_ONCE(1, "HSR: Could not send supervision frame\n");
	kfree_skb(skb);
}
