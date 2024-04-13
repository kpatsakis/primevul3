void calcrdslopes(jpc_enc_cblk_t *cblk)
{
	jpc_enc_pass_t *endpasses;
	jpc_enc_pass_t *pass0;
	jpc_enc_pass_t *pass1;
	jpc_enc_pass_t *pass2;
	jpc_flt_t slope0;
	jpc_flt_t slope;
	jpc_flt_t dd;
	long dr;

	endpasses = &cblk->passes[cblk->numpasses];
	pass2 = cblk->passes;
	slope0 = 0;
	while (pass2 != endpasses) {
		pass0 = 0;
		for (pass1 = cblk->passes; pass1 != endpasses; ++pass1) {
			dd = pass1->cumwmsedec;
			dr = pass1->end;
			if (pass0) {
				dd -= pass0->cumwmsedec;
				dr -= pass0->end;
			}
			if (dd <= 0) {
				pass1->rdslope = JPC_BADRDSLOPE;
				if (pass1 >= pass2) {
					pass2 = &pass1[1];
				}
				continue;
			}
			if (pass1 < pass2 && pass1->rdslope <= 0) {
				continue;
			}
			if (!dr) {
				assert(pass0);
				pass0->rdslope = 0;
				break;
			}
			slope = dd / dr;
			if (pass0 && slope >= slope0) {
				pass0->rdslope = 0;
				break;
			}
			pass1->rdslope = slope;
			if (pass1 >= pass2) {
				pass2 = &pass1[1];
			}
			pass0 = pass1;
			slope0 = slope;
		}
	}

#if 0
	for (pass0 = cblk->passes; pass0 != endpasses; ++pass0) {
if (pass0->rdslope > 0.0) {
		jas_eprintf("pass %02d nmsedec=%lf dec=%lf end=%d %lf\n", pass0 - cblk->passes,
		  fixtodbl(pass0->nmsedec), pass0->wmsedec, pass0->end, pass0->rdslope);
}
	}
#endif
}
