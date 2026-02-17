/** main.c - Daemon's main() procedure
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#include "app.h"

#if !defined(DELAY_IN_SEC) || (DELAY_IN_SEC + 0) < 0
# warning "DELAY_IN_SEC defaults to 180"
# define DELAY_IN_SEC 180
#endif
#if !defined(THRESHOLD) || (THRESHOLD + 0) < 0
# warning "THRESHOLD defaults to 20"
# define THRESHOLD 20
#endif
#if !defined(CRIT_THRESHOLD) || (CRIT_THRESHOLD + 0) < 0
# warning "CRIT_THRESHOLD defaults to 5"
# define CRIT_THRESHOLD 5
#endif

int
main(void)
{
  run_battery_pool(DELAY_IN_SEC, THRESHOLD, CRIT_THRESHOLD);
  return 0;
}
