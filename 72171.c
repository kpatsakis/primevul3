static int __mlx5_ib_modify_qp(struct ib_qp *ibqp,
			       const struct ib_qp_attr *attr, int attr_mask,
			       enum ib_qp_state cur_state, enum ib_qp_state new_state,
			       const struct mlx5_ib_modify_qp *ucmd)
{
	static const u16 optab[MLX5_QP_NUM_STATE][MLX5_QP_NUM_STATE] = {
		[MLX5_QP_STATE_RST] = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
			[MLX5_QP_STATE_INIT]	= MLX5_CMD_OP_RST2INIT_QP,
		},
		[MLX5_QP_STATE_INIT]  = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
			[MLX5_QP_STATE_INIT]	= MLX5_CMD_OP_INIT2INIT_QP,
			[MLX5_QP_STATE_RTR]	= MLX5_CMD_OP_INIT2RTR_QP,
		},
		[MLX5_QP_STATE_RTR]   = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
			[MLX5_QP_STATE_RTS]	= MLX5_CMD_OP_RTR2RTS_QP,
		},
		[MLX5_QP_STATE_RTS]   = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
			[MLX5_QP_STATE_RTS]	= MLX5_CMD_OP_RTS2RTS_QP,
		},
		[MLX5_QP_STATE_SQD] = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
		},
		[MLX5_QP_STATE_SQER] = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
			[MLX5_QP_STATE_RTS]	= MLX5_CMD_OP_SQERR2RTS_QP,
		},
		[MLX5_QP_STATE_ERR] = {
			[MLX5_QP_STATE_RST]	= MLX5_CMD_OP_2RST_QP,
			[MLX5_QP_STATE_ERR]	= MLX5_CMD_OP_2ERR_QP,
		}
	};

	struct mlx5_ib_dev *dev = to_mdev(ibqp->device);
	struct mlx5_ib_qp *qp = to_mqp(ibqp);
	struct mlx5_ib_qp_base *base = &qp->trans_qp.base;
	struct mlx5_ib_cq *send_cq, *recv_cq;
	struct mlx5_qp_context *context;
	struct mlx5_ib_pd *pd;
	struct mlx5_ib_port *mibport = NULL;
	enum mlx5_qp_state mlx5_cur, mlx5_new;
	enum mlx5_qp_optpar optpar;
	int mlx5_st;
	int err;
	u16 op;
	u8 tx_affinity = 0;

	mlx5_st = to_mlx5_st(ibqp->qp_type == IB_QPT_DRIVER ?
			     qp->qp_sub_type : ibqp->qp_type);
	if (mlx5_st < 0)
		return -EINVAL;

	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return -ENOMEM;

	context->flags = cpu_to_be32(mlx5_st << 16);

	if (!(attr_mask & IB_QP_PATH_MIG_STATE)) {
		context->flags |= cpu_to_be32(MLX5_QP_PM_MIGRATED << 11);
	} else {
		switch (attr->path_mig_state) {
		case IB_MIG_MIGRATED:
			context->flags |= cpu_to_be32(MLX5_QP_PM_MIGRATED << 11);
			break;
		case IB_MIG_REARM:
			context->flags |= cpu_to_be32(MLX5_QP_PM_REARM << 11);
			break;
		case IB_MIG_ARMED:
			context->flags |= cpu_to_be32(MLX5_QP_PM_ARMED << 11);
			break;
		}
	}

	if ((cur_state == IB_QPS_RESET) && (new_state == IB_QPS_INIT)) {
		if ((ibqp->qp_type == IB_QPT_RC) ||
		    (ibqp->qp_type == IB_QPT_UD &&
		     !(qp->flags & MLX5_IB_QP_SQPN_QP1)) ||
		    (ibqp->qp_type == IB_QPT_UC) ||
		    (ibqp->qp_type == IB_QPT_RAW_PACKET) ||
		    (ibqp->qp_type == IB_QPT_XRC_INI) ||
		    (ibqp->qp_type == IB_QPT_XRC_TGT)) {
			if (mlx5_lag_is_active(dev->mdev)) {
				u8 p = mlx5_core_native_port_num(dev->mdev);
				tx_affinity = (unsigned int)atomic_add_return(1,
						&dev->roce[p].next_port) %
						MLX5_MAX_PORTS + 1;
				context->flags |= cpu_to_be32(tx_affinity << 24);
			}
		}
	}

	if (is_sqp(ibqp->qp_type)) {
		context->mtu_msgmax = (IB_MTU_256 << 5) | 8;
	} else if ((ibqp->qp_type == IB_QPT_UD &&
		    !(qp->flags & MLX5_IB_QP_UNDERLAY)) ||
		   ibqp->qp_type == MLX5_IB_QPT_REG_UMR) {
		context->mtu_msgmax = (IB_MTU_4096 << 5) | 12;
	} else if (attr_mask & IB_QP_PATH_MTU) {
		if (attr->path_mtu < IB_MTU_256 ||
		    attr->path_mtu > IB_MTU_4096) {
			mlx5_ib_warn(dev, "invalid mtu %d\n", attr->path_mtu);
			err = -EINVAL;
			goto out;
		}
		context->mtu_msgmax = (attr->path_mtu << 5) |
				      (u8)MLX5_CAP_GEN(dev->mdev, log_max_msg);
	}

	if (attr_mask & IB_QP_DEST_QPN)
		context->log_pg_sz_remote_qpn = cpu_to_be32(attr->dest_qp_num);

	if (attr_mask & IB_QP_PKEY_INDEX)
		context->pri_path.pkey_index = cpu_to_be16(attr->pkey_index);

	/* todo implement counter_index functionality */

	if (is_sqp(ibqp->qp_type))
		context->pri_path.port = qp->port;

	if (attr_mask & IB_QP_PORT)
		context->pri_path.port = attr->port_num;

	if (attr_mask & IB_QP_AV) {
		err = mlx5_set_path(dev, qp, &attr->ah_attr, &context->pri_path,
				    attr_mask & IB_QP_PORT ? attr->port_num : qp->port,
				    attr_mask, 0, attr, false);
		if (err)
			goto out;
	}

	if (attr_mask & IB_QP_TIMEOUT)
		context->pri_path.ackto_lt |= attr->timeout << 3;

	if (attr_mask & IB_QP_ALT_PATH) {
		err = mlx5_set_path(dev, qp, &attr->alt_ah_attr,
				    &context->alt_path,
				    attr->alt_port_num,
				    attr_mask | IB_QP_PKEY_INDEX | IB_QP_TIMEOUT,
				    0, attr, true);
		if (err)
			goto out;
	}

	pd = get_pd(qp);
	get_cqs(qp->ibqp.qp_type, qp->ibqp.send_cq, qp->ibqp.recv_cq,
		&send_cq, &recv_cq);

	context->flags_pd = cpu_to_be32(pd ? pd->pdn : to_mpd(dev->devr.p0)->pdn);
	context->cqn_send = send_cq ? cpu_to_be32(send_cq->mcq.cqn) : 0;
	context->cqn_recv = recv_cq ? cpu_to_be32(recv_cq->mcq.cqn) : 0;
	context->params1  = cpu_to_be32(MLX5_IB_ACK_REQ_FREQ << 28);

	if (attr_mask & IB_QP_RNR_RETRY)
		context->params1 |= cpu_to_be32(attr->rnr_retry << 13);

	if (attr_mask & IB_QP_RETRY_CNT)
		context->params1 |= cpu_to_be32(attr->retry_cnt << 16);

	if (attr_mask & IB_QP_MAX_QP_RD_ATOMIC) {
		if (attr->max_rd_atomic)
			context->params1 |=
				cpu_to_be32(fls(attr->max_rd_atomic - 1) << 21);
	}

	if (attr_mask & IB_QP_SQ_PSN)
		context->next_send_psn = cpu_to_be32(attr->sq_psn);

	if (attr_mask & IB_QP_MAX_DEST_RD_ATOMIC) {
		if (attr->max_dest_rd_atomic)
			context->params2 |=
				cpu_to_be32(fls(attr->max_dest_rd_atomic - 1) << 21);
	}

	if (attr_mask & (IB_QP_ACCESS_FLAGS | IB_QP_MAX_DEST_RD_ATOMIC))
		context->params2 |= to_mlx5_access_flags(qp, attr, attr_mask);

	if (attr_mask & IB_QP_MIN_RNR_TIMER)
		context->rnr_nextrecvpsn |= cpu_to_be32(attr->min_rnr_timer << 24);

	if (attr_mask & IB_QP_RQ_PSN)
		context->rnr_nextrecvpsn |= cpu_to_be32(attr->rq_psn);

	if (attr_mask & IB_QP_QKEY)
		context->qkey = cpu_to_be32(attr->qkey);

	if (qp->rq.wqe_cnt && cur_state == IB_QPS_RESET && new_state == IB_QPS_INIT)
		context->db_rec_addr = cpu_to_be64(qp->db.dma);

	if (cur_state == IB_QPS_RESET && new_state == IB_QPS_INIT) {
		u8 port_num = (attr_mask & IB_QP_PORT ? attr->port_num :
			       qp->port) - 1;

		/* Underlay port should be used - index 0 function per port */
		if (qp->flags & MLX5_IB_QP_UNDERLAY)
			port_num = 0;

		mibport = &dev->port[port_num];
		context->qp_counter_set_usr_page |=
			cpu_to_be32((u32)(mibport->cnts.set_id) << 24);
	}

	if (!ibqp->uobject && cur_state == IB_QPS_RESET && new_state == IB_QPS_INIT)
		context->sq_crq_size |= cpu_to_be16(1 << 4);

	if (qp->flags & MLX5_IB_QP_SQPN_QP1)
		context->deth_sqpn = cpu_to_be32(1);

	mlx5_cur = to_mlx5_state(cur_state);
	mlx5_new = to_mlx5_state(new_state);

	if (mlx5_cur >= MLX5_QP_NUM_STATE || mlx5_new >= MLX5_QP_NUM_STATE ||
	    !optab[mlx5_cur][mlx5_new]) {
		err = -EINVAL;
		goto out;
	}

	op = optab[mlx5_cur][mlx5_new];
	optpar = ib_mask_to_mlx5_opt(attr_mask);
	optpar &= opt_mask[mlx5_cur][mlx5_new][mlx5_st];

	if (qp->ibqp.qp_type == IB_QPT_RAW_PACKET ||
	    qp->flags & MLX5_IB_QP_UNDERLAY) {
		struct mlx5_modify_raw_qp_param raw_qp_param = {};

		raw_qp_param.operation = op;
		if (cur_state == IB_QPS_RESET && new_state == IB_QPS_INIT) {
			raw_qp_param.rq_q_ctr_id = mibport->cnts.set_id;
			raw_qp_param.set_mask |= MLX5_RAW_QP_MOD_SET_RQ_Q_CTR_ID;
		}

		if (attr_mask & IB_QP_RATE_LIMIT) {
			raw_qp_param.rl.rate = attr->rate_limit;

			if (ucmd->burst_info.max_burst_sz) {
				if (attr->rate_limit &&
				    MLX5_CAP_QOS(dev->mdev, packet_pacing_burst_bound)) {
					raw_qp_param.rl.max_burst_sz =
						ucmd->burst_info.max_burst_sz;
				} else {
					err = -EINVAL;
					goto out;
				}
			}

			if (ucmd->burst_info.typical_pkt_sz) {
				if (attr->rate_limit &&
				    MLX5_CAP_QOS(dev->mdev, packet_pacing_typical_size)) {
					raw_qp_param.rl.typical_pkt_sz =
						ucmd->burst_info.typical_pkt_sz;
				} else {
					err = -EINVAL;
					goto out;
				}
			}

			raw_qp_param.set_mask |= MLX5_RAW_QP_RATE_LIMIT;
		}

		err = modify_raw_packet_qp(dev, qp, &raw_qp_param, tx_affinity);
	} else {
		err = mlx5_core_qp_modify(dev->mdev, op, optpar, context,
					  &base->mqp);
	}

	if (err)
		goto out;

	qp->state = new_state;

	if (attr_mask & IB_QP_ACCESS_FLAGS)
		qp->trans_qp.atomic_rd_en = attr->qp_access_flags;
	if (attr_mask & IB_QP_MAX_DEST_RD_ATOMIC)
		qp->trans_qp.resp_depth = attr->max_dest_rd_atomic;
	if (attr_mask & IB_QP_PORT)
		qp->port = attr->port_num;
	if (attr_mask & IB_QP_ALT_PATH)
		qp->trans_qp.alt_port = attr->alt_port_num;

	/*
	 * If we moved a kernel QP to RESET, clean up all old CQ
	 * entries and reinitialize the QP.
	 */
	if (new_state == IB_QPS_RESET &&
	    !ibqp->uobject && ibqp->qp_type != IB_QPT_XRC_TGT) {
		mlx5_ib_cq_clean(recv_cq, base->mqp.qpn,
				 ibqp->srq ? to_msrq(ibqp->srq) : NULL);
		if (send_cq != recv_cq)
			mlx5_ib_cq_clean(send_cq, base->mqp.qpn, NULL);

		qp->rq.head = 0;
		qp->rq.tail = 0;
		qp->sq.head = 0;
		qp->sq.tail = 0;
		qp->sq.cur_post = 0;
		qp->sq.last_poll = 0;
		qp->db.db[MLX5_RCV_DBR] = 0;
		qp->db.db[MLX5_SND_DBR] = 0;
	}

out:
	kfree(context);
	return err;
}
