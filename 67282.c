int git_delta_index_init(
	git_delta_index **out, const void *buf, size_t bufsize)
{
	unsigned int i, hsize, hmask, entries, prev_val, *hash_count;
	const unsigned char *data, *buffer = buf;
	struct git_delta_index *index;
	struct index_entry *entry, **hash;
	void *mem;
	unsigned long memsize;

	*out = NULL;

	if (!buf || !bufsize)
		return 0;

	/* Determine index hash size.  Note that indexing skips the
	   first byte to allow for optimizing the rabin polynomial
	   initialization in create_delta(). */
	entries = (unsigned int)(bufsize - 1) / RABIN_WINDOW;
	if (bufsize >= 0xffffffffUL) {
		/*
		 * Current delta format can't encode offsets into
		 * reference buffer with more than 32 bits.
		 */
		entries = 0xfffffffeU / RABIN_WINDOW;
	}
	hsize = entries / 4;
	for (i = 4; i < 31 && (1u << i) < hsize; i++);
	hsize = 1 << i;
	hmask = hsize - 1;

	if (lookup_index_alloc(&mem, &memsize, entries, hsize) < 0)
		return -1;

	index = mem;
	mem = index->hash;
	hash = mem;
	mem = hash + hsize;
	entry = mem;

	index->memsize = memsize;
	index->src_buf = buf;
	index->src_size = bufsize;
	index->hash_mask = hmask;
	memset(hash, 0, hsize * sizeof(*hash));

	/* allocate an array to count hash entries */
	hash_count = git__calloc(hsize, sizeof(*hash_count));
	if (!hash_count) {
		git__free(index);
		return -1;
	}

	/* then populate the index */
	prev_val = ~0;
	for (data = buffer + entries * RABIN_WINDOW - RABIN_WINDOW;
	     data >= buffer;
	     data -= RABIN_WINDOW) {
		unsigned int val = 0;
		for (i = 1; i <= RABIN_WINDOW; i++)
			val = ((val << 8) | data[i]) ^ T[val >> RABIN_SHIFT];
		if (val == prev_val) {
			/* keep the lowest of consecutive identical blocks */
			entry[-1].ptr = data + RABIN_WINDOW;
		} else {
			prev_val = val;
			i = val & hmask;
			entry->ptr = data + RABIN_WINDOW;
			entry->val = val;
			entry->next = hash[i];
			hash[i] = entry++;
			hash_count[i]++;
		}
	}

	/*
	 * Determine a limit on the number of entries in the same hash
	 * bucket.  This guard us against patological data sets causing
	 * really bad hash distribution with most entries in the same hash
	 * bucket that would bring us to O(m*n) computing costs (m and n
	 * corresponding to reference and target buffer sizes).
	 *
	 * Make sure none of the hash buckets has more entries than
	 * we're willing to test.  Otherwise we cull the entry list
	 * uniformly to still preserve a good repartition across
	 * the reference buffer.
	 */
	for (i = 0; i < hsize; i++) {
		if (hash_count[i] < HASH_LIMIT)
			continue;

		entry = hash[i];
		do {
			struct index_entry *keep = entry;
			int skip = hash_count[i] / HASH_LIMIT / 2;
			do {
				entry = entry->next;
			} while(--skip && entry);
			keep->next = entry;
		} while (entry);
	}
	git__free(hash_count);

	*out = index;
	return 0;
}
