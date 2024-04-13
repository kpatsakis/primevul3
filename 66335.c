GF_Err gf_isom_dump_ismacryp_protection(GF_ISOFile *the_file, u32 trackNumber, FILE * trace)
{
	u32 i, count;
	GF_SampleEntryBox *entry;
	GF_Err e;
	GF_TrackBox *trak;

	trak = gf_isom_get_track_from_file(the_file, trackNumber);
	if (!trak) return GF_BAD_PARAM;


	fprintf(trace, "<ISMACrypSampleDescriptions>\n");
	count = gf_isom_get_sample_description_count(the_file, trackNumber);
	for (i=0; i<count; i++) {
		e = Media_GetSampleDesc(trak->Media, i+1, (GF_SampleEntryBox **) &entry, NULL);
		if (e) return e;

		switch (entry->type) {
		case GF_ISOM_BOX_TYPE_ENCA:
		case GF_ISOM_BOX_TYPE_ENCV:
		case GF_ISOM_BOX_TYPE_ENCT:
		case GF_ISOM_BOX_TYPE_ENCS:
			break;
		default:
			continue;
		}
		gf_isom_box_dump(entry, trace);
	}
	fprintf(trace, "</ISMACrypSampleDescriptions>\n");
	return GF_OK;
}
