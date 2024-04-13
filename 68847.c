static int hns_nic_dev_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct net_device *ndev;
	struct hns_nic_priv *priv;
	u32 port_id;
	int ret;

	ndev = alloc_etherdev_mq(sizeof(struct hns_nic_priv), NIC_MAX_Q_PER_VF);
	if (!ndev)
		return -ENOMEM;

	platform_set_drvdata(pdev, ndev);

	priv = netdev_priv(ndev);
	priv->dev = dev;
	priv->netdev = ndev;

	if (dev_of_node(dev)) {
		struct device_node *ae_node;

		if (of_device_is_compatible(dev->of_node,
					    "hisilicon,hns-nic-v1"))
			priv->enet_ver = AE_VERSION_1;
		else
			priv->enet_ver = AE_VERSION_2;

		ae_node = of_parse_phandle(dev->of_node, "ae-handle", 0);
		if (IS_ERR_OR_NULL(ae_node)) {
			ret = PTR_ERR(ae_node);
			dev_err(dev, "not find ae-handle\n");
			goto out_read_prop_fail;
		}
		priv->fwnode = &ae_node->fwnode;
	} else if (is_acpi_node(dev->fwnode)) {
		struct acpi_reference_args args;

		if (acpi_dev_found(hns_enet_acpi_match[0].id))
			priv->enet_ver = AE_VERSION_1;
		else if (acpi_dev_found(hns_enet_acpi_match[1].id))
			priv->enet_ver = AE_VERSION_2;
		else
			return -ENXIO;

		/* try to find port-idx-in-ae first */
		ret = acpi_node_get_property_reference(dev->fwnode,
						       "ae-handle", 0, &args);
		if (ret) {
			dev_err(dev, "not find ae-handle\n");
			goto out_read_prop_fail;
		}
		priv->fwnode = acpi_fwnode_handle(args.adev);
	} else {
		dev_err(dev, "cannot read cfg data from OF or acpi\n");
		return -ENXIO;
	}

	ret = device_property_read_u32(dev, "port-idx-in-ae", &port_id);
	if (ret) {
		/* only for old code compatible */
		ret = device_property_read_u32(dev, "port-id", &port_id);
		if (ret)
			goto out_read_prop_fail;
		/* for old dts, we need to caculate the port offset */
		port_id = port_id < HNS_SRV_OFFSET ? port_id + HNS_DEBUG_OFFSET
			: port_id - HNS_SRV_OFFSET;
	}
	priv->port_id = port_id;

	hns_init_mac_addr(ndev);

	ndev->watchdog_timeo = HNS_NIC_TX_TIMEOUT;
	ndev->priv_flags |= IFF_UNICAST_FLT;
	ndev->netdev_ops = &hns_nic_netdev_ops;
	hns_ethtool_set_ops(ndev);

	ndev->features |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM |
		NETIF_F_RXCSUM | NETIF_F_SG | NETIF_F_GSO |
		NETIF_F_GRO;
	ndev->vlan_features |=
		NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM | NETIF_F_RXCSUM;
	ndev->vlan_features |= NETIF_F_SG | NETIF_F_GSO | NETIF_F_GRO;

	/* MTU range: 68 - 9578 (v1) or 9706 (v2) */
	ndev->min_mtu = MAC_MIN_MTU;
	switch (priv->enet_ver) {
	case AE_VERSION_2:
		ndev->features |= NETIF_F_TSO | NETIF_F_TSO6;
		ndev->hw_features |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM |
			NETIF_F_RXCSUM | NETIF_F_SG | NETIF_F_GSO |
			NETIF_F_GRO | NETIF_F_TSO | NETIF_F_TSO6;
		ndev->max_mtu = MAC_MAX_MTU_V2 -
				(ETH_HLEN + ETH_FCS_LEN + VLAN_HLEN);
		break;
	default:
		ndev->max_mtu = MAC_MAX_MTU -
				(ETH_HLEN + ETH_FCS_LEN + VLAN_HLEN);
		break;
	}

	SET_NETDEV_DEV(ndev, dev);

	if (!dma_set_mask_and_coherent(dev, DMA_BIT_MASK(64)))
		dev_dbg(dev, "set mask to 64bit\n");
	else
		dev_err(dev, "set mask to 64bit fail!\n");

	/* carrier off reporting is important to ethtool even BEFORE open */
	netif_carrier_off(ndev);

	setup_timer(&priv->service_timer, hns_nic_service_timer,
		    (unsigned long)priv);
	INIT_WORK(&priv->service_task, hns_nic_service_task);

	set_bit(NIC_STATE_SERVICE_INITED, &priv->state);
	clear_bit(NIC_STATE_SERVICE_SCHED, &priv->state);
	set_bit(NIC_STATE_DOWN, &priv->state);

	if (hns_nic_try_get_ae(priv->netdev)) {
		priv->notifier_block.notifier_call = hns_nic_notifier_action;
		ret = hnae_register_notifier(&priv->notifier_block);
		if (ret) {
			dev_err(dev, "register notifier fail!\n");
			goto out_notify_fail;
		}
		dev_dbg(dev, "has not handle, register notifier!\n");
	}

	return 0;

out_notify_fail:
	(void)cancel_work_sync(&priv->service_task);
out_read_prop_fail:
	free_netdev(ndev);
	return ret;
}
