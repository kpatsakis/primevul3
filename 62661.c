rs_filter_class_init(RSFilterClass *klass)
{
	RS_DEBUG(FILTERS, "rs_filter_class_init(%p)", klass);
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	signals[CHANGED_SIGNAL] = g_signal_new ("changed",
		G_TYPE_FROM_CLASS (klass),
		G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		0,
		NULL, 
		NULL,                
		g_cclosure_marshal_VOID__INT,
		G_TYPE_NONE, 1, G_TYPE_INT);

	klass->get_image = NULL;
	klass->get_image8 = NULL;
	klass->get_size = NULL;
	klass->previous_changed = NULL;

	object_class->dispose = dispose;
}
