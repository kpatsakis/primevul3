static pdf_t *init_pdf(FILE *fp, const char *name)
{
    pdf_t *pdf;

    pdf = pdf_new(name);
    pdf_get_version(fp, pdf);
    if (pdf_load_xrefs(fp, pdf) == -1) {
      pdf_delete(pdf);
      return NULL;
    }
    pdf_load_pages_kids(fp, pdf);

    return pdf;
 }
