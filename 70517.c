static void Process_ipfix_templates(exporter_ipfix_domain_t *exporter, void *flowset_header, uint32_t size_left, FlowSource_t *fs) {
ipfix_template_record_t *ipfix_template_record;
void *DataPtr;
uint32_t count;

	size_left 	   -= 4;	// subtract message header
	DataPtr = flowset_header + 4;

	ipfix_template_record = (ipfix_template_record_t *)DataPtr;

	count = ntohs(ipfix_template_record->FieldCount);

	if ( count == 0 ) {
		Process_ipfix_template_withdraw(exporter, DataPtr, size_left, fs);
	} else {
		Process_ipfix_template_add(exporter, DataPtr, size_left, fs);
	}

} // End of Process_ipfix_templates
