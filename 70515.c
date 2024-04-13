static void  Process_ipfix_option_data(exporter_ipfix_domain_t *exporter, void *data_flowset, FlowSource_t *fs) {
option_offset_t *offset_table;
uint32_t	id;
uint8_t	 *in;

	id  = GET_FLOWSET_ID(data_flowset);

	offset_table = fs->option_offset_table;
	while ( offset_table && offset_table->id != id )
		offset_table = offset_table->next;

	if ( !offset_table ) {
		LogError( "Process_ipfix: Panic! - No Offset table found! : %s line %d", __FILE__, __LINE__);
		return;
	}

#ifdef DEVEL
	uint32_t size_left = GET_FLOWSET_LENGTH(data_flowset) - 4; // -4 for data flowset header -> id and length
	dbg_printf("[%u] Process option data flowset size: %u\n", exporter->info.id, size_left);
#endif

	in	= (uint8_t *)(data_flowset + 4);  // skip flowset header

	if ( TestFlag(offset_table->flags, HAS_SAMPLER_DATA) ) {
		int32_t  id;
		uint16_t mode;
		uint32_t interval;
		if (offset_table->sampler_id_length == 2) {
			id = Get_val16((void *)&in[offset_table->offset_id]);
		} else {
			id = in[offset_table->offset_id];
		}

		mode = offset_table->offset_mode ? in[offset_table->offset_mode] : 0;
		interval = Get_val32((void *)&in[offset_table->offset_interval]); 
	
		InsertSampler(fs, exporter, id, mode, interval);

		dbg_printf("Extracted Sampler data:\n");
		dbg_printf("Sampler ID	  : %u\n", id);
		dbg_printf("Sampler mode	: %u\n", mode);
		dbg_printf("Sampler interval: %u\n", interval);
	}

	if ( TestFlag(offset_table->flags, HAS_STD_SAMPLER_DATA) ) {
		int32_t  id	   = -1;
		uint16_t mode	 = offset_table->offset_std_sampler_algorithm ? in[offset_table->offset_std_sampler_algorithm] : 0;
		uint32_t interval = Get_val32((void *)&in[offset_table->offset_std_sampler_interval]);

 		InsertSampler(fs, exporter, id, mode, interval);

		dbg_printf("Extracted Std Sampler data:\n");
		dbg_printf("Sampler ID	   : %i\n", id);
		dbg_printf("Sampler algorithm: %u\n", mode);
		dbg_printf("Sampler interval : %u\n", interval);

		dbg_printf("Set std sampler: algorithm: %u, interval: %u\n", 
				mode, interval);
	}
	processed_records++;

} // End of Process_ipfix_option_data
