static int ras_getdata(jas_stream_t *in, ras_hdr_t *hdr, ras_cmap_t *cmap,
  jas_image_t *image)
{
	int ret;

	switch (hdr->type) {
	case RAS_TYPE_OLD:
	case RAS_TYPE_STD:
		ret = ras_getdatastd(in, hdr, cmap, image);
		break;
	case RAS_TYPE_RLE:
		jas_eprintf("error: RLE encoding method not supported\n");
		ret = -1;
		break;
	default:
		jas_eprintf("error: encoding method not supported\n");
		ret = -1;
		break;
	}
	return ret;
}
