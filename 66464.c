GF_Err trgt_dump(GF_Box *a, FILE * trace)
{
	GF_TrackGroupTypeBox *ptr = (GF_TrackGroupTypeBox *) a;
	a->type = ptr->group_type;
	gf_isom_box_dump_start(a, "TrackGroupTypeBox", trace);
	a->type = GF_ISOM_BOX_TYPE_TRGT;
	fprintf(trace, "track_group_id=\"%d\">\n", ptr->track_group_id);
	gf_isom_box_dump_done("TrackGroupTypeBox", a, trace);
	return GF_OK;
}
