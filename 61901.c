static int jp2_getcs(jp2_colr_t *colr)
{
	if (colr->method == JP2_COLR_ENUM) {
		switch (colr->csid) {
		case JP2_COLR_SRGB:
			return JAS_CLRSPC_SRGB;
			break;
		case JP2_COLR_SYCC:
			return JAS_CLRSPC_SYCBCR;
			break;
		case JP2_COLR_SGRAY:
			return JAS_CLRSPC_SGRAY;
			break;
		}
	}
	return JAS_CLRSPC_UNKNOWN;
}
