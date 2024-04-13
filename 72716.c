static int open_file(AVFormatContext **	ic_ptr, const char * 	filename, AVInputFormat * 	fmt, void *ops) {
#ifdef USE_PRE_0_7
	return av_open_input_file(ic_ptr, filename, fmt, 0, NULL);
#else
	return avformat_open_input(ic_ptr, filename, fmt, (AVDictionary**)ops);
#endif
}
