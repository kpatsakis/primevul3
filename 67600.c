static void nsc_encode_argb_to_aycocg_sse2(NSC_CONTEXT* context,
        const BYTE* data, UINT32 scanline)
{
	UINT16 x;
	UINT16 y;
	UINT16 rw;
	BYTE ccl;
	const BYTE* src;
	BYTE* yplane = NULL;
	BYTE* coplane = NULL;
	BYTE* cgplane = NULL;
	BYTE* aplane = NULL;
	__m128i r_val;
	__m128i g_val;
	__m128i b_val;
	__m128i a_val;
	__m128i y_val;
	__m128i co_val;
	__m128i cg_val;
	UINT32 tempWidth;
	tempWidth = ROUND_UP_TO(context->width, 8);
	rw = (context->ChromaSubsamplingLevel > 0 ? tempWidth : context->width);
	ccl = context->ColorLossLevel;

	for (y = 0; y < context->height; y++)
	{
		src = data + (context->height - 1 - y) * scanline;
		yplane = context->priv->PlaneBuffers[0] + y * rw;
		coplane = context->priv->PlaneBuffers[1] + y * rw;
		cgplane = context->priv->PlaneBuffers[2] + y * rw;
		aplane = context->priv->PlaneBuffers[3] + y * context->width;

		for (x = 0; x < context->width; x += 8)
		{
			switch (context->format)
			{
				case PIXEL_FORMAT_BGRX32:
					b_val = _mm_set_epi16(*(src + 28), *(src + 24), *(src + 20), *(src + 16),
					                      *(src + 12), *(src + 8), *(src + 4), *src);
					g_val = _mm_set_epi16(*(src + 29), *(src + 25), *(src + 21), *(src + 17),
					                      *(src + 13), *(src + 9), *(src + 5), *(src + 1));
					r_val = _mm_set_epi16(*(src + 30), *(src + 26), *(src + 22), *(src + 18),
					                      *(src + 14), *(src + 10), *(src + 6), *(src + 2));
					a_val = _mm_set1_epi16(0xFF);
					src += 32;
					break;

				case PIXEL_FORMAT_BGRA32:
					b_val = _mm_set_epi16(*(src + 28), *(src + 24), *(src + 20), *(src + 16),
					                      *(src + 12), *(src + 8), *(src + 4), *src);
					g_val = _mm_set_epi16(*(src + 29), *(src + 25), *(src + 21), *(src + 17),
					                      *(src + 13), *(src + 9), *(src + 5), *(src + 1));
					r_val = _mm_set_epi16(*(src + 30), *(src + 26), *(src + 22), *(src + 18),
					                      *(src + 14), *(src + 10), *(src + 6), *(src + 2));
					a_val = _mm_set_epi16(*(src + 31), *(src + 27), *(src + 23), *(src + 19),
					                      *(src + 15), *(src + 11), *(src + 7), *(src + 3));
					src += 32;
					break;

				case PIXEL_FORMAT_RGBX32:
					r_val = _mm_set_epi16(*(src + 28), *(src + 24), *(src + 20), *(src + 16),
					                      *(src + 12), *(src + 8), *(src + 4), *src);
					g_val = _mm_set_epi16(*(src + 29), *(src + 25), *(src + 21), *(src + 17),
					                      *(src + 13), *(src + 9), *(src + 5), *(src + 1));
					b_val = _mm_set_epi16(*(src + 30), *(src + 26), *(src + 22), *(src + 18),
					                      *(src + 14), *(src + 10), *(src + 6), *(src + 2));
					a_val = _mm_set1_epi16(0xFF);
					src += 32;
					break;

				case PIXEL_FORMAT_RGBA32:
					r_val = _mm_set_epi16(*(src + 28), *(src + 24), *(src + 20), *(src + 16),
					                      *(src + 12), *(src + 8), *(src + 4), *src);
					g_val = _mm_set_epi16(*(src + 29), *(src + 25), *(src + 21), *(src + 17),
					                      *(src + 13), *(src + 9), *(src + 5), *(src + 1));
					b_val = _mm_set_epi16(*(src + 30), *(src + 26), *(src + 22), *(src + 18),
					                      *(src + 14), *(src + 10), *(src + 6), *(src + 2));
					a_val = _mm_set_epi16(*(src + 31), *(src + 27), *(src + 23), *(src + 19),
					                      *(src + 15), *(src + 11), *(src + 7), *(src + 3));
					src += 32;
					break;

				case PIXEL_FORMAT_BGR24:
					b_val = _mm_set_epi16(*(src + 21), *(src + 18), *(src + 15), *(src + 12),
					                      *(src + 9), *(src + 6), *(src + 3), *src);
					g_val = _mm_set_epi16(*(src + 22), *(src + 19), *(src + 16), *(src + 13),
					                      *(src + 10), *(src + 7), *(src + 4), *(src + 1));
					r_val = _mm_set_epi16(*(src + 23), *(src + 20), *(src + 17), *(src + 14),
					                      *(src + 11), *(src + 8), *(src + 5), *(src + 2));
					a_val = _mm_set1_epi16(0xFF);
					src += 24;
					break;

				case PIXEL_FORMAT_RGB24:
					r_val = _mm_set_epi16(*(src + 21), *(src + 18), *(src + 15), *(src + 12),
					                      *(src + 9), *(src + 6), *(src + 3), *src);
					g_val = _mm_set_epi16(*(src + 22), *(src + 19), *(src + 16), *(src + 13),
					                      *(src + 10), *(src + 7), *(src + 4), *(src + 1));
					b_val = _mm_set_epi16(*(src + 23), *(src + 20), *(src + 17), *(src + 14),
					                      *(src + 11), *(src + 8), *(src + 5), *(src + 2));
					a_val = _mm_set1_epi16(0xFF);
					src += 24;
					break;

				case PIXEL_FORMAT_BGR16:
					b_val = _mm_set_epi16(
					            (((*(src + 15)) & 0xF8) | ((*(src + 15)) >> 5)),
					            (((*(src + 13)) & 0xF8) | ((*(src + 13)) >> 5)),
					            (((*(src + 11)) & 0xF8) | ((*(src + 11)) >> 5)),
					            (((*(src + 9)) & 0xF8) | ((*(src + 9)) >> 5)),
					            (((*(src + 7)) & 0xF8) | ((*(src + 7)) >> 5)),
					            (((*(src + 5)) & 0xF8) | ((*(src + 5)) >> 5)),
					            (((*(src + 3)) & 0xF8) | ((*(src + 3)) >> 5)),
					            (((*(src + 1)) & 0xF8) | ((*(src + 1)) >> 5)));
					g_val = _mm_set_epi16(
					            ((((*(src + 15)) & 0x07) << 5) | (((*(src + 14)) & 0xE0) >> 3)),
					            ((((*(src + 13)) & 0x07) << 5) | (((*(src + 12)) & 0xE0) >> 3)),
					            ((((*(src + 11)) & 0x07) << 5) | (((*(src + 10)) & 0xE0) >> 3)),
					            ((((*(src + 9)) & 0x07) << 5) | (((*(src + 8)) & 0xE0) >> 3)),
					            ((((*(src + 7)) & 0x07) << 5) | (((*(src + 6)) & 0xE0) >> 3)),
					            ((((*(src + 5)) & 0x07) << 5) | (((*(src + 4)) & 0xE0) >> 3)),
					            ((((*(src + 3)) & 0x07) << 5) | (((*(src + 2)) & 0xE0) >> 3)),
					            ((((*(src + 1)) & 0x07) << 5) | (((*src) & 0xE0) >> 3)));
					r_val = _mm_set_epi16(
					            ((((*(src + 14)) & 0x1F) << 3) | (((*(src + 14)) >> 2) & 0x07)),
					            ((((*(src + 12)) & 0x1F) << 3) | (((*(src + 12)) >> 2) & 0x07)),
					            ((((*(src + 10)) & 0x1F) << 3) | (((*(src + 10)) >> 2) & 0x07)),
					            ((((*(src + 8)) & 0x1F) << 3) | (((*(src + 8)) >> 2) & 0x07)),
					            ((((*(src + 6)) & 0x1F) << 3) | (((*(src + 6)) >> 2) & 0x07)),
					            ((((*(src + 4)) & 0x1F) << 3) | (((*(src + 4)) >> 2) & 0x07)),
					            ((((*(src + 2)) & 0x1F) << 3) | (((*(src + 2)) >> 2) & 0x07)),
					            ((((*src) & 0x1F) << 3) | (((*src) >> 2) & 0x07)));
					a_val = _mm_set1_epi16(0xFF);
					src += 16;
					break;

				case PIXEL_FORMAT_RGB16:
					r_val = _mm_set_epi16(
					            (((*(src + 15)) & 0xF8) | ((*(src + 15)) >> 5)),
					            (((*(src + 13)) & 0xF8) | ((*(src + 13)) >> 5)),
					            (((*(src + 11)) & 0xF8) | ((*(src + 11)) >> 5)),
					            (((*(src + 9)) & 0xF8) | ((*(src + 9)) >> 5)),
					            (((*(src + 7)) & 0xF8) | ((*(src + 7)) >> 5)),
					            (((*(src + 5)) & 0xF8) | ((*(src + 5)) >> 5)),
					            (((*(src + 3)) & 0xF8) | ((*(src + 3)) >> 5)),
					            (((*(src + 1)) & 0xF8) | ((*(src + 1)) >> 5)));
					g_val = _mm_set_epi16(
					            ((((*(src + 15)) & 0x07) << 5) | (((*(src + 14)) & 0xE0) >> 3)),
					            ((((*(src + 13)) & 0x07) << 5) | (((*(src + 12)) & 0xE0) >> 3)),
					            ((((*(src + 11)) & 0x07) << 5) | (((*(src + 10)) & 0xE0) >> 3)),
					            ((((*(src + 9)) & 0x07) << 5) | (((*(src + 8)) & 0xE0) >> 3)),
					            ((((*(src + 7)) & 0x07) << 5) | (((*(src + 6)) & 0xE0) >> 3)),
					            ((((*(src + 5)) & 0x07) << 5) | (((*(src + 4)) & 0xE0) >> 3)),
					            ((((*(src + 3)) & 0x07) << 5) | (((*(src + 2)) & 0xE0) >> 3)),
					            ((((*(src + 1)) & 0x07) << 5) | (((*src) & 0xE0) >> 3)));
					b_val = _mm_set_epi16(
					            ((((*(src + 14)) & 0x1F) << 3) | (((*(src + 14)) >> 2) & 0x07)),
					            ((((*(src + 12)) & 0x1F) << 3) | (((*(src + 12)) >> 2) & 0x07)),
					            ((((*(src + 10)) & 0x1F) << 3) | (((*(src + 10)) >> 2) & 0x07)),
					            ((((*(src + 8)) & 0x1F) << 3) | (((*(src + 8)) >> 2) & 0x07)),
					            ((((*(src + 6)) & 0x1F) << 3) | (((*(src + 6)) >> 2) & 0x07)),
					            ((((*(src + 4)) & 0x1F) << 3) | (((*(src + 4)) >> 2) & 0x07)),
					            ((((*(src + 2)) & 0x1F) << 3) | (((*(src + 2)) >> 2) & 0x07)),
					            ((((*src) & 0x1F) << 3) | (((*src) >> 2) & 0x07)));
					a_val = _mm_set1_epi16(0xFF);
					src += 16;
					break;

				case PIXEL_FORMAT_A4:
					{
						int shift;
						BYTE idx[8];

						for (shift = 7; shift >= 0; shift--)
						{
							idx[shift] = ((*src) >> shift) & 1;
							idx[shift] |= (((*(src + 1)) >> shift) & 1) << 1;
							idx[shift] |= (((*(src + 2)) >> shift) & 1) << 2;
							idx[shift] |= (((*(src + 3)) >> shift) & 1) << 3;
							idx[shift] *= 3;
						}

						r_val = _mm_set_epi16(
						            context->palette[idx[0]],
						            context->palette[idx[1]],
						            context->palette[idx[2]],
						            context->palette[idx[3]],
						            context->palette[idx[4]],
						            context->palette[idx[5]],
						            context->palette[idx[6]],
						            context->palette[idx[7]]);
						g_val = _mm_set_epi16(
						            context->palette[idx[0] + 1],
						            context->palette[idx[1] + 1],
						            context->palette[idx[2] + 1],
						            context->palette[idx[3] + 1],
						            context->palette[idx[4] + 1],
						            context->palette[idx[5] + 1],
						            context->palette[idx[6] + 1],
						            context->palette[idx[7] + 1]);
						b_val = _mm_set_epi16(
						            context->palette[idx[0] + 2],
						            context->palette[idx[1] + 2],
						            context->palette[idx[2] + 2],
						            context->palette[idx[3] + 2],
						            context->palette[idx[4] + 2],
						            context->palette[idx[5] + 2],
						            context->palette[idx[6] + 2],
						            context->palette[idx[7] + 2]);
						src += 4;
					}

					a_val = _mm_set1_epi16(0xFF);
					break;

				case PIXEL_FORMAT_RGB8:
					{
						r_val = _mm_set_epi16(
						            context->palette[(*(src + 7)) * 3],
						            context->palette[(*(src + 6)) * 3],
						            context->palette[(*(src + 5)) * 3],
						            context->palette[(*(src + 4)) * 3],
						            context->palette[(*(src + 3)) * 3],
						            context->palette[(*(src + 2)) * 3],
						            context->palette[(*(src + 1)) * 3],
						            context->palette[(*src) * 3]);
						g_val = _mm_set_epi16(
						            context->palette[(*(src + 7)) * 3 + 1],
						            context->palette[(*(src + 6)) * 3 + 1],
						            context->palette[(*(src + 5)) * 3 + 1],
						            context->palette[(*(src + 4)) * 3 + 1],
						            context->palette[(*(src + 3)) * 3 + 1],
						            context->palette[(*(src + 2)) * 3 + 1],
						            context->palette[(*(src + 1)) * 3 + 1],
						            context->palette[(*src) * 3 + 1]);
						b_val = _mm_set_epi16(
						            context->palette[(*(src + 7)) * 3 + 2],
						            context->palette[(*(src + 6)) * 3 + 2],
						            context->palette[(*(src + 5)) * 3 + 2],
						            context->palette[(*(src + 4)) * 3 + 2],
						            context->palette[(*(src + 3)) * 3 + 2],
						            context->palette[(*(src + 2)) * 3 + 2],
						            context->palette[(*(src + 1)) * 3 + 2],
						            context->palette[(*src) * 3 + 2]);
						src += 8;
					}

					a_val = _mm_set1_epi16(0xFF);
					break;

				default:
					r_val = g_val = b_val = a_val = _mm_set1_epi16(0);
					break;
			}

			y_val = _mm_srai_epi16(r_val, 2);
			y_val = _mm_add_epi16(y_val, _mm_srai_epi16(g_val, 1));
			y_val = _mm_add_epi16(y_val, _mm_srai_epi16(b_val, 2));
			co_val = _mm_sub_epi16(r_val, b_val);
			co_val = _mm_srai_epi16(co_val, ccl);
			cg_val = _mm_sub_epi16(g_val, _mm_srai_epi16(r_val, 1));
			cg_val = _mm_sub_epi16(cg_val, _mm_srai_epi16(b_val, 1));
			cg_val = _mm_srai_epi16(cg_val, ccl);
			y_val = _mm_packus_epi16(y_val, y_val);
			_mm_storeu_si128((__m128i*) yplane, y_val);
			co_val = _mm_packs_epi16(co_val, co_val);
			_mm_storeu_si128((__m128i*) coplane, co_val);
			cg_val = _mm_packs_epi16(cg_val, cg_val);
			_mm_storeu_si128((__m128i*) cgplane, cg_val);
			a_val = _mm_packus_epi16(a_val, a_val);
			_mm_storeu_si128((__m128i*) aplane, a_val);
			yplane += 8;
			coplane += 8;
			cgplane += 8;
			aplane += 8;
		}

		if (context->ChromaSubsamplingLevel > 0 && (context->width % 2) == 1)
		{
			context->priv->PlaneBuffers[0][y * rw + context->width] =
			    context->priv->PlaneBuffers[0][y * rw + context->width - 1];
			context->priv->PlaneBuffers[1][y * rw + context->width] =
			    context->priv->PlaneBuffers[1][y * rw + context->width - 1];
			context->priv->PlaneBuffers[2][y * rw + context->width] =
			    context->priv->PlaneBuffers[2][y * rw + context->width - 1];
		}
	}

	if (context->ChromaSubsamplingLevel > 0 && (y % 2) == 1)
	{
		yplane = context->priv->PlaneBuffers[0] + y * rw;
		coplane = context->priv->PlaneBuffers[1] + y * rw;
		cgplane = context->priv->PlaneBuffers[2] + y * rw;
		CopyMemory(yplane, yplane - rw, rw);
		CopyMemory(coplane, coplane - rw, rw);
		CopyMemory(cgplane, cgplane - rw, rw);
	}
}
