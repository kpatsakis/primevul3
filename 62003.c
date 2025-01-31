static int mif_process_cmpt(mif_hdr_t *hdr, char *buf)
{
	jas_tvparser_t *tvp;
	mif_cmpt_t *cmpt;
	int id;

	cmpt = 0;
	tvp = 0;

	if (!(cmpt = mif_cmpt_create())) {
		jas_eprintf("cannot create component\n");
		goto error;
	}
	cmpt->tlx = 0;
	cmpt->tly = 0;
	cmpt->sampperx = 0;
	cmpt->samppery = 0;
	cmpt->width = 0;
	cmpt->height = 0;
	cmpt->prec = 0;
	cmpt->sgnd = -1;
	cmpt->data = 0;

	if (!(tvp = jas_tvparser_create(buf))) {
		jas_eprintf("cannot create parser\n");
		goto error;
	}

	if ((id = jas_tvparser_next(tvp))) {
		abort();
	}

	while (!(id = jas_tvparser_next(tvp))) {
		switch (jas_taginfo_nonull(jas_taginfos_lookup(mif_tags,
		  jas_tvparser_gettag(tvp)))->id) {
		case MIF_TLX:
			cmpt->tlx = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_TLY:
			cmpt->tly = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_WIDTH:
			cmpt->width = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_HEIGHT:
			cmpt->height = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_HSAMP:
			cmpt->sampperx = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_VSAMP:
			cmpt->samppery = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_PREC:
			cmpt->prec = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_SGND:
			cmpt->sgnd = atoi(jas_tvparser_getval(tvp));
			break;
		case MIF_DATA:
			if (!(cmpt->data = jas_strdup(jas_tvparser_getval(tvp)))) {
				goto error;
			}
			break;
		default:
			jas_eprintf("invalid component information: %s\n", buf);
			goto error;
			break;
		}
	}
	if (!cmpt->sampperx || !cmpt->samppery) {
		goto error;
	}
	if (!cmpt->width || !cmpt->height || !cmpt->prec || cmpt->sgnd < 0) {
		goto error;
	}
	if (mif_hdr_addcmpt(hdr, hdr->numcmpts, cmpt)) {
		jas_eprintf("cannot add component\n");
		goto error;
	}
	jas_tvparser_destroy(tvp);
	return 0;

error:
	if (cmpt) {
		mif_cmpt_destroy(cmpt);
	}
	if (tvp) {
		jas_tvparser_destroy(tvp);
	}
	return -1;
}
