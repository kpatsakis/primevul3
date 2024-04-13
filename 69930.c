vips_foreign_load_gif_generate( VipsRegion *or, 
	void *seq, void *a, void *b, gboolean *stop )
{
        VipsRect *r = &or->valid;
	VipsForeignLoadGif *gif = (VipsForeignLoadGif *) a;

	int y;

	for( y = 0; y < r->height; y++ ) {
		/* The page for this output line, and the line number in page.
		 */
		int page = (r->top + y) / gif->file->SHeight + gif->page;
		int line = (r->top + y) % gif->file->SHeight;

		VipsPel *p, *q;
		int x;

		g_assert( line >= 0 && line < gif->frame->Ysize );
		g_assert( page >= 0 && page < gif->n_pages );

		/* current_page == 0 means we've not loaded any pages yet. So 
		 * we need to have loaded the page beyond the page we want.
		 */
		while( gif->current_page <= page ) {
			if( vips_foreign_load_gif_next_page( gif ) )
				return( -1 );

			gif->current_page += 1; 
		}

		/* @frame is always RGBA, but or may be G, GA, RGB or RGBA. 
		 * We have to pick out the values we want.
		 */
		p = VIPS_IMAGE_ADDR( gif->frame, 0, line );
		q = VIPS_REGION_ADDR( or, 0, r->top + y );
		switch( or->im->Bands ) {
		case 1:
			for( x = 0; x < gif->frame->Xsize; x++ ) {
				q[0] = p[1];

				q += 1;
				p += 4;
			}
			break;

		case 2:
			for( x = 0; x < gif->frame->Xsize; x++ ) {
				q[0] = p[1];
				q[1] = p[3];

				q += 2;
				p += 4;
			}
			break;

		case 3:
			for( x = 0; x < gif->frame->Xsize; x++ ) {
				q[0] = p[0];
				q[1] = p[1];
				q[2] = p[2];

				q += 3;
				p += 4;
			}
			break;

		case 4:
			memcpy( q, p, VIPS_IMAGE_SIZEOF_LINE( gif->frame ) );
			break;

		default:
			g_assert_not_reached();
			break;
		}
	}

	return( 0 );
}
