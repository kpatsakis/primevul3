GF_Err npck_dump(GF_Box *a, FILE * trace)
{
	GF_NPCKBox *p;
	p = (GF_NPCKBox *)a;
	gf_isom_box_dump_start(a, "TotalPacketBox", trace);
	fprintf(trace, "packetsSent=\"%d\">\n", p->nbPackets);
	gf_isom_box_dump_done("TotalPacketBox", a, trace);
	return GF_OK;
}
