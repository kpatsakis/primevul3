GF_Err gf_import_timed_text(GF_MediaImporter *import)
{
	GF_Err e;
	u32 fmt;
	e = gf_text_guess_format(import->in_name, &fmt);
	if (e) return e;
	if (import->streamFormat) {
		if (!strcmp(import->streamFormat, "VTT")) fmt = GF_TEXT_IMPORT_WEBVTT;
		else if (!strcmp(import->streamFormat, "TTML")) fmt = GF_TEXT_IMPORT_TTML;
		if ((strstr(import->in_name, ".swf") || strstr(import->in_name, ".SWF")) && !stricmp(import->streamFormat, "SVG")) fmt = GF_TEXT_IMPORT_SWF_SVG;
	}
	if (!fmt) {
		GF_LOG(GF_LOG_WARNING, GF_LOG_PARSER, ("[TTXT Import] Input %s does not look like a supported text format - ignoring\n", import->in_name));
		return GF_NOT_SUPPORTED;
	}
	if (import->flags & GF_IMPORT_PROBE_ONLY) {
		if (fmt==GF_TEXT_IMPORT_SUB) import->flags |= GF_IMPORT_OVERRIDE_FPS;
		return GF_OK;
	}
	switch (fmt) {
	case GF_TEXT_IMPORT_SRT:
		return gf_text_import_srt(import);
	case GF_TEXT_IMPORT_SUB:
		return gf_text_import_sub(import);
	case GF_TEXT_IMPORT_TTXT:
		return gf_text_import_ttxt(import);
	case GF_TEXT_IMPORT_TEXML:
		return gf_text_import_texml(import);
#ifndef GPAC_DISABLE_VTT
	case GF_TEXT_IMPORT_WEBVTT:
		return gf_text_import_webvtt(import);
#endif
	case GF_TEXT_IMPORT_SWF_SVG:
		return gf_text_import_swf(import);
	case GF_TEXT_IMPORT_TTML:
		return gf_text_import_ttml(import);
	default:
		return GF_BAD_PARAM;
	}
}
