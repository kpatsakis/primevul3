static mz_bool tdefl_compress_block(tdefl_compressor *d, mz_bool static_block) {
  if (static_block)
    tdefl_start_static_block(d);
  else
    tdefl_start_dynamic_block(d);
  return tdefl_compress_lz_codes(d);
}