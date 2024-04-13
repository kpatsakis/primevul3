static void hns_init_mac_addr(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);

	if (!device_get_mac_address(priv->dev, ndev->dev_addr, ETH_ALEN)) {
		eth_hw_addr_random(ndev);
		dev_warn(priv->dev, "No valid mac, use random mac %pM",
			 ndev->dev_addr);
	}
}
