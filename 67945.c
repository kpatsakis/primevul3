vips_foreign_save_summary_class( VipsObjectClass *object_class, VipsBuf *buf )
{
	VipsForeignSaveClass *class = VIPS_FOREIGN_SAVE_CLASS( object_class );

	VIPS_OBJECT_CLASS( vips_foreign_save_parent_class )->
		summary_class( object_class, buf );

	vips_buf_appendf( buf, ", %s", 
		vips_enum_nick( VIPS_TYPE_SAVEABLE, class->saveable ) );
}
