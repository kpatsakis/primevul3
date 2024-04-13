vips_foreign_find_save_buffer_sub( VipsForeignSaveClass *save_class, 
	const char *suffix )
{
	VipsObjectClass *object_class = VIPS_OBJECT_CLASS( save_class );
	VipsForeignClass *class = VIPS_FOREIGN_CLASS( save_class );

	if( class->suffs &&
		vips_ispostfix( object_class->nickname, "_buffer" ) &&
		vips_filename_suffix_match( suffix, class->suffs ) )
		return( save_class );

	return( NULL );
}
