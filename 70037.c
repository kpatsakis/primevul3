void module_data_cleanup(pam_handle_t *pamh, void *data, int error_status)
{
	struct module_data *module_data = data;
	if (module_data) {
		if (1 == module_data->module_loaded) {
			PKCS11_release_all_slots(module_data->ctx, module_data->slots, module_data->nslots);
			PKCS11_CTX_unload(module_data->ctx);
		}
		PKCS11_CTX_free(module_data->ctx);
		EVP_cleanup();
		ERR_free_strings();
		free(module_data);
	}
}
