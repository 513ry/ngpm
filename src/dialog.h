/** dialog.h
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#ifndef _DIALOG_H
#define _DIALOG_H
# ifdef WITH_GTK

typedef int gint;

void init_dialog(void);
void new_dialog(gint capacity);

# endif
#endif // _DIALOG_H
