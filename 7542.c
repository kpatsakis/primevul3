static u32 ccp_gen_jobid(struct ccp_device *ccp)
{
	return atomic_inc_return(&ccp->current_id) & CCP_JOBID_MASK;
}