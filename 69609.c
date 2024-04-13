static size_t order_to_size(unsigned int order)
{
	return (size_t)PAGE_SIZE << order;
}
