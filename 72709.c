static u32 FFD_RegisterMimeTypes(const GF_InputService *plug) {
	u32 i;
	for (i = 0 ; FFD_MIME_TYPES[i]; i+=3)
		gf_service_register_mime(plug, FFD_MIME_TYPES[i], FFD_MIME_TYPES[i+1], FFD_MIME_TYPES[i+2]);
	return i/3;
}
