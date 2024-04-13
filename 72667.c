static void ResetCaption()
{
	GF_Event event;
	if (display_rti) return;
	event.type = GF_EVENT_SET_CAPTION;
	if (is_connected) {
		char szName[1024];
		NetInfoCommand com;

		event.caption.caption = NULL;
		/*get any service info*/
		if (!startup_file && gf_term_get_service_info(term, gf_term_get_root_object(term), &com) == GF_OK) {
			strcpy(szName, "");
			if (com.track_info) {
				char szBuf[10];
				sprintf(szBuf, "%02d ", (u32) (com.track_info>>16) );
				strcat(szName, szBuf);
			}
			if (com.artist) {
				strcat(szName, com.artist);
				strcat(szName, " ");
			}
			if (com.name) {
				strcat(szName, com.name);
				strcat(szName, " ");
			}
			if (com.album) {
				strcat(szName, "(");
				strcat(szName, com.album);
				strcat(szName, ")");
			}
			if (com.provider) {
				strcat(szName, "(");
				strcat(szName, com.provider);
				strcat(szName, ")");
			}

			if (strlen(szName)) event.caption.caption = szName;
		}
		if (!event.caption.caption) {
			char *str = strrchr(the_url, '\\');
			if (!str) str = strrchr(the_url, '/');
			event.caption.caption = str ? str+1 : the_url;
		}
	} else {
		event.caption.caption = "GPAC MP4Client " GPAC_FULL_VERSION;
	}
	gf_term_user_event(term, &event);
}
