static int CheckSequenceMap(input_translation_t *table) {

	if ( table->number_of_sequences < table->max_number_of_sequences )
		return 1;

	dbg_printf("Extend sequence map %u -> ", table->max_number_of_sequences);
	void *p = realloc(table->sequence,  (table->max_number_of_sequences + 1) * sizeof(sequence_map_t));
	if ( !p ) {
		LogError("Process_ipfix: realloc() at %s line %d: %s", 
			__FILE__, __LINE__, strerror (errno));
		dbg_printf("\nProcess_ipfix: realloc() at %s line %d: %s", 
			__FILE__, __LINE__, strerror (errno));
		return 0;
	}
	table->sequence = p;
	table->max_number_of_sequences += 1;
	dbg_printf("%u\n", table->max_number_of_sequences);
	return 1;

} // End of CheckSequenceMap
