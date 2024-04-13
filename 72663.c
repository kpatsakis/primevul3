void PrintODTiming(GF_Terminal *term, GF_ObjectManager *odm, u32 indent)
{
	GF_MediaInfo odi;
	u32 ind = indent;
	u32 i, count;
	if (!odm) return;

	if (gf_term_get_object_info(term, odm, &odi) != GF_OK) return;
	if (!odi.od) {
		fprintf(stderr, "Service not attached\n");
		return;
	}
	while (ind) {
		fprintf(stderr, " ");
		ind--;
	}

	if (! odi.generated_scene) {

		fprintf(stderr, "- OD %d: ", odi.od->objectDescriptorID);
		switch (odi.status) {
		case 1:
			fprintf(stderr, "Playing - ");
			break;
		case 2:
			fprintf(stderr, "Paused - ");
			break;
		default:
			fprintf(stderr, "Stopped - ");
			break;
		}
		if (odi.buffer>=0) fprintf(stderr, "Buffer: %d ms - ", odi.buffer);
		else fprintf(stderr, "Not buffering - ");
		fprintf(stderr, "Clock drift: %d ms", odi.clock_drift);
		fprintf(stderr, " - time: ");
		PrintTime((u32) (odi.current_time*1000));
		fprintf(stderr, "\n");

	} else {
		fprintf(stderr, "+ Service %s:\n", odi.service_url);
	}

	count = gf_term_get_object_count(term, odm);
	for (i=0; i<count; i++) {
		GF_ObjectManager *an_odm = gf_term_get_object(term, odm, i);
		PrintODTiming(term, an_odm, indent+1);
	}
	return;

}
