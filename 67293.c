static u16 GetGifWord(struct ngiflib_img * i, struct ngiflib_decode_context * context) {
	u16 r;
	int bits_todo;
	u16 newbyte;

	bits_todo = (int)context->nbbit - (int)context->restbits;
	if( bits_todo <= 0) {	/* nbbit <= restbits */
		r = context->lbyte;
		context->restbits -= context->nbbit;
		context->lbyte >>= context->nbbit;
	} else if( bits_todo > 8 ) {	/* nbbit > restbits + 8 */
		if(context->restbyte >= 2) {
			context->restbyte -= 2;
			r = *context->srcbyte++;
		} else {
			if(context->restbyte == 0) {
				context->restbyte = GetByte(i->parent);
#if defined(DEBUG) && !defined(NGIFLIB_NO_FILE)
				if(i->parent->log) fprintf(i->parent->log, "restbyte = %02X\n", context->restbyte);
#endif /* defined(DEBUG) && !defined(NGIFLIB_NO_FILE) */
				GetByteStr(i->parent, context->byte_buffer, context->restbyte);
				context->srcbyte = context->byte_buffer;
			}
			r = *context->srcbyte++;
			if(--context->restbyte == 0) {
				context->restbyte = GetByte(i->parent);
#if defined(DEBUG) && !defined(NGIFLIB_NO_FILE)
				if(i->parent->log) fprintf(i->parent->log, "restbyte = %02X\n", context->restbyte);
#endif /* defined(DEBUG) && !defined(NGIFLIB_NO_FILE) */
				GetByteStr(i->parent, context->byte_buffer, context->restbyte);
				context->srcbyte = context->byte_buffer;
			}
			context->restbyte--;
		}
		newbyte = *context->srcbyte++;
		r |= newbyte << 8;
		r = (r << context->restbits) | context->lbyte;
		context->restbits = 16 - bits_todo;
		context->lbyte = newbyte >> (bits_todo - 8);
	} else /*if( bits_todo > 0 )*/ { /* nbbit > restbits */
		if(context->restbyte == 0) {
			context->restbyte = GetByte(i->parent);
#if defined(DEBUG) && !defined(NGIFLIB_NO_FILE)
			if(i->parent->log) fprintf(i->parent->log, "restbyte = %02X\n", context->restbyte);
#endif /* defined(DEBUG) && !defined(NGIFLIB_NO_FILE) */
			GetByteStr(i->parent, context->byte_buffer, context->restbyte);
			context->srcbyte = context->byte_buffer;
		}
		newbyte = *context->srcbyte++;
		context->restbyte--;
		r = (newbyte << context->restbits) | context->lbyte;
		context->restbits = 8 - bits_todo;
		context->lbyte = newbyte >> bits_todo;
	}
	return (r & context->max);	/* applique le bon masque pour eliminer les bits en trop */
}
