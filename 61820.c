int jas_iccprof_gethdr(jas_iccprof_t *prof, jas_icchdr_t *hdr)
{
	*hdr = prof->hdr;
	return 0;
}
