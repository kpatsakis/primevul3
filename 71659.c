int btreeRead(struct READER *reader, struct BTREE *btree) {
	char buf[4];

	/* read signature */
	if (fread(buf, 1, 4, reader->fhd) != 4 || strncmp(buf, "BTHD", 4)) {
		log("cannot read signature of BTHD\n");
		return MYSOFA_INVALID_FORMAT;
	} log("%08lX %.4s\n", (uint64_t )ftell(reader->fhd) - 4, buf);

	if (fgetc(reader->fhd) != 0) {
		log("object BTHD must have version 0\n");
		return MYSOFA_INVALID_FORMAT;
	}

	btree->type = (uint8_t)fgetc(reader->fhd);
	btree->node_size = (uint32_t)readValue(reader, 4);
	btree->record_size = (uint16_t)readValue(reader, 2);
	btree->depth = (uint16_t)readValue(reader, 2);

	btree->split_percent = (uint8_t)fgetc(reader->fhd);
	btree->merge_percent = (uint8_t)fgetc(reader->fhd);
	btree->root_node_address = (uint64_t)readValue(reader,
					     reader->superblock.size_of_offsets);
	btree->number_of_records = (uint16_t)readValue(reader, 2);
	if(btree->number_of_records>0x1000)
			return MYSOFA_UNSUPPORTED_FORMAT;
	btree->total_number = (uint64_t)readValue(reader, reader->superblock.size_of_lengths);

	/*	fseek(reader->fhd, 4, SEEK_CUR);  skip checksum */

	if(btree->total_number > 0x10000000)
		return MYSOFA_NO_MEMORY;
	btree->records = malloc(sizeof(btree->records[0]) * btree->total_number);
	if (!btree->records)
		return MYSOFA_NO_MEMORY;
	memset(btree->records, 0, sizeof(btree->records[0]) * btree->total_number);

	/* read records */
	if(fseek(reader->fhd, btree->root_node_address, SEEK_SET)<0)
		return errno;
	return readBTLF(reader, btree, btree->number_of_records, btree->records);
}
