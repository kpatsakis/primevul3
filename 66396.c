GF_Err nump_dump(GF_Box *a, FILE * trace)
{
	GF_NUMPBox *p;

	p = (GF_NUMPBox *)a;
	gf_isom_box_dump_start(a, "LargeTotalPacketBox", trace);
	fprintf(trace, "PacketsSent=\""LLD"\">\n", LLD_CAST p->nbPackets);
	gf_isom_box_dump_done("LargeTotalPacketBox", a, trace);
	return GF_OK;
}
