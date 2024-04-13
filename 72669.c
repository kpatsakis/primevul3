void ViewODs(GF_Terminal *term, Bool show_timing)
{
	GF_ObjectManager *root_odm = gf_term_get_root_object(term);
	if (!root_odm) return;

	if (show_timing) {
		PrintODTiming(term, root_odm, 0);
	} else {
		PrintODBuffer(term, root_odm, 0);
	}
	fprintf(stderr, "\n");
}
