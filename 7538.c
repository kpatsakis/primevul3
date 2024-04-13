static unsigned int ccp_empty_queue_buf(struct ccp_data *data)
{
	return ccp_queue_buf(data, 1);
}