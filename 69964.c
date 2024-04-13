static netdev_features_t hsr_features_recompute(struct hsr_priv *hsr,
						netdev_features_t features)
{
	netdev_features_t mask;
	struct hsr_port *port;

	mask = features;

	/* Mask out all features that, if supported by one device, should be
	 * enabled for all devices (see NETIF_F_ONE_FOR_ALL).
	 *
	 * Anything that's off in mask will not be enabled - so only things
	 * that were in features originally, and also is in NETIF_F_ONE_FOR_ALL,
	 * may become enabled.
	 */
	features &= ~NETIF_F_ONE_FOR_ALL;
	hsr_for_each_port(hsr, port)
		features = netdev_increment_features(features,
						     port->dev->features,
						     mask);

	return features;
}
