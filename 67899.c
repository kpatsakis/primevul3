static void mac80211_hwsim_tx_frame_nl(struct ieee80211_hw *hw,
				       struct sk_buff *my_skb,
				       int dst_portid)
{
	struct sk_buff *skb;
	struct mac80211_hwsim_data *data = hw->priv;
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) my_skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(my_skb);
	void *msg_head;
	unsigned int hwsim_flags = 0;
	int i;
	struct hwsim_tx_rate tx_attempts[IEEE80211_TX_MAX_RATES];
	struct hwsim_tx_rate_flag tx_attempts_flags[IEEE80211_TX_MAX_RATES];
	uintptr_t cookie;

	if (data->ps != PS_DISABLED)
		hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);
	/* If the queue contains MAX_QUEUE skb's drop some */
	if (skb_queue_len(&data->pending) >= MAX_QUEUE) {
		/* Droping until WARN_QUEUE level */
		while (skb_queue_len(&data->pending) >= WARN_QUEUE) {
			ieee80211_free_txskb(hw, skb_dequeue(&data->pending));
			data->tx_dropped++;
		}
	}

	skb = genlmsg_new(GENLMSG_DEFAULT_SIZE, GFP_ATOMIC);
	if (skb == NULL)
		goto nla_put_failure;

	msg_head = genlmsg_put(skb, 0, 0, &hwsim_genl_family, 0,
			       HWSIM_CMD_FRAME);
	if (msg_head == NULL) {
		pr_debug("mac80211_hwsim: problem with msg_head\n");
		goto nla_put_failure;
	}

	if (nla_put(skb, HWSIM_ATTR_ADDR_TRANSMITTER,
		    ETH_ALEN, data->addresses[1].addr))
		goto nla_put_failure;

	/* We get the skb->data */
	if (nla_put(skb, HWSIM_ATTR_FRAME, my_skb->len, my_skb->data))
		goto nla_put_failure;

	/* We get the flags for this transmission, and we translate them to
	   wmediumd flags  */

	if (info->flags & IEEE80211_TX_CTL_REQ_TX_STATUS)
		hwsim_flags |= HWSIM_TX_CTL_REQ_TX_STATUS;

	if (info->flags & IEEE80211_TX_CTL_NO_ACK)
		hwsim_flags |= HWSIM_TX_CTL_NO_ACK;

	if (nla_put_u32(skb, HWSIM_ATTR_FLAGS, hwsim_flags))
		goto nla_put_failure;

	if (nla_put_u32(skb, HWSIM_ATTR_FREQ, data->channel->center_freq))
		goto nla_put_failure;

	/* We get the tx control (rate and retries) info*/

	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++) {
		tx_attempts[i].idx = info->status.rates[i].idx;
		tx_attempts_flags[i].idx = info->status.rates[i].idx;
		tx_attempts[i].count = info->status.rates[i].count;
		tx_attempts_flags[i].flags =
				trans_tx_rate_flags_ieee2hwsim(
						&info->status.rates[i]);
	}

	if (nla_put(skb, HWSIM_ATTR_TX_INFO,
		    sizeof(struct hwsim_tx_rate)*IEEE80211_TX_MAX_RATES,
		    tx_attempts))
		goto nla_put_failure;

	if (nla_put(skb, HWSIM_ATTR_TX_INFO_FLAGS,
		    sizeof(struct hwsim_tx_rate_flag) * IEEE80211_TX_MAX_RATES,
		    tx_attempts_flags))
		goto nla_put_failure;

	/* We create a cookie to identify this skb */
	data->pending_cookie++;
	cookie = data->pending_cookie;
	info->rate_driver_data[0] = (void *)cookie;
	if (nla_put_u64_64bit(skb, HWSIM_ATTR_COOKIE, cookie, HWSIM_ATTR_PAD))
		goto nla_put_failure;

	genlmsg_end(skb, msg_head);
	if (hwsim_unicast_netgroup(data, skb, dst_portid))
		goto err_free_txskb;

	/* Enqueue the packet */
	skb_queue_tail(&data->pending, my_skb);
	data->tx_pkts++;
	data->tx_bytes += my_skb->len;
	return;

nla_put_failure:
	nlmsg_free(skb);
err_free_txskb:
	pr_debug("mac80211_hwsim: error occurred in %s\n", __func__);
	ieee80211_free_txskb(hw, my_skb);
	data->tx_failed++;
}
