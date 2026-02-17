/** cli.c
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#ifndef WITH_GTK
# include "cli.h"

# include <stdio.h>

void new_dialog(gint capacity)
{
  fprintf(stderr, "Battery capacity low: %i%%\n", capacity);
}
#endif
