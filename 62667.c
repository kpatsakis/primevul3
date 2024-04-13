rs_filter_init(RSFilter *self)
{
	RS_DEBUG(FILTERS, "rs_filter_init(%p)", self);
	self->previous = NULL;
	self->next_filters = NULL;
	self->enabled = TRUE;
}
