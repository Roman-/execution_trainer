#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>
#include <vector>
#include <map>
#include "mylabel.h"
#include "generator.h"
#include "my_chains.h"
#include "tables_loader.h"
#include "globals.h"

using std::vector;
using std::map;

namespace Ui {
class MainWindow;
}

enum element {
    el_x, el_t, el_w, el_e, el_c
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void save_settings();

private:
    // ui-specific
    vector<MyLabel*> labels;
    QLabel* algInfoLabel;
    QLabel* chainInfoLabel;
    int current_label;
    int current_font_size;
    Ui::MainWindow *ui;

    // BLD-specific
    Probs* p;
    vector<QString> letters_perm;
    map<QString,map<QString, QString> > images;
    map<QString,map<QString, QString> > algs;
    bool current_length; // true = long (20 for centers), false = short (12)
    element current_element;

    void resizeEvent(QResizeEvent* event);
    void closeEvent (QCloseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void clear_labels(); // remove from form and clear vector
    void create_labels(); // delete previous labels and create new
    void allocate_labels(); // make cool-looking labels grid
    void create_new_chain();
    void showAlgInfo();
    void set_length(bool l);
    void set_element(element el);
    void commit_and_save_probs(); // applies changes to probs, saves to file and clears screen
    void update_chainInfoLabel(); // based on current event and length
    void load_settings();

    // navigation
    int index_of_label_below_current();
    int index_of_label_above_current();
    int index_of_endline_label();
    int index_of_startline_label();
};

#endif // MAINWINDOW_H
