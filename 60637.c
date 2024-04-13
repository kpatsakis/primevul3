_gdImageGd2 (gdImagePtr im, gdIOCtx * out, int cs, int fmt)
{
	int ncx, ncy, cx, cy;
	int x, y, ylo, yhi, xlo, xhi;
	int chunkLen;
	int chunkNum = 0;
	char *chunkData = NULL;	/* So we can gdFree it with impunity. */
	char *compData = NULL;	/* So we can gdFree it with impunity. */
	uLongf compLen;
	int idxPos = 0;
	int idxSize;
	t_chunk_info *chunkIdx = NULL;
	int posSave;
	int bytesPerPixel = im->trueColor ? 4 : 1;
	int compMax = 0;

	/*printf("Trying to write GD2 file\n"); */

	/* */
	/* Force fmt to a valid value since we don't return anything. */
	/* */
	if ((fmt != GD2_FMT_RAW) && (fmt != GD2_FMT_COMPRESSED)) {
		fmt = GD2_FMT_COMPRESSED;
	};
	if (im->trueColor) {
		fmt += 2;
	}
	/* */
	/* Make sure chunk size is valid. These are arbitrary values; 64 because it seems */
	/* a little silly to expect performance improvements on a 64x64 bit scale, and  */
	/* 4096 because we buffer one chunk, and a 16MB buffer seems a little large - it may be */
	/* OK for one user, but for another to read it, they require the buffer. */
	/* */
	if (cs == 0) {
		cs = GD2_CHUNKSIZE;
	} else if (cs < GD2_CHUNKSIZE_MIN) {
		cs = GD2_CHUNKSIZE_MIN;
	} else if (cs > GD2_CHUNKSIZE_MAX) {
		cs = GD2_CHUNKSIZE_MAX;
	};

	/* Work out number of chunks. */
	ncx = (im->sx + cs - 1) / cs;
	ncy = (im->sy + cs - 1) / cs;

	/* Write the standard header. */
	_gd2PutHeader (im, out, cs, fmt, ncx, ncy);

	if (gd2_compressed (fmt)) {
		/* */
		/* Work out size of buffer for compressed data, If CHUNKSIZE is large, */
		/* then these will be large! */
		/* */
		/* The zlib notes say output buffer size should be (input size) * 1.01 * 12 */
		/* - we'll use 1.02 to be paranoid. */
		/* */
		compMax = cs * bytesPerPixel * cs * 1.02 + 12;

		/* */
		/* Allocate the buffers.  */
		/* */
		chunkData = gdCalloc (cs * bytesPerPixel * cs, 1);
		if (!chunkData) {
			goto fail;
		}
		compData = gdCalloc (compMax, 1);
		if (!compData) {
			goto fail;
		}

		/* */
		/* Save the file position of chunk index, and allocate enough space for */
		/* each chunk_info block . */
		/* */
		idxPos = gdTell (out);
		idxSize = ncx * ncy * sizeof (t_chunk_info);
		GD2_DBG (printf ("Index size is %d\n", idxSize));
		gdSeek (out, idxPos + idxSize);

		chunkIdx = gdCalloc (idxSize * sizeof (t_chunk_info), 1);
		if (!chunkIdx) {
			goto fail;
		}
	};

	_gdPutColors (im, out);

	GD2_DBG (printf ("Size: %dx%d\n", im->sx, im->sy));
	GD2_DBG (printf ("Chunks: %dx%d\n", ncx, ncy));

	for (cy = 0; (cy < ncy); cy++) {
		for (cx = 0; (cx < ncx); cx++) {

			ylo = cy * cs;
			yhi = ylo + cs;
			if (yhi > im->sy) {
				yhi = im->sy;
			};

			GD2_DBG (printf
			         ("Processing Chunk (%dx%d), y from %d to %d\n", cx, cy,
			          ylo, yhi));
			chunkLen = 0;
			for (y = ylo; (y < yhi); y++) {

				/*GD2_DBG(printf("y=%d: ",y)); */

				xlo = cx * cs;
				xhi = xlo + cs;
				if (xhi > im->sx) {
					xhi = im->sx;
				};

				if (gd2_compressed (fmt)) {
					for (x = xlo; x < xhi; x++) {
						/* 2.0.11: use truecolor pixel array. TBB */
						/*GD2_DBG(printf("%d...",x)); */
						if (im->trueColor) {
							int p = im->tpixels[y][x];
							chunkData[chunkLen++] = gdTrueColorGetAlpha (p);
							chunkData[chunkLen++] = gdTrueColorGetRed (p);
							chunkData[chunkLen++] = gdTrueColorGetGreen (p);
							chunkData[chunkLen++] = gdTrueColorGetBlue (p);
						} else {
							int p = im->pixels[y][x];
							chunkData[chunkLen++] = p;
						}
					};
				} else {
					for (x = xlo; x < xhi; x++) {
						/*GD2_DBG(printf("%d, ",x)); */

						if (im->trueColor) {
							gdPutInt (im->tpixels[y][x], out);
						} else {
							gdPutC ((unsigned char) im->pixels[y][x], out);
						}
					};
				};
				/*GD2_DBG(printf("y=%d done.\n",y)); */
			};
			if (gd2_compressed (fmt)) {
				compLen = compMax;
				if (compress ((unsigned char *)
				              &compData[0], &compLen,
				              (unsigned char *) &chunkData[0],
				              chunkLen) != Z_OK) {
					printf ("Error from compressing\n");
				} else {
					chunkIdx[chunkNum].offset = gdTell (out);
					chunkIdx[chunkNum++].size = compLen;
					GD2_DBG (printf
					         ("Chunk %d size %d offset %d\n", chunkNum,
					          chunkIdx[chunkNum - 1].size,
					          chunkIdx[chunkNum - 1].offset));

					if (gdPutBuf (compData, compLen, out) <= 0) {
						gd_error("gd write error\n");
					};
				};
			};
		};
	};
	if (gd2_compressed (fmt)) {
		/* Save the position, write the index, restore position (paranoia). */
		GD2_DBG (printf ("Seeking %d to write index\n", idxPos));
		posSave = gdTell (out);
		gdSeek (out, idxPos);
		GD2_DBG (printf ("Writing index\n"));
		for (x = 0; x < chunkNum; x++) {
			GD2_DBG (printf
			         ("Chunk %d size %d offset %d\n", x, chunkIdx[x].size,
			          chunkIdx[x].offset));
			gdPutInt (chunkIdx[x].offset, out);
			gdPutInt (chunkIdx[x].size, out);
		};
		/* We don't use fwrite for *endian reasons. */
		/*fwrite(chunkIdx, sizeof(int)*2, chunkNum, out); */
		gdSeek (out, posSave);
	};

	/*printf("Memory block size is %d\n",gdTell(out)); */
fail:
	GD2_DBG (printf ("Freeing memory\n"));

	if (chunkData) {
		gdFree (chunkData);
	}
	if (compData) {
		gdFree (compData);
	}
	if (chunkIdx) {
		gdFree (chunkIdx);
	}
	GD2_DBG (printf ("Done\n"));

}
