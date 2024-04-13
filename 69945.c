vips_giflib_buffer_read( GifFileType *file, GifByteType *buf, int n )
{
	VipsForeignLoadGif *gif = (VipsForeignLoadGif *) file->UserData;
	VipsForeignLoadGifBuffer *buffer = (VipsForeignLoadGifBuffer *) gif;
	size_t will_read = VIPS_MIN( n, buffer->bytes_to_go );

	memcpy( buf, buffer->p, will_read );
	buffer->p += will_read;
	buffer->bytes_to_go -= will_read;

	if( will_read == 0 )
		gif->eof = TRUE;

	return( will_read ); 
}
