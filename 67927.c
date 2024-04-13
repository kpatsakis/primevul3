vips_foreign_load( const char *name, VipsImage **out, ... )
{
	char filename[VIPS_PATH_MAX];
	char option_string[VIPS_PATH_MAX];
	const char *operation_name;
	va_list ap;
	int result;

	vips__filename_split8( name, filename, option_string );
	if( !(operation_name = vips_foreign_find_load( filename )) )
		return( -1 );

	va_start( ap, out );
	result = vips_call_split_option_string( operation_name, option_string, 
		ap, filename, out );
	va_end( ap );

	return( result );
}
