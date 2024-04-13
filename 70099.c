static int genl_allocate_reserve_groups(int n_groups, int *first_id)
{
	unsigned long *new_groups;
	int start = 0;
	int i;
	int id;
	bool fits;

	do {
		if (start == 0)
			id = find_first_zero_bit(mc_groups,
						 mc_groups_longs *
						 BITS_PER_LONG);
		else
			id = find_next_zero_bit(mc_groups,
						mc_groups_longs * BITS_PER_LONG,
						start);

		fits = true;
		for (i = id;
		     i < min_t(int, id + n_groups,
			       mc_groups_longs * BITS_PER_LONG);
		     i++) {
			if (test_bit(i, mc_groups)) {
				start = i;
				fits = false;
				break;
			}
		}

		if (id + n_groups > mc_groups_longs * BITS_PER_LONG) {
			unsigned long new_longs = mc_groups_longs +
						  BITS_TO_LONGS(n_groups);
			size_t nlen = new_longs * sizeof(unsigned long);

			if (mc_groups == &mc_group_start) {
				new_groups = kzalloc(nlen, GFP_KERNEL);
				if (!new_groups)
					return -ENOMEM;
				mc_groups = new_groups;
				*mc_groups = mc_group_start;
			} else {
				new_groups = krealloc(mc_groups, nlen,
						      GFP_KERNEL);
				if (!new_groups)
					return -ENOMEM;
				mc_groups = new_groups;
				for (i = 0; i < BITS_TO_LONGS(n_groups); i++)
					mc_groups[mc_groups_longs + i] = 0;
			}
			mc_groups_longs = new_longs;
		}
	} while (!fits);

	for (i = id; i < id + n_groups; i++)
		set_bit(i, mc_groups);
	*first_id = id;
	return 0;
}
