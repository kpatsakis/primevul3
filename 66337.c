static GF_Err gf_isom_dump_svg_track(GF_ISOFile *the_file, u32 track, FILE *dump)
{
	char nhmlFileName[1024];
	FILE *nhmlFile;
	u32 i, count, di, ts, cur_frame;
	u64 start, end;
	GF_BitStream *bs;

	GF_TrackBox *trak = gf_isom_get_track_from_file(the_file, track);
	if (!trak) return GF_BAD_PARAM;
	switch (trak->Media->handler->handlerType) {
	case GF_ISOM_MEDIA_TEXT:
	case GF_ISOM_MEDIA_SUBT:
		break;
	default:
		return GF_BAD_PARAM;
	}

	strcpy(nhmlFileName, the_file->fileName);
	strcat(nhmlFileName, ".nhml");
	nhmlFile = gf_fopen(nhmlFileName, "wt");
	fprintf(nhmlFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(nhmlFile, "<NHNTStream streamType=\"3\" objectTypeIndication=\"10\" timeScale=\"%d\" baseMediaFile=\"file.svg\" inRootOD=\"yes\">\n", trak->Media->mediaHeader->timeScale);
	fprintf(nhmlFile, "<NHNTSample isRAP=\"yes\" DTS=\"0\" xmlFrom=\"doc.start\" xmlTo=\"text_1.start\"/>\n");

	ts = trak->Media->mediaHeader->timeScale;
	cur_frame = 0;
	end = 0;

	fprintf(dump, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(dump, "<svg version=\"1.2\" baseProfile=\"tiny\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"%d\" height=\"%d\" fill=\"black\">\n", trak->Header->width >> 16 , trak->Header->height >> 16);
	fprintf(dump, "<g transform=\"translate(%d, %d)\" text-anchor=\"middle\">\n", (trak->Header->width >> 16)/2 , (trak->Header->height >> 16)/2);

	count = gf_isom_get_sample_count(the_file, track);
	for (i=0; i<count; i++) {
		GF_TextSample *txt;
		GF_ISOSample *s = gf_isom_get_sample(the_file, track, i+1, &di);
		if (!s) continue;

		start = s->DTS;
		if (s->dataLength==2) {
			gf_isom_sample_del(&s);
			continue;
		}
		if (i+1<count) {
			GF_ISOSample *next = gf_isom_get_sample_info(the_file, track, i+2, NULL, NULL);
			if (next) {
				end = next->DTS;
				gf_isom_sample_del(&next);
			}
		}

		cur_frame++;
		bs = gf_bs_new(s->data, s->dataLength, GF_BITSTREAM_READ);
		txt = gf_isom_parse_texte_sample(bs);
		gf_bs_del(bs);

		if (!txt->len) continue;

		fprintf(dump, " <text id=\"text_%d\" display=\"none\">%s\n", cur_frame, txt->text);
		fprintf(dump, "  <set attributeName=\"display\" to=\"inline\" begin=\"%g\" end=\"%g\"/>\n", ((s64)start*1.0)/ts, ((s64)end*1.0)/ts);
		fprintf(dump, "  <discard begin=\"%g\"/>\n", ((s64)end*1.0)/ts);
		fprintf(dump, " </text>\n");
		gf_isom_sample_del(&s);
		gf_isom_delete_text_sample(txt);
		fprintf(dump, "\n");
		gf_set_progress("SRT Extract", i, count);

		if (i == count - 2) {
			fprintf(nhmlFile, "<NHNTSample isRAP=\"no\" DTS=\"%f\" xmlFrom=\"text_%d.start\" xmlTo=\"doc.end\"/>\n", ((s64)start*1.0), cur_frame);
		} else {
			fprintf(nhmlFile, "<NHNTSample isRAP=\"no\" DTS=\"%f\" xmlFrom=\"text_%d.start\" xmlTo=\"text_%d.start\"/>\n", ((s64)start*1.0), cur_frame, cur_frame+1);
		}

	}
	fprintf(dump, "</g>\n");
	fprintf(dump, "</svg>\n");

	fprintf(nhmlFile, "</NHNTStream>\n");
	gf_fclose(nhmlFile);

	if (count) gf_set_progress("SRT Extract", i, count);
	return GF_OK;
}
