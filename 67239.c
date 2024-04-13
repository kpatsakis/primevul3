static int bin_pe_init_metadata_hdr(struct PE_(r_bin_pe_obj_t)* bin) {
	PE_DWord metadata_directory = bin->clr_hdr? bin_pe_rva_to_paddr (bin, bin->clr_hdr->MetaDataDirectoryAddress): 0;
	PE_(image_metadata_header) * metadata = R_NEW0 (PE_(image_metadata_header));
	int rr;
	if (!metadata) {
		return 0;
	}
	if (!metadata_directory) {
		free (metadata);
		return 0;
	}


	rr = r_buf_fread_at (bin->b, metadata_directory,
		(ut8*) metadata, bin->big_endian? "1I2S": "1i2s", 1);
	if (rr < 1) {
		goto fail;
	}

	rr = r_buf_fread_at (bin->b, metadata_directory + 8,
		(ut8*) (&metadata->Reserved), bin->big_endian? "1I": "1i", 1);
	if (rr < 1) {
		goto fail;
	}

	rr = r_buf_fread_at (bin->b, metadata_directory + 12,
		(ut8*) (&metadata->VersionStringLength), bin->big_endian? "1I": "1i", 1);
	if (rr < 1) {
		goto fail;
	}

	eprintf ("Metadata Signature: 0x%"PFMT64x" 0x%"PFMT64x" %d\n",
		(ut64)metadata_directory, (ut64)metadata->Signature, (int)metadata->VersionStringLength);

	int len = metadata->VersionStringLength; // XXX: dont trust this length
	if (len > 0) {
		metadata->VersionString = calloc (1, len + 1);
		if (!metadata->VersionString) {
			goto fail;
		}

		rr = r_buf_read_at (bin->b, metadata_directory + 16, (ut8*)(metadata->VersionString),  len);
		if (rr != len) {
			eprintf ("Warning: read (metadata header) - cannot parse version string\n");
			free (metadata->VersionString);
			free (metadata);
			return 0;
		}
		eprintf (".NET Version: %s\n", metadata->VersionString);
	}

	rr = r_buf_fread_at (bin->b, metadata_directory + 16 + metadata->VersionStringLength,
		(ut8*) (&metadata->Flags), bin->big_endian? "2S": "2s", 1);

	if (rr < 1) {
		goto fail;
	}

	eprintf ("Number of Metadata Streams: %d\n", metadata->NumberOfStreams);
	bin->metadata_header = metadata;


	int start_of_stream = metadata_directory + 20 + metadata->VersionStringLength;
	PE_(image_metadata_stream) * stream;
	PE_(image_metadata_stream) **streams = calloc (sizeof (PE_(image_metadata_stream)*), metadata->NumberOfStreams);
	if (!streams) {
		goto fail;
	}
	int count = 0;

	while (count < metadata->NumberOfStreams) {
		stream = R_NEW0 (PE_(image_metadata_stream));
		if (!stream) {
			free (streams);
			goto fail;
		}

		if (r_buf_fread_at (bin->b, start_of_stream, (ut8*) stream, bin->big_endian? "2I": "2i", 1) < 1) {
			free (stream);
			free (streams);
			goto fail;
		}
		eprintf ("DirectoryAddress: %x Size: %x\n", stream->Offset, stream->Size);
		char* stream_name = calloc (1, MAX_METADATA_STRING_LENGTH + 1);

		if (!stream_name) {
			free (stream);
			free (streams);
			goto fail;
		}

		if (r_buf_size (bin->b) < (start_of_stream + 8 + MAX_METADATA_STRING_LENGTH)) {
			free (stream_name);
			free (stream);
			free (streams);
			goto fail;
		}
		int c = bin_pe_read_metadata_string (stream_name,
			(char *)(bin->b->buf + start_of_stream + 8));
		if (c == 0) {
			free (stream_name);
			free (stream);
			free (streams);
			goto fail;
		}
		eprintf ("Stream name: %s %d\n", stream_name, c);
		stream->Name = stream_name;
		streams[count] = stream;
		start_of_stream += 8 + c;
		count += 1;
	}
	bin->streams = streams;
	return 1;
fail:
	eprintf ("Warning: read (metadata header)\n");
	free (metadata);
	return 0;
}
