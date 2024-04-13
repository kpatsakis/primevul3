vips_foreign_load_gif_file_dispose( GObject *gobject )
{
	VipsForeignLoadGifFile *file = (VipsForeignLoadGifFile *) gobject;

	VIPS_FREEF( fclose, file->fp ); 

	G_OBJECT_CLASS( vips_foreign_load_gif_file_parent_class )->
		dispose( gobject );
}
