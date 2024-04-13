vips_foreign_find_save( const char *name )
{
	char filename[VIPS_PATH_MAX];
	char option_string[VIPS_PATH_MAX];
	VipsForeignSaveClass *save_class;

	vips__filename_split8( name, filename, option_string );

	if( !(save_class = (VipsForeignSaveClass *) vips_foreign_map( 
		"VipsForeignSave",
		(VipsSListMap2Fn) vips_foreign_find_save_sub, 
		(void *) filename, NULL )) ) {
		vips_error( "VipsForeignSave",
			_( "\"%s\" is not a known file format" ), name );

		return( NULL );
	}

	return( G_OBJECT_CLASS_NAME( save_class ) );
}
