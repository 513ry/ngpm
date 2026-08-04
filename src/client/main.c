/** main.c - Client entry point
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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

static void capacity(void);
static void status(void);
static void list(void);
static void usage(void);
static void version(void);

typedef enum {
  OPT_CAPACITY,
  OPT_STATUS,
  OPT_LIST,
  OPT_USAGE,
  OPT_VERSION,
  OPT_COUNT
} option_id;

typedef void (*action_fn)(void);

static const action_fn actions[OPT_COUNT] = {
  [OPT_CAPACITY] = capacity,
  [OPT_STATUS]   = status,
  [OPT_LIST]     = list,
  [OPT_USAGE]    = usage,
  [OPT_VERSION]  = version
};

typedef unsigned int option_set;

#define OPT_BIT(opt) (1u << (opt))

/* Print error and usage and return errno */
void raise(int errno, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  fprintf(stderr, "Error: ");
  vfprintf(stderr, fmt, ap);
  fputc('\n', stderr);

  usage();

  va_end(ap);

  exit(errno);
}

int
main(int argc, char **argv)
{
  if (argc < 2) {
    usage();
    return 0;
  }

  /* parse options */

  option_set opts = 0;

  for (int i = 1; i < argc; ++i) {
    char *arg = argv[i];
    if (arg[0] != '-')
      raise(1, "option shall start with <hyphen-minus> character followed by a single alpha-numerical character");

    if (arg[1] == '\0')
      raise(1, "invalid option '-'");

    if (strcmp(arg, "--") == 0) {
      ++i;
      /* ngpm client does not have operands but POSIX.1‐2017, Section 12.2
         expects -- to end options */
      break;
    }

    for (char *p = &arg[1]; *p; ++p)
      switch (*p) {
      case 'c':
        opts |= OPT_BIT(OPT_CAPACITY);
        break;
      case 's':
        opts |= OPT_BIT(OPT_STATUS);
        break;
      case 'l':
        opts |= OPT_BIT(OPT_LIST);
        break;
      case 'h':
        opts |= OPT_BIT(OPT_USAGE);
        break;
      case 'v':
        opts |= OPT_BIT(OPT_VERSION);
        break;
      default:
        raise(1, "unknown option -%c", *p);
      }
  }

  /* check for collisions */

  unsigned meta = 
    opts & (OPT_BIT(OPT_LIST) |
            OPT_BIT(OPT_USAGE) |
            OPT_BIT(OPT_VERSION));

  int meta_count = __builtin_popcount(meta);

  if (meta_count > 1)
    raise(1, "options -h, -v and -l are mutually exclusive");

  if (meta_count && opts & (OPT_BIT(OPT_CAPACITY) |
                            OPT_BIT(OPT_STATUS)))
    raise(1, "meta-information options cannot be combined with runtime queries");

  /* execute command */

  for (int i = 0; i < OPT_COUNT; ++i)
    if (opts & OPT_BIT(i))
      actions[i]();

  return 0;
}

static void
capacity(void)
{
  printf("%i\n", battery_capacity());
}

static void
status(void)
{
  int status = battery_status();
  switch (status) {
  case 0:
    puts("Discharging");
    break;
  case 1:
    puts("Charging");
    break;
  default:
    raise(status, "could not read status file");
  }
}

static void
list(void)
{
  printf("BATTERY: %s\nTHRESHOLD: %s\nCRIT_THRESHOLD: %s\nDELAY_IN_SEC: %s\n",
         BATTERY_STR, THRESHOLD_STR, CRIT_THRESHOLD_STR, DELAY_IN_SEC_STR);
}

static void
usage(void)
{
  puts("Usage: ngpm [-c] [-s]\n"                             \
       "       ngpm [-l]\n"                                  \
       "       ngpm [-h]\n"                                  \
       "       ngpm [-v]\n\n"                                \
       "Options:\n"                                          \
       "  -c      Print battery capacity\n"                  \
       "  -s      Print battery status\n"                    \
       "  -l      List compiled constants and exit\n"        \
       "  -h      Print this help screen and exit\n"         \
       "  -v      Print version and exit");
}

static void
version(void)
{
  printf("ngpm v. %s, %s\n", VERSION_STR, COPYRIGHT_STR);
}
