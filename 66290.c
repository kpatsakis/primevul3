GF_Err bloc_dump(GF_Box *a, FILE * trace)
{
	GF_BaseLocationBox *p = (GF_BaseLocationBox *) a;

	gf_isom_box_dump_start(a, "BaseLocationBox", trace);

	fprintf(trace, "baseLocation=\"%s\" basePurlLocation=\"%s\">\n", p->baseLocation, p->basePurlLocation);
	gf_isom_box_dump_done("BaseLocationBox", a, trace);
	return GF_OK;
}
