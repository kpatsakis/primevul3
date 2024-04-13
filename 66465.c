static void trif_dump(FILE * trace, char *data, u32 data_size)
{
	GF_BitStream *bs;
	u32 id, independent, filter_disabled;
	Bool full_picture, has_dep, tile_group;

	if (!data) {
		fprintf(trace, "<TileRegionGroupEntry ID=\"\" tileGroup=\"\" independent=\"\" full_picture=\"\" filter_disabled=\"\" x=\"\" y=\"\" w=\"\" h=\"\">\n");
		fprintf(trace, "<TileRegionDependency tileID=\"\"/>\n");
		fprintf(trace, "</TileRegionGroupEntry>\n");
		return;
	}

	bs = gf_bs_new(data, data_size, GF_BITSTREAM_READ);
	id = gf_bs_read_u16(bs);
	tile_group = gf_bs_read_int(bs, 1);
	fprintf(trace, "<TileRegionGroupEntry ID=\"%d\" tileGroup=\"%d\" ", id, tile_group);
	if (tile_group) {
		independent = gf_bs_read_int(bs, 2);
		full_picture = (Bool)gf_bs_read_int(bs, 1);
		filter_disabled = gf_bs_read_int(bs, 1);
		has_dep = gf_bs_read_int(bs, 1);
		gf_bs_read_int(bs, 2);
		fprintf(trace, "independent=\"%d\" full_picture=\"%d\" filter_disabled=\"%d\" ", independent, full_picture, filter_disabled);

		if (!full_picture) {
			fprintf(trace, "x=\"%d\" y=\"%d\" ", gf_bs_read_u16(bs), gf_bs_read_u16(bs));
		}
		fprintf(trace, "w=\"%d\" h=\"%d\" ", gf_bs_read_u16(bs), gf_bs_read_u16(bs));
		if (!has_dep) {
			fprintf(trace, "/>\n");
		} else {
			u32 count = gf_bs_read_u16(bs);
			fprintf(trace, ">\n");
			while (count) {
				count--;
				fprintf(trace, "<TileRegionDependency tileID=\"%d\"/>\n", gf_bs_read_u16(bs) );
			}
			fprintf(trace, "</TileRegionGroupEntry>\n");
		}
	}
	gf_bs_del(bs);
}
