static void nsc_encode_subsampling_sse2(NSC_CONTEXT* context)
{
	UINT16 x;
	UINT16 y;
	BYTE* co_dst;
	BYTE* cg_dst;
	INT8* co_src0;
	INT8* co_src1;
	INT8* cg_src0;
	INT8* cg_src1;
	UINT32 tempWidth;
	UINT32 tempHeight;
	__m128i t;
	__m128i val;
	__m128i mask = _mm_set1_epi16(0xFF);
	tempWidth = ROUND_UP_TO(context->width, 8);
	tempHeight = ROUND_UP_TO(context->height, 2);

	for (y = 0; y < tempHeight >> 1; y++)
	{
		co_dst = context->priv->PlaneBuffers[1] + y * (tempWidth >> 1);
		cg_dst = context->priv->PlaneBuffers[2] + y * (tempWidth >> 1);
		co_src0 = (INT8*) context->priv->PlaneBuffers[1] + (y << 1) * tempWidth;
		co_src1 = co_src0 + tempWidth;
		cg_src0 = (INT8*) context->priv->PlaneBuffers[2] + (y << 1) * tempWidth;
		cg_src1 = cg_src0 + tempWidth;

		for (x = 0; x < tempWidth >> 1; x += 8)
		{
			t = _mm_loadu_si128((__m128i*) co_src0);
			t = _mm_avg_epu8(t, _mm_loadu_si128((__m128i*) co_src1));
			val = _mm_and_si128(_mm_srli_si128(t, 1), mask);
			val = _mm_avg_epu16(val, _mm_and_si128(t, mask));
			val = _mm_packus_epi16(val, val);
			_mm_storeu_si128((__m128i*) co_dst, val);
			co_dst += 8;
			co_src0 += 16;
			co_src1 += 16;
			t = _mm_loadu_si128((__m128i*) cg_src0);
			t = _mm_avg_epu8(t, _mm_loadu_si128((__m128i*) cg_src1));
			val = _mm_and_si128(_mm_srli_si128(t, 1), mask);
			val = _mm_avg_epu16(val, _mm_and_si128(t, mask));
			val = _mm_packus_epi16(val, val);
			_mm_storeu_si128((__m128i*) cg_dst, val);
			cg_dst += 8;
			cg_src0 += 16;
			cg_src1 += 16;
		}
 	}
 }
