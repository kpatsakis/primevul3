static int ccp_init_data(struct ccp_data *data, struct ccp_cmd_queue *cmd_q,
			 struct scatterlist *sg, u64 sg_len,
			 unsigned int dm_len,
			 enum dma_data_direction dir)
{
	int ret;

	memset(data, 0, sizeof(*data));

	ret = ccp_init_sg_workarea(&data->sg_wa, cmd_q->ccp->dev, sg, sg_len,
				   dir);
	if (ret)
		goto e_err;

	ret = ccp_init_dm_workarea(&data->dm_wa, cmd_q, dm_len, dir);
	if (ret)
		goto e_err;

	return 0;

e_err:
	ccp_free_data(data, cmd_q);

	return ret;
}