void pdf_delete(pdf_t *pdf)
{
    int i;

    for (i=0; i<pdf->n_xrefs; i++)
    {
        free(pdf->xrefs[i].creator);
        free(pdf->xrefs[i].entries);
        free(pdf->xrefs[i].kids);
    }

    free(pdf->name);
    free(pdf->xrefs);
    free(pdf);
}
