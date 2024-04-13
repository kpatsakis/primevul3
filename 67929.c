vips_foreign_load_dispose( GObject *gobject )
{
	VipsForeignLoad *load = VIPS_FOREIGN_LOAD( gobject );

	VIPS_UNREF( load->real );

	G_OBJECT_CLASS( vips_foreign_load_parent_class )->dispose( gobject );
}
