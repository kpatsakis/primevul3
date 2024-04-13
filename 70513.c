void Process_IPFIX(void *in_buff, ssize_t in_buff_cnt, FlowSource_t *fs) {
exporter_ipfix_domain_t	*exporter;
ssize_t				size_left;
uint32_t			ExportTime, Sequence, flowset_length;
ipfix_header_t		*ipfix_header;
void				*flowset_header;

#ifdef DEVEL
static uint32_t		packet_cntr = 0;
uint32_t 			ObservationDomain;
#endif

	size_left 	 = in_buff_cnt;
	if ( size_left < IPFIX_HEADER_LENGTH ) {
		LogError("Process_ipfix: Too little data for ipfix packet: '%lli'", (long long)size_left);
		return;
	}

	ipfix_header = (ipfix_header_t *)in_buff;
	ExportTime 			 = ntohl(ipfix_header->ExportTime);
	Sequence 			 = ntohl(ipfix_header->LastSequence);

#ifdef DEVEL
	ObservationDomain 	 = ntohl(ipfix_header->ObservationDomain);
	packet_cntr++;
	printf("Next packet: %u\n", packet_cntr);
#endif

	exporter	= GetExporter(fs, ipfix_header);
	if ( !exporter ) {
		LogError("Process_ipfix: Exporter NULL: Abort ipfix record processing");
		return;
	}
	exporter->packets++;
	flowset_header	= (void *)ipfix_header + IPFIX_HEADER_LENGTH;
	size_left 	   -= IPFIX_HEADER_LENGTH;

	dbg_printf("\n[%u] process packet: %u, exported: %s, TemplateRecords: %llu, DataRecords: %llu, buffer: %li \n", 
		ObservationDomain, packet_cntr, UNIX2ISO(ExportTime), (long long unsigned)exporter->TemplateRecords, 
		(long long unsigned)exporter->DataRecords, size_left);

	dbg_printf("[%u] Sequence: %u\n", ObservationDomain, Sequence);

	if ( Sequence != exporter->PacketSequence ) {
		if ( exporter->DataRecords != 0 ) {
			fs->nffile->stat_record->sequence_failure++;
			exporter->sequence_failure++;
			dbg_printf("[%u] Sequence check failed: last seq: %u, seq %u\n", 
				exporter->info.id, Sequence, exporter->PacketSequence);
			/* maybee to noise on buggy exporters
			LogError("Process_ipfix [%u] Sequence error: last seq: %u, seq %u\n", 
				info.id, exporter->LastSequence, Sequence);
			*/
		} else {
			dbg_printf("[%u] Sync Sequence: %u\n", exporter->info.id, Sequence);
		}
		exporter->PacketSequence = Sequence;
	} else {
		dbg_printf("[%u] Sequence check ok\n", exporter->info.id);
	}

	flowset_length = 0;
	while (size_left) {
		uint16_t	flowset_id;

		if ( size_left && size_left < 4 ) {
			size_left = 0;
			continue;
		}

		flowset_header = flowset_header + flowset_length;

		flowset_id 		= GET_FLOWSET_ID(flowset_header);
		flowset_length 	= GET_FLOWSET_LENGTH(flowset_header);

		dbg_printf("Process_ipfix: Next flowset id %u, length %u.\n", flowset_id, flowset_length);

		if ( flowset_length == 0 ) {
			/* 	this should never happen, as 4 is an empty flowset 
				and smaller is an illegal flowset anyway ...
				if it happends, we can't determine the next flowset, so skip the entire export packet
			 */
			LogError("Process_ipfix: flowset zero length error.");
			dbg_printf("Process_ipfix: flowset zero length error.\n");
			return;

		}

		if ( flowset_length <= 4 ) {
			size_left = 0;
			continue;
		}

		if ( flowset_length > size_left ) {
			LogError("Process_ipfix: flowset length error. Expected bytes: %u > buffersize: %lli", 
				flowset_length, (long long)size_left);
			size_left = 0;
			continue;
		}


		switch (flowset_id) {
			case IPFIX_TEMPLATE_FLOWSET_ID:
				exporter->TemplateRecords++;
				dbg_printf("Process template flowset, length: %u\n", flowset_length);
				Process_ipfix_templates(exporter, flowset_header, flowset_length, fs);
				break;
			case IPFIX_OPTIONS_FLOWSET_ID:
				exporter->TemplateRecords++;
				dbg_printf("Process option template flowset, length: %u\n", flowset_length);
				Process_ipfix_option_templates(exporter, flowset_header, fs);
				break;
			default: {
				if ( flowset_id < IPFIX_MIN_RECORD_FLOWSET_ID ) {
					dbg_printf("Invalid flowset id: %u. Skip flowset\n", flowset_id);
					LogError("Process_ipfix: Invalid flowset id: %u. Skip flowset", flowset_id);
				} else {
					input_translation_t *table;
					dbg_printf("Process data flowset, length: %u\n", flowset_length);
					table = GetTranslationTable(exporter, flowset_id);
					if ( table ) {
						Process_ipfix_data(exporter, ExportTime, flowset_header, fs, table);
						exporter->DataRecords++;
					} else if ( HasOptionTable(fs, flowset_id) ) {
						Process_ipfix_option_data(exporter, flowset_header, fs);
					} else {
						dbg_printf("Process ipfix: [%u] No table for id %u -> Skip record\n", 
							exporter->info.id, flowset_id);
					}

				}
			}
		} // End of switch

		size_left -= flowset_length;

	} // End of while

} // End of Process_IPFIX
