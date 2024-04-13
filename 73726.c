static int netlbl_cipsov4_add_std(struct genl_info *info)
{
	int ret_val = -EINVAL;
	struct cipso_v4_doi *doi_def = NULL;
	struct nlattr *nla_a;
	struct nlattr *nla_b;
	int nla_a_rem;
	int nla_b_rem;
	u32 iter;

	if (!info->attrs[NLBL_CIPSOV4_A_TAGLST] ||
	    !info->attrs[NLBL_CIPSOV4_A_MLSLVLLST])
		return -EINVAL;

	if (nla_validate_nested(info->attrs[NLBL_CIPSOV4_A_MLSLVLLST],
				NLBL_CIPSOV4_A_MAX,
				netlbl_cipsov4_genl_policy) != 0)
		return -EINVAL;

	doi_def = kmalloc(sizeof(*doi_def), GFP_KERNEL);
	if (doi_def == NULL)
		return -ENOMEM;
	doi_def->map.std = kzalloc(sizeof(*doi_def->map.std), GFP_KERNEL);
	if (doi_def->map.std == NULL) {
		ret_val = -ENOMEM;
		goto add_std_failure;
	}
	doi_def->type = CIPSO_V4_MAP_STD;

	ret_val = netlbl_cipsov4_add_common(info, doi_def);
	if (ret_val != 0)
		goto add_std_failure;
	ret_val = -EINVAL;

	nla_for_each_nested(nla_a,
			    info->attrs[NLBL_CIPSOV4_A_MLSLVLLST],
			    nla_a_rem)
		if (nla_a->nla_type == NLBL_CIPSOV4_A_MLSLVL) {
			if (nla_validate_nested(nla_a,
					    NLBL_CIPSOV4_A_MAX,
					    netlbl_cipsov4_genl_policy) != 0)
					goto add_std_failure;
			nla_for_each_nested(nla_b, nla_a, nla_b_rem)
				switch (nla_b->nla_type) {
				case NLBL_CIPSOV4_A_MLSLVLLOC:
					if (nla_get_u32(nla_b) >
					    CIPSO_V4_MAX_LOC_LVLS)
						goto add_std_failure;
					if (nla_get_u32(nla_b) >=
					    doi_def->map.std->lvl.local_size)
					     doi_def->map.std->lvl.local_size =
						     nla_get_u32(nla_b) + 1;
					break;
				case NLBL_CIPSOV4_A_MLSLVLREM:
					if (nla_get_u32(nla_b) >
					    CIPSO_V4_MAX_REM_LVLS)
						goto add_std_failure;
					if (nla_get_u32(nla_b) >=
					    doi_def->map.std->lvl.cipso_size)
					     doi_def->map.std->lvl.cipso_size =
						     nla_get_u32(nla_b) + 1;
					break;
				}
		}
	doi_def->map.std->lvl.local = kcalloc(doi_def->map.std->lvl.local_size,
					      sizeof(u32),
					      GFP_KERNEL);
	if (doi_def->map.std->lvl.local == NULL) {
		ret_val = -ENOMEM;
		goto add_std_failure;
	}
	doi_def->map.std->lvl.cipso = kcalloc(doi_def->map.std->lvl.cipso_size,
					      sizeof(u32),
					      GFP_KERNEL);
	if (doi_def->map.std->lvl.cipso == NULL) {
		ret_val = -ENOMEM;
		goto add_std_failure;
	}
	for (iter = 0; iter < doi_def->map.std->lvl.local_size; iter++)
		doi_def->map.std->lvl.local[iter] = CIPSO_V4_INV_LVL;
	for (iter = 0; iter < doi_def->map.std->lvl.cipso_size; iter++)
		doi_def->map.std->lvl.cipso[iter] = CIPSO_V4_INV_LVL;
	nla_for_each_nested(nla_a,
			    info->attrs[NLBL_CIPSOV4_A_MLSLVLLST],
			    nla_a_rem)
		if (nla_a->nla_type == NLBL_CIPSOV4_A_MLSLVL) {
			struct nlattr *lvl_loc;
			struct nlattr *lvl_rem;

			lvl_loc = nla_find_nested(nla_a,
						  NLBL_CIPSOV4_A_MLSLVLLOC);
			lvl_rem = nla_find_nested(nla_a,
						  NLBL_CIPSOV4_A_MLSLVLREM);
			if (lvl_loc == NULL || lvl_rem == NULL)
				goto add_std_failure;
			doi_def->map.std->lvl.local[nla_get_u32(lvl_loc)] =
				nla_get_u32(lvl_rem);
			doi_def->map.std->lvl.cipso[nla_get_u32(lvl_rem)] =
				nla_get_u32(lvl_loc);
		}

	if (info->attrs[NLBL_CIPSOV4_A_MLSCATLST]) {
		if (nla_validate_nested(info->attrs[NLBL_CIPSOV4_A_MLSCATLST],
					NLBL_CIPSOV4_A_MAX,
					netlbl_cipsov4_genl_policy) != 0)
			goto add_std_failure;

		nla_for_each_nested(nla_a,
				    info->attrs[NLBL_CIPSOV4_A_MLSCATLST],
				    nla_a_rem)
			if (nla_a->nla_type == NLBL_CIPSOV4_A_MLSCAT) {
				if (nla_validate_nested(nla_a,
					      NLBL_CIPSOV4_A_MAX,
					      netlbl_cipsov4_genl_policy) != 0)
					goto add_std_failure;
				nla_for_each_nested(nla_b, nla_a, nla_b_rem)
					switch (nla_b->nla_type) {
					case NLBL_CIPSOV4_A_MLSCATLOC:
						if (nla_get_u32(nla_b) >
						    CIPSO_V4_MAX_LOC_CATS)
							goto add_std_failure;
						if (nla_get_u32(nla_b) >=
					      doi_def->map.std->cat.local_size)
					     doi_def->map.std->cat.local_size =
						     nla_get_u32(nla_b) + 1;
						break;
					case NLBL_CIPSOV4_A_MLSCATREM:
						if (nla_get_u32(nla_b) >
						    CIPSO_V4_MAX_REM_CATS)
							goto add_std_failure;
						if (nla_get_u32(nla_b) >=
					      doi_def->map.std->cat.cipso_size)
					     doi_def->map.std->cat.cipso_size =
						     nla_get_u32(nla_b) + 1;
						break;
					}
			}
		doi_def->map.std->cat.local = kcalloc(
			                      doi_def->map.std->cat.local_size,
					      sizeof(u32),
					      GFP_KERNEL);
		if (doi_def->map.std->cat.local == NULL) {
			ret_val = -ENOMEM;
			goto add_std_failure;
		}
		doi_def->map.std->cat.cipso = kcalloc(
			                      doi_def->map.std->cat.cipso_size,
					      sizeof(u32),
					      GFP_KERNEL);
		if (doi_def->map.std->cat.cipso == NULL) {
			ret_val = -ENOMEM;
			goto add_std_failure;
		}
		for (iter = 0; iter < doi_def->map.std->cat.local_size; iter++)
			doi_def->map.std->cat.local[iter] = CIPSO_V4_INV_CAT;
		for (iter = 0; iter < doi_def->map.std->cat.cipso_size; iter++)
			doi_def->map.std->cat.cipso[iter] = CIPSO_V4_INV_CAT;
		nla_for_each_nested(nla_a,
				    info->attrs[NLBL_CIPSOV4_A_MLSCATLST],
				    nla_a_rem)
			if (nla_a->nla_type == NLBL_CIPSOV4_A_MLSCAT) {
				struct nlattr *cat_loc;
				struct nlattr *cat_rem;

				cat_loc = nla_find_nested(nla_a,
						     NLBL_CIPSOV4_A_MLSCATLOC);
				cat_rem = nla_find_nested(nla_a,
						     NLBL_CIPSOV4_A_MLSCATREM);
				if (cat_loc == NULL || cat_rem == NULL)
					goto add_std_failure;
				doi_def->map.std->cat.local[
				                        nla_get_u32(cat_loc)] =
					nla_get_u32(cat_rem);
				doi_def->map.std->cat.cipso[
					                nla_get_u32(cat_rem)] =
					nla_get_u32(cat_loc);
			}
	}

	ret_val = cipso_v4_doi_add(doi_def);
	if (ret_val != 0)
		goto add_std_failure;
	return 0;

add_std_failure:
	if (doi_def)
		netlbl_cipsov4_doi_free(&doi_def->rcu);
	return ret_val;
}
