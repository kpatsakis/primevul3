void nfc_genl_data_init(struct nfc_genl_data *genl_data)
{
	genl_data->poll_req_portid = 0;
	mutex_init(&genl_data->genl_data_mutex);
}
