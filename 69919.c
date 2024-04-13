vips_foreign_load_gif_buffer_open( VipsForeignLoadGif *gif )
{
	VipsForeignLoadGifBuffer *buffer = (VipsForeignLoadGifBuffer *) gif;

	vips_foreign_load_gif_close( gif );
	buffer->p = buffer->buf->data;
	buffer->bytes_to_go = buffer->buf->length;
	gif->read_func = vips_giflib_buffer_read;;

	return( VIPS_FOREIGN_LOAD_GIF_CLASS( 
		vips_foreign_load_gif_file_parent_class )->open( gif ) );
}
