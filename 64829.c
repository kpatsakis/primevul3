test_bson_append_regex_w_len (void)
{
   bson_t *b;
   bson_t *b2;

   b = bson_new ();
   BSON_ASSERT (bson_append_regex_w_len (b, "regex", -1, "^abcd", 5, "ilx"));
   b2 = get_bson ("test27.bson");
   BSON_ASSERT_BSON_EQUAL (b, b2);
   bson_destroy (b);
   bson_destroy (b2);

   b = bson_new ();
   BSON_ASSERT (bson_append_regex_w_len (b, "regex", -1, "^abcd", -1, "ilx"));
   b2 = get_bson ("test27.bson");
   BSON_ASSERT_BSON_EQUAL (b, b2);
   bson_destroy (b);
   bson_destroy (b2);

   b = bson_new ();
   BSON_ASSERT (
      bson_append_regex_w_len (b, "regex", -1, "^abcd    ", 5, "ilx"));
   b2 = get_bson ("test27.bson");
   BSON_ASSERT_BSON_EQUAL (b, b2);
   bson_destroy (b);
   bson_destroy (b2);
}
