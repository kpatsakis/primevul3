vips_foreign_load_gif_file_open( VipsForeignLoadGif *gif )
{
	VipsForeignLoad *load = (VipsForeignLoad *) gif;
	VipsForeignLoadGifFile *file = (VipsForeignLoadGifFile *) gif;

	if( !file->fp ) {
		if( !(file->fp = 
			vips__file_open_read( file->filename, NULL, FALSE )) ) 
			return( -1 ); 

		VIPS_SETSTR( load->out->filename, file->filename );
	}
	else 
		rewind( file->fp );

	vips_foreign_load_gif_close( gif );
	gif->read_func = vips_giflib_file_read;

	return( VIPS_FOREIGN_LOAD_GIF_CLASS( 
		vips_foreign_load_gif_file_parent_class )->open( gif ) );
}
