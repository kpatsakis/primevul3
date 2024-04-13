static void hns_nic_set_priv_ops(struct net_device *netdev)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);
	struct hnae_handle *h = priv->ae_handle;

	if (AE_IS_VER1(priv->enet_ver)) {
		priv->ops.fill_desc = fill_desc;
		priv->ops.get_rxd_bnum = get_rx_desc_bnum;
		priv->ops.maybe_stop_tx = hns_nic_maybe_stop_tx;
	} else {
		priv->ops.get_rxd_bnum = get_v2rx_desc_bnum;
		if ((netdev->features & NETIF_F_TSO) ||
		    (netdev->features & NETIF_F_TSO6)) {
			priv->ops.fill_desc = fill_tso_desc;
			priv->ops.maybe_stop_tx = hns_nic_maybe_stop_tso;
			/* This chip only support 7*4096 */
			netif_set_gso_max_size(netdev, 7 * 4096);
		} else {
			priv->ops.fill_desc = fill_v2_desc;
			priv->ops.maybe_stop_tx = hns_nic_maybe_stop_tx;
		}
		/* enable tso when init
		 * control tso on/off through TSE bit in bd
		 */
		h->dev->ops->set_tso_stats(h, 1);
	}
}
