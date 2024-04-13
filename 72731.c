static Bool node_in_commands_subtree(GF_Node *node, GF_List *commands)
{
#ifndef GPAC_DISABLE_VRML
	u32 i, j, count, nb_fields;

	count = gf_list_count(commands);
	for (i=0; i<count; i++) {
		GF_Command *com = gf_list_get(commands, i);
		if (com->tag>=GF_SG_LAST_BIFS_COMMAND) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_SCENE, ("[Scene Manager] Command check for LASeR/DIMS not supported\n"));
			return 0;
		}
		if (com->tag==GF_SG_SCENE_REPLACE) {
			if (gf_node_parent_of(com->node, node)) return 1;
			continue;
		}
		nb_fields = gf_list_count(com->command_fields);
		for (j=0; j<nb_fields; j++) {
			GF_CommandField *field = gf_list_get(com->command_fields, j);
			switch (field->fieldType) {
			case GF_SG_VRML_SFNODE:
				if (field->new_node) {
					if (gf_node_parent_of(field->new_node, node)) return 1;
				}
				break;
			case GF_SG_VRML_MFNODE:
				if (field->field_ptr) {
					GF_ChildNodeItem *child;
					child = field->node_list;
					while (child) {
						if (gf_node_parent_of(child->node, node)) return 1;
						child = child->next;
					}
				}
				break;
			}
		}
	}
#endif
	return 0;
}
