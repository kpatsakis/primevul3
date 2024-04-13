void rds_inc_init(struct rds_incoming *inc, struct rds_connection *conn,
		 struct in6_addr *saddr)
{
	refcount_set(&inc->i_refcount, 1);
	INIT_LIST_HEAD(&inc->i_item);
	inc->i_conn = conn;
	inc->i_saddr = *saddr;
	inc->i_rdma_cookie = 0;
	inc->i_rx_tstamp = ktime_set(0, 0);

	memset(inc->i_rx_lat_trace, 0, sizeof(inc->i_rx_lat_trace));
}
