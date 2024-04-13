GF_Err gf_isom_text_dump(GF_ISOFile *the_file, u32 track, FILE *dump, GF_TextDumpType dump_type)
{
	switch (dump_type) {
	case GF_TEXTDUMPTYPE_SVG:
		return gf_isom_dump_svg_track(the_file, track, dump);
	case GF_TEXTDUMPTYPE_SRT:
		return gf_isom_dump_srt_track(the_file, track, dump);
	case GF_TEXTDUMPTYPE_TTXT:
	case GF_TEXTDUMPTYPE_TTXT_BOXES:
		return gf_isom_dump_ttxt_track(the_file, track, dump, (dump_type==GF_TEXTDUMPTYPE_TTXT_BOXES) ? GF_TRUE : GF_FALSE);
	default:
		return GF_BAD_PARAM;
	}
}
