bson_iter_find (bson_iter_t *iter, /* INOUT */
                const char *key)   /* IN */
{
   BSON_ASSERT (iter);
   BSON_ASSERT (key);

   return bson_iter_find_w_len (iter, key, -1);
}
