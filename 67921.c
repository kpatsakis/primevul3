vips_foreign_find_save_buffer( const char *name )
{
	char suffix[VIPS_PATH_MAX];
	char option_string[VIPS_PATH_MAX];
	VipsForeignSaveClass *save_class;

	vips__filename_split8( name, suffix, option_string );

	if( !(save_class = (VipsForeignSaveClass *) vips_foreign_map( 
		"VipsForeignSave",
		(VipsSListMap2Fn) vips_foreign_find_save_buffer_sub, 
		(void *) suffix, NULL )) ) {
		vips_error( "VipsForeignSave",
			_( "\"%s\" is not a known buffer format" ), name );

		return( NULL );
	}

	return( G_OBJECT_CLASS_NAME( save_class ) );
}
