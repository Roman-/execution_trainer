#include "mainwindow.h"
#include "generator.h"
#include "tables_loader.h"
#include "probs.h"
#include <QApplication>
#include <ctime>
#include <QDebug>

void test_make_chain_using_probs() {
    auto x_centers_table = load_algs_table("files/x_centers_algs.txt");
    Probs p(x_centers_table);
    // making some algs hard
    p("П", "Ш") *= 320;
    p("Ш", "Д") *= 319;
    p("Ш", "Е") *= 319;
    p("Ш", "Ж") *= 319;
    p("Ш", "З") *= 319;
    p.normalize();
    // making chain
    //p.save_to_file("files/x_probs");
    //Probs p2("files/x_probs");
    //p2.output();
//    auto images = load_images("files/images.txt");
//    auto x_centers_speffz = load_speffz("files/x_centers_speffz.txt");
//    auto letters_perm_x = generate_initial_chain_type1(x_centers_speffz, 12, p);
//    qDebug() << vector_to_words(letters_perm_x, images);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(0));
    //MainWindow w;
    //w.show();
    //for (int i = 0; i < 20; ++i)
        test_make_chain_using_probs();
    //qDebug() << "This thing is being used for test";






    return a.exec();
}
