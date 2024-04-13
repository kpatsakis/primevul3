static void InsertSampler(FlowSource_t *fs, exporter_ipfix_domain_t *exporter, int32_t id, uint16_t mode, uint32_t interval) {
generic_sampler_t *sampler;

	dbg_printf("[%u] Insert Sampler: Exporter is 0x%llu\n", exporter->info.id, (long long unsigned)exporter);
	if ( !exporter->sampler ) {
		sampler = (generic_sampler_t *)malloc(sizeof(generic_sampler_t));
		if ( !sampler ) {
			LogError( "Process_v9: Panic! malloc(): %s line %d: %s", __FILE__, __LINE__, strerror (errno));
			return;
		}

		sampler->info.header.type = SamplerInfoRecordype;
		sampler->info.header.size = sizeof(sampler_info_record_t);
		sampler->info.exporter_sysid = exporter->info.sysid;
		sampler->info.id	   = id;
		sampler->info.mode	 = mode;
		sampler->info.interval = interval;
		sampler->next		  = NULL;
		exporter->sampler = sampler;

		FlushInfoSampler(fs, &(sampler->info));
		LogInfo( "Add new sampler: ID: %i, mode: %u, interval: %u\n", 
			id, mode, interval);
		dbg_printf("Add new sampler: ID: %i, mode: %u, interval: %u\n", 
			id, mode, interval);

	} else {
		sampler = exporter->sampler;
		while ( sampler ) {
			if ( sampler->info.id == id ) {
				dbg_printf("Update existing sampler id: %i, mode: %u, interval: %u\n", 
					id, mode, interval);

				if ( mode != sampler->info.mode || interval != sampler->info.interval ) {
					FlushInfoSampler(fs, &(sampler->info));
					sampler->info.mode	 = mode;
					sampler->info.interval = interval;
					LogInfo( "Update existing sampler id: %i, mode: %u, interval: %u\n", 
						id, mode, interval);
				} else {
					dbg_printf("Sampler unchanged!\n");
				}

				break;
			}

			if ( sampler->next == NULL ) {
				sampler->next = (generic_sampler_t *)malloc(sizeof(generic_sampler_t));
				if ( !sampler->next ) {
					LogError( "Process_v9: Panic! malloc(): %s line %d: %s", __FILE__, __LINE__, strerror (errno));
					return;
				}
				sampler = sampler->next;

				sampler->info.header.type	 = SamplerInfoRecordype;
				sampler->info.header.size	 = sizeof(sampler_info_record_t);
				sampler->info.exporter_sysid = exporter->info.sysid;
				sampler->info.id	   	= id;
				sampler->info.mode		= mode;
				sampler->info.interval	= interval;
				sampler->next			= NULL;

				FlushInfoSampler(fs, &(sampler->info));

				LogInfo( "Append new sampler: ID: %u, mode: %u, interval: %u\n", 
					id, mode, interval);
				dbg_printf("Append new sampler: ID: %u, mode: %u, interval: %u\n", 
					id, mode, interval);
				break;
			}

			sampler = sampler->next;
		}
	} 
	
} // End of InsertSampler
