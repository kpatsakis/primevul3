static int myanmar_nextSyllableBoundary(const HB_UChar16 *s, int start, int end, HB_Bool *invalid)
{
    const HB_UChar16 *uc = s + start;
    int state = 0;
    int pos = start;
    *invalid = FALSE;

    while (pos < end) {
        MymrCharClass charClass = getMyanmarCharClass(*uc);
        state = mymrStateTable[state][charClass & Mymr_CF_CLASS_MASK];
        if (pos == start)
            *invalid = (HB_Bool)(charClass & Mymr_CF_DOTTED_CIRCLE);

        MMDEBUG("state[%d]=%d class=%8x (uc=%4x)", pos - start, state, charClass, *uc);

        if (state < 0) {
            if (state < -1)
                --pos;
            break;
        }
        ++uc;
        ++pos;
    }
    return pos;
}
