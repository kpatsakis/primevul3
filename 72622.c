void update_max_interval(void)
{
	max_load_balance_interval = HZ*num_online_cpus()/10;
}
