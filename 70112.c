static void genl_rcv(struct sk_buff *skb)
{
	down_read(&cb_lock);
	netlink_rcv_skb(skb, &genl_rcv_msg);
	up_read(&cb_lock);
}
