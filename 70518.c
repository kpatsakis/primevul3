static void PushSequence(input_translation_t *table, uint16_t Type, uint32_t *offset, void *stack) {
uint32_t i = table->number_of_sequences;
uint32_t index = cache.lookup_info[Type].index;

	if ( !CheckSequenceMap(table) )
		return;

	table->sequence[i].skip_count = 0;
	table->sequence[i].type = Type;
	if ( cache.lookup_info[Type].found ) {
			table->sequence[i].id = ipfix_element_map[index].sequence;
			table->sequence[i].output_offset = offset ? *offset : 0;
			table->sequence[i].stack = stack;
			table->sequence[i].input_length = cache.lookup_info[Type].length;
	} else {
			table->sequence[i].id = ipfix_element_map[index].zero_sequence;
			table->sequence[i].output_offset = offset ? *offset : 0;
			table->sequence[i].stack = NULL;
	}
	dbg_printf("Push: sequence: %u, Type: %u, in length: %u, out length: %u, id: %u, out offset: %u\n",
		i, Type, ipfix_element_map[index].length, ipfix_element_map[index].out_length, 
		table->sequence[i].id, table->sequence[i].output_offset);
	table->number_of_sequences++;
	if ( offset ) 
		(*offset) += ipfix_element_map[index].out_length;

} // End of PushSequence
