vips_foreign_load_gif_buffer_class_init( 
	VipsForeignLoadGifBufferClass *class )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( class );
	VipsObjectClass *object_class = (VipsObjectClass *) class;
	VipsForeignLoadClass *load_class = (VipsForeignLoadClass *) class;
	VipsForeignLoadGifClass *gif_class = (VipsForeignLoadGifClass *) class;

	gobject_class->set_property = vips_object_set_property;
	gobject_class->get_property = vips_object_get_property;

	object_class->nickname = "gifload_buffer";
	object_class->description = _( "load GIF with giflib" );

	load_class->is_a_buffer = vips_foreign_load_gif_is_a_buffer;

	gif_class->open = vips_foreign_load_gif_buffer_open;

	VIPS_ARG_BOXED( class, "buffer", 1, 
		_( "Buffer" ),
		_( "Buffer to load from" ),
		VIPS_ARGUMENT_REQUIRED_INPUT, 
		G_STRUCT_OFFSET( VipsForeignLoadGifBuffer, buf ),
		VIPS_TYPE_BLOB );

}
