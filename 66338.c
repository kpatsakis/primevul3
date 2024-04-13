static GF_Err gf_isom_dump_ttxt_track(GF_ISOFile *the_file, u32 track, FILE *dump, Bool box_dump)
{
	u32 i, j, count, di, nb_descs, shift_offset[20], so_count;
	u64 last_DTS;
	size_t len;
	GF_Box *a;
	Bool has_scroll;
	char szDur[100];
	GF_Tx3gSampleEntryBox *txt;

	GF_TrackBox *trak = gf_isom_get_track_from_file(the_file, track);
	if (!trak) return GF_BAD_PARAM;
	switch (trak->Media->handler->handlerType) {
	case GF_ISOM_MEDIA_TEXT:
	case GF_ISOM_MEDIA_SUBT:
		break;
	default:
		return GF_BAD_PARAM;
	}

	txt = (GF_Tx3gSampleEntryBox *)gf_list_get(trak->Media->information->sampleTable->SampleDescription->other_boxes, 0);
	switch (txt->type) {
	case GF_ISOM_BOX_TYPE_TX3G:
	case GF_ISOM_BOX_TYPE_TEXT:
		break;
	case GF_ISOM_BOX_TYPE_STPP:
	case GF_ISOM_BOX_TYPE_SBTT:
	default:
		return GF_BAD_PARAM;
	}

	if (box_dump) {
		fprintf(dump, "<TextTrack trackID=\"%d\" version=\"1.1\">\n", gf_isom_get_track_id(the_file, track) );
	} else {
		fprintf(dump, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
		fprintf(dump, "<!-- GPAC 3GPP Text Stream -->\n");

		fprintf(dump, "<TextStream version=\"1.1\">\n");
	}
	fprintf(dump, "<TextStreamHeader width=\"%d\" height=\"%d\" layer=\"%d\" translation_x=\"%d\" translation_y=\"%d\">\n", trak->Header->width >> 16 , trak->Header->height >> 16, trak->Header->layer, trak->Header->matrix[6] >> 16, trak->Header->matrix[7] >> 16);

	nb_descs = gf_list_count(trak->Media->information->sampleTable->SampleDescription->other_boxes);
	for (i=0; i<nb_descs; i++) {
		GF_Tx3gSampleEntryBox *txt = (GF_Tx3gSampleEntryBox *)gf_list_get(trak->Media->information->sampleTable->SampleDescription->other_boxes, i);

		if (box_dump) {
			gf_isom_box_dump((GF_Box*) txt, dump);
		} else if  (txt->type==GF_ISOM_BOX_TYPE_TX3G) {
			fprintf(dump, "<TextSampleDescription horizontalJustification=\"");
			switch (txt->horizontal_justification) {
			case 1:
				fprintf(dump, "center");
				break;
			case -1:
				fprintf(dump, "right");
				break;
			default:
				fprintf(dump, "left");
				break;
			}
			fprintf(dump, "\" verticalJustification=\"");
			switch (txt->vertical_justification) {
			case 1:
				fprintf(dump, "center");
				break;
			case -1:
				fprintf(dump, "bottom");
				break;
			default:
				fprintf(dump, "top");
				break;
			}
			fprintf(dump, "\" ");
			tx3g_dump_rgba8(dump, "backColor", txt->back_color);
			fprintf(dump, " verticalText=\"%s\"", (txt->displayFlags & GF_TXT_VERTICAL) ? "yes" : "no");
			fprintf(dump, " fillTextRegion=\"%s\"", (txt->displayFlags & GF_TXT_FILL_REGION) ? "yes" : "no");
			fprintf(dump, " continuousKaraoke=\"%s\"", (txt->displayFlags & GF_TXT_KARAOKE) ? "yes" : "no");
			has_scroll = GF_FALSE;
			if (txt->displayFlags & GF_TXT_SCROLL_IN) {
				has_scroll = GF_TRUE;
				if (txt->displayFlags & GF_TXT_SCROLL_OUT) fprintf(dump, " scroll=\"InOut\"");
				else fprintf(dump, " scroll=\"In\"");
			} else if (txt->displayFlags & GF_TXT_SCROLL_OUT) {
				has_scroll = GF_TRUE;
				fprintf(dump, " scroll=\"Out\"");
			} else {
				fprintf(dump, " scroll=\"None\"");
			}
			if (has_scroll) {
				u32 mode = (txt->displayFlags & GF_TXT_SCROLL_DIRECTION)>>7;
				switch (mode) {
				case GF_TXT_SCROLL_CREDITS:
					fprintf(dump, " scrollMode=\"Credits\"");
					break;
				case GF_TXT_SCROLL_MARQUEE:
					fprintf(dump, " scrollMode=\"Marquee\"");
					break;
				case GF_TXT_SCROLL_DOWN:
					fprintf(dump, " scrollMode=\"Down\"");
					break;
				case GF_TXT_SCROLL_RIGHT:
					fprintf(dump, " scrollMode=\"Right\"");
					break;
				default:
					fprintf(dump, " scrollMode=\"Unknown\"");
					break;
				}
			}
			fprintf(dump, ">\n");
			fprintf(dump, "<FontTable>\n");
			if (txt->font_table) {
				for (j=0; j<txt->font_table->entry_count; j++) {
					fprintf(dump, "<FontTableEntry fontName=\"%s\" fontID=\"%d\"/>\n", txt->font_table->fonts[j].fontName, txt->font_table->fonts[j].fontID);

				}
			}
			fprintf(dump, "</FontTable>\n");
			if ((txt->default_box.bottom == txt->default_box.top) || (txt->default_box.right == txt->default_box.left)) {
				txt->default_box.top = txt->default_box.left = 0;
				txt->default_box.right = trak->Header->width / 65536;
				txt->default_box.bottom = trak->Header->height / 65536;
			}
			tx3g_dump_box_nobox(dump, &txt->default_box);
			tx3g_dump_style_nobox(dump, &txt->default_style, NULL, 0);
			fprintf(dump, "</TextSampleDescription>\n");
		} else {
			GF_TextSampleEntryBox *text = (GF_TextSampleEntryBox *)gf_list_get(trak->Media->information->sampleTable->SampleDescription->other_boxes, i);
			fprintf(dump, "<TextSampleDescription horizontalJustification=\"");
			switch (text->textJustification) {
			case 1:
				fprintf(dump, "center");
				break;
			case -1:
				fprintf(dump, "right");
				break;
			default:
				fprintf(dump, "left");
				break;
			}
			fprintf(dump, "\"");

			tx3g_dump_rgb16(dump, " backColor", text->background_color);

			if ((text->default_box.bottom == text->default_box.top) || (text->default_box.right == text->default_box.left)) {
				text->default_box.top = text->default_box.left = 0;
				text->default_box.right = trak->Header->width / 65536;
				text->default_box.bottom = trak->Header->height / 65536;
			}

			if (text->displayFlags & GF_TXT_SCROLL_IN) {
				if (text->displayFlags & GF_TXT_SCROLL_OUT) fprintf(dump, " scroll=\"InOut\"");
				else fprintf(dump, " scroll=\"In\"");
			} else if (text->displayFlags & GF_TXT_SCROLL_OUT) {
				fprintf(dump, " scroll=\"Out\"");
			} else {
				fprintf(dump, " scroll=\"None\"");
			}
			fprintf(dump, ">\n");

			tx3g_dump_box_nobox(dump, &text->default_box);
			fprintf(dump, "</TextSampleDescription>\n");
		}
	}
	fprintf(dump, "</TextStreamHeader>\n");

	last_DTS = 0;
	count = gf_isom_get_sample_count(the_file, track);
	for (i=0; i<count; i++) {
		GF_BitStream *bs;
		GF_TextSample *txt;
		GF_ISOSample *s = gf_isom_get_sample(the_file, track, i+1, &di);
		if (!s) continue;

		fprintf(dump, "<TextSample sampleTime=\"%s\" sampleDescriptionIndex=\"%d\"", tx3g_format_time(s->DTS, trak->Media->mediaHeader->timeScale, szDur, GF_FALSE), di);
		bs = gf_bs_new(s->data, s->dataLength, GF_BITSTREAM_READ);
		txt = gf_isom_parse_texte_sample(bs);
		gf_bs_del(bs);

		if (!box_dump) {
			if (txt->highlight_color) {
				fprintf(dump, " ");
				tx3g_dump_rgba8(dump, "highlightColor", txt->highlight_color->hil_color);
			}
			if (txt->scroll_delay) {
				Double delay = txt->scroll_delay->scroll_delay;
				delay /= trak->Media->mediaHeader->timeScale;
				fprintf(dump, " scrollDelay=\"%g\"", delay);
			}
			if (txt->wrap) fprintf(dump, " wrap=\"%s\"", (txt->wrap->wrap_flag==0x01) ? "Automatic" : "None");
		}

		so_count = 0;

		fprintf(dump, " xml:space=\"preserve\">");
		if (!txt->len) {
			last_DTS = (u32) trak->Media->mediaHeader->duration;
		} else {
			unsigned short utf16Line[10000];
			last_DTS = s->DTS;
			/*UTF16*/
			if ((txt->len>2) && ((unsigned char) txt->text[0] == (unsigned char) 0xFE) && ((unsigned char) txt->text[1] == (unsigned char) 0xFF)) {
				/*copy 2 more chars because the lib always add 2 '0' at the end for UTF16 end of string*/
				memcpy((char *) utf16Line, txt->text+2, sizeof(char) * (txt->len));
				len = gf_utf8_wcslen((const u16*)utf16Line);
			} else {
				char *str;
				str = txt->text;
				len = gf_utf8_mbstowcs((u16*)utf16Line, 10000, (const char **) &str);
			}
			if (len != (size_t) -1) {
				utf16Line[len] = 0;
				for (j=0; j<len; j++) {
					if ((utf16Line[j]=='\n') || (utf16Line[j]=='\r') || (utf16Line[j]==0x85) || (utf16Line[j]==0x2028) || (utf16Line[j]==0x2029) ) {
						fprintf(dump, "\n");
						if ((utf16Line[j]=='\r') && (utf16Line[j+1]=='\n')) {
							shift_offset[so_count] = j;
							so_count++;
							j++;
						}
					}
					else {
						switch (utf16Line[j]) {
						case '\'':
							fprintf(dump, "&apos;");
							break;
						case '\"':
							fprintf(dump, "&quot;");
							break;
						case '&':
							fprintf(dump, "&amp;");
							break;
						case '>':
							fprintf(dump, "&gt;");
							break;
						case '<':
							fprintf(dump, "&lt;");
							break;
						default:
							if (utf16Line[j] < 128) {
								fprintf(dump, "%c", (u8) utf16Line[j]);
							} else {
								fprintf(dump, "&#%d;", utf16Line[j]);
							}
							break;
						}
					}
				}
			}
		}

		if (box_dump) {

			if (txt->highlight_color)
				gf_isom_box_dump((GF_Box*) txt->highlight_color, dump);
			if (txt->scroll_delay)
				gf_isom_box_dump((GF_Box*) txt->scroll_delay, dump);
			if (txt->wrap)
				gf_isom_box_dump((GF_Box*) txt->wrap, dump);
			if (txt->box)
				gf_isom_box_dump((GF_Box*) txt->box, dump);
			if (txt->styles)
				gf_isom_box_dump((GF_Box*) txt->styles, dump);
		} else {

			if (txt->box) tx3g_dump_box_nobox(dump, &txt->box->box);
			if (txt->styles) {
				for (j=0; j<txt->styles->entry_count; j++) {
					tx3g_dump_style_nobox(dump, &txt->styles->styles[j], shift_offset, so_count);
				}
			}
		}
		j=0;
		while ((a = (GF_Box *)gf_list_enum(txt->others, &j))) {
			if (box_dump) {
				gf_isom_box_dump((GF_Box*) a, dump);
				continue;
			}

			switch (a->type) {
			case GF_ISOM_BOX_TYPE_HLIT:
				fprintf(dump, "<Highlight ");
				tx3g_print_char_offsets(dump, ((GF_TextHighlightBox *)a)->startcharoffset, ((GF_TextHighlightBox *)a)->endcharoffset, shift_offset, so_count);
				fprintf(dump, "/>\n");
				break;
			case GF_ISOM_BOX_TYPE_HREF:
			{
				GF_TextHyperTextBox *ht = (GF_TextHyperTextBox *)a;
				fprintf(dump, "<HyperLink ");
				tx3g_print_char_offsets(dump, ht->startcharoffset, ht->endcharoffset, shift_offset, so_count);
				fprintf(dump, "URL=\"%s\" URLToolTip=\"%s\"/>\n", ht->URL ? ht->URL : "", ht->URL_hint ? ht->URL_hint : "");
			}
			break;
			case GF_ISOM_BOX_TYPE_BLNK:
				fprintf(dump, "<Blinking ");
				tx3g_print_char_offsets(dump, ((GF_TextBlinkBox *)a)->startcharoffset, ((GF_TextBlinkBox *)a)->endcharoffset, shift_offset, so_count);
				fprintf(dump, "/>\n");
				break;
			case GF_ISOM_BOX_TYPE_KROK:
			{
				u32 k;
				Double t;
				GF_TextKaraokeBox *krok = (GF_TextKaraokeBox *)a;
				t = krok->highlight_starttime;
				t /= trak->Media->mediaHeader->timeScale;
				fprintf(dump, "<Karaoke startTime=\"%g\">\n", t);
				for (k=0; k<krok->nb_entries; k++) {
					t = krok->records[k].highlight_endtime;
					t /= trak->Media->mediaHeader->timeScale;
					fprintf(dump, "<KaraokeRange ");
					tx3g_print_char_offsets(dump, krok->records[k].start_charoffset, krok->records[k].end_charoffset, shift_offset, so_count);
					fprintf(dump, "endTime=\"%g\"/>\n", t);
				}
				fprintf(dump, "</Karaoke>\n");
			}
				break;
			}
		}

		fprintf(dump, "</TextSample>\n");
		gf_isom_sample_del(&s);
		gf_isom_delete_text_sample(txt);
		gf_set_progress("TTXT Extract", i, count);
	}
	if (last_DTS < trak->Media->mediaHeader->duration) {
		fprintf(dump, "<TextSample sampleTime=\"%s\" text=\"\" />\n", tx3g_format_time(trak->Media->mediaHeader->duration, trak->Media->mediaHeader->timeScale, szDur, GF_FALSE));
	}

	if (box_dump) {
		fprintf(dump, "</TextTrack>\n");
	} else {
		fprintf(dump, "</TextStream>\n");
	}
	if (count) gf_set_progress("TTXT Extract", count, count);
	return GF_OK;
}
