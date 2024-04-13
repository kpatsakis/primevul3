OPJ_BOOL opj_alloc_tile_component_data(opj_tcd_tilecomp_t *l_tilec)
{
	if ((l_tilec->data == 00) || ((l_tilec->data_size_needed > l_tilec->data_size) && (l_tilec->ownsData == OPJ_FALSE))) {
		l_tilec->data = (OPJ_INT32 *) opj_malloc(l_tilec->data_size_needed);
		if (! l_tilec->data ) {
			return OPJ_FALSE;
		}
		/*fprintf(stderr, "tAllocate data of tilec (int): %d x OPJ_UINT32n",l_data_size);*/
		l_tilec->data_size = l_tilec->data_size_needed;
		l_tilec->ownsData = OPJ_TRUE;
	}
	else if (l_tilec->data_size_needed > l_tilec->data_size) {
		OPJ_INT32 * new_data = (OPJ_INT32 *) opj_realloc(l_tilec->data, l_tilec->data_size_needed);
		/* opj_event_msg(p_manager, EVT_ERROR, "Not enough memory to handle tile datan"); */
		/* fprintf(stderr, "Not enough memory to handle tile data"); */
		if (! new_data) {
			opj_free(l_tilec->data);
			l_tilec->data = NULL;
			l_tilec->data_size = 0;
			l_tilec->data_size_needed = 0;
			l_tilec->ownsData = OPJ_FALSE;
			return OPJ_FALSE;
		}
		l_tilec->data = new_data;
		/*fprintf(stderr, "tReallocate data of tilec (int): from %d to %d x OPJ_UINT32n", l_tilec->data_size, l_data_size);*/
		l_tilec->data_size = l_tilec->data_size_needed;
		l_tilec->ownsData = OPJ_TRUE;
	}
	return OPJ_TRUE;
}
