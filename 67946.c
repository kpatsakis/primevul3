vips_foreign_summary_class( VipsObjectClass *object_class, VipsBuf *buf )
{
	VipsForeignClass *class = VIPS_FOREIGN_CLASS( object_class );

	VIPS_OBJECT_CLASS( vips_foreign_parent_class )->
		summary_class( object_class, buf );

	if( class->suffs ) {
		const char **p;

		vips_buf_appends( buf, " (" );
		for( p = class->suffs; *p; p++ ) {
			vips_buf_appendf( buf, "%s", *p );
			if( p[1] )
				vips_buf_appends( buf, ", " );
		}
		vips_buf_appends( buf, ")" );
	}

	vips_buf_appendf( buf, ", priority=%d", class->priority );

}
