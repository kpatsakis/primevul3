test_bson_new (void)
{
   bson_t *b;

   b = bson_new ();
   ASSERT_CMPUINT32 (b->len, ==, (uint32_t) 5);
   bson_destroy (b);

   b = bson_sized_new (32);
   ASSERT_CMPUINT32 (b->len, ==, (uint32_t) 5);
   bson_destroy (b);
}
