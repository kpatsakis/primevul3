static int create_rss_raw_qp_tir(struct mlx5_ib_dev *dev, struct mlx5_ib_qp *qp,
				 struct ib_pd *pd,
				 struct ib_qp_init_attr *init_attr,
				 struct ib_udata *udata)
{
	struct ib_uobject *uobj = pd->uobject;
	struct ib_ucontext *ucontext = uobj->context;
	struct mlx5_ib_ucontext *mucontext = to_mucontext(ucontext);
	struct mlx5_ib_create_qp_resp resp = {};
	int inlen;
	int err;
	u32 *in;
	void *tirc;
	void *hfso;
	u32 selected_fields = 0;
	u32 outer_l4;
	size_t min_resp_len;
	u32 tdn = mucontext->tdn;
	struct mlx5_ib_create_qp_rss ucmd = {};
	size_t required_cmd_sz;

	if (init_attr->qp_type != IB_QPT_RAW_PACKET)
		return -EOPNOTSUPP;

	if (init_attr->create_flags || init_attr->send_cq)
		return -EINVAL;

	min_resp_len = offsetof(typeof(resp), bfreg_index) + sizeof(resp.bfreg_index);
	if (udata->outlen < min_resp_len)
		return -EINVAL;

	required_cmd_sz = offsetof(typeof(ucmd), flags) + sizeof(ucmd.flags);
	if (udata->inlen < required_cmd_sz) {
		mlx5_ib_dbg(dev, "invalid inlen\n");
		return -EINVAL;
	}

	if (udata->inlen > sizeof(ucmd) &&
	    !ib_is_udata_cleared(udata, sizeof(ucmd),
				 udata->inlen - sizeof(ucmd))) {
		mlx5_ib_dbg(dev, "inlen is not supported\n");
		return -EOPNOTSUPP;
	}

	if (ib_copy_from_udata(&ucmd, udata, min(sizeof(ucmd), udata->inlen))) {
		mlx5_ib_dbg(dev, "copy failed\n");
		return -EFAULT;
	}

	if (ucmd.comp_mask) {
		mlx5_ib_dbg(dev, "invalid comp mask\n");
		return -EOPNOTSUPP;
	}

	if (ucmd.flags & ~MLX5_QP_FLAG_TUNNEL_OFFLOADS) {
		mlx5_ib_dbg(dev, "invalid flags\n");
		return -EOPNOTSUPP;
	}

	if (ucmd.flags & MLX5_QP_FLAG_TUNNEL_OFFLOADS &&
	    !tunnel_offload_supported(dev->mdev)) {
		mlx5_ib_dbg(dev, "tunnel offloads isn't supported\n");
		return -EOPNOTSUPP;
	}

	if (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_INNER &&
	    !(ucmd.flags & MLX5_QP_FLAG_TUNNEL_OFFLOADS)) {
		mlx5_ib_dbg(dev, "Tunnel offloads must be set for inner RSS\n");
		return -EOPNOTSUPP;
	}

	err = ib_copy_to_udata(udata, &resp, min(udata->outlen, sizeof(resp)));
	if (err) {
		mlx5_ib_dbg(dev, "copy failed\n");
		return -EINVAL;
	}

	inlen = MLX5_ST_SZ_BYTES(create_tir_in);
	in = kvzalloc(inlen, GFP_KERNEL);
	if (!in)
		return -ENOMEM;

	tirc = MLX5_ADDR_OF(create_tir_in, in, ctx);
	MLX5_SET(tirc, tirc, disp_type,
		 MLX5_TIRC_DISP_TYPE_INDIRECT);
	MLX5_SET(tirc, tirc, indirect_table,
		 init_attr->rwq_ind_tbl->ind_tbl_num);
	MLX5_SET(tirc, tirc, transport_domain, tdn);

	hfso = MLX5_ADDR_OF(tirc, tirc, rx_hash_field_selector_outer);

	if (ucmd.flags & MLX5_QP_FLAG_TUNNEL_OFFLOADS)
		MLX5_SET(tirc, tirc, tunneled_offload_en, 1);

	if (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_INNER)
		hfso = MLX5_ADDR_OF(tirc, tirc, rx_hash_field_selector_inner);
	else
		hfso = MLX5_ADDR_OF(tirc, tirc, rx_hash_field_selector_outer);

	switch (ucmd.rx_hash_function) {
	case MLX5_RX_HASH_FUNC_TOEPLITZ:
	{
		void *rss_key = MLX5_ADDR_OF(tirc, tirc, rx_hash_toeplitz_key);
		size_t len = MLX5_FLD_SZ_BYTES(tirc, rx_hash_toeplitz_key);

		if (len != ucmd.rx_key_len) {
			err = -EINVAL;
			goto err;
		}

		MLX5_SET(tirc, tirc, rx_hash_fn, MLX5_RX_HASH_FN_TOEPLITZ);
		MLX5_SET(tirc, tirc, rx_hash_symmetric, 1);
		memcpy(rss_key, ucmd.rx_hash_key, len);
		break;
	}
	default:
		err = -EOPNOTSUPP;
		goto err;
	}

	if (!ucmd.rx_hash_fields_mask) {
		/* special case when this TIR serves as steering entry without hashing */
		if (!init_attr->rwq_ind_tbl->log_ind_tbl_size)
			goto create_tir;
		err = -EINVAL;
		goto err;
	}

	if (((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_IPV4) ||
	     (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_IPV4)) &&
	     ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_IPV6) ||
	     (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_IPV6))) {
		err = -EINVAL;
		goto err;
	}

	/* If none of IPV4 & IPV6 SRC/DST was set - this bit field is ignored */
	if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_IPV4) ||
	    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_IPV4))
		MLX5_SET(rx_hash_field_select, hfso, l3_prot_type,
			 MLX5_L3_PROT_TYPE_IPV4);
	else if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_IPV6) ||
		 (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_IPV6))
		MLX5_SET(rx_hash_field_select, hfso, l3_prot_type,
			 MLX5_L3_PROT_TYPE_IPV6);

	outer_l4 = ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_PORT_TCP) ||
		    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_PORT_TCP)) << 0 |
		   ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_PORT_UDP) ||
		    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_PORT_UDP)) << 1 |
		   (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_IPSEC_SPI) << 2;

	/* Check that only one l4 protocol is set */
	if (outer_l4 & (outer_l4 - 1)) {
		err = -EINVAL;
		goto err;
	}

	/* If none of TCP & UDP SRC/DST was set - this bit field is ignored */
	if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_PORT_TCP) ||
	    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_PORT_TCP))
		MLX5_SET(rx_hash_field_select, hfso, l4_prot_type,
			 MLX5_L4_PROT_TYPE_TCP);
	else if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_PORT_UDP) ||
		 (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_PORT_UDP))
		MLX5_SET(rx_hash_field_select, hfso, l4_prot_type,
			 MLX5_L4_PROT_TYPE_UDP);

	if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_IPV4) ||
	    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_IPV6))
		selected_fields |= MLX5_HASH_FIELD_SEL_SRC_IP;

	if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_IPV4) ||
	    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_IPV6))
		selected_fields |= MLX5_HASH_FIELD_SEL_DST_IP;

	if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_PORT_TCP) ||
	    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_SRC_PORT_UDP))
		selected_fields |= MLX5_HASH_FIELD_SEL_L4_SPORT;

	if ((ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_PORT_TCP) ||
	    (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_DST_PORT_UDP))
		selected_fields |= MLX5_HASH_FIELD_SEL_L4_DPORT;

	if (ucmd.rx_hash_fields_mask & MLX5_RX_HASH_IPSEC_SPI)
		selected_fields |= MLX5_HASH_FIELD_SEL_IPSEC_SPI;

	MLX5_SET(rx_hash_field_select, hfso, selected_fields, selected_fields);

create_tir:
	if (dev->rep)
		MLX5_SET(tirc, tirc, self_lb_block,
			 MLX5_TIRC_SELF_LB_BLOCK_BLOCK_UNICAST_);

	err = mlx5_core_create_tir(dev->mdev, in, inlen, &qp->rss_qp.tirn);

	if (err)
		goto err;

	kvfree(in);
	/* qpn is reserved for that QP */
	qp->trans_qp.base.mqp.qpn = 0;
	qp->flags |= MLX5_IB_QP_RSS;
	return 0;

err:
	kvfree(in);
	return err;
}
