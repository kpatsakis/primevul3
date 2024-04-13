GF_ISOFile *package_file(char *file_name, char *fcc, const char *tmpdir, Bool make_wgt)
{
	GF_ISOFile *file = NULL;
	GF_Err e;
	GF_SAXParser *sax;
	GF_List *imports;
	Bool ascii;
	char root_dir[GF_MAX_PATH];
	char *isom_src = NULL;
	u32 i, count, mtype, skip_chars;
	char *type;

	type = gf_xml_get_root_type(file_name, &e);
	if (!type) {
		fprintf(stderr, "Cannot process XML file %s: %s\n", file_name, gf_error_to_string(e) );
		return NULL;
	}
	if (make_wgt) {
		if (strcmp(type, "widget")) {
			fprintf(stderr, "XML Root type %s differs from \"widget\" \n", type);
			gf_free(type);
			return NULL;
		}
		gf_free(type);
		type = gf_strdup("application/mw-manifest+xml");
		fcc = "mwgt";
	}
	imports = gf_list_new();


	root_dir[0] = 0;
	if (make_wgt) {
		WGTEnum wgt;
		char *sep = strrchr(file_name, '\\');
		if (!sep) sep = strrchr(file_name, '/');
		if (sep) {
			char c = sep[1];
			sep[1]=0;
			strcpy(root_dir, file_name);
			sep[1] = c;
		} else {
			strcpy(root_dir, "./");
		}
		wgt.dir = root_dir;
		wgt.root_file = file_name;
		wgt.imports = imports;
		gf_enum_directory(wgt.dir, 0, wgt_enum_files, &wgt, NULL);
		gf_enum_directory(wgt.dir, 1, wgt_enum_dir, &wgt, NULL);
		ascii = 1;
	} else {
		sax = gf_xml_sax_new(sax_node_start, NULL, NULL, imports);
		e = gf_xml_sax_parse_file(sax, file_name, NULL);
		ascii = !gf_xml_sax_binary_file(sax);
		gf_xml_sax_del(sax);
		if (e<0) goto exit;
		e = GF_OK;
	}

	if (fcc) {
		mtype = GF_4CC(fcc[0],fcc[1],fcc[2],fcc[3]);
	} else {
		mtype = 0;
		if (!stricmp(type, "svg")) mtype = ascii ? GF_META_TYPE_SVG : GF_META_TYPE_SVGZ;
		else if (!stricmp(type, "smil")) mtype = ascii ? GF_META_TYPE_SMIL : GF_META_TYPE_SMLZ;
		else if (!stricmp(type, "x3d")) mtype = ascii ? GF_META_TYPE_X3D  : GF_META_TYPE_X3DZ  ;
		else if (!stricmp(type, "xmt-a")) mtype = ascii ? GF_META_TYPE_XMTA : GF_META_TYPE_XMTZ;
	}
	if (!mtype) {
		fprintf(stderr, "Missing 4CC code for meta name - please use ABCD:fileName\n");
		e = GF_BAD_PARAM;
		goto exit;
	}


	if (!make_wgt) {
		count = gf_list_count(imports);
		for (i=0; i<count; i++) {
			char *item = gf_list_get(imports, i);

			FILE *test = gf_fopen(item, "rb");
			if (!test) {
				gf_list_rem(imports, i);
				i--;
				count--;
				gf_free(item);
				continue;
			}
			gf_fclose(test);
			if (gf_isom_probe_file(item)) {
				if (isom_src) {
					fprintf(stderr, "Cannot package several IsoMedia files together\n");
					e = GF_NOT_SUPPORTED;
					goto exit;
				}
				gf_list_rem(imports, i);
				i--;
				count--;
				isom_src = item;
				continue;
			}
		}
	}

	if (isom_src) {
		file = gf_isom_open(isom_src, GF_ISOM_OPEN_EDIT, tmpdir);
	} else {
		file = gf_isom_open("package", GF_ISOM_WRITE_EDIT, tmpdir);
	}

	e = gf_isom_set_meta_type(file, 1, 0, mtype);
	if (e) goto exit;
	/*add self ref*/
	if (isom_src) {
		e = gf_isom_add_meta_item(file, 1, 0, 1, NULL, isom_src, 0, 0, NULL, NULL, NULL,  NULL, NULL);
		if (e) goto exit;
	}
	e = gf_isom_set_meta_xml(file, 1, 0, file_name, !ascii);
	if (e) goto exit;

	skip_chars = (u32) strlen(root_dir);
	count = gf_list_count(imports);
	for (i=0; i<count; i++) {
		char *ext, *mime, *encoding, *name = NULL;
		char *item = gf_list_get(imports, i);

		name = gf_strdup(item + skip_chars);

		if (make_wgt) {
			char *sep;
			while (1) {
				sep = strchr(name, '\\');
				if (!sep) break;
				sep[0] = '/';
			}
		}


		mime = encoding = NULL;
		ext = strrchr(item, '.');
		if (!stricmp(ext, ".gz")) ext = strrchr(ext-1, '.');

		if (!stricmp(ext, ".jpg") || !stricmp(ext, ".jpeg")) mime = "image/jpeg";
		else if (!stricmp(ext, ".png")) mime = "image/png";
		else if (!stricmp(ext, ".svg")) mime = "image/svg+xml";
		else if (!stricmp(ext, ".x3d")) mime = "model/x3d+xml";
		else if (!stricmp(ext, ".xmt")) mime = "application/x-xmt";
		else if (!stricmp(ext, ".js")) {
			mime = "application/javascript";
		}
		else if (!stricmp(ext, ".svgz") || !stricmp(ext, ".svg.gz")) {
			mime = "image/svg+xml";
			encoding = "binary-gzip";
		}
		else if (!stricmp(ext, ".x3dz") || !stricmp(ext, ".x3d.gz")) {
			mime = "model/x3d+xml";
			encoding = "binary-gzip";
		}
		else if (!stricmp(ext, ".xmtz") || !stricmp(ext, ".xmt.gz")) {
			mime = "application/x-xmt";
			encoding = "binary-gzip";
		}

		e = gf_isom_add_meta_item(file, 1, 0, 0, item, name, 0, GF_META_ITEM_TYPE_MIME, mime, encoding, NULL,  NULL, NULL);
		gf_free(name);
		if (e) goto exit;
	}

exit:
	while (gf_list_count(imports)) {
		char *item = gf_list_last(imports);
		gf_list_rem_last(imports);
		gf_free(item);
	}
	gf_list_del(imports);
	if (isom_src) gf_free(isom_src);
	if (type) gf_free(type);
	if (e) {
		if (file) gf_isom_delete(file);
		return NULL;
	}
	return file;
}
