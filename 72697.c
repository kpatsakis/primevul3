void sax_node_start(void *sax_cbck, const char *node_name, const char *name_space, const GF_XMLAttribute *attributes, u32 nb_attributes)
{
	char szCheck[100];
	GF_List *imports = sax_cbck;
	GF_XMLAttribute *att;
	u32 i=0;

	/*do not process hyperlinks*/
	if (!strcmp(node_name, "a") || !strcmp(node_name, "Anchor")) return;

	for (i=0; i<nb_attributes; i++) {
		att = (GF_XMLAttribute *) &attributes[i];
		if (stricmp(att->name, "xlink:href") && stricmp(att->name, "url")) continue;
		if (att->value[0]=='#') continue;
		if (!strnicmp(att->value, "od:", 3)) continue;
		sprintf(szCheck, "%d", atoi(att->value));
		if (!strcmp(szCheck, att->value)) continue;
		gf_list_add(imports, gf_strdup(att->value) );
	}
}
