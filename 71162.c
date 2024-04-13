static void rds_tcp_set_unloading(void)
{
	atomic_set(&rds_tcp_unloading, 1);
}
