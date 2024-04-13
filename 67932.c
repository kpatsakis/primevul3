vips_foreign_load_invalidate( VipsImage *image )
{
	VipsOperation *operation; 

#ifdef DEBUG
	printf( "vips_foreign_load_invalidate: %p\n", image ); 
#endif /*DEBUG*/

	if( (operation = g_object_get_qdata( G_OBJECT( image ), 
		vips__foreign_load_operation )) ) {
		vips_operation_invalidate( operation ); 
	}
}
