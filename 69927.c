vips_foreign_load_gif_file_class_init( 
	VipsForeignLoadGifFileClass *class )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( class );
	VipsObjectClass *object_class = (VipsObjectClass *) class;
	VipsForeignClass *foreign_class = (VipsForeignClass *) class;
	VipsForeignLoadClass *load_class = (VipsForeignLoadClass *) class;
	VipsForeignLoadGifClass *gif_class = (VipsForeignLoadGifClass *) class;

	gobject_class->dispose = vips_foreign_load_gif_file_dispose;
	gobject_class->set_property = vips_object_set_property;
	gobject_class->get_property = vips_object_get_property;

	object_class->nickname = "gifload";
	object_class->description = _( "load GIF with giflib" );

	foreign_class->suffs = vips_foreign_gif_suffs;

	load_class->is_a = vips_foreign_load_gif_is_a;

	gif_class->open = vips_foreign_load_gif_file_open;

	VIPS_ARG_STRING( class, "filename", 1, 
		_( "Filename" ),
		_( "Filename to load from" ),
		VIPS_ARGUMENT_REQUIRED_INPUT, 
		G_STRUCT_OFFSET( VipsForeignLoadGifFile, filename ),
		NULL );

}
