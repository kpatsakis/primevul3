vips_foreign_load_gif_render_line( VipsForeignLoadGif *gif,
	int width, VipsPel * restrict q, VipsPel * restrict p )
{
	guint32 *iq;
	int x;

	iq = (guint32 *) q;
	for( x = 0; x < width; x++ ) {
		VipsPel v = p[x];
		
		if( v == gif->transparency ) {
			/* In DISPOSE_DO_NOT mode, the previous frame shows
			 * through (ie. we do nothing). In all other modes, 
			 * it's just transparent.
			 */
			if( gif->dispose != DISPOSE_DO_NOT ) 
				iq[x] = 0;
		}
		else
			/* Blast in the RGBA for this value.
			 */
			iq[x] = gif->cmap[v];
	}
}
