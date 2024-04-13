static int readBTLF(struct READER *reader, struct BTREE *btree,
		    int number_of_records, union RECORD *records) {

	int i;

	uint8_t type, message_flags;
	uint32_t creation_order, hash_of_name;
	uint64_t heap_id;

	char buf[4];

	UNUSED(heap_id);
	UNUSED(hash_of_name);
	UNUSED(creation_order);
	UNUSED(message_flags);

	/* read signature */
	if (fread(buf, 1, 4, reader->fhd) != 4 || strncmp(buf, "BTLF", 4)) {
		log("cannot read signature of BTLF\n");
		return MYSOFA_INVALID_FORMAT;
	} log("%08lX %.4s\n", (uint64_t )ftell(reader->fhd) - 4, buf);

	if (fgetc(reader->fhd) != 0) {
		log("object BTLF must have version 0\n");
		return MYSOFA_INVALID_FORMAT;
	}

	type = (uint8_t)fgetc(reader->fhd);

	for (i = 0; i < number_of_records; i++) {

		switch (type) {
		case 5:
			records->type5.hash_of_name = (uint32_t)readValue(reader, 4);
			records->type5.heap_id = readValue(reader, 7);
			log(" type5 %08X %14lX\n", records->type5.hash_of_name,
			    records->type5.heap_id);
			records++;
			break;

		case 6:
			/*creation_order = */readValue(reader, 8);
			/*heap_id = */readValue(reader, 7);
			break;

		case 8:
			/*heap_id = */readValue(reader, 8);
			/*message_flags = */fgetc(reader->fhd);
			/*creation_order = */readValue(reader, 4);
			/*hash_of_name = */readValue(reader, 4);
			break;

		case 9:
			/*heap_id = */readValue(reader, 8);
			/*message_flags = */fgetc(reader->fhd);
			/*creation_order = */readValue(reader, 4);
			break;

		default:
			log("object BTLF has unknown type %d\n", type);
			return MYSOFA_INVALID_FORMAT;
		}
	}

/*	fseeko(reader->fhd, bthd->root_node_address + bthd->node_size, SEEK_SET); skip checksum */

	return MYSOFA_OK;
}
