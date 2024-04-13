vips_foreign_save_build( VipsObject *object )
{
	VipsForeignSave *save = VIPS_FOREIGN_SAVE( object );

	if( save->in ) {
		VipsForeignSaveClass *class = 
			VIPS_FOREIGN_SAVE_GET_CLASS( save );
		VipsImage *ready;

		if( vips__foreign_convert_saveable( save->in, &ready,
			class->saveable, class->format_table, class->coding,
			save->background ) )
			return( -1 );

		if( save->page_height )
			vips_image_set_int( ready, 
				VIPS_META_PAGE_HEIGHT, save->page_height );

		VIPS_UNREF( save->ready );
		save->ready = ready;
	}

	if( VIPS_OBJECT_CLASS( vips_foreign_save_parent_class )->
		build( object ) )
		return( -1 );

	return( 0 );
}
