/** dialog.c - Display a non-blocking dialog
 * Copyright (C) 2026 Daniel Sierpiński and contributors
 *
 * This software is licensed under ISC License.
 * Read LICENSE file for details.
 */

#ifdef WITH_GTK
#include <gtk/gtk.h>
#include <stdio.h>

#include "dialog.h"
#include "common.h"
#include "suspend.h"

#ifndef ETC
# define _ETC "/etc"
#else
# define _ETC TOSTRING(ETC)
#endif

static void cancel(GtkDialog *dialog) { gtk_window_close(GTK_WINDOW(dialog)); }

/* Run appropriate callback on button response */
static void on_response(GtkDialog *dialog, gint response_id) {
  if (response_id == 1)
    cancel(dialog);
  else if (response_id == 2)
    suspend();

  gtk_widget_destroy(GTK_WIDGET(dialog));
}

static _Bool is_file(const char *path) {
  FILE *file = fopen(path, "r");

  if (file) {
    fclose(file);
    return (_Bool)1;
  }
  return (_Bool)0;
}

void init_dialog() {
  gtk_init(NULL, NULL);
}

void
new_dialog(gint capacity)
{
  GtkWidget *dialog;
  GtkWidget *cancel_button, *suspend_button;
  GtkCssProvider *provider;
  GtkStyleContext *body_context;
  GtkStyleContext *cancel_button_context, *suspend_button_context;
  GtkDialogFlags flags;
  char *style_path;

  flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_message_dialog_new(NULL, flags, GTK_MESSAGE_WARNING,
				  GTK_BUTTONS_NONE,
				  "Battery capacity is low (%d%%)", capacity);
  gtk_window_set_title(GTK_WINDOW(dialog), "NGPM Warning");

  // Set style provider
  provider = gtk_css_provider_new();
  style_path = DEST _ETC "/ngpm/style.css";
  if (is_file(style_path))
    gtk_css_provider_load_from_path(provider, style_path, NULL);
  else
    fprintf(stderr, "No stylesheet file found: %s\n", style_path);
  gtk_style_context_add_provider_for_screen(
    gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),
    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
  );

  // Set buttons
  cancel_button = gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", 1);
  suspend_button = gtk_dialog_add_button(GTK_DIALOG(dialog), "Suspend", 2);
  g_signal_connect(dialog, "response", G_CALLBACK(on_response), NULL);

  // Expose css context
  body_context = gtk_widget_get_style_context(dialog);
  cancel_button_context = gtk_widget_get_style_context(cancel_button);
  suspend_button_context = gtk_widget_get_style_context(suspend_button);
  gtk_style_context_add_class(body_context, "warning-dialog");
  gtk_style_context_add_class(cancel_button_context, "cancel-button");
  gtk_style_context_add_class(cancel_button_context, "button");
  gtk_style_context_add_class(suspend_button_context, "suspend-button");
  gtk_style_context_add_class(suspend_button_context, "button");

  // Run and destroy in memory
  gtk_widget_show_all(dialog);
}
#endif
