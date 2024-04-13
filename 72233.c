struct ib_qp *mlx5_ib_create_qp(struct ib_pd *pd,
				struct ib_qp_init_attr *verbs_init_attr,
				struct ib_udata *udata)
{
	struct mlx5_ib_dev *dev;
	struct mlx5_ib_qp *qp;
	u16 xrcdn = 0;
	int err;
	struct ib_qp_init_attr mlx_init_attr;
	struct ib_qp_init_attr *init_attr = verbs_init_attr;

	if (pd) {
		dev = to_mdev(pd->device);

		if (init_attr->qp_type == IB_QPT_RAW_PACKET) {
			if (!pd->uobject) {
				mlx5_ib_dbg(dev, "Raw Packet QP is not supported for kernel consumers\n");
				return ERR_PTR(-EINVAL);
			} else if (!to_mucontext(pd->uobject->context)->cqe_version) {
				mlx5_ib_dbg(dev, "Raw Packet QP is only supported for CQE version > 0\n");
				return ERR_PTR(-EINVAL);
			}
		}
	} else {
		/* being cautious here */
		if (init_attr->qp_type != IB_QPT_XRC_TGT &&
		    init_attr->qp_type != MLX5_IB_QPT_REG_UMR) {
			pr_warn("%s: no PD for transport %s\n", __func__,
				ib_qp_type_str(init_attr->qp_type));
			return ERR_PTR(-EINVAL);
		}
		dev = to_mdev(to_mxrcd(init_attr->xrcd)->ibxrcd.device);
	}

	if (init_attr->qp_type == IB_QPT_DRIVER) {
		struct mlx5_ib_create_qp ucmd;

		init_attr = &mlx_init_attr;
		memcpy(init_attr, verbs_init_attr, sizeof(*verbs_init_attr));
		err = set_mlx_qp_type(dev, init_attr, &ucmd, udata);
		if (err)
			return ERR_PTR(err);

		if (init_attr->qp_type == MLX5_IB_QPT_DCI) {
			if (init_attr->cap.max_recv_wr ||
			    init_attr->cap.max_recv_sge) {
				mlx5_ib_dbg(dev, "DCI QP requires zero size receive queue\n");
				return ERR_PTR(-EINVAL);
			}
		} else {
			return mlx5_ib_create_dct(pd, init_attr, &ucmd);
		}
	}

	switch (init_attr->qp_type) {
	case IB_QPT_XRC_TGT:
	case IB_QPT_XRC_INI:
		if (!MLX5_CAP_GEN(dev->mdev, xrc)) {
			mlx5_ib_dbg(dev, "XRC not supported\n");
			return ERR_PTR(-ENOSYS);
		}
		init_attr->recv_cq = NULL;
		if (init_attr->qp_type == IB_QPT_XRC_TGT) {
			xrcdn = to_mxrcd(init_attr->xrcd)->xrcdn;
			init_attr->send_cq = NULL;
		}

		/* fall through */
	case IB_QPT_RAW_PACKET:
	case IB_QPT_RC:
	case IB_QPT_UC:
	case IB_QPT_UD:
	case IB_QPT_SMI:
	case MLX5_IB_QPT_HW_GSI:
	case MLX5_IB_QPT_REG_UMR:
	case MLX5_IB_QPT_DCI:
		qp = kzalloc(sizeof(*qp), GFP_KERNEL);
		if (!qp)
			return ERR_PTR(-ENOMEM);

		err = create_qp_common(dev, pd, init_attr, udata, qp);
		if (err) {
			mlx5_ib_dbg(dev, "create_qp_common failed\n");
			kfree(qp);
			return ERR_PTR(err);
		}

		if (is_qp0(init_attr->qp_type))
			qp->ibqp.qp_num = 0;
		else if (is_qp1(init_attr->qp_type))
			qp->ibqp.qp_num = 1;
		else
			qp->ibqp.qp_num = qp->trans_qp.base.mqp.qpn;

		mlx5_ib_dbg(dev, "ib qpnum 0x%x, mlx qpn 0x%x, rcqn 0x%x, scqn 0x%x\n",
			    qp->ibqp.qp_num, qp->trans_qp.base.mqp.qpn,
			    init_attr->recv_cq ? to_mcq(init_attr->recv_cq)->mcq.cqn : -1,
			    init_attr->send_cq ? to_mcq(init_attr->send_cq)->mcq.cqn : -1);

		qp->trans_qp.xrcdn = xrcdn;

		break;

	case IB_QPT_GSI:
		return mlx5_ib_gsi_create_qp(pd, init_attr);

	case IB_QPT_RAW_IPV6:
	case IB_QPT_RAW_ETHERTYPE:
	case IB_QPT_MAX:
	default:
		mlx5_ib_dbg(dev, "unsupported qp type %d\n",
			    init_attr->qp_type);
		/* Don't support raw QPs */
		return ERR_PTR(-EINVAL);
	}

	if (verbs_init_attr->qp_type == IB_QPT_DRIVER)
		qp->qp_sub_type = init_attr->qp_type;

	return &qp->ibqp;
}
