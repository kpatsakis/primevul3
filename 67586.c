void tmx_pretran_unlink_safe(int slotid)
{
	if(_tmx_proc_ptran==NULL)
		return;
	if(_tmx_proc_ptran->linked == 0)
		return;
	if(_tmx_ptran_table[slotid].plist==NULL) {
		_tmx_proc_ptran->prev = _tmx_proc_ptran->next = NULL;
		_tmx_proc_ptran->linked = 0;
		return;
	}
	if(_tmx_proc_ptran->prev==NULL) {
		_tmx_ptran_table[slotid].plist = _tmx_proc_ptran->next;
		if(_tmx_ptran_table[slotid].plist!=NULL)
			_tmx_ptran_table[slotid].plist->prev = NULL;
	} else {
		_tmx_proc_ptran->prev->next = _tmx_proc_ptran->next;
		if(_tmx_proc_ptran->next)
			_tmx_proc_ptran->next->prev = _tmx_proc_ptran->prev;
	}
	_tmx_proc_ptran->prev = _tmx_proc_ptran->next = NULL;
	_tmx_proc_ptran->linked = 0;
	return;
}
