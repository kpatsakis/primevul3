int netif_set_xps_queue(struct net_device *dev, const struct cpumask *mask,
			u16 index)
{
	struct xps_dev_maps *dev_maps, *new_dev_maps = NULL;
	int i, cpu, tci, numa_node_id = -2;
	int maps_sz, num_tc = 1, tc = 0;
	struct xps_map *map, *new_map;
	bool active = false;

	if (dev->num_tc) {
		num_tc = dev->num_tc;
		tc = netdev_txq_to_tc(dev, index);
		if (tc < 0)
			return -EINVAL;
	}

	maps_sz = XPS_DEV_MAPS_SIZE(num_tc);
	if (maps_sz < L1_CACHE_BYTES)
		maps_sz = L1_CACHE_BYTES;

	mutex_lock(&xps_map_mutex);

	dev_maps = xmap_dereference(dev->xps_maps);

	/* allocate memory for queue storage */
	for_each_cpu_and(cpu, cpu_online_mask, mask) {
		if (!new_dev_maps)
			new_dev_maps = kzalloc(maps_sz, GFP_KERNEL);
		if (!new_dev_maps) {
			mutex_unlock(&xps_map_mutex);
			return -ENOMEM;
		}

		tci = cpu * num_tc + tc;
		map = dev_maps ? xmap_dereference(dev_maps->cpu_map[tci]) :
				 NULL;

		map = expand_xps_map(map, cpu, index);
		if (!map)
			goto error;

		RCU_INIT_POINTER(new_dev_maps->cpu_map[tci], map);
	}

	if (!new_dev_maps)
		goto out_no_new_maps;

	for_each_possible_cpu(cpu) {
		/* copy maps belonging to foreign traffic classes */
		for (i = tc, tci = cpu * num_tc; dev_maps && i--; tci++) {
			/* fill in the new device map from the old device map */
			map = xmap_dereference(dev_maps->cpu_map[tci]);
			RCU_INIT_POINTER(new_dev_maps->cpu_map[tci], map);
		}

		/* We need to explicitly update tci as prevous loop
		 * could break out early if dev_maps is NULL.
		 */
		tci = cpu * num_tc + tc;

		if (cpumask_test_cpu(cpu, mask) && cpu_online(cpu)) {
			/* add queue to CPU maps */
			int pos = 0;

			map = xmap_dereference(new_dev_maps->cpu_map[tci]);
			while ((pos < map->len) && (map->queues[pos] != index))
				pos++;

			if (pos == map->len)
				map->queues[map->len++] = index;
#ifdef CONFIG_NUMA
			if (numa_node_id == -2)
				numa_node_id = cpu_to_node(cpu);
			else if (numa_node_id != cpu_to_node(cpu))
				numa_node_id = -1;
#endif
		} else if (dev_maps) {
			/* fill in the new device map from the old device map */
			map = xmap_dereference(dev_maps->cpu_map[tci]);
			RCU_INIT_POINTER(new_dev_maps->cpu_map[tci], map);
		}

		/* copy maps belonging to foreign traffic classes */
		for (i = num_tc - tc, tci++; dev_maps && --i; tci++) {
			/* fill in the new device map from the old device map */
			map = xmap_dereference(dev_maps->cpu_map[tci]);
			RCU_INIT_POINTER(new_dev_maps->cpu_map[tci], map);
		}
	}

	rcu_assign_pointer(dev->xps_maps, new_dev_maps);

	/* Cleanup old maps */
	if (!dev_maps)
		goto out_no_old_maps;

	for_each_possible_cpu(cpu) {
		for (i = num_tc, tci = cpu * num_tc; i--; tci++) {
			new_map = xmap_dereference(new_dev_maps->cpu_map[tci]);
			map = xmap_dereference(dev_maps->cpu_map[tci]);
			if (map && map != new_map)
				kfree_rcu(map, rcu);
		}
	}

	kfree_rcu(dev_maps, rcu);

out_no_old_maps:
	dev_maps = new_dev_maps;
	active = true;

out_no_new_maps:
	/* update Tx queue numa node */
	netdev_queue_numa_node_write(netdev_get_tx_queue(dev, index),
				     (numa_node_id >= 0) ? numa_node_id :
				     NUMA_NO_NODE);

	if (!dev_maps)
		goto out_no_maps;

	/* removes queue from unused CPUs */
	for_each_possible_cpu(cpu) {
		for (i = tc, tci = cpu * num_tc; i--; tci++)
			active |= remove_xps_queue(dev_maps, tci, index);
		if (!cpumask_test_cpu(cpu, mask) || !cpu_online(cpu))
			active |= remove_xps_queue(dev_maps, tci, index);
		for (i = num_tc - tc, tci++; --i; tci++)
			active |= remove_xps_queue(dev_maps, tci, index);
	}

	/* free map if not active */
	if (!active) {
		RCU_INIT_POINTER(dev->xps_maps, NULL);
		kfree_rcu(dev_maps, rcu);
	}

out_no_maps:
	mutex_unlock(&xps_map_mutex);

	return 0;
error:
	/* remove any maps that we added */
	for_each_possible_cpu(cpu) {
		for (i = num_tc, tci = cpu * num_tc; i--; tci++) {
			new_map = xmap_dereference(new_dev_maps->cpu_map[tci]);
			map = dev_maps ?
			      xmap_dereference(dev_maps->cpu_map[tci]) :
			      NULL;
			if (new_map && new_map != map)
				kfree(new_map);
		}
	}

	mutex_unlock(&xps_map_mutex);

	kfree(new_dev_maps);
	return -ENOMEM;
}
