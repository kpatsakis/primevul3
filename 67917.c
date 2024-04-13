vips_foreign_class_init( VipsForeignClass *class )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( class );
	VipsObjectClass *object_class = (VipsObjectClass *) class;

	gobject_class->set_property = vips_object_set_property;
	gobject_class->get_property = vips_object_get_property;

	object_class->nickname = "foreign";
	object_class->description = _( "load and save image files" );
	object_class->summary_class = vips_foreign_summary_class;
}
