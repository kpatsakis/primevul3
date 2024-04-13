static void free_cv_nb10_header(SCV_NB10_HEADER* cv_nb10_header) {
	R_FREE (cv_nb10_header->file_name);
}
