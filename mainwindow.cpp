#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p = nullptr;
    load_settings();

    chainInfoLabel = new QLabel("...", ui->centralWidget);
    chainInfoLabel->setStyleSheet("QLabel { background-color : #161616; color : #228888; }");
    chainInfoLabel->setFont(QFont(kChainInfoLabelFontFamily, current_font_size * 0.7f));
    algInfoLabel = new QLabel("hint", ui->centralWidget);
    algInfoLabel->setStyleSheet("QLabel { background-color : #080808; color : #4477aa; }");
    algInfoLabel->setFont(QFont(kAlgInfoLabelFontFamily, current_font_size * 0.8f));

    set_element(current_element);

    images = load_images(kDirImages);


    allocate_labels();
    update_chainInfoLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   allocate_labels();
}

int MainWindow::index_of_label_below_current() {
    int c_y = labels[current_label]->y();
    uint i = current_label;
    while (i < labels.size() && labels[i]->y() == c_y)
        ++i;
    if (i == labels.size())
        return labels.size() - 1; // last row
    //new row found
    while (labels[current_label]->x() > labels[i]->x() && i < labels.size() - 1)
        ++i;
    return i;
}

int MainWindow::index_of_label_above_current() {
    int c_y = labels[current_label]->y();
    int i = current_label;
    while (i > 0 && labels[i]->y() == c_y)
        --i;
    if (i < 0)
        return 0; // last row
    //new row found
    while (labels[current_label]->x() < labels[i]->x() && i > 0)
        --i;
    return i;
}

void MainWindow::clear_labels() {
    //clear
    for (uint i = 0; i < labels.size(); ++i) {
        delete labels[i];//->~MyLabel();
    }
    labels.clear();
}

void MainWindow::create_labels() {
    for (uint i = 0; i < letters_perm.size(); i += 2) {
        MyLabel* label = new MyLabel(images[letters_perm[i]][letters_perm[i+1]], ui->centralWidget, current_font_size);
        labels.push_back(label);
        layout()->addWidget(label);
    }
    labels[0]->underline();
    current_label = 0;
    allocate_labels();
}

int MainWindow::index_of_endline_label() {
    int c_y = labels[current_label]->y();
    uint i = current_label;
    while (i < labels.size() && labels[i]->y() == c_y)
        ++i;
    return i - 1;
}

int MainWindow::index_of_startline_label() {
    int c_y = labels[current_label]->y();
    int i = current_label;
    while (i >= 0 && labels[i]->y() == c_y)
        --i;
    return i + 1;
}

void MainWindow::allocate_labels() {
    //top label with chain info
    chainInfoLabel->adjustSize();
    chainInfoLabel->setGeometry(2, 2, width() - 4, chainInfoLabel->height() + 1);
    //labels with words
    QPoint topleft(10, chainInfoLabel->height() + 10); // top-left corner where grd starts
    const int v_space = 7, h_space = current_font_size * 0.8; // vertical and gorisontal indentations
    int l = topleft.x(), t = topleft.y();
    for (uint i = 0; i < labels.size(); i++) {
        l = (i == 0) ? topleft.x() : labels[i-1]->pos().x() +  labels[i-1]->width() + h_space;
        if (l + labels[i]->width() + v_space > width()) {
            t += labels[i]->height() + v_space;
            l = topleft.x();
        }
        labels[i]->setPos(l, t);
    }
    if (labels.size() != 0)
        labels[current_label]->underline();
    //hint label
    int layoutEnd = (labels.size() == 0) ?
                chainInfoLabel->y() + chainInfoLabel->width() + v_space :
                labels[labels.size() - 1]->y() + labels[labels.size() - 1]->height() + v_space;
    algInfoLabel->setGeometry(10, layoutEnd, width() - 20, height() - layoutEnd);
}

void MainWindow::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Space) {
        commit_and_save_probs();
        clear_labels();
        create_new_chain();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_S) { // save
        if (labels.size() == 0)
            return;
        commit_and_save_probs();
        clear_labels();
        algInfoLabel->setText("saved");
    }
    if (event->key() == Qt::Key_L) { // length
        commit_and_save_probs();
        clear_labels();
        algInfoLabel->setText("length changed to " + QString(current_length ? "short" : "long"));
        set_length(!current_length);
        update_chainInfoLabel();
    }
    if (event->key() == Qt::Key_Right) {
        if (labels.size() == 0)
            return;
        labels[current_label]->deunderline();
        current_label++;
        if (current_label == labels.size())
            current_label = 0;
        labels[current_label]->underline();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_Left) {
        if (labels.size() == 0)
            return;
        labels[current_label]->deunderline();
        current_label--;
        if (current_label < 0)
            current_label = labels.size() - 1;
        labels[current_label]->underline();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_Return) {
        if (labels.size() == 0)
            return;
        labels[current_label]->change_state();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_Down) {
        if (labels.size() == 0)
            return;
        labels[current_label]->deunderline();
        current_label = index_of_label_below_current();
        labels[current_label]->underline();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_Up) {
        if (labels.size() == 0)
            return;
        labels[current_label]->deunderline();
        current_label = index_of_label_above_current();
        labels[current_label]->underline();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_End) {
        if (labels.size() == 0)
            return;
        labels[current_label]->deunderline();
        current_label = index_of_endline_label();
        labels[current_label]->underline();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_Home) {
        if (labels.size() == 0)
            return;
        labels[current_label]->deunderline();
        current_label = index_of_startline_label();
        labels[current_label]->underline();
        showAlgInfo();
    }
    if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) { // increase font size
        current_font_size++;
        chainInfoLabel->setFont(QFont(kChainInfoLabelFontFamily, current_font_size * 0.7f));
        algInfoLabel->setFont(QFont(kAlgInfoLabelFontFamily, current_font_size * 0.8f));
        for (auto& l: labels) {
            l->setFont(QFont(kLabelsFontFamily, current_font_size));
            l->adjustSize();
        }
        allocate_labels();
    }
    if (event->key() == Qt::Key_Minus) { // decrease font size
        if (current_font_size > 8)
            current_font_size--;
        chainInfoLabel->setFont(QFont(kChainInfoLabelFontFamily, current_font_size * 0.7f));
        algInfoLabel->setFont(QFont(kAlgInfoLabelFontFamily, current_font_size * 0.8f));
        for (auto& l: labels) {
            l->setFont(QFont(kLabelsFontFamily, current_font_size));
            l->adjustSize();
        }
        allocate_labels();
    }
    if (event->key() == Qt::Key_Escape) { // clear or exit
        if (labels.size() == 0) {
            save_settings();
            exit(0);
        }
        clear_labels(); // without saving
        algInfoLabel->setText("clear");
        update_chainInfoLabel();
    }
    if (event->key() == Qt::Key_X) { // switch elem
        commit_and_save_probs();
        clear_labels();
        algInfoLabel->setText("changed to x-centers");
        set_element(el_x);
        update_chainInfoLabel();
    }
    if (event->key() == Qt::Key_W) { // switch elem
        commit_and_save_probs();
        clear_labels();
        algInfoLabel->setText("changed to wings");
        set_element(el_w);
        update_chainInfoLabel();
    }
}

void MainWindow::showAlgInfo() {
    QString& l1 = letters_perm[current_label * 2];
    QString& l2 = letters_perm[current_label * 2 + 1];
    algInfoLabel->setText(l1 + l2 + "\n" + get_alg(l1, l2, algs) + "\n" +
                          QString::number((*p)(l1, l2)));
}

void MainWindow::create_new_chain() {
    int exact_length = current_length ? 20 : 12; // c, w, t
    if (current_element == el_x) {
        auto x_centers_speffz = load_speffz(kDirXSpeffz);
        if (p != nullptr)
            delete p;
        p = new Probs(kDirXProbs);
        letters_perm = generate_initial_chain_type1(x_centers_speffz, exact_length, *p);
        create_labels();
    }
    if (current_element == el_w) {
        auto wings_speffz = load_letters(kDirWSpeffz);
        if (p != nullptr)
            delete p;
        p = new Probs(kDirWProbs);
        letters_perm = generate_initial_chain_wings(wings_speffz, exact_length, *p);
        create_labels();
    }
}

void MainWindow::set_length(bool l) {
    current_length = l;
    update_chainInfoLabel();
}

void MainWindow::set_element(element el) {
    current_element = el;
    switch(current_element) {
    case el_x: algs = load_algs_table(kDirXAlgs); break;
    case el_t: algs = load_algs_table(kDirTAlgs); break;
    case el_w: algs = load_algs_table(kDirWAlgs); break;
    case el_c: algs = load_algs_table(kDirCAlgs); break;
    case el_e: algs = load_algs_table(kDirEAlgs); break;
    }
    update_chainInfoLabel();
}

void MainWindow::commit_and_save_probs() {
    if (p == nullptr || labels.size() == 0)
        return; // haven't loaded probs yet
    // applying changes
    for (uint i = 0; i < labels.size(); ++i) {
        double& c_prob = p->operator ()(letters_perm[i*2], letters_perm[i*2 + 1]);
        if (labels[i]->state() == 0) // easy
            c_prob *= k_easy;
        else if (labels[i]->state() == 1) // neutral
            c_prob *= k_neutral;
        else if (labels[i]->state() == 2) // difficult
            c_prob *= k_hard;
    }
    // normalizing
    p->normalize();
    // saving
    switch(current_element) {
    case el_x: p->save_to_file(kDirXProbs); break;
    case el_t: p->save_to_file(kDirTProbs); break;
    case el_w: p->save_to_file(kDirWProbs); break;
    case el_c: p->save_to_file(kDirCProbs); break;
    case el_e: p->save_to_file(kDirEProbs); break;
    }
}

void MainWindow::update_chainInfoLabel() {
    QString s;
    switch(current_element) {
    case el_x: s = "x-centers"; break;
    case el_t: s = "t_centers"; break;
    case el_w: s = "wings"; break;
    case el_c: s = "corners"; break;
    case el_e: s = "edges"; break;
    }
    s += (current_length) ? " | long" : " | short";
    chainInfoLabel->setText(s);
    chainInfoLabel->adjustSize();
    chainInfoLabel->setGeometry(2, 2, width() - 4, chainInfoLabel->height() + 1);
}

void MainWindow::load_settings() {
    QSettings* settings_ = new QSettings(kDirSettings, QSettings::NativeFormat);
    QFile settings_file(kDirSettings);
    if (settings_file.exists() == false) { // settings file not created yet
        settings_->setValue("exec/font_size", 15);
        settings_->setValue("exec/element", (int)el_x);
        settings_->setValue("exec/length", false);
        settings_->setValue("exec/geometry", QRect(30,30,300,300));
        settings_->sync(); // create file

        QMessageBox msgBox;
        msgBox.setText((QString)"Settings file created: " + kDirSettings + "\nPlease configure before next launch");
        msgBox.exec();
        exit(0);
    }
    // load
    current_font_size = settings_->value("exec/font_size", 15).toInt();
    current_element = (element)(settings_->value("exec/element", (int)el_x).toInt());
    setGeometry(settings_->value("exec/geometry", QRect(30,30,300,300)).toRect());
    current_length = settings_->value("exec/length", false).toBool();
}

void MainWindow::save_settings() {
    QSettings* settings_ = new QSettings(kDirSettings, QSettings::NativeFormat);
    settings_->setValue("exec/font_size", current_font_size);
    settings_->setValue("exec/element", (int)current_element);
    settings_->setValue("exec/length", current_length);
    settings_->setValue("exec/geometry", geometry());
    settings_->sync();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    save_settings();
    event->accept();
}
