static int jas_iccprof_writehdr(jas_stream_t *out, jas_icchdr_t *hdr)
{
	if (jas_iccputuint32(out, hdr->size) ||
	  jas_iccputuint32(out, hdr->cmmtype) ||
	  jas_iccputuint32(out, hdr->version) ||
	  jas_iccputuint32(out, hdr->clas) ||
	  jas_iccputuint32(out, hdr->colorspc) ||
	  jas_iccputuint32(out, hdr->refcolorspc) ||
	  jas_iccputtime(out, &hdr->ctime) ||
	  jas_iccputuint32(out, hdr->magic) ||
	  jas_iccputuint32(out, hdr->platform) ||
	  jas_iccputuint32(out, hdr->flags) ||
	  jas_iccputuint32(out, hdr->maker) ||
	  jas_iccputuint32(out, hdr->model) ||
	  jas_iccputuint64(out, hdr->attr) ||
	  jas_iccputuint32(out, hdr->intent) ||
	  jas_iccputxyz(out, &hdr->illum) ||
	  jas_iccputuint32(out, hdr->creator) ||
	  jas_stream_pad(out, 44, 0) != 44)
		return -1;
	return 0;
}
