vips_gifload( const char *filename, VipsImage **out, ... )
{
	va_list ap;
	int result;

	va_start( ap, out );
	result = vips_call_split( "gifload", ap, filename, out );
	va_end( ap );

	return( result );
}
