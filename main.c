#include "kalkulator.h"

#include <gtk/gtk.h>
#include <string.h>

extern wielomian oblicz(const char *tekst,wielomian w2);

GtkWidget* wynik;
GtkWidget* reszta;

//TODO find why program works in debug but not in release

G_MODULE_EXPORT void oblicz_wyrazenie( GtkWidget *widget,GtkWidget *text)
{
    gchar wejscie[100+2];
    strcpy(wejscie, gtk_entry_get_text(GTK_ENTRY(text)));
    wielomian w1=NULL;
    wielomian w2=g_malloc(sizeof(Wielomian));
    w2->size=0;
    w1=oblicz(wejscie,w2);
    //TODO fix memory leaks: error -1073740940 (0xC0000374) == heap_corruption
    if(w1!=NULL){
        gtk_entry_set_text(GTK_ENTRY(wynik),(gchar*)print(w1));
    }
    else{
        gtk_entry_set_text(GTK_ENTRY(wynik),"0");
    }
    if(w2->size!=0){
        gtk_entry_set_text(GTK_ENTRY(reszta),(gchar*)print(w2));
    }
    else{
        gtk_entry_set_text(GTK_ENTRY(reszta),"0");
    }
    gtk_entry_set_text(GTK_ENTRY(text),"");
}

G_MODULE_EXPORT void test_nacisniecia(GtkWidget *widget){
    g_print("GTK-test - %s zastal nacisniety\n", (gchar*)gtk_button_get_label((GtkButton *) widget));
}



G_MODULE_EXPORT void dodaj_do_text(GtkWidget *widget, gpointer data) {
    gint tmp_pos = gtk_entry_get_text_length(GTK_ENTRY(data));
    gtk_editable_insert_text(GTK_EDITABLE(data), gtk_button_get_label(GTK_BUTTON(widget)), -1, &tmp_pos);
    if(strcmp(gtk_button_get_label(GTK_BUTTON(widget)),"compute")==0||strcmp(gtk_button_get_label(GTK_BUTTON(widget)),"GCD")==0){
        tmp_pos = gtk_entry_get_text_length(GTK_ENTRY(data));
        gtk_editable_insert_text(GTK_EDITABLE(data), "(", -1, &tmp_pos);
    }
}
static void make_window(){
    GtkBuilder* builder=gtk_builder_new_from_file("Builder.glade");
    int my_user_data = 0xDEADBEEF;  //podpięcie sygnałów
    gtk_builder_connect_signals(builder, &my_user_data);
    GObject* window=gtk_builder_get_object(builder,"window");
    GObject* wyjscie1=gtk_builder_get_object(builder,"Wyjscie1");
    wynik=GTK_WIDGET(wyjscie1);
    GObject* wyjscie2=gtk_builder_get_object(builder,"Wyjscie2");
    reszta=GTK_WIDGET(wyjscie2);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(GTK_WIDGET(window));
}



void msg (gchar *message){
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_MODAL;

    dialog = gtk_dialog_new_with_buttons ("Błąd",NULL,flags,"_OK",GTK_RESPONSE_NONE,NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new (message);

    g_signal_connect_swapped (dialog,"response",G_CALLBACK (gtk_widget_destroy),dialog);

    gtk_container_add (GTK_CONTAINER (content_area), label);
    gtk_widget_show_all (dialog);
}


int main( int argc,char *argv[] ){

    gtk_init(&argc, &argv);
    make_window();
    gtk_main();
    return 0;
}

