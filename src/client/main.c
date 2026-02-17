/** main.c - Client's main() procedure
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#include <stdio.h>
#include <ngpm.h>

#include "../common.h"

#ifndef BATTERY
# define BATTERY_STR "Undefined"
#else
# define BATTERY_STR TOSTRING(BATTERY)
#endif
#ifndef THRESHOLD
# define THRESHOLD_STR "Undefined"
#else
# define THRESHOLD_STR TOSTRING(THRESHOLD)
#endif
#ifndef CRIT_THRESHOLD
# define CRIT_THRESHOLD_STR "Undefined"
#else
# define CRIT_THRESHOLD_STR TOSTRING(CRIT_THRESHOLD)
#endif
#ifndef DELAY_IN_SEC
# define DELAY_IN_SEC_STR "Undefined"
#else
# define DELAY_IN_SEC_STR TOSTRING(DELAY_IN_SEC)
#endif
#ifndef VERSION
# define VERSION_STR "Undefined"
#else
# define VERSION_STR TOSTRING(VERSION)
#endif
#ifndef COPYRIGHT
# define COPYRIGHT_STR "Undefined"
#else
# define COPYRIGHT_STR TOSTRING(COPYRIGHT)
#endif

static void usage(void);
static void version(void);

int
main(int argc, char **argv)
{
  if (argc < 2) {
    usage();
    return 0;
  }

  if (argv[1][0] == '-')
    switch (argv[1][1]) {
    case 'c':
      printf("%i", battery_capacity());
      break;
    case 's':
      switch (battery_status()) {
      case -1:
      case -2:
        fprintf(stderr, "Could not read status file\n");
        return -1;
      case 0:
        printf("Discharging");
        break;
      case 1:
	printf("Charging");
	break;
      default:
	printf("Unknown");
      }
      break;
    case 'l':
      printf("BATTERY: %s\nTHRESHOLD: %s\nCRIT_THRESHOLD: %s\nDELAY_IN_SEC: %s\n",
	     BATTERY_STR, THRESHOLD_STR, CRIT_THRESHOLD_STR, DELAY_IN_SEC_STR);
      break;
    case 'h':
      usage();
      break;
    case 'v':
      version();
      break;
    default:
      fprintf(stderr, "Unknown option -%c\n", argv[1][1]);
    }

  return 0;
}

static void
usage(void)
{
  puts("Usage: ngpm [[-c] [-s] [-l] [-h] [-v]]\n\n"	\
       "Options:\n"					\
       "-c      Print battery capacity\n"		\
       "-s      Print battery status\n"			\
       "-l      List tunable variables\n"		\
       "-h      Print this help screen\n"		\
       "-v      Print version");
}

static void
version(void)
{
  printf("ngpm v. %s, %s\n",VERSION_STR, COPYRIGHT_STR);
}
