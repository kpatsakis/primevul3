static void put_ts_pktinfo(struct msghdr *msg, struct sk_buff *skb)
{
	struct scm_ts_pktinfo ts_pktinfo;
	struct net_device *orig_dev;

	if (!skb_mac_header_was_set(skb))
		return;

	memset(&ts_pktinfo, 0, sizeof(ts_pktinfo));

	rcu_read_lock();
	orig_dev = dev_get_by_napi_id(skb_napi_id(skb));
	if (orig_dev)
		ts_pktinfo.if_index = orig_dev->ifindex;
	rcu_read_unlock();

	ts_pktinfo.pkt_length = skb->len - skb_mac_offset(skb);
	put_cmsg(msg, SOL_SOCKET, SCM_TIMESTAMPING_PKTINFO,
		 sizeof(ts_pktinfo), &ts_pktinfo);
}
