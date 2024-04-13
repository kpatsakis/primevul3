void PrintODList(GF_Terminal *term, GF_ObjectManager *root_odm, u32 num, u32 indent, char *root_name)
{
	GF_MediaInfo odi;
	u32 i, count;
	char szIndent[50];
	GF_ObjectManager *odm;

	if (!root_odm) {
		fprintf(stderr, "Currently loaded objects:\n");
		root_odm = gf_term_get_root_object(term);
	}
	if (!root_odm) return;

	count = gf_term_get_current_service_id(term);
	if (count)
		fprintf(stderr, "Current service ID %d\n", count);

	if (gf_term_get_object_info(term, root_odm, &odi) != GF_OK) return;
	if (!odi.od) {
		fprintf(stderr, "Service not attached\n");
		return;
	}

	for (i=0; i<indent; i++) szIndent[i]=' ';
	szIndent[indent]=0;

	fprintf(stderr, "%s", szIndent);
	fprintf(stderr, "#%d %s - ", num, root_name);
	if (odi.od->ServiceID) fprintf(stderr, "Service ID %d ", odi.od->ServiceID);
	if (odi.media_url) {
		fprintf(stderr, "%s\n", odi.media_url);
	} else {
		fprintf(stderr, "OD ID %d\n", odi.od->objectDescriptorID);
	}

	szIndent[indent]=' ';
	szIndent[indent+1]=0;
	indent++;

	count = gf_term_get_object_count(term, root_odm);
	for (i=0; i<count; i++) {
		odm = gf_term_get_object(term, root_odm, i);
		if (!odm) break;
		num++;
		if (gf_term_get_object_info(term, odm, &odi) == GF_OK) {
			switch (gf_term_object_subscene_type(term, odm)) {
			case 1:
				PrintODList(term, odm, num, indent, "Root");
				break;
			case 2:
				PrintODList(term, odm, num, indent, "Inline Scene");
				break;
			case 3:
				PrintODList(term, odm, num, indent, "EXTERNPROTO Library");
				break;
			default:
				fprintf(stderr, "%s", szIndent);
				fprintf(stderr, "#%d - ", num);
				if (odi.media_url) {
					fprintf(stderr, "%s", odi.media_url);
				} else if (odi.od) {
				 	if (odi.od->URLString) {
						fprintf(stderr, "%s", odi.od->URLString);
					} else {
						fprintf(stderr, "ID %d", odi.od->objectDescriptorID);
					}
				} else if (odi.service_url) {
					fprintf(stderr, "%s", odi.service_url);
				} else {
					fprintf(stderr, "unknown");
				}
				fprintf(stderr, " - %s", (odi.od_type==GF_STREAM_VISUAL) ? "Video" : (odi.od_type==GF_STREAM_AUDIO) ? "Audio" : "Systems");
				if (odi.od && odi.od->ServiceID) fprintf(stderr, " - Service ID %d", odi.od->ServiceID);
				fprintf(stderr, "\n");
				break;
			}
		}
	}
}
