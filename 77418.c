void HB_MyanmarAttributes(HB_Script script, const HB_UChar16 *text, hb_uint32 from, hb_uint32 len, HB_CharAttributes *attributes)
{
    int end = from + len;
    const HB_UChar16 *uc = text + from;
    hb_uint32 i = 0;
    HB_UNUSED(script);
    attributes += from;
    while (i < len) {
	HB_Bool invalid;
	hb_uint32 boundary = myanmar_nextSyllableBoundary(text, from+i, end, &invalid) - from;

	attributes[i].charStop = TRUE;
        if (i)
            attributes[i-1].lineBreakType = HB_Break;

	if (boundary > len-1)
            boundary = len;
	i++;
	while (i < boundary) {
	    attributes[i].charStop = FALSE;
	    ++uc;
	    ++i;
	}
	assert(i == boundary);
    }
}
