static input_translation_t *setup_translation_table (exporter_ipfix_domain_t *exporter, uint16_t id) {
input_translation_t *table;
extension_map_t 	*extension_map;
uint32_t			i, ipv6, offset, next_extension;
size_t				size_required;

	ipv6 = 0;

	table = GetTranslationTable(exporter, id);
	if ( !table ) {
		LogInfo("Process_ipfix: [%u] Add template %u", exporter->info.id, id);
		table = add_translation_table(exporter, id);
		if ( !table ) {
			return NULL;
		}
		size_required = Max_num_extensions * sizeof(uint16_t) + sizeof(extension_map_t);
		size_required = (size_required + 3) &~(size_t)3;
		extension_map = malloc(size_required);
		if ( !extension_map ) {
			LogError("Process_ipfix: Panic! malloc() error in %s line %d: %s", __FILE__, __LINE__, strerror (errno));
			return  NULL;
		}
		extension_map->type 	   = ExtensionMapType;
		extension_map->size 	   = sizeof(extension_map_t);
		extension_map->map_id 	   = INIT_ID;
		extension_map->extension_size = 0;

		table->extension_info.map 	 = extension_map;
		table->extension_map_changed = 1;
		table->number_of_sequences 	 = 0;
 	} else {
		extension_map = table->extension_info.map;

		extension_map->size 	   	  = sizeof(extension_map_t);
		extension_map->extension_size = 0;
		free((void *)table->sequence);

		dbg_printf("[%u] Refresh template %u\n", exporter->info.id, id);
	}
	table->sequence = calloc(cache.max_ipfix_elements, sizeof(sequence_map_t));
	if ( !table->sequence ) {
		LogError("Process_ipfix: Panic! malloc() error in %s line %d: %s", __FILE__, __LINE__, strerror (errno));
		return  NULL;
	}
	table->number_of_sequences = 0;
	table->max_number_of_sequences = cache.max_ipfix_elements;

	table->updated  	= time(NULL);
	table->flags			= 0;
	SetFlag(table->flags, FLAG_PKG_64);
	SetFlag(table->flags, FLAG_BYTES_64);
	table->delta_time		= 0;
	table->icmpTypeCodeIPv4	= 0;
	table->router_ip_offset = 0;
	table->received_offset  = 0;

	dbg_printf("[%u] Build sequence table %u\n", exporter->info.id, id);

	table->id 			= id;

	/* 
	 * common data block: The common record is expected in the output stream. If not available
	 * in the template, fill values with 0
	 */

	offset = BYTE_OFFSET_first;
	if ( cache.lookup_info[IPFIX_flowStartDeltaMicroseconds].found ) {
		PushSequence( table, IPFIX_flowStartDeltaMicroseconds, NULL, &table->flow_start);
		PushSequence( table, IPFIX_flowEndDeltaMicroseconds, NULL, &table->flow_end);
		offset = BYTE_OFFSET_first + 8;
		table->delta_time = 1;
		dbg_printf("Time stamp: flow start/end delta microseconds: %u/%u\n",
			IPFIX_flowStartDeltaMicroseconds, IPFIX_flowEndDeltaMicroseconds);
	} else if ( cache.lookup_info[IPFIX_flowStartMilliseconds].found ) {
		PushSequence( table, IPFIX_flowStartMilliseconds, NULL, &table->flow_start);
		PushSequence( table, IPFIX_flowEndMilliseconds, NULL, &table->flow_end);
		offset = BYTE_OFFSET_first + 8;
		dbg_printf("Time stamp: flow start/end absolute milliseconds: %u/%u\n", 
			IPFIX_flowStartMilliseconds, IPFIX_flowEndMilliseconds);
	} else if ( cache.lookup_info[IPFIX_flowStartSysUpTime].found ) {
		PushSequence( table, IPFIX_flowStartSysUpTime, NULL, &table->flow_start);
		PushSequence( table, IPFIX_flowEndSysUpTime, NULL, &table->flow_end);
		offset = BYTE_OFFSET_first + 8;
		dbg_printf("Time stamp: flow start/end relative milliseconds: %u/%u\n", 
			IPFIX_flowStartSysUpTime, IPFIX_flowEndSysUpTime);
	} else if ( cache.lookup_info[IPFIX_flowStartSeconds].found ) {
		PushSequence( table, IPFIX_flowStartSeconds, NULL, &table->flow_start);
		PushSequence( table, IPFIX_flowEndSeconds, NULL, &table->flow_end);
		offset = BYTE_OFFSET_first + 8;
		dbg_printf("Time stamp: flow start/end absolute seconds: %u/%u\n", 
			IPFIX_flowStartSeconds, IPFIX_flowEndSeconds);
	}else {
		dbg_printf("Time stamp: No known format found\n");
		offset = BYTE_OFFSET_first + 8;
	}
	PushSequence( table, IPFIX_forwardingStatus, &offset, NULL);
	PushSequence( table, IPFIX_tcpControlBits, &offset, NULL);
	PushSequence( table, IPFIX_protocolIdentifier, &offset, NULL);
	PushSequence( table, IPFIX_ipClassOfService, &offset, NULL);

	PushSequence( table, IPFIX_SourceTransportPort, &offset, NULL);
	PushSequence( table, IPFIX_DestinationTransportPort, &offset, NULL);

	offset += 4;

	/* IP address record
	 * This record is expected in the output stream. If not available
	 * in the template, assume empty v4 address.
	 */
	if ( cache.lookup_info[IPFIX_SourceIPv4Address].found ) {
		PushSequence( table, IPFIX_SourceIPv4Address, &offset, NULL);
		PushSequence( table, IPFIX_DestinationIPv4Address, &offset, NULL);
	} else if ( cache.lookup_info[IPFIX_SourceIPv6Address].found ) {
		PushSequence( table, IPFIX_SourceIPv6Address, &offset, NULL);
		PushSequence( table, IPFIX_DestinationIPv6Address, &offset, NULL);
		SetFlag(table->flags, FLAG_IPV6_ADDR);
		ipv6 = 1;
	} else {
		PushSequence( table, IPFIX_SourceIPv4Address, &offset, NULL);
		PushSequence( table, IPFIX_DestinationIPv4Address, &offset, NULL);
	}

	if ( cache.lookup_info[IPFIX_packetTotalCount].found )
		PushSequence( table, IPFIX_packetTotalCount, &offset, &table->packets);
	else
		PushSequence( table, IPFIX_packetDeltaCount, &offset, &table->packets);
	SetFlag(table->flags, FLAG_PKG_64);

	if ( cache.lookup_info[IPFIX_octetTotalCount].found )
		PushSequence( table, IPFIX_octetTotalCount, &offset, &table->bytes);
	else
		PushSequence( table, IPFIX_octetDeltaCount, &offset, &table->bytes);
	SetFlag(table->flags, FLAG_BYTES_64);


	next_extension = 0;
	for (i=4; extension_descriptor[i].id; i++ ) {
		uint32_t map_index = i;

		if ( cache.common_extensions[i] == 0 )
			continue;

		switch(i) {
			case EX_IO_SNMP_2:
				PushSequence( table, IPFIX_ingressInterface, &offset, NULL);
				PushSequence( table, IPFIX_egressInterface, &offset, NULL);
				break;
			case EX_IO_SNMP_4:
				PushSequence( table, IPFIX_ingressInterface, &offset, NULL);
				PushSequence( table, IPFIX_egressInterface, &offset, NULL);
				break;
			case EX_AS_2:
				PushSequence( table, IPFIX_bgpSourceAsNumber, &offset, NULL);
				PushSequence( table, IPFIX_bgpDestinationAsNumber, &offset, NULL);
				break;
			case EX_AS_4:
				PushSequence( table, IPFIX_bgpSourceAsNumber, &offset, NULL);
				PushSequence( table, IPFIX_bgpDestinationAsNumber, &offset, NULL);
				break;
			case EX_MULIPLE:
				PushSequence( table, IPFIX_postIpClassOfService, &offset, NULL);
				PushSequence( table, IPFIX_flowDirection, &offset, NULL);
				if ( ipv6 ) {
					PushSequence( table, IPFIX_SourceIPv6PrefixLength, &offset, NULL);
					PushSequence( table, IPFIX_DestinationIPv6PrefixLength, &offset, NULL);
				} else {
					PushSequence( table, IPFIX_SourceIPv4PrefixLength, &offset, NULL);
					PushSequence( table, IPFIX_DestinationIPv4PrefixLength, &offset, NULL);
				}
				break;
			case EX_NEXT_HOP_v4:
				PushSequence( table, IPFIX_ipNextHopIPv4Address, &offset, NULL);
				break;
			case EX_NEXT_HOP_v6:
				PushSequence( table, IPFIX_ipNextHopIPv6Address, &offset, NULL);
				SetFlag(table->flags, FLAG_IPV6_NH);
				break;
			case EX_NEXT_HOP_BGP_v4:
				PushSequence( table, IPFIX_bgpNextHopIPv4Address, &offset, NULL);
				break;
			case EX_NEXT_HOP_BGP_v6:
				PushSequence( table, IPFIX_bgpNextHopIPv6Address, &offset, NULL);
				SetFlag(table->flags, FLAG_IPV6_NHB);
				break;
			case EX_VLAN:
				PushSequence( table, IPFIX_vlanId, &offset, NULL);
				PushSequence( table, IPFIX_postVlanId, &offset, NULL);
				break;
			case EX_OUT_PKG_4:
				PushSequence( table, IPFIX_postPacketDeltaCount, &offset, NULL);
				break;
			case EX_OUT_PKG_8:
				PushSequence( table, IPFIX_postPacketDeltaCount, &offset, NULL);
				break;
			case EX_OUT_BYTES_4:
				PushSequence( table, IPFIX_postOctetDeltaCount, &offset, NULL);
				break;
			case EX_OUT_BYTES_8:
				PushSequence( table, IPFIX_postOctetDeltaCount, &offset, NULL);
				break;
			case EX_AGGR_FLOWS_8:
				break;
			case EX_MAC_1:
				PushSequence( table, IPFIX_SourceMacAddress, &offset, NULL);
				PushSequence( table, IPFIX_postDestinationMacAddress, &offset, NULL);
				break;
			case EX_MAC_2:
				PushSequence( table, IPFIX_DestinationMacAddress, &offset, NULL);
				PushSequence( table, IPFIX_postSourceMacAddress, &offset, NULL);
				break;
			case EX_MPLS:
				PushSequence( table, IPFIX_mplsTopLabelStackSection, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection2, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection3, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection4, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection5, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection6, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection7, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection8, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection9, &offset, NULL);
				PushSequence( table, IPFIX_mplsLabelStackSection10, &offset, NULL);
				break;
			case EX_ROUTER_IP_v4:
			case EX_ROUTER_IP_v6:
				if ( exporter->info.sa_family == PF_INET6 ) {
					table->router_ip_offset = offset;
					dbg_printf("Router IPv6: offset: %u, olen: %u\n", offset, 16 );
					offset			 	   += 16;
					SetFlag(table->flags, FLAG_IPV6_EXP);
					map_index = EX_ROUTER_IP_v6;
				} else {
					table->router_ip_offset = offset;
					dbg_printf("Router IPv4: offset: %u, olen: %u\n", offset, 4 );
					offset				   += 4;
					ClearFlag(table->flags, FLAG_IPV6_EXP);
					map_index = EX_ROUTER_IP_v4;
				}
				break;
			case EX_ROUTER_ID:
				break;
			case EX_RECEIVED:
				table->received_offset = offset;
				dbg_printf("Received offset: %u\n", offset);
				offset				   += 8;
				break;

		}
		extension_map->size += sizeof(uint16_t);
		extension_map->extension_size += extension_descriptor[map_index].size;


		if ( extension_map->ex_id[next_extension] != map_index ) {
			extension_map->ex_id[next_extension] = map_index;
			table->extension_map_changed = 1;

		}
		next_extension++;

	}
	extension_map->ex_id[next_extension++] = 0;

	if ( extension_map->size & 0x3 ) {
		extension_map->ex_id[next_extension] = 0;
		extension_map->size = ( extension_map->size + 3 ) &~ 0x3;
	}
 
	table->output_record_size = offset;

	if ( cache.lookup_info[IPFIX_icmpTypeCodeIPv4].found && cache.lookup_info[IPFIX_icmpTypeCodeIPv4].length == 2 ) {
		PushSequence( table, IPFIX_icmpTypeCodeIPv4, NULL, &table->icmpTypeCodeIPv4);
	}

#ifdef DEVEL
	if ( table->extension_map_changed ) {
		printf("Extension Map id=%u changed!\n", extension_map->map_id);
	} else {
		printf("[%u] template %u unchanged\n", exporter->info.id, id);
	}

	printf("Process_ipfix: Check extension map: id: %d, size: %u, extension_size: %u\n", 
		extension_map->map_id, extension_map->size, extension_map->extension_size);
	{ int i;
	for (i=0; i<table->number_of_sequences; i++ ) {
		printf("Sequence %i: id: %u, Type: %u, Length: %u, Output offset: %u, stack: %llu\n",
			i, table->sequence[i].id, table->sequence[i].type, table->sequence[i].input_length, 
			table->sequence[i].output_offset, (unsigned long long)table->sequence[i].stack);
	}
	printf("Flags: 0x%x output record size: %u\n", table->flags, table->output_record_size); 
	}
	PrintExtensionMap(extension_map);
#endif

	return table;

} // End of setup_translation_table
