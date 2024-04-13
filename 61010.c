void t2p_tile_collapse_left(
	tdata_t buffer, 
	tsize_t scanwidth, 
	uint32 tilewidth, 
	uint32 edgetilewidth, 
	uint32 tilelength){
	
	uint32 i;
	tsize_t edgescanwidth=0;
	
	edgescanwidth = (scanwidth * edgetilewidth + (tilewidth - 1))/ tilewidth;
	for(i=0;i<tilelength;i++){
                /* We use memmove() since there can be overlaps in src and dst buffers for the first items */
		memmove( 
			&(((char*)buffer)[edgescanwidth*i]), 
			&(((char*)buffer)[scanwidth*i]), 
			edgescanwidth);
	}
	
	return;
}
