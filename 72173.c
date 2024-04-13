static int _mlx5_ib_post_send(struct ib_qp *ibqp, const struct ib_send_wr *wr,
			      const struct ib_send_wr **bad_wr, bool drain)
{
	struct mlx5_wqe_ctrl_seg *ctrl = NULL;  /* compiler warning */
	struct mlx5_ib_dev *dev = to_mdev(ibqp->device);
	struct mlx5_core_dev *mdev = dev->mdev;
	struct mlx5_ib_qp *qp;
	struct mlx5_ib_mr *mr;
	struct mlx5_wqe_data_seg *dpseg;
	struct mlx5_wqe_xrc_seg *xrc;
	struct mlx5_bf *bf;
	int uninitialized_var(size);
	void *qend;
	unsigned long flags;
	unsigned idx;
	int err = 0;
	int num_sge;
	void *seg;
	int nreq;
	int i;
	u8 next_fence = 0;
	u8 fence;

	if (unlikely(ibqp->qp_type == IB_QPT_GSI))
		return mlx5_ib_gsi_post_send(ibqp, wr, bad_wr);

	qp = to_mqp(ibqp);
	bf = &qp->bf;
	qend = qp->sq.qend;

	spin_lock_irqsave(&qp->sq.lock, flags);

	if (mdev->state == MLX5_DEVICE_STATE_INTERNAL_ERROR && !drain) {
		err = -EIO;
		*bad_wr = wr;
		nreq = 0;
		goto out;
	}

	for (nreq = 0; wr; nreq++, wr = wr->next) {
		if (unlikely(wr->opcode >= ARRAY_SIZE(mlx5_ib_opcode))) {
			mlx5_ib_warn(dev, "\n");
			err = -EINVAL;
			*bad_wr = wr;
			goto out;
		}

		num_sge = wr->num_sge;
		if (unlikely(num_sge > qp->sq.max_gs)) {
			mlx5_ib_warn(dev, "\n");
			err = -EINVAL;
			*bad_wr = wr;
			goto out;
		}

		err = begin_wqe(qp, &seg, &ctrl, wr, &idx, &size, nreq);
		if (err) {
			mlx5_ib_warn(dev, "\n");
			err = -ENOMEM;
			*bad_wr = wr;
			goto out;
		}

		if (wr->opcode == IB_WR_LOCAL_INV ||
		    wr->opcode == IB_WR_REG_MR) {
			fence = dev->umr_fence;
			next_fence = MLX5_FENCE_MODE_INITIATOR_SMALL;
		} else if (wr->send_flags & IB_SEND_FENCE) {
			if (qp->next_fence)
				fence = MLX5_FENCE_MODE_SMALL_AND_FENCE;
			else
				fence = MLX5_FENCE_MODE_FENCE;
		} else {
			fence = qp->next_fence;
		}

		switch (ibqp->qp_type) {
		case IB_QPT_XRC_INI:
			xrc = seg;
			seg += sizeof(*xrc);
			size += sizeof(*xrc) / 16;
			/* fall through */
		case IB_QPT_RC:
			switch (wr->opcode) {
			case IB_WR_RDMA_READ:
			case IB_WR_RDMA_WRITE:
			case IB_WR_RDMA_WRITE_WITH_IMM:
				set_raddr_seg(seg, rdma_wr(wr)->remote_addr,
					      rdma_wr(wr)->rkey);
				seg += sizeof(struct mlx5_wqe_raddr_seg);
				size += sizeof(struct mlx5_wqe_raddr_seg) / 16;
				break;

			case IB_WR_ATOMIC_CMP_AND_SWP:
			case IB_WR_ATOMIC_FETCH_AND_ADD:
			case IB_WR_MASKED_ATOMIC_CMP_AND_SWP:
				mlx5_ib_warn(dev, "Atomic operations are not supported yet\n");
				err = -ENOSYS;
				*bad_wr = wr;
				goto out;

			case IB_WR_LOCAL_INV:
				qp->sq.wr_data[idx] = IB_WR_LOCAL_INV;
				ctrl->imm = cpu_to_be32(wr->ex.invalidate_rkey);
				set_linv_wr(qp, &seg, &size);
				num_sge = 0;
				break;

			case IB_WR_REG_MR:
				qp->sq.wr_data[idx] = IB_WR_REG_MR;
				ctrl->imm = cpu_to_be32(reg_wr(wr)->key);
				err = set_reg_wr(qp, reg_wr(wr), &seg, &size);
				if (err) {
					*bad_wr = wr;
					goto out;
				}
				num_sge = 0;
				break;

			case IB_WR_REG_SIG_MR:
				qp->sq.wr_data[idx] = IB_WR_REG_SIG_MR;
				mr = to_mmr(sig_handover_wr(wr)->sig_mr);

				ctrl->imm = cpu_to_be32(mr->ibmr.rkey);
				err = set_sig_umr_wr(wr, qp, &seg, &size);
				if (err) {
					mlx5_ib_warn(dev, "\n");
					*bad_wr = wr;
					goto out;
				}

				finish_wqe(qp, ctrl, size, idx, wr->wr_id, nreq,
					   fence, MLX5_OPCODE_UMR);
				/*
				 * SET_PSV WQEs are not signaled and solicited
				 * on error
				 */
				err = __begin_wqe(qp, &seg, &ctrl, wr, &idx,
						  &size, nreq, false, true);
				if (err) {
					mlx5_ib_warn(dev, "\n");
					err = -ENOMEM;
					*bad_wr = wr;
					goto out;
				}

				err = set_psv_wr(&sig_handover_wr(wr)->sig_attrs->mem,
						 mr->sig->psv_memory.psv_idx, &seg,
						 &size);
				if (err) {
					mlx5_ib_warn(dev, "\n");
					*bad_wr = wr;
					goto out;
				}

				finish_wqe(qp, ctrl, size, idx, wr->wr_id, nreq,
					   fence, MLX5_OPCODE_SET_PSV);
				err = __begin_wqe(qp, &seg, &ctrl, wr, &idx,
						  &size, nreq, false, true);
				if (err) {
					mlx5_ib_warn(dev, "\n");
					err = -ENOMEM;
					*bad_wr = wr;
					goto out;
				}

				err = set_psv_wr(&sig_handover_wr(wr)->sig_attrs->wire,
						 mr->sig->psv_wire.psv_idx, &seg,
						 &size);
				if (err) {
					mlx5_ib_warn(dev, "\n");
					*bad_wr = wr;
					goto out;
				}

				finish_wqe(qp, ctrl, size, idx, wr->wr_id, nreq,
					   fence, MLX5_OPCODE_SET_PSV);
				qp->next_fence = MLX5_FENCE_MODE_INITIATOR_SMALL;
				num_sge = 0;
				goto skip_psv;

			default:
				break;
			}
			break;

		case IB_QPT_UC:
			switch (wr->opcode) {
			case IB_WR_RDMA_WRITE:
			case IB_WR_RDMA_WRITE_WITH_IMM:
				set_raddr_seg(seg, rdma_wr(wr)->remote_addr,
					      rdma_wr(wr)->rkey);
				seg  += sizeof(struct mlx5_wqe_raddr_seg);
				size += sizeof(struct mlx5_wqe_raddr_seg) / 16;
				break;

			default:
				break;
			}
			break;

		case IB_QPT_SMI:
			if (unlikely(!mdev->port_caps[qp->port - 1].has_smi)) {
				mlx5_ib_warn(dev, "Send SMP MADs is not allowed\n");
				err = -EPERM;
				*bad_wr = wr;
				goto out;
			}
			/* fall through */
		case MLX5_IB_QPT_HW_GSI:
			set_datagram_seg(seg, wr);
			seg += sizeof(struct mlx5_wqe_datagram_seg);
			size += sizeof(struct mlx5_wqe_datagram_seg) / 16;
			if (unlikely((seg == qend)))
				seg = mlx5_get_send_wqe(qp, 0);
			break;
		case IB_QPT_UD:
			set_datagram_seg(seg, wr);
			seg += sizeof(struct mlx5_wqe_datagram_seg);
			size += sizeof(struct mlx5_wqe_datagram_seg) / 16;

			if (unlikely((seg == qend)))
				seg = mlx5_get_send_wqe(qp, 0);

			/* handle qp that supports ud offload */
			if (qp->flags & IB_QP_CREATE_IPOIB_UD_LSO) {
				struct mlx5_wqe_eth_pad *pad;

				pad = seg;
				memset(pad, 0, sizeof(struct mlx5_wqe_eth_pad));
				seg += sizeof(struct mlx5_wqe_eth_pad);
				size += sizeof(struct mlx5_wqe_eth_pad) / 16;

				seg = set_eth_seg(seg, wr, qend, qp, &size);

				if (unlikely((seg == qend)))
					seg = mlx5_get_send_wqe(qp, 0);
			}
			break;
		case MLX5_IB_QPT_REG_UMR:
			if (wr->opcode != MLX5_IB_WR_UMR) {
				err = -EINVAL;
				mlx5_ib_warn(dev, "bad opcode\n");
				goto out;
			}
			qp->sq.wr_data[idx] = MLX5_IB_WR_UMR;
			ctrl->imm = cpu_to_be32(umr_wr(wr)->mkey);
			err = set_reg_umr_segment(dev, seg, wr, !!(MLX5_CAP_GEN(mdev, atomic)));
			if (unlikely(err))
				goto out;
			seg += sizeof(struct mlx5_wqe_umr_ctrl_seg);
			size += sizeof(struct mlx5_wqe_umr_ctrl_seg) / 16;
			if (unlikely((seg == qend)))
				seg = mlx5_get_send_wqe(qp, 0);
			set_reg_mkey_segment(seg, wr);
			seg += sizeof(struct mlx5_mkey_seg);
			size += sizeof(struct mlx5_mkey_seg) / 16;
			if (unlikely((seg == qend)))
				seg = mlx5_get_send_wqe(qp, 0);
			break;

		default:
			break;
		}

		if (wr->send_flags & IB_SEND_INLINE && num_sge) {
			int uninitialized_var(sz);

			err = set_data_inl_seg(qp, wr, seg, &sz);
			if (unlikely(err)) {
				mlx5_ib_warn(dev, "\n");
				*bad_wr = wr;
				goto out;
			}
			size += sz;
		} else {
			dpseg = seg;
			for (i = 0; i < num_sge; i++) {
				if (unlikely(dpseg == qend)) {
					seg = mlx5_get_send_wqe(qp, 0);
					dpseg = seg;
				}
				if (likely(wr->sg_list[i].length)) {
					set_data_ptr_seg(dpseg, wr->sg_list + i);
					size += sizeof(struct mlx5_wqe_data_seg) / 16;
					dpseg++;
				}
			}
		}

		qp->next_fence = next_fence;
		finish_wqe(qp, ctrl, size, idx, wr->wr_id, nreq, fence,
			   mlx5_ib_opcode[wr->opcode]);
skip_psv:
		if (0)
			dump_wqe(qp, idx, size);
	}

out:
	if (likely(nreq)) {
		qp->sq.head += nreq;

		/* Make sure that descriptors are written before
		 * updating doorbell record and ringing the doorbell
		 */
		wmb();

		qp->db.db[MLX5_SND_DBR] = cpu_to_be32(qp->sq.cur_post);

		/* Make sure doorbell record is visible to the HCA before
		 * we hit doorbell */
		wmb();

		/* currently we support only regular doorbells */
		mlx5_write64((__be32 *)ctrl, bf->bfreg->map + bf->offset, NULL);
		/* Make sure doorbells don't leak out of SQ spinlock
		 * and reach the HCA out of order.
		 */
		mmiowb();
		bf->offset ^= bf->buf_size;
	}

	spin_unlock_irqrestore(&qp->sq.lock, flags);

	return err;
}
