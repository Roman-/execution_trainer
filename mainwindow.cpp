#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generator.h"
#include "my_chains.h"
#include "tables_loader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   ui->label->setFixedWidth(ui->centralWidget->width() - 20);
   ui->label->setFixedHeight(ui->centralWidget->height() - 10);
   ui->pushButton->setGeometry(0, ui->centralWidget->height() - 30, 70, 30);
   ui->spinBox->setGeometry(70, ui->centralWidget->height() - 30, 50, 30);
}

void MainWindow::on_pushButton_clicked()
{
    auto images = load_images("files/images.txt");
    auto x_centers_speffz = load_speffz("files/x_centers_speffz.txt");
    auto t_centers_speffz = load_speffz("files/t_centers_speffz.txt");
    auto corners_speffz = load_speffz("files/corners_speffz.txt");
    auto edges_speffz = load_speffz("files/edges_speffz.txt");
    auto wings_letters = load_letters("files/t_centers_speffz.txt");

    auto letters_perm_x = generate_initial_chain_type1(x_centers_speffz, ui->spinBox->value());
    auto letters_perm_t = generate_initial_chain_type1(t_centers_speffz, ui->spinBox->value());
    auto letters_perm_corners = generate_chain_by_shuffling(my_corners_chain(), corners_speffz);
    auto letters_perm_wings = generate_initial_chain_wings(wings_letters, ui->spinBox->value());
    auto letters_perm_edges = generate_chain_by_shuffling(my_edges_chain(), edges_speffz);

    QString s =
            "c: " + vector_to_qstring(letters_perm_corners) + "\n" + "\n" +
            "e: " + vector_to_words(letters_perm_edges, images) + "\n" + "\n" +
            "x: " + vector_to_words(letters_perm_x, images) + "\n" + "\n" +
            "t: " + vector_to_words(letters_perm_t, images) + "\n" + "\n" +
            "w: " + vector_to_words(letters_perm_wings, images) + "\n" + "\n";
    ui->label->setText(s);
}
