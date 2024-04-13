bson_iter_key_len (const bson_iter_t *iter)
{
   /*
    * f i e l d n a m e \0 _
    * ^                    ^
    * |                    |
    * iter->key            iter->d1
    *
    */
   BSON_ASSERT (iter->d1 > iter->key);
   return iter->d1 - iter->key - 1;
}
