vips_foreign_load_class_init( VipsForeignLoadClass *class )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( class );
	VipsObjectClass *object_class = (VipsObjectClass *) class;
	VipsOperationClass *operation_class = (VipsOperationClass *) class;

	gobject_class->dispose = vips_foreign_load_dispose;
	gobject_class->set_property = vips_object_set_property;
	gobject_class->get_property = vips_object_get_property;

	object_class->build = vips_foreign_load_build;
	object_class->summary_class = vips_foreign_load_summary_class;
	object_class->new_from_string = vips_foreign_load_new_from_string;
	object_class->nickname = "fileload";
	object_class->description = _( "file loaders" );

	operation_class->get_flags = vips_foreign_load_operation_get_flags;

	VIPS_ARG_IMAGE( class, "out", 2, 
		_( "Output" ), 
		_( "Output image" ),
		VIPS_ARGUMENT_REQUIRED_OUTPUT, 
		G_STRUCT_OFFSET( VipsForeignLoad, out ) );

	VIPS_ARG_FLAGS( class, "flags", 6, 
		_( "Flags" ), 
		_( "Flags for this file" ),
		VIPS_ARGUMENT_OPTIONAL_OUTPUT,
		G_STRUCT_OFFSET( VipsForeignLoad, flags ),
		VIPS_TYPE_FOREIGN_FLAGS, VIPS_FOREIGN_NONE ); 

	VIPS_ARG_BOOL( class, "memory", 7, 
		_( "Memory" ), 
		_( "Force open via memory" ),
		VIPS_ARGUMENT_OPTIONAL_INPUT,
		G_STRUCT_OFFSET( VipsForeignLoad, memory ),
		FALSE );

	VIPS_ARG_ENUM( class, "access", 8, 
		_( "Access" ), 
		_( "Required access pattern for this file" ),
		VIPS_ARGUMENT_OPTIONAL_INPUT,
		G_STRUCT_OFFSET( VipsForeignLoad, access ),
		VIPS_TYPE_ACCESS, VIPS_ACCESS_RANDOM ); 

	VIPS_ARG_BOOL( class, "sequential", 10, 
		_( "Sequential" ), 
		_( "Sequential read only" ),
		VIPS_ARGUMENT_OPTIONAL_INPUT | VIPS_ARGUMENT_DEPRECATED,
		G_STRUCT_OFFSET( VipsForeignLoad, sequential ),
		FALSE );

	VIPS_ARG_BOOL( class, "fail", 11, 
		_( "Fail" ), 
		_( "Fail on first error" ),
		VIPS_ARGUMENT_OPTIONAL_INPUT,
		G_STRUCT_OFFSET( VipsForeignLoad, fail ),
		FALSE );

	VIPS_ARG_BOOL( class, "disc", 12, 
		_( "Disc" ), 
		_( "Open to disc" ),
		VIPS_ARGUMENT_OPTIONAL_INPUT | VIPS_ARGUMENT_DEPRECATED,
		G_STRUCT_OFFSET( VipsForeignLoad, disc ),
		TRUE );

}
