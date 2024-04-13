GF_Err prft_dump(GF_Box *a, FILE * trace)
{
	Double fracs;
	GF_ProducerReferenceTimeBox *ptr = (GF_ProducerReferenceTimeBox *) a;
	time_t secs;
	struct tm t;
	secs = (ptr->ntp >> 32) - GF_NTP_SEC_1900_TO_1970;
	if (secs < 0) {
		if (ptr->size) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("NTP time is not valid, using value 0\n"));
		}
		secs = 0;
	}
	t = *gmtime(&secs);
	fracs = (Double) (ptr->ntp & 0xFFFFFFFFULL);
	fracs /= 0xFFFFFFFF;
	fracs *= 1000;
	gf_isom_box_dump_start(a, "ProducerReferenceTimeBox", trace);

	fprintf(trace, "referenceTrackID=\"%d\" timestamp=\""LLU"\" NTP=\""LLU"\" UTC=\"%d-%02d-%02dT%02d:%02d:%02d.%03dZ\">\n", ptr->refTrackID, ptr->timestamp, ptr->ntp, 1900+t.tm_year, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, (u32) t.tm_sec, (u32) fracs);
	gf_isom_box_dump_done("ProducerReferenceTimeBox", a, trace);

	return GF_OK;
}
