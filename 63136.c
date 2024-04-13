void edge_sparse_csr_reader_float( const char*           i_csr_file_in,
                                   unsigned int**        o_row_idx,
                                   unsigned int**        o_column_idx,
                                   float**               o_values,
                                   unsigned int*         o_row_count,
                                   unsigned int*         o_column_count,
                                   unsigned int*         o_element_count ) {
  double* l_values;
  unsigned int i;

  /* read using double */
  edge_sparse_csr_reader_double( i_csr_file_in, o_row_idx, o_column_idx, &l_values,
                                   o_row_count, o_column_count, o_element_count );

  /* converting double values into float */
  *o_values = (float*) malloc((*o_element_count)*sizeof(float));
  for ( i = 0; i < (*o_element_count); ++i ) {
    (*o_values)[i] = (float)l_values[i];
  }

  free(l_values);
}
