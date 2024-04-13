static void update_datarate(DataRateData *drd, int64_t count)
{
    if (!drd->time1 && !drd->count1) {
        drd->time1 = drd->time2 = cur_time;
        drd->count1 = drd->count2 = count;
    } else if (cur_time - drd->time2 > 5000) {
        drd->time1 = drd->time2;
        drd->count1 = drd->count2;
        drd->time2 = cur_time;
        drd->count2 = count;
    }
}
