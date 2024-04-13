void X86_get_insn_id(cs_struct *h, cs_insn *insn, unsigned int id)
{
	int i = insn_find(insns, ARR_SIZE(insns), id, &h->insn_cache);
	if (i != 0) {
		insn->id = insns[i].mapid;

		if (h->detail) {
#ifndef CAPSTONE_DIET
			memcpy(insn->detail->regs_read, insns[i].regs_use, sizeof(insns[i].regs_use));
			insn->detail->regs_read_count = (uint8_t)count_positive(insns[i].regs_use);

			switch(id) {
				default:
					memcpy(insn->detail->regs_write, insns[i].regs_mod, sizeof(insns[i].regs_mod));
					insn->detail->regs_write_count = (uint8_t)count_positive(insns[i].regs_mod);
					break;
				case X86_RDTSC:
					if (h->mode == CS_MODE_64) {
						memcpy(insn->detail->regs_write, insns[i].regs_mod, sizeof(insns[i].regs_mod));
						insn->detail->regs_write_count = (uint8_t)count_positive(insns[i].regs_mod);
					} else {
						insn->detail->regs_write[0] = X86_REG_EAX;
						insn->detail->regs_write[1] = X86_REG_EDX;
						insn->detail->regs_write_count = 2;
					}
					break;
				case X86_RDTSCP:
					if (h->mode == CS_MODE_64) {
						memcpy(insn->detail->regs_write, insns[i].regs_mod, sizeof(insns[i].regs_mod));
						insn->detail->regs_write_count = (uint8_t)count_positive(insns[i].regs_mod);
					} else {
						insn->detail->regs_write[0] = X86_REG_EAX;
						insn->detail->regs_write[1] = X86_REG_ECX;
						insn->detail->regs_write[2] = X86_REG_EDX;
						insn->detail->regs_write_count = 3;
					}
					break;
			}

			switch(insn->id) {
				default:
					break;

				case X86_INS_LOOP:
				case X86_INS_LOOPE:
				case X86_INS_LOOPNE:
					switch(h->mode) {
						default: break;
						case CS_MODE_16:
								 insn->detail->regs_read[0] = X86_REG_CX;
								 insn->detail->regs_read_count = 1;
								 insn->detail->regs_write[0] = X86_REG_CX;
								 insn->detail->regs_write_count = 1;
								 break;
						case CS_MODE_32:
								 insn->detail->regs_read[0] = X86_REG_ECX;
								 insn->detail->regs_read_count = 1;
								 insn->detail->regs_write[0] = X86_REG_ECX;
								 insn->detail->regs_write_count = 1;
								 break;
						case CS_MODE_64:
								 insn->detail->regs_read[0] = X86_REG_RCX;
								 insn->detail->regs_read_count = 1;
								 insn->detail->regs_write[0] = X86_REG_RCX;
								 insn->detail->regs_write_count = 1;
								 break;
					}

					if (insn->id != X86_INS_LOOP) {
						insn->detail->regs_read[1] = X86_REG_EFLAGS;
						insn->detail->regs_read_count = 2;
					}

					break;

				case X86_INS_LODSB:
				case X86_INS_LODSD:
				case X86_INS_LODSQ:
				case X86_INS_LODSW:
					switch(h->mode) {
						default:
							break;
						case CS_MODE_16:
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_ESI, X86_REG_SI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_ESI, X86_REG_SI);
							break;
						case CS_MODE_64:
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_ESI, X86_REG_RSI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_ESI, X86_REG_RSI);
							break;
					}
					break;

				case X86_INS_SCASB:
				case X86_INS_SCASW:
				case X86_INS_SCASQ:
				case X86_INS_STOSB:
				case X86_INS_STOSD:
				case X86_INS_STOSQ:
				case X86_INS_STOSW:
					switch(h->mode) {
						default:
							break;
						case CS_MODE_16:
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_EDI, X86_REG_DI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_EDI, X86_REG_DI);
							break;
						case CS_MODE_64:
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_EDI, X86_REG_RDI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_EDI, X86_REG_RDI);
							break;
					}
					break;

				case X86_INS_CMPSB:
				case X86_INS_CMPSD:
				case X86_INS_CMPSQ:
				case X86_INS_CMPSW:
				case X86_INS_MOVSB:
				case X86_INS_MOVSW:
				case X86_INS_MOVSD:
				case X86_INS_MOVSQ:
					switch(h->mode) {
						default:
							break;
						case CS_MODE_16:
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_EDI, X86_REG_DI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_EDI, X86_REG_DI);
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_ESI, X86_REG_SI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_ESI, X86_REG_SI);
							break;
						case CS_MODE_64:
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_EDI, X86_REG_RDI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_EDI, X86_REG_RDI);
							arr_replace(insn->detail->regs_read, insn->detail->regs_read_count, X86_REG_ESI, X86_REG_RSI);
							arr_replace(insn->detail->regs_write, insn->detail->regs_write_count, X86_REG_ESI, X86_REG_RSI);
							break;
					}
					break;
			}

			memcpy(insn->detail->groups, insns[i].groups, sizeof(insns[i].groups));
			insn->detail->groups_count = (uint8_t)count_positive8(insns[i].groups);

			if (insns[i].branch || insns[i].indirect_branch) {
				insn->detail->groups[insn->detail->groups_count] = X86_GRP_JUMP;
				insn->detail->groups_count++;
			}

			switch (insns[i].id) {
				case X86_OUT8ir:
				case X86_OUT16ir:
				case X86_OUT32ir:
					if (insn->detail->x86.operands[0].imm == -78) {
						insn->detail->groups[insn->detail->groups_count] = X86_GRP_INT;
						insn->detail->groups_count++;
					}
					break;

				default:
					break;
			}
#endif
		}
	}
}
