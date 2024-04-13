static int hns_nic_net_stop(struct net_device *ndev)
{
	hns_nic_net_down(ndev);

	return 0;
}
