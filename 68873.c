static int hns_nic_rx_poll_one(struct hns_nic_ring_data *ring_data,
			       int budget, void *v)
{
	struct hnae_ring *ring = ring_data->ring;
	struct sk_buff *skb;
	int num, bnum;
#define RCB_NOF_ALLOC_RX_BUFF_ONCE 16
	int recv_pkts, recv_bds, clean_count, err;
	int unused_count = hns_desc_unused(ring);

	num = readl_relaxed(ring->io_base + RCB_REG_FBDNUM);
	rmb(); /* make sure num taken effect before the other data is touched */

	recv_pkts = 0, recv_bds = 0, clean_count = 0;
	num -= unused_count;

	while (recv_pkts < budget && recv_bds < num) {
		/* reuse or realloc buffers */
		if (clean_count + unused_count >= RCB_NOF_ALLOC_RX_BUFF_ONCE) {
			hns_nic_alloc_rx_buffers(ring_data,
						 clean_count + unused_count);
			clean_count = 0;
			unused_count = hns_desc_unused(ring);
		}

		/* poll one pkt */
		err = hns_nic_poll_rx_skb(ring_data, &skb, &bnum);
		if (unlikely(!skb)) /* this fault cannot be repaired */
			goto out;

		recv_bds += bnum;
		clean_count += bnum;
		if (unlikely(err)) {  /* do jump the err */
			recv_pkts++;
			continue;
		}

		/* do update ip stack process*/
		((void (*)(struct hns_nic_ring_data *, struct sk_buff *))v)(
							ring_data, skb);
		recv_pkts++;
	}

out:
	/* make all data has been write before submit */
	if (clean_count + unused_count > 0)
		hns_nic_alloc_rx_buffers(ring_data,
					 clean_count + unused_count);

	return recv_pkts;
}
