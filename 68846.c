static int hns_nic_clear_all_rx_fetch(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	struct hnae_ae_ops *ops = h->dev->ops;
	struct hns_nic_ring_data *rd;
	struct hnae_ring *ring;
	struct sk_buff *skb;
	u32 *org_indir;
	u32 *cur_indir;
	int indir_size;
	int head, tail;
	int fetch_num;
	int i, j;
	bool found;
	int retry_times;
	int ret = 0;

	/* alloc indir memory */
	indir_size = ops->get_rss_indir_size(h) * sizeof(*org_indir);
	org_indir = kzalloc(indir_size, GFP_KERNEL);
	if (!org_indir)
		return -ENOMEM;

	/* store the orginal indirection */
	ops->get_rss(h, org_indir, NULL, NULL);

	cur_indir = kzalloc(indir_size, GFP_KERNEL);
	if (!cur_indir) {
		ret = -ENOMEM;
		goto cur_indir_alloc_err;
	}

	/* set loopback */
	if (hns_enable_serdes_lb(ndev)) {
		ret = -EINVAL;
		goto enable_serdes_lb_err;
	}

	/* foreach every rx ring to clear fetch desc */
	for (i = 0; i < h->q_num; i++) {
		ring = &h->qs[i]->rx_ring;
		head = readl_relaxed(ring->io_base + RCB_REG_HEAD);
		tail = readl_relaxed(ring->io_base + RCB_REG_TAIL);
		found = false;
		fetch_num = ring_dist(ring, head, tail);

		while (head != tail) {
			if (ring->desc_cb[head].page_offset != 0) {
				found = true;
				break;
			}

			head++;
			if (head == ring->desc_num)
				head = 0;
		}

		if (found) {
			for (j = 0; j < indir_size / sizeof(*org_indir); j++)
				cur_indir[j] = i;
			ops->set_rss(h, cur_indir, NULL, 0);

			for (j = 0; j < fetch_num; j++) {
				/* alloc one skb and init */
				skb = hns_assemble_skb(ndev);
				if (!skb)
					goto out;
				rd = &tx_ring_data(priv, skb->queue_mapping);
				hns_nic_net_xmit_hw(ndev, skb, rd);

				retry_times = 0;
				while (retry_times++ < 10) {
					mdelay(10);
					/* clean rx */
					rd = &rx_ring_data(priv, i);
					if (rd->poll_one(rd, fetch_num,
							 hns_nic_drop_rx_fetch))
						break;
				}

				retry_times = 0;
				while (retry_times++ < 10) {
					mdelay(10);
					/* clean tx ring 0 send package */
					rd = &tx_ring_data(priv,
							   HNS_LB_TX_RING);
					if (rd->poll_one(rd, fetch_num, NULL))
						break;
				}
			}
		}
	}

out:
	/* restore everything */
	ops->set_rss(h, org_indir, NULL, 0);
	hns_disable_serdes_lb(ndev);
enable_serdes_lb_err:
	kfree(cur_indir);
cur_indir_alloc_err:
	kfree(org_indir);

	return ret;
}
