static void printval(void *value, int type, int level, struct printparam* pp){
	struct node pn, cn;
	struct property *p;
	int i;

	pn.iteration = NULL;
	pn.parent = NULL;
	pn.type = type;
	pn.value =  value;

	printstr(pp, "<item>");
	for(p = datatypes[type].properties; p; ) {
		cn.iteration = NULL;
		cn.parent = &pn;
		cn.type = p->type;
		cn.value =  (*p->e_f)(&pn);
		if(cn.value){
			for(i = 0; i < level; i++) printstr(pp, "\t");
			if(strcmp(p->name, "next")){
				printstr(pp, "<parameter>");
				printstr(pp, "<name>");
				printstr(pp, p->name);
				printstr(pp, "</name>");
				printstr(pp, "<type>");
				printstr(pp, datatypes[p->type].type);
				printstr(pp, "</type>");
				printstr(pp, "<description>");
				printstr(pp, p->description);
				printstr(pp, "</description>");
			}
			if(datatypes[p->type].p_f){
				printstr(pp, "<value><![CDATA[");
				(*datatypes[p->type].p_f)(&cn, stdcbf, pp);
				printstr(pp, "]]></value>\n");
				printstr(pp, "</parameter>");
			}
			else {
				if(!strcmp(p->name, "next")){
/*					printstr(pp, "<!-- -------------------- -->\n"); */
					printstr(pp, "</item>\n<item>");
					p = datatypes[type].properties;
					pn.value = value = cn.value;
					continue;
				}
				else {
					printstr(pp, "\n");
					printval(cn.value, cn.type, level+1, pp);
					printstr(pp, "</parameter>");
				}
			}
		}
		p=p->next;
	}
	printstr(pp, "</item>");
}
