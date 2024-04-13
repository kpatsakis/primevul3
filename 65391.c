void nntp_hash_destructor(int type, void *obj, intptr_t data)
{
  nntp_data_free(obj);
}
