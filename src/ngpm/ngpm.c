/** ngpm.c
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#include "../include/ngpm.h"
#include "../common.h"

#include <stdio.h>
#include <string.h>

#ifdef __linux__
# if !defined (BATTERY) || (BATTERY + 0) < 0
#  warning "BATTERY defaults to 0"
#  define BATTERY_STR "0"
# else
#  define BATTERY_STR TOSTRING(BATTERY)
# endif
# define BAT_PATH "/sys/class/power_supply/BAT" BATTERY_STR
# define STATUS_BUFFER 16

int
battery_status(void)
{
  char buffer[STATUS_BUFFER];
  FILE *status_file = fopen(BAT_PATH "/status", "r");

  if (!status_file)
    return -1;

  if (!fgets(buffer, STATUS_BUFFER, status_file)) {
    fclose(status_file);
    return -2;
  }

  fclose(status_file);

  if (strncmp(buffer, "Dis", 3) == 0)
    return 0;

  return 1;
}

int
battery_capacity(void)
{
  int capacity;
  FILE *capacity_file = fopen(BAT_PATH "/capacity", "r");

  if (capacity_file == NULL)
    return -1;

  if (fscanf(capacity_file, "%d", &capacity) != 1) {
    fclose(capacity_file);
    return -2;
  }

  fclose(capacity_file);
  return capacity;
}
#elif defined(__FreeBSD__)
# include <sys/types.h>
# include <sys/sysctl.h>

int
battery_status(void)
{
  int state;
  size_t len = sizeof(state);

  // FreeBSD provides handy virtual battery source
  if (sysctlbyname("hw.acpi.battery.state", &state, &len, NULL, 0) == -1)
    return -1;

  return (state & 0x4) ? 1 : 0;
}

int
battery_capacity(void)
{
  int life = -1;
  size_t len = sizeof(life);

  sysctlbyname("hw.acpi.battery.life", &life, &len, NULL, 0);

  return life;
}
#elif defined(__NetBSD__)
# include <sys/types.h>
# include <sys/sysctl.h>
# if !defined (BATTERY) || (BATTERY + 0) < 0
#  warning "BATTERY defaults to 0"
#  define BATTERY_STR "0"
# else
#  define BATTERY_STR TOSTRING(BATTERY)
# endif
# define BATTERY_PATH "hw.acpi.battery" BATTERY_STR
# define STATUS_BUFFER 16

int
battery_status(void)
{
  char buffer[STATUS_BUFFER];
  size_t size = STATUS_BUFFER;

  if (sysctlbyname(BATTERY_PATH ".state", buffer, &size, NULL, 0) == -1)
    return -1;

  if (strncmp(buffer, "dis", 3) == 0)
    return 0;

  return 1;
}

int
battery_capacity(void)
{
  int charge = -1;
  size_t len = sizeof(charge);

  sysctlbyname(BATTERY_PATH ".charge", &charge, &len, NULL, 0);

  return charge;
}
#else
# error Only __FreeBSD__, __NetBSD__, and __linux__ targets are supported
#endif
