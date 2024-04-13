static bool skb_is_swtx_tstamp(const struct sk_buff *skb, int false_tstamp)
{
	return skb->tstamp && !false_tstamp && skb_is_err_queue(skb);
}
