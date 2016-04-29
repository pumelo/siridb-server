/*
 * time.c - Time- and time precision functions and constants.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 09-03-2016
 *
 */
#include <siri/db/time.h>
#include <xmath/xmath.h>
#include <assert.h>

const char * siridb_time_short_map[] = {"s", "ms", "us", "ns"};

siridb_time_t * siridb_new_time(siridb_timep_t precision)
{
    siridb_time_t * time = (siridb_time_t *) malloc(sizeof(siridb_time_t));
    time->precision = precision;
    time->factor = ipow(1000, precision);
    time->ts_sz = (precision == SIRIDB_TIME_SECONDS) ?
            sizeof(uint32_t) : sizeof(uint64_t);

    return time;
}

uint64_t siridb_time_parse(const char * str, size_t len)
{
    uint64_t ts = atoll(str);
    switch (str[len - 1])
    {
    case 's':
        return ts;
    case 'm':
        return ts * 60;
    case 'h':
        return ts * 3600;
    case 'd':
        return ts * 86400;
    case 'w':
        return ts * 604800;
    }

    /* we should never get at this point */
    assert (0);

    return 0;
}

int siridb_int64_valid_ts(siridb_t * siridb, int64_t ts)
{
    if (siridb->time->precision == SIRIDB_TIME_SECONDS)
        return ts >= 0 && ts < 4294967296;
    return ts >= 0;
}

uint32_t siridb_time_in_seconds(siridb_t * siridb, int64_t ts)
{
    return ts / siridb->time->factor;
}

uint64_t siridb_time_now(siridb_t * siridb, struct timespec now)
{
    return now.tv_sec * siridb->time->factor +
        now.tv_nsec * (siridb->time->factor / 1000000000);
}