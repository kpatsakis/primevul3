dcmd_timeout_ocr_possible(struct megasas_instance *instance) {

	if (instance->adapter_type == MFI_SERIES)
		return KILL_ADAPTER;
	else if (instance->unload ||
			test_bit(MEGASAS_FUSION_IN_RESET, &instance->reset_flags))
		return IGNORE_TIMEOUT;
	else
		return INITIATE_OCR;
}
