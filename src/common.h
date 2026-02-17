/** common.c -
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#ifndef _COMMON_H
#define _COMMON_H

# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)

#ifndef DESTDIR
# define DESTDIR ""
#endif
# define DEST TOSTRING(DESTDIR)

#endif // _COMMON_H
