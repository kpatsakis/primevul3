void switch_bench(u32 is_on)
{
	bench_mode = is_on;
	display_rti = is_on ? 2 : 0;
	ResetCaption();
	gf_term_set_option(term, GF_OPT_VIDEO_BENCH, is_on);
}
