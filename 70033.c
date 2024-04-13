int LoadGif(struct ngiflib_gif * g) {
	struct ngiflib_gce gce;
	u8 sign;
	u8 tmp;
	int i;

	if(!g) return -1;
	gce.gce_present = 0;
	
	if(g->nimg==0) {
		GetByteStr(g, g->signature, 6);
		g->signature[6] = '\0';
		if(   g->signature[0] != 'G'
		   || g->signature[1] != 'I'
		   || g->signature[2] != 'F'
		   || g->signature[3] != '8') {
			return -1;
		}
#if !defined(NGIFLIB_NO_FILE)
		if(g->log) fprintf(g->log, "%s\n", g->signature);
#endif /* !defined(NGIFLIB_NO_FILE) */

		g->width = GetWord(g);
		g->height = GetWord(g);
		/* allocate frame buffer */
#ifndef NGIFLIB_INDEXED_ONLY
		if((g->mode & NGIFLIB_MODE_INDEXED)==0)
			g->frbuff.p32 = ngiflib_malloc(4*(long)g->height*(long)g->width);
		else
#endif /* NGIFLIB_INDEXED_ONLY */
			g->frbuff.p8 = ngiflib_malloc((long)g->height*(long)g->width);

		tmp = GetByte(g);/* <Packed Fields> = Global Color Table Flag       1 Bit
		                                      Color Resolution              3 Bits
		                                      Sort Flag                     1 Bit
		                                      Size of Global Color Table    3 Bits */
		g->colorresolution = ((tmp & 0x70) >> 4) + 1;
		g->sort_flag = (tmp & 8) >> 3;
		g->imgbits = (tmp & 7) + 1;	/* Global Palette color resolution */
		g->ncolors = 1 << g->imgbits;

		g->backgroundindex = GetByte(g);

#if !defined(NGIFLIB_NO_FILE)
		if(g->log) fprintf(g->log, "%hux%hu %hhubits %hu couleurs  bg=%hhu\n",
		                   g->width, g->height, g->imgbits, g->ncolors, g->backgroundindex);
#endif /* NGIFLIB_INDEXED_ONLY */

		g->pixaspectratio = GetByte(g);	/* pixel aspect ratio (0 : unspecified) */

		if(tmp&0x80) {
			/* la palette globale suit. */
			g->palette = (struct ngiflib_rgb *)ngiflib_malloc(sizeof(struct ngiflib_rgb)*g->ncolors);
			for(i=0; i<g->ncolors; i++) {
				g->palette[i].r = GetByte(g);
				g->palette[i].g = GetByte(g);
				g->palette[i].b = GetByte(g);
#if defined(DEBUG) && !defined(NGIFLIB_NO_FILE)
				if(g->log) fprintf(g->log, "%3d %02X %02X %02X\n", i, g->palette[i].r,g->palette[i].g,g->palette[i].b);
#endif /* defined(DEBUG) && !defined(NGIFLIB_NO_FILE) */
			}
#ifdef NGIFLIB_ENABLE_CALLBACKS
			if(g->palette_cb) g->palette_cb(g, g->palette, g->ncolors);
#endif /* NGIFLIB_ENABLE_CALLBACKS */
		} else {
			g->palette = NULL;
		}
		g->netscape_loop_count = -1;
	}

	for(;;) {
		char appid_auth[11];
		u8 id,size;
		int blockindex;

		sign = GetByte(g);	/* signature du prochain bloc */
#if !defined(NGIFLIB_NO_FILE)
		if(g->log) fprintf(g->log, "BLOCK SIGNATURE 0x%02X '%c'\n", sign, (sign >= 32) ? sign : '.');
#endif /* NGIFLIB_INDEXED_ONLY */
		switch(sign) {
		case 0x3B:	/* END OF GIF */
			return 0;
		case '!':	/* Extension introducer 0x21 */
			id = GetByte(g);
			blockindex = 0;
#if !defined(NGIFLIB_NO_FILE)
			if(g->log) fprintf(g->log, "extension (id=0x%02hhx)\n", id);
#endif /* NGIFLIB_NO_FILE */
			while( (size = GetByte(g)) ) {
				u8 ext[256];

				GetByteStr(g, ext, size);

				switch(id) {
				case 0xF9:	/* Graphic Control Extension */
					/* The scope of this extension is the first graphic
					 * rendering block to follow. */
					gce.gce_present = 1;
					gce.disposal_method = (ext[0] >> 2) & 7;
					gce.transparent_flag = ext[0] & 1;
					gce.user_input_flag = (ext[0] >> 1) & 1;
					gce.delay_time = ext[1] | (ext[2]<<8);
					gce.transparent_color = ext[3];
#if !defined(NGIFLIB_NO_FILE)
					if(g->log) fprintf(g->log, "disposal_method=%hhu delay_time=%hu (transp=%hhu)transparent_color=0x%02hhX\n",
					       gce.disposal_method, gce.delay_time, gce.transparent_flag, gce.transparent_color);
#endif /* NGIFLIB_INDEXED_ONLY */
					/* this propably should be adjusted depending on the disposal_method
					 * of the _previous_ image. */
					if(gce.transparent_flag && ((g->nimg == 0) || gce.disposal_method == 2)) {
						FillGifBackGround(g);
					}
					break;
				case 0xFE:	/* Comment Extension. */
#if !defined(NGIFLIB_NO_FILE)
					if(g->log) {
						if(blockindex==0) fprintf(g->log, "-------------------- Comment extension --------------------\n");
						ext[size] = '\0';
						fputs((char *)ext, g->log);
					}
#endif /* NGIFLIB_NO_FILE */
					break;
				case 0xFF:	/* application extension */
					/* NETSCAPE2.0 extension :
					 * http://www.vurdalakov.net/misc/gif/netscape-looping-application-extension */
					if(blockindex==0) {
						ngiflib_memcpy(appid_auth, ext, 11);
#if !defined(NGIFLIB_NO_FILE)
						if(g->log) {
							fprintf(g->log, "---------------- Application extension ---------------\n");
							fprintf(g->log, "Application identifier : '%.8s', auth code : %02X %02X %02X (",
							        appid_auth, ext[8], ext[9], ext[10]);
							fputc((ext[8]<32)?' ':ext[8], g->log);
							fputc((ext[9]<32)?' ':ext[9], g->log);
							fputc((ext[10]<32)?' ':ext[10], g->log);
							fprintf(g->log, ")\n");
						}
#endif /* NGIFLIB_INDEXED_ONLY */
					} else {
#if !defined(NGIFLIB_NO_FILE)
						if(g->log) {
							fprintf(g->log, "Datas (as hex) : ");
							for(i=0; i<size; i++) {
								fprintf(g->log, "%02x ", ext[i]);
							}
							fprintf(g->log, "\nDatas (as text) : '");
							for(i=0; i<size; i++) {
								putc((ext[i]<32)?' ':ext[i], g->log);
							}
							fprintf(g->log, "'\n");
						}
#endif /* NGIFLIB_INDEXED_ONLY */
						if(0 == ngiflib_memcmp(appid_auth, "NETSCAPE2.0", 11)) {
							/* ext[0] : Sub-block ID */
							if(ext[0] == 1) {
								/* 1 : Netscape Looping Extension. */
								g->netscape_loop_count = (int)ext[1] | ((int)ext[2] << 8);
#if !defined(NGIFLIB_NO_FILE)
								if(g->log) {
									fprintf(g->log, "NETSCAPE loop_count = %d\n", g->netscape_loop_count);
								}
#endif /* NGIFLIB_NO_FILE */
							}
						}
					}
					break;
				case 0x01:	/* plain text extension */
#if !defined(NGIFLIB_NO_FILE)
					if(g->log) {
						fprintf(g->log, "Plain text extension   blockindex=%d\n", blockindex);
						for(i=0; i<size; i++) {
							putc((ext[i]<32)?' ':ext[i], g->log);
						}
						putc('\n', g->log);
					}
#endif /* NGIFLIB_INDEXED_ONLY */
					break;
				}
				blockindex++;
			}
			switch(id) {
			case 0x01:	/* plain text extension */
			case 0xFE:	/* Comment Extension. */
			case 0xFF:	/* application extension */
#if !defined(NGIFLIB_NO_FILE)
				if(g->log) {
					fprintf(g->log, "-----------------------------------------------------------\n");
				}
#endif /* NGIFLIB_NO_FILE */
				break;
			}
			break;
		case 0x2C:	/* Image separator */
			if(g->nimg==0) {
				g->cur_img = ngiflib_malloc(sizeof(struct ngiflib_img));
				if(g->cur_img == NULL) return -2;    /* memory error */
				g->first_img = g->cur_img;
			} else {
				g->cur_img->next = ngiflib_malloc(sizeof(struct ngiflib_img));
				if(g->cur_img->next == NULL) return -2; /* memory error */
				g->cur_img = g->cur_img->next;
			}
			ngiflib_memset(g->cur_img, 0,  sizeof(struct ngiflib_img));
			g->cur_img->parent = g;
			if(gce.gce_present) {
				ngiflib_memcpy(&g->cur_img->gce, &gce, sizeof(struct ngiflib_gce));
			} else {
				ngiflib_memset(&g->cur_img->gce, 0,  sizeof(struct ngiflib_gce));
			}
			if (DecodeGifImg(g->cur_img) < 0) return -1;
			g->nimg++;

			tmp = GetByte(g);/* 0 final */
#if !defined(NGIFLIB_NO_FILE)
			if(g->log) fprintf(g->log, "ZERO TERMINATOR 0x%02X\n", tmp);
#endif /* NGIFLIB_INDEXED_ONLY */
			return 1;	/* image decode */
		default:
			/* unexpected byte */
#if !defined(NGIFLIB_NO_FILE)
			if(g->log) fprintf(g->log, "unexpected signature 0x%02X\n", sign);
#endif /* NGIFLIB_INDEXED_ONLY */
			return -1;
		}
	}
}
