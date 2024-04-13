int mlx5_ib_modify_qp(struct ib_qp *ibqp, struct ib_qp_attr *attr,
		      int attr_mask, struct ib_udata *udata)
{
	struct mlx5_ib_dev *dev = to_mdev(ibqp->device);
	struct mlx5_ib_qp *qp = to_mqp(ibqp);
	struct mlx5_ib_modify_qp ucmd = {};
	enum ib_qp_type qp_type;
	enum ib_qp_state cur_state, new_state;
	size_t required_cmd_sz;
	int err = -EINVAL;
	int port;
	enum rdma_link_layer ll = IB_LINK_LAYER_UNSPECIFIED;

	if (ibqp->rwq_ind_tbl)
		return -ENOSYS;

	if (udata && udata->inlen) {
		required_cmd_sz = offsetof(typeof(ucmd), reserved) +
			sizeof(ucmd.reserved);
		if (udata->inlen < required_cmd_sz)
			return -EINVAL;

		if (udata->inlen > sizeof(ucmd) &&
		    !ib_is_udata_cleared(udata, sizeof(ucmd),
					 udata->inlen - sizeof(ucmd)))
			return -EOPNOTSUPP;

		if (ib_copy_from_udata(&ucmd, udata,
				       min(udata->inlen, sizeof(ucmd))))
			return -EFAULT;

		if (ucmd.comp_mask ||
		    memchr_inv(&ucmd.reserved, 0, sizeof(ucmd.reserved)) ||
		    memchr_inv(&ucmd.burst_info.reserved, 0,
			       sizeof(ucmd.burst_info.reserved)))
			return -EOPNOTSUPP;
	}

	if (unlikely(ibqp->qp_type == IB_QPT_GSI))
		return mlx5_ib_gsi_modify_qp(ibqp, attr, attr_mask);

	if (ibqp->qp_type == IB_QPT_DRIVER)
		qp_type = qp->qp_sub_type;
	else
		qp_type = (unlikely(ibqp->qp_type == MLX5_IB_QPT_HW_GSI)) ?
			IB_QPT_GSI : ibqp->qp_type;

	if (qp_type == MLX5_IB_QPT_DCT)
		return mlx5_ib_modify_dct(ibqp, attr, attr_mask, udata);

	mutex_lock(&qp->mutex);

	cur_state = attr_mask & IB_QP_CUR_STATE ? attr->cur_qp_state : qp->state;
	new_state = attr_mask & IB_QP_STATE ? attr->qp_state : cur_state;

	if (!(cur_state == new_state && cur_state == IB_QPS_RESET)) {
		port = attr_mask & IB_QP_PORT ? attr->port_num : qp->port;
		ll = dev->ib_dev.get_link_layer(&dev->ib_dev, port);
	}

	if (qp->flags & MLX5_IB_QP_UNDERLAY) {
		if (attr_mask & ~(IB_QP_STATE | IB_QP_CUR_STATE)) {
			mlx5_ib_dbg(dev, "invalid attr_mask 0x%x when underlay QP is used\n",
				    attr_mask);
			goto out;
		}
	} else if (qp_type != MLX5_IB_QPT_REG_UMR &&
		   qp_type != MLX5_IB_QPT_DCI &&
		   !ib_modify_qp_is_ok(cur_state, new_state, qp_type, attr_mask, ll)) {
		mlx5_ib_dbg(dev, "invalid QP state transition from %d to %d, qp_type %d, attr_mask 0x%x\n",
			    cur_state, new_state, ibqp->qp_type, attr_mask);
		goto out;
	} else if (qp_type == MLX5_IB_QPT_DCI &&
		   !modify_dci_qp_is_ok(cur_state, new_state, attr_mask)) {
		mlx5_ib_dbg(dev, "invalid QP state transition from %d to %d, qp_type %d, attr_mask 0x%x\n",
			    cur_state, new_state, qp_type, attr_mask);
		goto out;
	}

	if ((attr_mask & IB_QP_PORT) &&
	    (attr->port_num == 0 ||
	     attr->port_num > dev->num_ports)) {
		mlx5_ib_dbg(dev, "invalid port number %d. number of ports is %d\n",
			    attr->port_num, dev->num_ports);
		goto out;
	}

	if (attr_mask & IB_QP_PKEY_INDEX) {
		port = attr_mask & IB_QP_PORT ? attr->port_num : qp->port;
		if (attr->pkey_index >=
		    dev->mdev->port_caps[port - 1].pkey_table_len) {
			mlx5_ib_dbg(dev, "invalid pkey index %d\n",
				    attr->pkey_index);
			goto out;
		}
	}

	if (attr_mask & IB_QP_MAX_QP_RD_ATOMIC &&
	    attr->max_rd_atomic >
	    (1 << MLX5_CAP_GEN(dev->mdev, log_max_ra_res_qp))) {
		mlx5_ib_dbg(dev, "invalid max_rd_atomic value %d\n",
			    attr->max_rd_atomic);
		goto out;
	}

	if (attr_mask & IB_QP_MAX_DEST_RD_ATOMIC &&
	    attr->max_dest_rd_atomic >
	    (1 << MLX5_CAP_GEN(dev->mdev, log_max_ra_req_qp))) {
		mlx5_ib_dbg(dev, "invalid max_dest_rd_atomic value %d\n",
			    attr->max_dest_rd_atomic);
		goto out;
	}

	if (cur_state == new_state && cur_state == IB_QPS_RESET) {
		err = 0;
		goto out;
	}

	err = __mlx5_ib_modify_qp(ibqp, attr, attr_mask, cur_state,
				  new_state, &ucmd);

out:
	mutex_unlock(&qp->mutex);
	return err;
}
