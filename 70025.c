void rds_inc_path_init(struct rds_incoming *inc, struct rds_conn_path *cp,
		       struct in6_addr  *saddr)
{
	refcount_set(&inc->i_refcount, 1);
	INIT_LIST_HEAD(&inc->i_item);
	inc->i_conn = cp->cp_conn;
	inc->i_conn_path = cp;
	inc->i_saddr = *saddr;
	inc->i_rdma_cookie = 0;
	inc->i_rx_tstamp = ktime_set(0, 0);
}
