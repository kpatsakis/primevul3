static int bmp_gethdr(jas_stream_t *in, bmp_hdr_t *hdr)
{
	if (bmp_getint16(in, &hdr->magic) || hdr->magic != BMP_MAGIC ||
	  bmp_getint32(in, &hdr->siz) || bmp_getint16(in, &hdr->reserved1) ||
	  bmp_getint16(in, &hdr->reserved2) || bmp_getint32(in, &hdr->off)) {
		return -1;
	}
	return 0;
}
