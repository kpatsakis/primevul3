static int jas_iccprof_readhdr(jas_stream_t *in, jas_icchdr_t *hdr)
{
	if (jas_iccgetuint32(in, &hdr->size) ||
	  jas_iccgetuint32(in, &hdr->cmmtype) ||
	  jas_iccgetuint32(in, &hdr->version) ||
	  jas_iccgetuint32(in, &hdr->clas) ||
	  jas_iccgetuint32(in, &hdr->colorspc) ||
	  jas_iccgetuint32(in, &hdr->refcolorspc) ||
	  jas_iccgettime(in, &hdr->ctime) ||
	  jas_iccgetuint32(in, &hdr->magic) ||
	  jas_iccgetuint32(in, &hdr->platform) ||
	  jas_iccgetuint32(in, &hdr->flags) ||
	  jas_iccgetuint32(in, &hdr->maker) ||
	  jas_iccgetuint32(in, &hdr->model) ||
	  jas_iccgetuint64(in, &hdr->attr) ||
	  jas_iccgetuint32(in, &hdr->intent) ||
	  jas_iccgetxyz(in, &hdr->illum) ||
	  jas_iccgetuint32(in, &hdr->creator) ||
	  jas_stream_gobble(in, 44) != 44)
		return -1;
	return 0;
}
