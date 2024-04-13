static void init_cv_nb10_header(SCV_NB10_HEADER* cv_nb10_header) {
	memset (cv_nb10_header, 0, sizeof (SCV_NB10_HEADER));
	cv_nb10_header->free = (void (*)(struct SCV_NB10_HEADER*))free_cv_nb10_header;
}
