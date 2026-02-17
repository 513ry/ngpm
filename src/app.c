/** app.c - Initialize application and Bootstrap glib main loop
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#include <glib-2.0/glib.h>
#include <glib-2.0/glib/gmain.h>
#include <glib-2.0/glib/gtypes.h>
#include <glib-2.0/glib-unix.h>
#include <stdio.h>
#include <ngpm.h>

#include "app.h"
#include "common.h"
#include "suspend.h"

#ifdef WITH_GTK
# include "dialog.h"
#else
# include "cli.h"
#endif

// user_data type
typedef struct {
  gint threshold;
  gint crit_threshold;
  gint last_status;
} BatteryData;

static gboolean check_battery_capacity(gpointer user_data) {
  gint status;
  gint capacity;
  BatteryData *args = (BatteryData *)user_data;

  switch (status = battery_status()) {
  case -1:
    g_error("Status file can't be read");
  case -2:
    g_error("Status parsing error");
  case 0:
    // 1. If power supply is not connected do:
    switch (capacity = battery_capacity()) {
    case -1:
      g_error("Capacity file can't be read");
    case -2:
      g_error("Capacity parsing error");
    }

    // 1. If capacity <= `CRIT_THRESHOLD` system suspension.
    if (capacity <= args->crit_threshold) {
      g_warning("Capacity is bellow critical threshold");
      suspend();
    }
    // 2. If state changed do:
    if (status == args->last_status)
      return G_SOURCE_CONTINUE;

    // 1. If capacity <= `THRESHOLD` display GTK 3 dialog or print message to stderr.
    if (capacity <= args->threshold)
      new_dialog(capacity);
  }

  args->last_status = status;
  return G_SOURCE_CONTINUE;
}

static gboolean quit_callback(gpointer user_data) {
  GMainLoop *loop = user_data;
  g_main_loop_unref(loop);
  g_main_loop_quit(loop);
  return G_SOURCE_REMOVE;
}

void run_battery_pool(unsigned int seconds, int threshold, int crit_threshold) {
  GMainLoop *loop = g_main_loop_new(NULL, 0);

  BatteryData *args = g_new(BatteryData, 1);
  args->threshold = threshold;
  args->crit_threshold = crit_threshold;
  args->last_status = -3;
  g_unix_signal_add(SIGTERM, quit_callback, loop);

#ifdef WITH_GTK
  init_dialog();
#endif

  g_timeout_add_seconds_full(
    G_PRIORITY_DEFAULT, seconds, check_battery_capacity, (gpointer)args, g_free
  );

  g_main_loop_run(loop);
}
