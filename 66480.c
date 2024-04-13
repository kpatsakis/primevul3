static void tx3g_print_char_offsets(FILE * trace, u32 start, u32 end, u32 *shift_offset, u32 so_count)
{
	u32 i;
	if (shift_offset) {
		for (i=0; i<so_count; i++) {
			if (start>shift_offset[i]) {
				start --;
				break;
			}
		}
		for (i=0; i<so_count; i++) {
			if (end>shift_offset[i]) {
				end --;
				break;
			}
		}
	}
	if (start || end) fprintf(trace, "fromChar=\"%d\" toChar=\"%d\" ", start, end);
}
