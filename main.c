/*
 * main.c - SiriDB.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <logger/logger.h>
#include <siri/args/args.h>
#include <siri/cfg/cfg.h>
#include <siri/siri.h>

#ifdef DEBUG
#include <test/test.h>
#endif

int main(int argc, char * argv[])
{
    int rc;

    #ifdef DEBUG
    printf( "*******************************\n"
            "*     Start DEBUG Release     *\n"
            "*******************************\n");
    #endif

    /*
     * set local to LC_ALL
     * more info at: http://www.cprogramming.com/tutorial/unicode.html
     */
    setlocale(LC_ALL, "");

    /* initialize random */
    srand(time(NULL));

    /* set default timezone to UTC */
    putenv("TZ=:UTC");
    tzset();

    /* parse arguments first since this can exit the program */
    siri_args_parse(argc, argv);

    /* setup logger, this must be done before logging the first line */
    siri_setup_logger();

    #ifdef DEBUG
    /* run tests when we are in debug mode */
    rc = run_tests(0);
    if (rc)
        exit(1);
    #endif

    /* read siridb main application configuration */
    siri_cfg_init();

    /* start SiriDB. (this start the event loop etc.) */
    rc = siri_start();

    /* free siridb */
    siri_free();

    log_info("Bye!\n");

    return rc;
}