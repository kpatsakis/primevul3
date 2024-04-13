static GF_FilterProbeScore probe_meta_check_builtin_format(GF_FilterSession *fsess, GF_FilterRegister *freg, const char *url, const char *mime, char *fargs)
{
	char szExt[100];
	const char *ext = gf_file_ext_start(url);
	u32 len=0, i, j, count = gf_list_count(fsess->registry);
	if (ext) {
		ext++;
		len = (u32) strlen(ext);
	}
	//check in filter args if we have a format set, in which case replace URL ext by the given format
	if (fargs) {
		char szExtN[10];
		char *ext_arg;
		sprintf(szExtN, "ext%c", fsess->sep_name);
		ext_arg = strstr(fargs, szExtN);
		if (ext_arg) {
			char *next_arg;
			ext_arg+=4;
			next_arg = strchr(ext_arg, fsess->sep_args);
			if (next_arg) {
				len = (u32) (next_arg - ext_arg);
			} else {
				len = (u32) strlen(ext_arg);
			}
			if (len>99) len=99;
			strncpy(szExt, ext_arg, len);
			szExt[len] = 0;
			ext = szExt;
		}
	}

	if (mime) {
		if (strstr(mime, "/mp4")) return GF_FPROBE_MAYBE_SUPPORTED;
	}

	for (i=0; i<count; i++) {
		const GF_FilterArgs *dst_arg=NULL;
		GF_FilterRegister *reg = gf_list_get(fsess->registry, i);
		if (reg==freg) continue;
		if (reg->flags & GF_FS_REG_META) continue;

		j=0;
		while (reg->args) {
			dst_arg = &reg->args[j];
			if (!dst_arg || !dst_arg->arg_name) {
				dst_arg=NULL;
				break;
			}
			if (!strcmp(dst_arg->arg_name, "dst") && !(dst_arg->flags&GF_FS_ARG_SINK_ALIAS)) break;
			dst_arg = NULL;
			j++;
		}
		/*check prober*/
		if (dst_arg) {
			if (reg->probe_url) {
				GF_FilterProbeScore s = reg->probe_url(url, mime);
				if (s==GF_FPROBE_SUPPORTED)
					return GF_FPROBE_MAYBE_SUPPORTED;
			}
			continue;
		}

		/* check muxers*/
		for (j=0; j<reg->nb_caps; j++) {
			char *value=NULL;
			const char *pattern = NULL;
			const GF_FilterCapability *cap = &reg->caps[j];
			if (! (cap->flags & GF_CAPFLAG_OUTPUT) )
				continue;
			if (cap->flags & GF_CAPFLAG_EXCLUDED)
				continue;

			if (cap->code==GF_PROP_PID_FILE_EXT) {
				if (ext) {
					value = cap->val.value.string;
					pattern = ext;
				}
			} else if (cap->code==GF_PROP_PID_MIME) {
				if (mime) {
					value = cap->val.value.string;
					pattern = mime;
				}
			}
			while (value) {
				char *match = strstr(value, pattern);
				if (!match) break;
				if (!match[len] || match[len]=='|')
					return GF_FPROBE_MAYBE_SUPPORTED;
				value = match+1;
			}
		}
	}
	return GF_FPROBE_SUPPORTED;
}