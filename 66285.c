GF_Err audio_sample_entry_dump(GF_Box *a, FILE * trace)
{
	char *szName;
	Bool is_3gpp = GF_FALSE;
	GF_MPEGAudioSampleEntryBox *p = (GF_MPEGAudioSampleEntryBox *)a;

	switch (p->type) {
	case GF_ISOM_SUBTYPE_3GP_AMR:
		szName = "AMRSampleDescriptionBox";
		is_3gpp = GF_TRUE;
		break;
	case GF_ISOM_SUBTYPE_3GP_AMR_WB:
		szName = "AMR_WB_SampleDescriptionBox";
		is_3gpp = GF_TRUE;
		break;
	case GF_ISOM_SUBTYPE_3GP_EVRC:
		szName = "EVRCSampleDescriptionBox";
		is_3gpp = GF_TRUE;
		break;
	case GF_ISOM_SUBTYPE_3GP_QCELP:
		szName = "QCELPSampleDescriptionBox";
		is_3gpp = GF_TRUE;
		break;
	case GF_ISOM_SUBTYPE_3GP_SMV:
		szName = "SMVSampleDescriptionBox";
		is_3gpp = GF_TRUE;
		break;
	case GF_ISOM_BOX_TYPE_MP4A:
		szName = "MPEGAudioSampleDescriptionBox";
		break;
	case GF_ISOM_BOX_TYPE_AC3:
		szName = "AC3SampleEntryBox";
		break;
	case GF_ISOM_BOX_TYPE_EC3:
		szName = "EC3SampleEntryBox";
		break;
	default:
		szName = "AudioSampleDescriptionBox";
		break;
	}

	gf_isom_box_dump_start(a, szName, trace);
	base_audio_entry_dump((GF_AudioSampleEntryBox *)p, trace);
	fprintf(trace, ">\n");

	if (p->esd) {
		gf_isom_box_dump(p->esd, trace);
	} else if (p->cfg_3gpp) {
		gf_isom_box_dump(p->cfg_3gpp, trace);

	} else if (p->cfg_ac3) {
		if (p->size)
			gf_isom_box_dump(p->cfg_ac3, trace);

	} else if (p->size) {
		if (is_3gpp) {
			fprintf(trace, "<!-- INVALID 3GPP FILE: Config not present in Sample Description-->\n");
		} else {
			fprintf(trace, "<!--INVALID MP4 FILE: ESDBox not present in MPEG Sample Description or corrupted-->\n");
		}
	}
	if (a->type == GF_ISOM_BOX_TYPE_ENCA) {
		gf_isom_box_array_dump(p->protections, trace);
	}
	gf_isom_box_dump_done(szName, a, trace);
	return GF_OK;
}
