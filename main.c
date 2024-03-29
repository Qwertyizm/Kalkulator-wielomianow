#include "wielomiany.h"
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

extern wielomian oblicz(const char *tekst, wielomian w2);

typedef struct {
    GtkWidget *wejscie;
    GtkWidget *wyjscie;
    GtkWidget *reszta;
} paczka;

void msg(gchar *message) {
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_MODAL;

    dialog = gtk_dialog_new_with_buttons("Błąd", NULL, flags, "_OK", GTK_RESPONSE_NONE, NULL);
    content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
    label = gtk_label_new(message);

    g_signal_connect_swapped (dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_container_add(GTK_CONTAINER (content_area), label);
    gtk_widget_show_all(dialog);
}

G_MODULE_EXPORT void oblicz_wyrazenie(GtkWidget *widget, paczka *data) {
    gchar wejscie[100 + 2];
    strcpy(wejscie, gtk_entry_get_text(GTK_ENTRY(data->wejscie)));
    wielomian w1 = NULL;
    wielomian w2 = g_malloc(sizeof(Wielomian));
    w2->size = 0;
    w2->val=NULL;
    w1 = oblicz(wejscie, w2);

    if (w1 != NULL) {
        gchar *temp = print(w1, true);
        gtk_entry_set_text(GTK_ENTRY(data->wyjscie), temp);
        free(temp);
    } else {
        gtk_entry_set_text(GTK_ENTRY(data->wyjscie), "0");
    }
    if (w2 != NULL && (w2->size != 0)) {
        gchar *temp = print(w2, true);
        gtk_entry_set_text(GTK_ENTRY(data->reszta), temp);
        free(temp);
    } else {
        del(w2);
        gtk_entry_set_text(GTK_ENTRY(data->reszta), "0");
    }
    if(widget!=NULL){
        int i=0;
        while(i>0){
            printf("checkout");
            i++;
        }
    }
}

G_MODULE_EXPORT void test_nacisniecia(GtkWidget *widget) {
    g_print("GTK-test - %s zostal nacisniety\n", (gchar *) gtk_button_get_label((GtkButton *) widget));
}


G_MODULE_EXPORT void dodaj_do_text(GtkWidget *widget, gpointer data) {
    gint tmp_pos = gtk_entry_get_text_length(GTK_ENTRY(data));
    gtk_editable_insert_text(GTK_EDITABLE(data), gtk_button_get_label(GTK_BUTTON(widget)), -1, &tmp_pos);
    if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "compute") == 0 ||
        strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "GCD") == 0 ||
        strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "m_zero") == 0 ||
        strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "derivative") == 0) {
        tmp_pos = gtk_entry_get_text_length(GTK_ENTRY(data));
        gtk_editable_insert_text(GTK_EDITABLE(data), "(", -1, &tmp_pos);
    }
}

G_MODULE_EXPORT void clear_text(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(data, "");
    if(widget!=NULL){
        int i=0;
        while(i>0){
            printf("checkout");
            i++;
        }
    }
}



static void make_window() {
    GtkBuilder *builder = gtk_builder_new_from_file("Builder.glade");
    gtk_builder_connect_signals(builder, NULL);
    GObject *window = gtk_builder_get_object(builder, "window");
    GtkWidget *wejscie = GTK_WIDGET(gtk_builder_get_object(builder, "Wejscie"));
    GtkWidget *wynik;
    GtkWidget *reszta;
    GObject *wyjscie1 = gtk_builder_get_object(builder, "Wyjscie1");
    GObject *wyjscie2 = gtk_builder_get_object(builder, "Wyjscie2");
    wynik = GTK_WIDGET(wyjscie1);
    reszta = GTK_WIDGET(wyjscie2);
    paczka *okienka = malloc(sizeof(paczka));
    okienka->wejscie = wejscie;
    okienka->wyjscie = wynik;
    okienka->reszta = reszta;
    g_signal_connect(GTK_BUTTON(gtk_builder_get_object(builder, "Enter")), "clicked", G_CALLBACK(oblicz_wyrazenie),
                     okienka);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
}





int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);
    make_window();
    gtk_main();

    return 0;
}

