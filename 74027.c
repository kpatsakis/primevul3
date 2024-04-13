static void copy_to_array(const char *key, void *data, void *void_rock)
{
    uint32_t *attributes = (uint32_t *)data;
    struct list_rock_recursivematch *rock =
        (struct list_rock_recursivematch *)void_rock;
    assert(rock->count > 0);
    rock->array[--rock->count].name = key;
    rock->array[rock->count].attributes = *attributes;
}
