static void ccp_free_data(struct ccp_data *data, struct ccp_cmd_queue *cmd_q)
{
	ccp_dm_free(&data->dm_wa);
	ccp_sg_free(&data->sg_wa);
}