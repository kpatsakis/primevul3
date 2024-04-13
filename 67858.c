static int __init init_mac80211_hwsim(void)
{
	int i, err;

	if (radios < 0 || radios > 100)
		return -EINVAL;

	if (channels < 1)
		return -EINVAL;

	spin_lock_init(&hwsim_radio_lock);

	rhashtable_init(&hwsim_radios_rht, &hwsim_rht_params);

	err = register_pernet_device(&hwsim_net_ops);
	if (err)
		return err;

	err = platform_driver_register(&mac80211_hwsim_driver);
	if (err)
		goto out_unregister_pernet;

	hwsim_class = class_create(THIS_MODULE, "mac80211_hwsim");
	if (IS_ERR(hwsim_class)) {
		err = PTR_ERR(hwsim_class);
		goto out_unregister_driver;
	}

	err = hwsim_init_netlink();
	if (err < 0)
		goto out_unregister_driver;

	for (i = 0; i < radios; i++) {
		struct hwsim_new_radio_params param = { 0 };

		param.channels = channels;

		switch (regtest) {
		case HWSIM_REGTEST_DIFF_COUNTRY:
			if (i < ARRAY_SIZE(hwsim_alpha2s))
				param.reg_alpha2 = hwsim_alpha2s[i];
			break;
		case HWSIM_REGTEST_DRIVER_REG_FOLLOW:
			if (!i)
				param.reg_alpha2 = hwsim_alpha2s[0];
			break;
		case HWSIM_REGTEST_STRICT_ALL:
			param.reg_strict = true;
		case HWSIM_REGTEST_DRIVER_REG_ALL:
			param.reg_alpha2 = hwsim_alpha2s[0];
			break;
		case HWSIM_REGTEST_WORLD_ROAM:
			if (i == 0)
				param.regd = &hwsim_world_regdom_custom_01;
			break;
		case HWSIM_REGTEST_CUSTOM_WORLD:
			param.regd = &hwsim_world_regdom_custom_01;
			break;
		case HWSIM_REGTEST_CUSTOM_WORLD_2:
			if (i == 0)
				param.regd = &hwsim_world_regdom_custom_01;
			else if (i == 1)
				param.regd = &hwsim_world_regdom_custom_02;
			break;
		case HWSIM_REGTEST_STRICT_FOLLOW:
			if (i == 0) {
				param.reg_strict = true;
				param.reg_alpha2 = hwsim_alpha2s[0];
			}
			break;
		case HWSIM_REGTEST_STRICT_AND_DRIVER_REG:
			if (i == 0) {
				param.reg_strict = true;
				param.reg_alpha2 = hwsim_alpha2s[0];
			} else if (i == 1) {
				param.reg_alpha2 = hwsim_alpha2s[1];
			}
			break;
		case HWSIM_REGTEST_ALL:
			switch (i) {
			case 0:
				param.regd = &hwsim_world_regdom_custom_01;
				break;
			case 1:
				param.regd = &hwsim_world_regdom_custom_02;
				break;
			case 2:
				param.reg_alpha2 = hwsim_alpha2s[0];
				break;
			case 3:
				param.reg_alpha2 = hwsim_alpha2s[1];
				break;
			case 4:
				param.reg_strict = true;
				param.reg_alpha2 = hwsim_alpha2s[2];
				break;
			}
			break;
		default:
			break;
		}

		param.p2p_device = support_p2p_device;
		param.use_chanctx = channels > 1;

		err = mac80211_hwsim_new_radio(NULL, &param);
		if (err < 0)
			goto out_free_radios;
	}

	hwsim_mon = alloc_netdev(0, "hwsim%d", NET_NAME_UNKNOWN,
				 hwsim_mon_setup);
	if (hwsim_mon == NULL) {
		err = -ENOMEM;
		goto out_free_radios;
	}

	rtnl_lock();
	err = dev_alloc_name(hwsim_mon, hwsim_mon->name);
	if (err < 0) {
		rtnl_unlock();
		goto out_free_radios;
	}

	err = register_netdevice(hwsim_mon);
	if (err < 0) {
		rtnl_unlock();
		goto out_free_mon;
	}
	rtnl_unlock();

	return 0;

out_free_mon:
	free_netdev(hwsim_mon);
out_free_radios:
	mac80211_hwsim_free();
out_unregister_driver:
	platform_driver_unregister(&mac80211_hwsim_driver);
out_unregister_pernet:
	unregister_pernet_device(&hwsim_net_ops);
	return err;
}
