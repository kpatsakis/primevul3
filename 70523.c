static int reorder_sequencer(input_translation_t *table) {
int i, n;
sequence_map_t *sequence = table->sequence;

#ifdef DEVEL
	printf("\nReorder Sequencer. Sequence steps: %u\n", table->number_of_sequences);
	for ( i=0; i<table->number_of_sequences; i++ ) {
		printf("Order: %i, Sequence: %u, Type: %u, Input length: %u, Output offset: %u, Skip Count: %u\n",
			i, sequence[i].id, sequence[i].type, sequence[i].input_length, 
			sequence[i].output_offset, sequence[i].skip_count);
	}
#endif

	n = 0;	// index into sequencer table
	for ( i=0; i<cache.input_count; i++ ) {
		if ( cache.input_order[i].type == SKIP_ELEMENT ) {
			if ( cache.input_order[i].length == DYN_FIELD_LENGTH || n == 0 ) {
				int j;

				if ( !CheckSequenceMap(table) )
					return 0;

				for ( j=table->number_of_sequences-1; j>=n; j-- ) {
					sequence[j+1] = sequence[j];
				}

				if ( cache.input_order[i].length == DYN_FIELD_LENGTH ) {
					sequence[n].id = dyn_skip;
					sequence[n].skip_count = 0;
				} else {
					sequence[n].id = nop;
					sequence[n].skip_count = cache.input_order[i].length;
				}
				sequence[n].type = SKIP_ELEMENT;
				sequence[n].input_length = 0;
				sequence[n].stack = NULL;
				table->number_of_sequences++;
				dbg_printf("Insert skip sequence in slot: %u, skip count: %u, dyn: %u\n", 
					n, sequence[n].skip_count, cache.input_order[i].length == DYN_FIELD_LENGTH ? 1 : 0);
			} else {
				sequence[n-1].skip_count += cache.input_order[i].length;
				dbg_printf("Merge skip count: %u into previous sequence: %u\n", cache.input_order[i].length, n-1);
				continue;
			}
		} else {
			if ( sequence[n].type != cache.input_order[i].type ) {
				sequence_map_t _s;
				int j = n+1;

				while ( sequence[j].type != cache.input_order[i].type && j < table->number_of_sequences )
					j++;

				if ( j == table->number_of_sequences ) {
					if ( n == 0 ) {
						return 0;
					} else {
						sequence[n-1].skip_count += cache.input_order[i].length;
						dbg_printf("Merge skip count: %u into previous sequence: %u\n", cache.input_order[i].length, n-1);
						continue;
					}
					return 0;
				}
				_s = sequence[n];
				sequence[n] = sequence[j];
				sequence[j] = _s;
				dbg_printf("Swap slots %u <-> %u\n", n, j);
			} else {
				dbg_printf("In order slot %u\n", n);
			}
		}
		n++;
	}

#ifdef DEVEL
	printf("\nReordered Sequencer. Sequence steps: %u\n", table->number_of_sequences);
	for ( int i=0; i<table->number_of_sequences; i++ ) {
		printf("Order: %i, Sequence: %u, Type: %u, Input length: %u, Output offset: %u, Skip Count: %u\n",
			i, sequence[i].id, sequence[i].type, sequence[i].input_length, 
			sequence[i].output_offset, sequence[i].skip_count);
	}
	printf("\n");
#endif

	return 1;

} // End of reorder_sequencer
