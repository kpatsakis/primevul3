static int prepare_user_rq(struct ib_pd *pd,
			   struct ib_wq_init_attr *init_attr,
			   struct ib_udata *udata,
			   struct mlx5_ib_rwq *rwq)
{
	struct mlx5_ib_dev *dev = to_mdev(pd->device);
	struct mlx5_ib_create_wq ucmd = {};
	int err;
	size_t required_cmd_sz;

	required_cmd_sz = offsetof(typeof(ucmd), single_stride_log_num_of_bytes)
		+ sizeof(ucmd.single_stride_log_num_of_bytes);
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

	if (ucmd.comp_mask & (~MLX5_IB_CREATE_WQ_STRIDING_RQ)) {
		mlx5_ib_dbg(dev, "invalid comp mask\n");
		return -EOPNOTSUPP;
	} else if (ucmd.comp_mask & MLX5_IB_CREATE_WQ_STRIDING_RQ) {
		if (!MLX5_CAP_GEN(dev->mdev, striding_rq)) {
			mlx5_ib_dbg(dev, "Striding RQ is not supported\n");
			return -EOPNOTSUPP;
		}
		if ((ucmd.single_stride_log_num_of_bytes <
		    MLX5_MIN_SINGLE_STRIDE_LOG_NUM_BYTES) ||
		    (ucmd.single_stride_log_num_of_bytes >
		     MLX5_MAX_SINGLE_STRIDE_LOG_NUM_BYTES)) {
			mlx5_ib_dbg(dev, "Invalid log stride size (%u. Range is %u - %u)\n",
				    ucmd.single_stride_log_num_of_bytes,
				    MLX5_MIN_SINGLE_STRIDE_LOG_NUM_BYTES,
				    MLX5_MAX_SINGLE_STRIDE_LOG_NUM_BYTES);
			return -EINVAL;
		}
		if ((ucmd.single_wqe_log_num_of_strides >
		    MLX5_MAX_SINGLE_WQE_LOG_NUM_STRIDES) ||
		     (ucmd.single_wqe_log_num_of_strides <
			MLX5_MIN_SINGLE_WQE_LOG_NUM_STRIDES)) {
			mlx5_ib_dbg(dev, "Invalid log num strides (%u. Range is %u - %u)\n",
				    ucmd.single_wqe_log_num_of_strides,
				    MLX5_MIN_SINGLE_WQE_LOG_NUM_STRIDES,
				    MLX5_MAX_SINGLE_WQE_LOG_NUM_STRIDES);
			return -EINVAL;
		}
		rwq->single_stride_log_num_of_bytes =
			ucmd.single_stride_log_num_of_bytes;
		rwq->log_num_strides = ucmd.single_wqe_log_num_of_strides;
		rwq->two_byte_shift_en = !!ucmd.two_byte_shift_en;
		rwq->create_flags |= MLX5_IB_WQ_FLAGS_STRIDING_RQ;
	}

	err = set_user_rq_size(dev, init_attr, &ucmd, rwq);
	if (err) {
		mlx5_ib_dbg(dev, "err %d\n", err);
		return err;
	}

	err = create_user_rq(dev, pd, rwq, &ucmd);
	if (err) {
		mlx5_ib_dbg(dev, "err %d\n", err);
		if (err)
			return err;
	}

	rwq->user_index = ucmd.user_index;
	return 0;
}
