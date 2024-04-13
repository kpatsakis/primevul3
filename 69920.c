vips_foreign_load_gif_build_cmap( VipsForeignLoadGif *gif )
{
	ColorMapObject *map = gif->file->Image.ColorMap ?
		gif->file->Image.ColorMap : gif->file->SColorMap;

	int v;

	for( v = 0; v < 256; v++ ) {
		VipsPel *q = (VipsPel *) &gif->cmap[v];

		if( map &&
			v < map->ColorCount ) {
			q[0] = map->Colors[v].Red;
			q[1] = map->Colors[v].Green;
			q[2] = map->Colors[v].Blue;
			q[3] = 255;
		}
		else {
			/* If there's no map, just save the index.
			 */
			q[0] = v;
			q[1] = v;
			q[2] = v;
			q[3] = 255;
		}
	}
}
