GF_Err ilst_item_dump(GF_Box *a, FILE * trace)
{
	GF_BitStream *bs;
	u32 val;
	Bool no_dump = GF_FALSE;
	char *name = "UnknownBox";
	GF_ListItemBox *itune = (GF_ListItemBox *)a;
	switch (itune->type) {
	case GF_ISOM_BOX_TYPE_0xA9NAM:
		name = "NameBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9CMT:
		name = "CommentBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9DAY:
		name = "CreatedBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9ART:
		name = "ArtistBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9TRK:
		name = "TrackBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9ALB:
		name = "AlbumBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9COM:
		name = "CompositorBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9WRT:
		name = "WriterBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9TOO:
		name = "ToolBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9CPY:
		name = "CopyrightBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9DES:
		name = "DescriptionBox";
		break;
	case GF_ISOM_BOX_TYPE_0xA9GEN:
	case GF_ISOM_BOX_TYPE_GNRE:
		name = "GenreBox";
		break;
	case GF_ISOM_BOX_TYPE_aART:
		name = "AlbumArtistBox";
		break;
	case GF_ISOM_BOX_TYPE_PGAP:
		name = "GapelessBox";
		break;
	case GF_ISOM_BOX_TYPE_DISK:
		name = "DiskBox";
		break;
	case GF_ISOM_BOX_TYPE_TRKN:
		name = "TrackNumberBox";
		break;
	case GF_ISOM_BOX_TYPE_TMPO:
		name = "TempoBox";
		break;
	case GF_ISOM_BOX_TYPE_CPIL:
		name = "CompilationBox";
		break;
	case GF_ISOM_BOX_TYPE_COVR:
		name = "CoverArtBox";
		no_dump = GF_TRUE;
		break;
	case GF_ISOM_BOX_TYPE_iTunesSpecificInfo:
		name = "iTunesSpecificBox";
		no_dump = GF_TRUE;
		break;
	case GF_ISOM_BOX_TYPE_0xA9GRP:
		name = "GroupBox";
		break;
	case GF_ISOM_ITUNE_ENCODER:
		name = "EncoderBox";
		break;
	}
	gf_isom_box_dump_start(a, name, trace);

	if (!no_dump) {
		switch (itune->type) {
		case GF_ISOM_BOX_TYPE_DISK:
		case GF_ISOM_BOX_TYPE_TRKN:
			bs = gf_bs_new(itune->data->data, itune->data->dataSize, GF_BITSTREAM_READ);
			gf_bs_read_int(bs, 16);
			val = gf_bs_read_int(bs, 16);
			if (itune->type==GF_ISOM_BOX_TYPE_DISK) {
				fprintf(trace, " DiskNumber=\"%d\" NbDisks=\"%d\" ", val, gf_bs_read_int(bs, 16) );
			} else {
				fprintf(trace, " TrackNumber=\"%d\" NbTracks=\"%d\" ", val, gf_bs_read_int(bs, 16) );
			}
			gf_bs_del(bs);
			break;
		case GF_ISOM_BOX_TYPE_TMPO:
			bs = gf_bs_new(itune->data->data, itune->data->dataSize, GF_BITSTREAM_READ);
			fprintf(trace, " BPM=\"%d\" ", gf_bs_read_int(bs, 16) );
			gf_bs_del(bs);
			break;
		case GF_ISOM_BOX_TYPE_CPIL:
			fprintf(trace, " IsCompilation=\"%s\" ", (itune->data && itune->data->data && itune->data->data[0]) ? "yes" : "no");
			break;
		case GF_ISOM_BOX_TYPE_PGAP:
			fprintf(trace, " IsGapeless=\"%s\" ", (itune->data && itune->data->data && itune->data->data[0]) ? "yes" : "no");
			break;
		default:
			if (strcmp(name, "UnknownBox") && itune->data && itune->data->data) {
				fprintf(trace, " value=\"");
				if (itune->data && itune->data->data[0]) {
					dump_data_string(trace, itune->data->data, itune->data->dataSize);
				} else {
					dump_data(trace, itune->data->data, itune->data->dataSize);
				}
				fprintf(trace, "\" ");
			}
			break;
		}
	}
	fprintf(trace, ">\n");
	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
