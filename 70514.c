static void Process_ipfix_data(exporter_ipfix_domain_t *exporter, uint32_t ExportTime, void *data_flowset, FlowSource_t *fs, input_translation_t *table ){
uint64_t			sampling_rate;
uint32_t			size_left;
uint8_t				*in, *out;
int					i;
char				*string;

	size_left = GET_FLOWSET_LENGTH(data_flowset) - 4; // -4 for data flowset header -> id and length

	in  	  = (uint8_t *)(data_flowset + 4);	// skip flowset header

	dbg_printf("[%u] Process data flowset size: %u\n", exporter->info.id, size_left);


	sampling_rate = 1;

	generic_sampler_t *sampler = exporter->sampler;
	while ( sampler && sampler->info.id != -1 ) 
		sampler = sampler->next;

	if ( sampler ) {
		sampling_rate = sampler->info.interval;
		dbg_printf("[%u] Std sampling available for this flow source: Rate: %llu\n", exporter->info.id, (long long unsigned)sampling_rate);
	} else {
		sampling_rate = default_sampling;
		dbg_printf("[%u] No Sampling record found\n", exporter->info.id);
	}

	if ( overwrite_sampling > 0 )  {
		sampling_rate = overwrite_sampling;
		dbg_printf("[%u] Hard overwrite sampling rate: %llu\n", exporter->info.id, (long long unsigned)sampling_rate);
	} 

	if ( sampling_rate != 1 )
		SetFlag(table->flags, FLAG_SAMPLED);

	while (size_left) {
		int input_offset;
		common_record_t		*data_record;

		if ( size_left < 4 ) {	// rounding pads
			size_left = 0;
			continue;
		}

		if ( !CheckBufferSpace(fs->nffile, table->output_record_size) ) {
			LogError("Process_ipfix: output buffer size error. Abort ipfix record processing");
			dbg_printf("Process_ipfix: output buffer size error. Abort ipfix record processing");
			return;
		}
		processed_records++;
		exporter->PacketSequence++;

		data_record	= (common_record_t *)fs->nffile->buff_ptr;
		out 	  = (uint8_t *)data_record;

		dbg_printf("[%u] Process data record: %u addr: %llu, buffer size_left: %u\n", 
			exporter->info.id, processed_records, (long long unsigned)((ptrdiff_t)in - (ptrdiff_t)data_flowset), 
			size_left);

		data_record->flags 		    = table->flags;
		data_record->size  		    = table->output_record_size;
		data_record->type  		    = CommonRecordType;
	  	data_record->ext_map	    = table->extension_info.map->map_id;
		data_record->exporter_sysid = exporter->info.sysid;
		data_record->reserved 		= 0;

		table->flow_start 		    = 0;
		table->flow_end 		    = 0;
		table->packets 		  	    = 0;
		table->bytes 		  	    = 0;
		table->out_packets 	  	    = 0;
		table->out_bytes 	  	    = 0;

		input_offset = 0;
		for ( i=0; i<table->number_of_sequences; i++ ) {
			int output_offset = table->sequence[i].output_offset;
			void *stack = table->sequence[i].stack;

			if ( input_offset > size_left ) {
				LogError("Process ipfix: buffer overrun!! input_offset: %i > size left data buffer: %u\n", input_offset, size_left);
				return;
			} 

			switch (table->sequence[i].id) {
				case nop:
					break;
				case dyn_skip: {
					uint16_t skip = in[input_offset];
					if ( skip < 255 ) {
						input_offset += (skip+1);
					} else {
						skip = Get_val16((void *)&in[input_offset+1]);
						input_offset += (skip+3);
					}
					} break;
				case move8:
					out[output_offset] = in[input_offset];
					break;
				case move16:
					*((uint16_t *)&out[output_offset]) = Get_val16((void *)&in[input_offset]);
					break;
				case move32:
					*((uint32_t *)&out[output_offset]) = Get_val32((void *)&in[input_offset]);
					break;
				case move40:
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;

						t.val.val64 = Get_val40((void *)&in[input_offset]);
						*((uint32_t *)&out[output_offset]) 	 = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					}
					break;
				case move48:
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;
						t.val.val64 = Get_val48((void *)&in[input_offset]);
						*((uint32_t *)&out[output_offset]) 	 = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					}
					break;
				case move56:
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;

						t.val.val64 = Get_val56((void *)&in[input_offset]);
						*((uint32_t *)&out[output_offset]) 	 = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					}
					break;
				case move64: 
					{ type_mask_t t;
						t.val.val64 = Get_val64((void *)&in[input_offset]);

						*((uint32_t *)&out[output_offset]) 	 = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					} break;
				case move128: 
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;
					  
						t.val.val64 = Get_val64((void *)&in[input_offset]);
						*((uint32_t *)&out[output_offset]) 	  = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4])  = t.val.val32[1];

						t.val.val64 = Get_val64((void *)&in[input_offset+8]);
						*((uint32_t *)&out[output_offset+8])  = t.val.val32[0];
						*((uint32_t *)&out[output_offset+12]) = t.val.val32[1];
					} break;
				case move32_sampling:
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;
						t.val.val64 = Get_val32((void *)&in[input_offset]);
						t.val.val64 *= sampling_rate;
						*((uint32_t *)&out[output_offset]) 	 = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					  	*(uint64_t *)stack = t.val.val64;
					} break;
				case move64_sampling:
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;
						t.val.val64 = Get_val64((void *)&in[input_offset]);

						t.val.val64 *= sampling_rate;
						*((uint32_t *)&out[output_offset]) 	 = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					  	*(uint64_t *)stack = t.val.val64;
					} break;
				case Time64Mili:
					{ uint64_t DateMiliseconds = Get_val64((void *)&in[input_offset]);
					  *(uint64_t *)stack = DateMiliseconds;

					} break;
				case TimeUnix:
					{ uint64_t DateSeconds = Get_val32((void *)&in[input_offset]);
					  *(uint64_t *)stack = DateSeconds *1000LL;

					} break;
				case TimeDeltaMicro:
					{ uint64_t DeltaMicroSec = Get_val32((void *)&in[input_offset]);
					  *(uint64_t *)stack = ((1000000LL * (uint64_t)ExportTime) - DeltaMicroSec) / 1000LL;

					} break;
				case saveICMP:
					*(uint32_t *)stack = Get_val16((void *)&in[input_offset]);
					break;
				case move_mpls:
					*((uint32_t *)&out[output_offset]) = Get_val24((void *)&in[input_offset]);
					break;
				case move_flags: {
					uint16_t flags = Get_val16((void *)&in[input_offset]);
					out[output_offset] = flags & 0xFF;
					} break;
				case move_mac:
					/* 64bit access to potentially unaligned output buffer. use 2 x 32bit for _LP64 CPUs */
					{ type_mask_t t;

						t.val.val64 = Get_val48((void *)&in[input_offset]);
						*((uint32_t *)&out[output_offset])   = t.val.val32[0];
						*((uint32_t *)&out[output_offset+4]) = t.val.val32[1];
					}
					break;
				case zero8:
					out[output_offset] = 0;
					break;
				case zero16:
					*((uint16_t *)&out[output_offset]) = 0;
					break;
				case zero32:
					*((uint32_t *)&out[output_offset]) = 0;
					break;
				case zero64: 
						*((uint64_t *)&out[output_offset]) = 0;
					 break;
				case zero128: 
						*((uint64_t *)&out[output_offset]) = 0;
						*((uint64_t *)&out[output_offset+8]) = 0;
					break;
				
				default:
					LogError("Process_ipfix: Software bug! Unknown Sequence: %u. at %s line %d", 
						table->sequence[i].id, __FILE__, __LINE__);
					dbg_printf("Software bug! Unknown Sequence: %u. at %s line %d\n", 
						table->sequence[i].id, __FILE__, __LINE__);
			}
			input_offset += (table->sequence[i].input_length + table->sequence[i].skip_count);
		}

		if ( data_record->prot == IPPROTO_ICMP || data_record->prot == IPPROTO_ICMPV6 ) {
			if ( table->icmpTypeCodeIPv4 ) {
				data_record->srcport = 0;
				data_record->dstport = table->icmpTypeCodeIPv4;
			}
		}

		if ( table->received_offset ) {
			type_mask_t t;
			t.val.val64 = (uint64_t)((uint64_t)fs->received.tv_sec * 1000LL) + (uint64_t)((uint64_t)fs->received.tv_usec / 1000LL);
				*((uint32_t *)&out[table->received_offset])   = t.val.val32[0];
				*((uint32_t *)&out[table->received_offset+4]) = t.val.val32[1];
		}

		data_record->first 		= table->flow_start / 1000;
		data_record->msec_first = table->flow_start % 1000;

		data_record->last 		= table->flow_end / 1000;
		data_record->msec_last	= table->flow_end % 1000;

		if ( table->flow_start < fs->first_seen )
			fs->first_seen = table->flow_start;
		if ( table->flow_end > fs->last_seen )
			fs->last_seen = table->flow_end;

		if ( table->router_ip_offset ) {
			int output_offset = table->router_ip_offset;
			if ( exporter->info.sa_family == PF_INET6 ) {
				type_mask_t t;
					  
				t.val.val64 = exporter->info.ip.V6[0];
				*((uint32_t *)&out[output_offset]) 	  = t.val.val32[0];
				*((uint32_t *)&out[output_offset+4])  = t.val.val32[1];

				t.val.val64 = exporter->info.ip.V6[1];
				*((uint32_t *)&out[output_offset+8])  = t.val.val32[0];
				*((uint32_t *)&out[output_offset+12]) = t.val.val32[1];
			} else {
				*((uint32_t *)&out[output_offset]) = exporter->info.ip.V4;
			}
		}

		switch (data_record->prot ) { // switch protocol of
			case IPPROTO_ICMP:
				fs->nffile->stat_record->numflows_icmp++;
				fs->nffile->stat_record->numpackets_icmp  += table->packets;
				fs->nffile->stat_record->numbytes_icmp    += table->bytes;
				fs->nffile->stat_record->numpackets_icmp  += table->out_packets;
				fs->nffile->stat_record->numbytes_icmp    += table->out_bytes;
				break;
			case IPPROTO_TCP:
				fs->nffile->stat_record->numflows_tcp++;
				fs->nffile->stat_record->numpackets_tcp   += table->packets;
				fs->nffile->stat_record->numbytes_tcp     += table->bytes;
				fs->nffile->stat_record->numpackets_tcp   += table->out_packets;
				fs->nffile->stat_record->numbytes_tcp     += table->out_bytes;
				break;
			case IPPROTO_UDP:
				fs->nffile->stat_record->numflows_udp++;
				fs->nffile->stat_record->numpackets_udp   += table->packets;
				fs->nffile->stat_record->numbytes_udp     += table->bytes;
				fs->nffile->stat_record->numpackets_udp   += table->out_packets;
				fs->nffile->stat_record->numbytes_udp     += table->out_bytes;
				break;
			default:
				fs->nffile->stat_record->numflows_other++;
				fs->nffile->stat_record->numpackets_other += table->packets;
				fs->nffile->stat_record->numbytes_other   += table->bytes;
				fs->nffile->stat_record->numpackets_other += table->out_packets;
				fs->nffile->stat_record->numbytes_other   += table->out_bytes;
		}
		exporter->flows++;
		fs->nffile->stat_record->numflows++;
		fs->nffile->stat_record->numpackets	+= table->packets;
		fs->nffile->stat_record->numbytes	+= table->bytes;
		fs->nffile->stat_record->numpackets	+= table->out_packets;
		fs->nffile->stat_record->numbytes	+= table->out_bytes;
	
		if ( verbose ) {
			master_record_t master_record;
			memset((void *)&master_record, 0, sizeof(master_record_t));
			ExpandRecord_v2((common_record_t *)data_record, &(table->extension_info), &(exporter->info), &master_record);
		 	format_file_block_record(&master_record, &string, 0);
			printf("%s\n", string);
		}

		fs->nffile->block_header->size  += data_record->size;
		fs->nffile->block_header->NumRecords++;
		fs->nffile->buff_ptr	= (common_record_t *)((pointer_addr_t)data_record + data_record->size);

		dbg_printf("Adjust input stream offset: %u\n", input_offset);
		if ( input_offset > size_left ) {
			LogError("Process ipfix: buffer overrun!! input_offset: %i > size left data buffer: %u\n", input_offset, size_left);
			return;
		} 
		size_left 		   -= input_offset;
		in  	  		   += input_offset;

		if ( fs->nffile->block_header->size  > BUFFSIZE ) {
			LogError("### Software error ###: %s line %d", __FILE__, __LINE__);
			LogError("Process ipfix: Output buffer overflow! Flush buffer and skip records.");
			LogError("Buffer size: %u > %u", fs->nffile->block_header->size, BUFFSIZE);

			fs->nffile->block_header->size 		= 0;
			fs->nffile->block_header->NumRecords = 0;
			fs->nffile->buff_ptr = (void *)((pointer_addr_t)fs->nffile->block_header + sizeof(data_block_header_t) );
			return;
		}

	}

} // End of Process_ipfix_data
