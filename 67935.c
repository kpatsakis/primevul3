vips_foreign_load_operation_get_flags( VipsOperation *operation )
{
	VipsForeignLoad *load = VIPS_FOREIGN_LOAD( operation );

	VipsOperationFlags flags;

	flags = VIPS_OPERATION_CLASS( vips_foreign_load_parent_class )->
		get_flags( operation );
	if( load->nocache )
		flags |= VIPS_OPERATION_NOCACHE;

	return( flags );
}
