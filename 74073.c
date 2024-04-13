partition_list_free(struct partition_list *current)
{
    while (current) {
        struct partition_list *next = current->next;

        free(current->name);
        free(current);

        current = next;
    }
}
