GF_Err snro_dump(GF_Box *a, FILE * trace)
{
	GF_SeqOffHintEntryBox *p;
	p = (GF_SeqOffHintEntryBox *)a;
	gf_isom_box_dump_start(a, "PacketSequenceOffsetBox", trace);
	fprintf(trace, "SeqNumOffset=\"%d\">\n", p->SeqOffset);
	gf_isom_box_dump_done("PacketSequenceOffsetBox", a, trace);
	return GF_OK;
}
