void nfc_genl_data_exit(struct nfc_genl_data *genl_data)
{
	mutex_destroy(&genl_data->genl_data_mutex);
}
