hns_nic_alloc_rx_buffers(struct hns_nic_ring_data *ring_data, int cleand_count)
{
	int i, ret;
	struct hnae_desc_cb res_cbs;
	struct hnae_desc_cb *desc_cb;
	struct hnae_ring *ring = ring_data->ring;
	struct net_device *ndev = ring_data->napi.dev;

	for (i = 0; i < cleand_count; i++) {
		desc_cb = &ring->desc_cb[ring->next_to_use];
		if (desc_cb->reuse_flag) {
			ring->stats.reuse_pg_cnt++;
			hnae_reuse_buffer(ring, ring->next_to_use);
		} else {
			ret = hnae_reserve_buffer_map(ring, &res_cbs);
			if (ret) {
				ring->stats.sw_err_cnt++;
				netdev_err(ndev, "hnae reserve buffer map failed.\n");
				break;
			}
			hnae_replace_buffer(ring, ring->next_to_use, &res_cbs);
		}

		ring_ptr_move_fw(ring, next_to_use);
	}

	wmb(); /* make all data has been write before submit */
	writel_relaxed(i, ring->io_base + RCB_REG_HEAD);
}
