static int hwsim_cloned_frame_received_nl(struct sk_buff *skb_2,
					  struct genl_info *info)
{
	struct mac80211_hwsim_data *data2;
	struct ieee80211_rx_status rx_status;
	const u8 *dst;
	int frame_data_len;
	void *frame_data;
	struct sk_buff *skb = NULL;

	if (!info->attrs[HWSIM_ATTR_ADDR_RECEIVER] ||
	    !info->attrs[HWSIM_ATTR_FRAME] ||
	    !info->attrs[HWSIM_ATTR_RX_RATE] ||
	    !info->attrs[HWSIM_ATTR_SIGNAL])
		goto out;

	dst = (void *)nla_data(info->attrs[HWSIM_ATTR_ADDR_RECEIVER]);
	frame_data_len = nla_len(info->attrs[HWSIM_ATTR_FRAME]);
	frame_data = (void *)nla_data(info->attrs[HWSIM_ATTR_FRAME]);

	/* Allocate new skb here */
	skb = alloc_skb(frame_data_len, GFP_KERNEL);
	if (skb == NULL)
		goto err;

	if (frame_data_len > IEEE80211_MAX_DATA_LEN)
		goto err;

	/* Copy the data */
	skb_put_data(skb, frame_data, frame_data_len);

	data2 = get_hwsim_data_ref_from_addr(dst);
	if (!data2)
		goto out;

	if (hwsim_net_get_netgroup(genl_info_net(info)) != data2->netgroup)
		goto out;

	if (info->snd_portid != data2->wmediumd)
		goto out;

	/* check if radio is configured properly */

	if (data2->idle || !data2->started)
		goto out;

	/* A frame is received from user space */
	memset(&rx_status, 0, sizeof(rx_status));
	if (info->attrs[HWSIM_ATTR_FREQ]) {
		/* throw away off-channel packets, but allow both the temporary
		 * ("hw" scan/remain-on-channel) and regular channel, since the
		 * internal datapath also allows this
		 */
		mutex_lock(&data2->mutex);
		rx_status.freq = nla_get_u32(info->attrs[HWSIM_ATTR_FREQ]);

		if (rx_status.freq != data2->channel->center_freq &&
		    (!data2->tmp_chan ||
		     rx_status.freq != data2->tmp_chan->center_freq)) {
			mutex_unlock(&data2->mutex);
			goto out;
		}
		mutex_unlock(&data2->mutex);
	} else {
		rx_status.freq = data2->channel->center_freq;
	}

	rx_status.band = data2->channel->band;
	rx_status.rate_idx = nla_get_u32(info->attrs[HWSIM_ATTR_RX_RATE]);
	rx_status.signal = nla_get_u32(info->attrs[HWSIM_ATTR_SIGNAL]);

	memcpy(IEEE80211_SKB_RXCB(skb), &rx_status, sizeof(rx_status));
	data2->rx_pkts++;
	data2->rx_bytes += skb->len;
	ieee80211_rx_irqsafe(data2->hw, skb);

	return 0;
err:
	pr_debug("mac80211_hwsim: error occurred in %s\n", __func__);
out:
	dev_kfree_skb(skb);
	return -EINVAL;
}
