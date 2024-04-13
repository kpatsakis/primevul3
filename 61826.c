int jas_iccprof_sethdr(jas_iccprof_t *prof, jas_icchdr_t *hdr)
{
	prof->hdr = *hdr;
	return 0;
}
