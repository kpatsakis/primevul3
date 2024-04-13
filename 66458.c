GF_Err traf_dump(GF_Box *a, FILE * trace)
{
	GF_TrackFragmentBox *p;
	p = (GF_TrackFragmentBox *)a;
	gf_isom_box_dump_start(a, "TrackFragmentBox", trace);
	fprintf(trace, ">\n");
	if (p->tfhd) gf_isom_box_dump(p->tfhd, trace);
	if (p->sdtp) gf_isom_box_dump(p->sdtp, trace);
	if (p->tfdt) gf_isom_box_dump(p->tfdt, trace);
	if (p->sub_samples) gf_isom_box_array_dump(p->sub_samples, trace);
	if (p->sampleGroupsDescription) gf_isom_box_array_dump(p->sampleGroupsDescription, trace);
	if (p->sampleGroups) gf_isom_box_array_dump(p->sampleGroups, trace);
	gf_isom_box_array_dump(p->TrackRuns, trace);
	if (p->sai_sizes) gf_isom_box_array_dump(p->sai_sizes, trace);
	if (p->sai_offsets) gf_isom_box_array_dump(p->sai_offsets, trace);
	if (p->sample_encryption) gf_isom_box_dump(p->sample_encryption, trace);
	gf_isom_box_dump_done("TrackFragmentBox", a, trace);
	return GF_OK;
}
