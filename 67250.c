static void computeOverlayOffset(ut64 offset, ut64 size, ut64 file_size, ut64* largest_offset, ut64* largest_size) {
	if (offset + size <= file_size && offset + size > (*largest_offset + *largest_size)) {
		*largest_offset = offset;
		*largest_size = size;
	}
}
