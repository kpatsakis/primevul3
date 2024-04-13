UNCURL_EXPORT int32_t uncurl_poll(struct uncurl_conn *ucc, int32_t timeout_ms)
{
	return net_poll(ucc->net, NET_POLLIN, timeout_ms);
}
