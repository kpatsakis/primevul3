static void Process_ipfix_option_templates(exporter_ipfix_domain_t *exporter, void *option_template_flowset, FlowSource_t *fs) {
uint8_t		*DataPtr;
uint32_t	size_left, size_required, i;
uint16_t	id, field_count, scope_field_count, offset;
uint16_t	offset_std_sampler_interval, offset_std_sampler_algorithm, found_std_sampling;

	i = 0;	// keep compiler happy
	size_left 		  = GET_FLOWSET_LENGTH(option_template_flowset) - 4; // -4 for flowset header -> id and length
	if ( size_left < 6 ) {
		LogError("Process_ipfix: [%u] option template length error: size left %u too small for an options template", 
			exporter->info.id, size_left);
		return;
	}

	DataPtr   		  = option_template_flowset + 4;
	id 	  			  = GET_OPTION_TEMPLATE_ID(DataPtr); 
	field_count 	  = GET_OPTION_TEMPLATE_FIELD_COUNT(DataPtr);
	scope_field_count = GET_OPTION_TEMPLATE_SCOPE_FIELD_COUNT(DataPtr);
	DataPtr   += 6;
	size_left -= 6;

	dbg_printf("Decode Option Template. id: %u, field count: %u, scope field count: %u\n",
		id, field_count, scope_field_count);

	if ( scope_field_count == 0  ) {
		LogError("Process_ipfx: [%u] scope field count error: length must not be zero", 
			exporter->info.id);
		dbg_printf("scope field count error: length must not be zero\n");
		return;
	}

	size_required = 2 * field_count * sizeof(uint16_t);
	dbg_printf("Size left: %u, size required: %u\n", size_left, size_required);
	if ( size_left < size_required ) {
		LogError("Process_ipfix: [%u] option template length error: size left %u too small for %u scopes length and %u options length", 
			exporter->info.id, size_left, field_count, scope_field_count);
		dbg_printf("option template length error: size left %u too small for field_count %u\n", 
			size_left, field_count);
		return;
	}

	if ( scope_field_count == 0  ) {
		LogError("Process_ipfxi: [%u] scope field count error: length must not be zero", 
			exporter->info.id);
		return;
	}

	offset_std_sampler_interval  = 0;
	offset_std_sampler_algorithm = 0;
	found_std_sampling			 = 0;
	offset = 0;

	for ( i=0; i<scope_field_count; i++ ) {
		uint16_t id, length;
		int Enterprise;

		if ( size_left && size_left < 4 ) {
			LogError("Process_ipfix [%u] Template size error at %s line %u" , 
				exporter->info.id, __FILE__, __LINE__, strerror (errno));
			return;
		}
		id 	   = Get_val16(DataPtr); DataPtr += 2;
		length = Get_val16(DataPtr); DataPtr += 2;
		size_left -= 4;
		Enterprise = id & 0x8000 ? 1 : 0;
		if ( Enterprise ) {
			size_required += 4;
			if ( size_left < 4 ) {
				LogError("Process_ipfix: [%u] option template length error: size left %u too small", 
					exporter->info.id, size_left);
				dbg_printf("option template length error: size left %u too small\n", size_left);
				return;
			}
			DataPtr += 4;
			size_left -= 4;
			dbg_printf(" [%i] Enterprise: 1, scope id: %u, scope length %u enterprise value: %u\n", 
				i, id, length, Get_val32(DataPtr));
		} else {
			dbg_printf(" [%i] Enterprise: 0, scope id: %u, scope length %u\n", i, id, length);
		}

		offset += length;
	}

	for ( ;i<field_count; i++ ) {
		uint32_t enterprise_value;
		uint16_t id, length;
		int Enterprise;

		UNUSED(enterprise_value);
		id 	   = Get_val16(DataPtr); DataPtr += 2;
		length = Get_val16(DataPtr); DataPtr += 2;
		size_left -= 4;
		Enterprise = id & 0x8000 ? 1 : 0;
		if ( Enterprise ) {
			size_required += 4;
			if ( size_left < 4 ) {
				LogError("Process_ipfix: [%u] option template length error: size left %u too", 
					exporter->info.id, size_left);
				dbg_printf("option template length error: size left %u too small\n", size_left);
				return;
			}
			enterprise_value = Get_val32(DataPtr);
			DataPtr += 4;
			size_left -= 4;
			dbg_printf(" [%i] Enterprise: 1, option id: %u, option length %u enterprise value: %u\n", 
				i, id, length, enterprise_value);
		} else {
			dbg_printf(" [%i] Enterprise: 0, option id: %u, option length %u\n", i, id, length);
		}

		switch (id) {
			case IPFIX_samplingInterval:		// legacy #34
			case IPFIX_samplingPacketInterval:	// #305
				if ( length == 4 ) {
					offset_std_sampler_interval = offset;
					found_std_sampling++;
					dbg_printf("	4 byte sampling interval option at offset: %u\n", offset);
				} else {
					LogError("Process_ipfix: [%u] option template error: sampling option lenth != 4 bytes: %u", 
						exporter->info.id, length);
				}
				break;
			case IPFIX_samplingAlgorithm:	// legacy #35
			case IPFIX_selectorAlgorithm:	// #304
				if ( length == 1 ) {
					offset_std_sampler_algorithm = offset;
					dbg_printf("	1 byte sampling algorithm option at offset: %u\n", offset);
					found_std_sampling++;
				} else {
					LogError("Process_ipfix: [%u] option template error: algorithm option lenth != 1 byte: %u", 
						exporter->info.id, length);
				}
				break;
		}

		offset += length;
	}

	if ( offset_std_sampler_interval ) {
        dbg_printf("[%u] Std sampling interval found. offset: %u\n", 
			exporter->info.id, offset_std_sampler_interval);
		if ( offset_std_sampler_algorithm )
        	dbg_printf("[%u] Std sampling algorithm found. offset: %u\n", 
			exporter->info.id, offset_std_sampler_algorithm);
        InsertStdSamplerOffset(fs, id, offset_std_sampler_interval, offset_std_sampler_algorithm);
		dbg_printf("\n");
	}

	processed_records++;

} // End of Process_ipfix_option_templates
