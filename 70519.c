static input_translation_t *add_translation_table(exporter_ipfix_domain_t *exporter, uint16_t id) {
input_translation_t **table;

	table = &(exporter->input_translation_table);
	while ( *table ) {
		table = &((*table)->next);
	}

	*table = calloc(1, sizeof(input_translation_t));
	if ( !(*table) ) {
			LogError("Process_ipfix: Panic! calloc() %s line %d: %s", __FILE__, __LINE__, strerror (errno));
			return NULL;
	}
	(*table)->max_number_of_sequences = 0;
	(*table)->number_of_sequences 	  = 0;
	(*table)->sequence = NULL;
	(*table)->id   	   = id;
	(*table)->next	   = NULL;

	dbg_printf("[%u] Get new translation table %u\n", exporter->info.id, id);

	return *table;

} // End of add_translation_table
