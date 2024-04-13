OPJ_BOOL opj_pi_check_next_level(	OPJ_INT32 pos,
								opj_cp_t *cp,
								OPJ_UINT32 tileno,
								OPJ_UINT32 pino,
								const OPJ_CHAR *prog)
{
	OPJ_INT32 i;
	opj_tcp_t *tcps =&cp->tcps[tileno];
	opj_poc_t *tcp = &tcps->pocs[pino];

	if(pos>=0){
		for(i=pos;pos>=0;i--){
			switch(prog[i]){
		    case 'R':
			    if(tcp->res_t==tcp->resE){
				    if(opj_pi_check_next_level(pos-1,cp,tileno,pino,prog)){
					    return OPJ_TRUE;
				    }else{
					    return OPJ_FALSE;
				    }
			    }else{
				    return OPJ_TRUE;
			    }
			    break;
		    case 'C':
			    if(tcp->comp_t==tcp->compE){
				    if(opj_pi_check_next_level(pos-1,cp,tileno,pino,prog)){
					    return OPJ_TRUE;
				    }else{
					    return OPJ_FALSE;
				    }
			    }else{
				    return OPJ_TRUE;
			    }
			    break;
		    case 'L':
			    if(tcp->lay_t==tcp->layE){
				    if(opj_pi_check_next_level(pos-1,cp,tileno,pino,prog)){
					    return OPJ_TRUE;
				    }else{
					    return OPJ_FALSE;
				    }
			    }else{
				    return OPJ_TRUE;
			    }
			    break;
		    case 'P':
			    switch(tcp->prg){
                    case OPJ_LRCP: /* fall through */
                    case OPJ_RLCP:
					    if(tcp->prc_t == tcp->prcE){
						    if(opj_pi_check_next_level(i-1,cp,tileno,pino,prog)){
							    return OPJ_TRUE;
						    }else{
							    return OPJ_FALSE;
						    }
					    }else{
						    return OPJ_TRUE;
					    }
					    break;
			    default:
				    if(tcp->tx0_t == tcp->txE){
					    /*TY*/
					    if(tcp->ty0_t == tcp->tyE){
						    if(opj_pi_check_next_level(i-1,cp,tileno,pino,prog)){
							    return OPJ_TRUE;
						    }else{
							    return OPJ_FALSE;
						    }
					    }else{
						    return OPJ_TRUE;
					    }/*TY*/
				    }else{
					    return OPJ_TRUE;
				    }
				    break;
			    }/*end case P*/
		    }/*end switch*/
		}/*end for*/
	}/*end if*/
	return OPJ_FALSE;
}
