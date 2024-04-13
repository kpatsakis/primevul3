void PrintODBuffer(GF_Terminal *term, GF_ObjectManager *odm, u32 indent)
{
	Float avg_dec_time;
	GF_MediaInfo odi;
	u32 ind, i, count;
	if (!odm) return;

	if (gf_term_get_object_info(term, odm, &odi) != GF_OK) return;
	if (!odi.od) {
		fprintf(stderr, "Service not attached\n");
		return;
	}

	ind = indent;
	while (ind) {
		fprintf(stderr, " ");
		ind--;
	}

	if (odi.generated_scene) {
		fprintf(stderr, "+ Service %s:\n", odi.service_url);
	} else {
		fprintf(stderr, "- OD %d: ", odi.od->objectDescriptorID);
		switch (odi.status) {
		case 1:
			fprintf(stderr, "Playing");
			break;
		case 2:
			fprintf(stderr, "Paused");
			break;
		default:
			fprintf(stderr, "Stopped");
			break;
		}
		if (odi.buffer>=0) fprintf(stderr, " - Buffer: %d ms", odi.buffer);
		if (odi.db_unit_count) fprintf(stderr, " - DB: %d AU", odi.db_unit_count);
		if (odi.cb_max_count) fprintf(stderr, " - CB: %d/%d CUs", odi.cb_unit_count, odi.cb_max_count);

		fprintf(stderr, "\n");
		ind = indent;
		while (ind) {
			fprintf(stderr, " ");
			ind--;
		}

		fprintf(stderr, " %d decoded frames - %d dropped frames\n", odi.nb_dec_frames, odi.nb_dropped);

		ind = indent;
		while (ind) {
			fprintf(stderr, " ");
			ind--;
		}

		avg_dec_time = 0;
		if (odi.nb_dec_frames) {
			avg_dec_time = (Float) odi.total_dec_time;
			avg_dec_time /= odi.nb_dec_frames;
		}
		fprintf(stderr, " Avg Bitrate %d kbps (%d max) - Avg Decoding Time %.2f us (%d max)\n",
		        (u32) odi.avg_bitrate/1024, odi.max_bitrate/1024, avg_dec_time, odi.max_dec_time);
	}

	count = gf_term_get_object_count(term, odm);
	for (i=0; i<count; i++) {
		GF_ObjectManager *an_odm = gf_term_get_object(term, odm, i);
		PrintODBuffer(term, an_odm, indent+1);
	}

}
