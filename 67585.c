int tmx_init_pretran_table(void)
{
	int n;
	int pn;

	pn = get_max_procs();

	if(pn<=0)
		return -1;
	if(_tmx_ptran_table!=NULL)
		return -1;
	/* get the highest power of two less than number of processes */
	n = -1;
	while (pn >> ++n > 0);
	n--;
	if(n<=1) n = 2;
	if(n>8) n = 8;
	_tmx_ptran_size = 1<<n;

	_tmx_ptran_table = (pretran_slot_t*)shm_malloc(_tmx_ptran_size*sizeof(pretran_slot_t));
	if(_tmx_ptran_table == NULL) {
		LM_ERR("not enough shared memory\n");
		return -1;
	}
	memset(_tmx_ptran_table, 0, _tmx_ptran_size*sizeof(pretran_slot_t));
	for(n=0; n<_tmx_ptran_size; n++) {
		if(lock_init(&_tmx_ptran_table[n].lock)==NULL)
		{
			LM_ERR("cannot init the lock %d\n", n);
			n--;
			while(n>=0) {
				lock_destroy(&_tmx_ptran_table[n].lock);
				n--;
			}
			shm_free(_tmx_ptran_table);
			_tmx_ptran_table = 0;
			_tmx_ptran_size = 0;
			return -1;
		}
	}
	return 0;
}
