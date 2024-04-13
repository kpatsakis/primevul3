static int hns_nic_poll_rx_skb(struct hns_nic_ring_data *ring_data,
			       struct sk_buff **out_skb, int *out_bnum)
{
	struct hnae_ring *ring = ring_data->ring;
	struct net_device *ndev = ring_data->napi.dev;
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct sk_buff *skb;
	struct hnae_desc *desc;
	struct hnae_desc_cb *desc_cb;
	unsigned char *va;
	int bnum, length, i;
	int pull_len;
	u32 bnum_flag;

	desc = &ring->desc[ring->next_to_clean];
	desc_cb = &ring->desc_cb[ring->next_to_clean];

	prefetch(desc);

	va = (unsigned char *)desc_cb->buf + desc_cb->page_offset;

	/* prefetch first cache line of first page */
	prefetch(va);
#if L1_CACHE_BYTES < 128
	prefetch(va + L1_CACHE_BYTES);
#endif

	skb = *out_skb = napi_alloc_skb(&ring_data->napi,
					HNS_RX_HEAD_SIZE);
	if (unlikely(!skb)) {
		netdev_err(ndev, "alloc rx skb fail\n");
		ring->stats.sw_err_cnt++;
		return -ENOMEM;
	}

	prefetchw(skb->data);
	length = le16_to_cpu(desc->rx.pkt_len);
	bnum_flag = le32_to_cpu(desc->rx.ipoff_bnum_pid_flag);
	priv->ops.get_rxd_bnum(bnum_flag, &bnum);
	*out_bnum = bnum;

	if (length <= HNS_RX_HEAD_SIZE) {
		memcpy(__skb_put(skb, length), va, ALIGN(length, sizeof(long)));

		/* we can reuse buffer as-is, just make sure it is local */
		if (likely(page_to_nid(desc_cb->priv) == numa_node_id()))
			desc_cb->reuse_flag = 1;
		else /* this page cannot be reused so discard it */
			put_page(desc_cb->priv);

		ring_ptr_move_fw(ring, next_to_clean);

		if (unlikely(bnum != 1)) { /* check err*/
			*out_bnum = 1;
			goto out_bnum_err;
		}
	} else {
		ring->stats.seg_pkt_cnt++;

		pull_len = hns_nic_get_headlen(va, bnum_flag, HNS_RX_HEAD_SIZE);
		memcpy(__skb_put(skb, pull_len), va,
		       ALIGN(pull_len, sizeof(long)));

		hns_nic_reuse_page(skb, 0, ring, pull_len, desc_cb);
		ring_ptr_move_fw(ring, next_to_clean);

		if (unlikely(bnum >= (int)MAX_SKB_FRAGS)) { /* check err*/
			*out_bnum = 1;
			goto out_bnum_err;
		}
		for (i = 1; i < bnum; i++) {
			desc = &ring->desc[ring->next_to_clean];
			desc_cb = &ring->desc_cb[ring->next_to_clean];

			hns_nic_reuse_page(skb, i, ring, 0, desc_cb);
			ring_ptr_move_fw(ring, next_to_clean);
		}
	}

	/* check except process, free skb and jump the desc */
	if (unlikely((!bnum) || (bnum > ring->max_desc_num_per_pkt))) {
out_bnum_err:
		*out_bnum = *out_bnum ? *out_bnum : 1; /* ntc moved,cannot 0*/
		netdev_err(ndev, "invalid bnum(%d,%d,%d,%d),%016llx,%016llx\n",
			   bnum, ring->max_desc_num_per_pkt,
			   length, (int)MAX_SKB_FRAGS,
			   ((u64 *)desc)[0], ((u64 *)desc)[1]);
		ring->stats.err_bd_num++;
		dev_kfree_skb_any(skb);
		return -EDOM;
	}

	bnum_flag = le32_to_cpu(desc->rx.ipoff_bnum_pid_flag);

	if (unlikely(!hnae_get_bit(bnum_flag, HNS_RXD_VLD_B))) {
		netdev_err(ndev, "no valid bd,%016llx,%016llx\n",
			   ((u64 *)desc)[0], ((u64 *)desc)[1]);
		ring->stats.non_vld_descs++;
		dev_kfree_skb_any(skb);
		return -EINVAL;
	}

	if (unlikely((!desc->rx.pkt_len) ||
		     hnae_get_bit(bnum_flag, HNS_RXD_DROP_B))) {
		ring->stats.err_pkt_len++;
		dev_kfree_skb_any(skb);
		return -EFAULT;
	}

	if (unlikely(hnae_get_bit(bnum_flag, HNS_RXD_L2E_B))) {
		ring->stats.l2_err++;
		dev_kfree_skb_any(skb);
		return -EFAULT;
	}

	ring->stats.rx_pkts++;
	ring->stats.rx_bytes += skb->len;

	/* indicate to upper stack if our hardware has already calculated
	 * the RX checksum
	 */
	hns_nic_rx_checksum(ring_data, skb, bnum_flag);

	return 0;
}
