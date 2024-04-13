static int compact_input_order(void) {
int i;

	dbg_printf("\nCompacting element input order: %u elements\n", cache.input_count);
	for ( i=0; i< cache.input_count; i++ ) {
		dbg_printf("%i: type: %u, length: %u\n", 
			i, cache.input_order[i].type, cache.input_order[i].length);
		
		if ( (cache.input_order[i].type == SKIP_ELEMENT) && (cache.input_order[i].length == DYN_FIELD_LENGTH) ) {
			dbg_printf("Dynamic length field: %u\n", cache.input_order[i].type);
			continue;
		}
		while ( (i+1) < cache.input_count &&
			(cache.input_order[i].type == SKIP_ELEMENT) && (cache.input_order[i].length != DYN_FIELD_LENGTH) &&
			(cache.input_order[i+1].type == SKIP_ELEMENT) && (cache.input_order[i+1].length != DYN_FIELD_LENGTH)) {
				int j;
				dbg_printf("%i: type: %u, length: %u\n", 
					i+1, cache.input_order[i+1].type, cache.input_order[i+1].length);
				dbg_printf("Merge order %u and %u\n", i, i+1);
				cache.input_order[i].type = SKIP_ELEMENT;
				cache.input_order[i].length += cache.input_order[i+1].length;
		
				for ( j=i+1; (j+1)<cache.input_count; j++) {
					cache.input_order[j] = cache.input_order[j+1];
				}
				cache.input_count--;
		}
	}

#ifdef DEVEL
	printf("\nCompacted input order table: count: %u\n", cache.input_count);
	for ( i=0; i< cache.input_count; i++ ) {
		dbg_printf("%i: Type: %u, Length: %u\n", 
			i, cache.input_order[i].type, cache.input_order[i].length);
	}
	printf("\n");
#endif

	for ( i=0; i< cache.input_count; i++ ) {
		if ( cache.input_order[i].type != SKIP_ELEMENT ) 
			return 1;
	}
	return 0;

} // End of compact_input_order
