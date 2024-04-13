vips_foreign_save( VipsImage *in, const char *name, ... )
{
	char filename[VIPS_PATH_MAX];
	char option_string[VIPS_PATH_MAX];
	const char *operation_name;
	va_list ap;
	int result;

	vips__filename_split8( name, filename, option_string );

	if( !(operation_name = vips_foreign_find_save( filename )) )
		return( -1 );

	va_start( ap, name );
	result = vips_call_split_option_string( operation_name, option_string, 
		ap, in, filename );
	va_end( ap );

	return( result );
}
