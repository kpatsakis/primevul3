evdns_result_is_answer(int result)
{
	return (result != DNS_ERR_NOTIMPL && result != DNS_ERR_REFUSED &&
	    result != DNS_ERR_SERVERFAILED && result != DNS_ERR_CANCEL);
}
