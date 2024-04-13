static int lzh_read_lens(struct kwajd_stream *lzh,
			 unsigned int type, unsigned int numsyms,
			 unsigned char *lens)
{
    register unsigned int bit_buffer;
    register int bits_left;
    unsigned char *i_ptr, *i_end;
    unsigned int i, c, sel;
    int err;

    RESTORE_BITS;
    switch (type) {
    case 0:
	i = numsyms; c = (i==16)?4: (i==32)?5: (i==64)?6: (i==256)?8 :0;
	for (i = 0; i < numsyms; i++) lens[i] = c;
	break;

    case 1:
	READ_BITS_SAFE(c, 4); lens[0] = c;
	for (i = 1; i < numsyms; i++) {
    	           READ_BITS_SAFE(sel, 1); if (sel == 0)  lens[i] = c;
	    else { READ_BITS_SAFE(sel, 1); if (sel == 0)  lens[i] = ++c;
	    else { READ_BITS_SAFE(c, 4);                  lens[i] = c; }}
	}
	break;

    case 2:
	READ_BITS_SAFE(c, 4); lens[0] = c;
	for (i = 1; i < numsyms; i++) {
	    READ_BITS_SAFE(sel, 2);
	    if (sel == 3) READ_BITS_SAFE(c, 4); else c += (char) sel-1;
	    lens[i] = c;
	}
	break;

    case 3:
	for (i = 0; i < numsyms; i++) {
	    READ_BITS_SAFE(c, 4); lens[i] = c;
	}
	break;
    }
    STORE_BITS;
    return MSPACK_ERR_OK;
}
