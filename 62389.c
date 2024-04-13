void lha_file_header_add_ref(LHAFileHeader *header)
{
	++header->_refcount;
}
