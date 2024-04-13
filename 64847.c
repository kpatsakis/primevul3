test_bson_empty_binary (void)
{
   uint8_t data = 0xAB;
   bson_t test;
   const bson_value_t *value;
   bson_value_t copy;
   bson_iter_t iter;

   bson_init (&test);
   bson_append_binary (&test, "test", 4, BSON_SUBTYPE_BINARY, &data, 0);
   BSON_ASSERT (bson_iter_init_find (&iter, &test, "test"));
   value = bson_iter_value (&iter);
   /* CDRIVER-2569, this would memcpy (0 bytes) to a NULL destination. */
   bson_value_copy (value, &copy);

   bson_value_destroy (&copy);
   bson_destroy (&test);
}
