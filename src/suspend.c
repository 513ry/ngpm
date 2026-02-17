/** suspend.c -
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#include "suspend.h"
#include <stdlib.h>

#ifdef SYSD
void suspend(void)
{
  system("systemctl suspend");
}
#else
void suspend(void)
{
  system("zzz");
}
#endif
