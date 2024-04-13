static int create_user_qp(struct mlx5_ib_dev *dev, struct ib_pd *pd,
			  struct mlx5_ib_qp *qp, struct ib_udata *udata,
			  struct ib_qp_init_attr *attr,
			  u32 **in,
			  struct mlx5_ib_create_qp_resp *resp, int *inlen,
			  struct mlx5_ib_qp_base *base)
{
	struct mlx5_ib_ucontext *context;
	struct mlx5_ib_create_qp ucmd;
	struct mlx5_ib_ubuffer *ubuffer = &base->ubuffer;
	int page_shift = 0;
	int uar_index = 0;
	int npages;
	u32 offset = 0;
	int bfregn;
	int ncont = 0;
	__be64 *pas;
	void *qpc;
	int err;

	err = ib_copy_from_udata(&ucmd, udata, sizeof(ucmd));
	if (err) {
		mlx5_ib_dbg(dev, "copy failed\n");
		return err;
	}

	context = to_mucontext(pd->uobject->context);
	if (ucmd.flags & MLX5_QP_FLAG_BFREG_INDEX) {
		uar_index = bfregn_to_uar_index(dev, &context->bfregi,
						ucmd.bfreg_index, true);
		if (uar_index < 0)
			return uar_index;

		bfregn = MLX5_IB_INVALID_BFREG;
	} else if (qp->flags & MLX5_IB_QP_CROSS_CHANNEL) {
		/*
		 * TBD: should come from the verbs when we have the API
		 */
		/* In CROSS_CHANNEL CQ and QP must use the same UAR */
		bfregn = MLX5_CROSS_CHANNEL_BFREG;
	}
	else {
		bfregn = alloc_bfreg(dev, &context->bfregi);
		if (bfregn < 0)
			return bfregn;
	}

	mlx5_ib_dbg(dev, "bfregn 0x%x, uar_index 0x%x\n", bfregn, uar_index);
	if (bfregn != MLX5_IB_INVALID_BFREG)
		uar_index = bfregn_to_uar_index(dev, &context->bfregi, bfregn,
						false);

	qp->rq.offset = 0;
	qp->sq.wqe_shift = ilog2(MLX5_SEND_WQE_BB);
	qp->sq.offset = qp->rq.wqe_cnt << qp->rq.wqe_shift;

	err = set_user_buf_size(dev, qp, &ucmd, base, attr);
	if (err)
		goto err_bfreg;

	if (ucmd.buf_addr && ubuffer->buf_size) {
		ubuffer->buf_addr = ucmd.buf_addr;
		err = mlx5_ib_umem_get(dev, pd, ubuffer->buf_addr,
				       ubuffer->buf_size,
				       &ubuffer->umem, &npages, &page_shift,
				       &ncont, &offset);
		if (err)
			goto err_bfreg;
	} else {
		ubuffer->umem = NULL;
	}

	*inlen = MLX5_ST_SZ_BYTES(create_qp_in) +
		 MLX5_FLD_SZ_BYTES(create_qp_in, pas[0]) * ncont;
	*in = kvzalloc(*inlen, GFP_KERNEL);
	if (!*in) {
		err = -ENOMEM;
		goto err_umem;
	}

	pas = (__be64 *)MLX5_ADDR_OF(create_qp_in, *in, pas);
	if (ubuffer->umem)
		mlx5_ib_populate_pas(dev, ubuffer->umem, page_shift, pas, 0);

	qpc = MLX5_ADDR_OF(create_qp_in, *in, qpc);

	MLX5_SET(qpc, qpc, log_page_size, page_shift - MLX5_ADAPTER_PAGE_SHIFT);
	MLX5_SET(qpc, qpc, page_offset, offset);

	MLX5_SET(qpc, qpc, uar_page, uar_index);
	if (bfregn != MLX5_IB_INVALID_BFREG)
		resp->bfreg_index = adjust_bfregn(dev, &context->bfregi, bfregn);
	else
		resp->bfreg_index = MLX5_IB_INVALID_BFREG;
	qp->bfregn = bfregn;

	err = mlx5_ib_db_map_user(context, ucmd.db_addr, &qp->db);
	if (err) {
		mlx5_ib_dbg(dev, "map failed\n");
		goto err_free;
	}

	err = ib_copy_to_udata(udata, resp, min(udata->outlen, sizeof(*resp)));
	if (err) {
		mlx5_ib_dbg(dev, "copy failed\n");
		goto err_unmap;
	}
	qp->create_type = MLX5_QP_USER;

	return 0;

err_unmap:
	mlx5_ib_db_unmap_user(context, &qp->db);

err_free:
	kvfree(*in);

err_umem:
	if (ubuffer->umem)
		ib_umem_release(ubuffer->umem);

err_bfreg:
	if (bfregn != MLX5_IB_INVALID_BFREG)
		mlx5_ib_free_bfreg(dev, &context->bfregi, bfregn);
	return err;
}
