#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    arrays = new XYArrays();

    setWindowTitle("Работа №3");
    setMinimumSize(275, 340);

    menu_bar = new QMenuBar(this);
    menu_bar->setGeometry(0, 0, 275, 25);

    file_menu = new QMenu("Файл");
    create_new = new QAction("Создать");
    connect(create_new, SIGNAL(triggered()), this, SLOT(calculateX()));
    file_menu->addAction(create_new);
    load = new QAction("Загрузить");
    connect(load, SIGNAL(triggered()), this, SLOT(load_file()));
    file_menu->addAction(load);
    save = new QAction("Сохранить");
    connect(save, SIGNAL(triggered()), this, SLOT(save_file()));
    file_menu->addAction(save);
    exit = new QAction("Выход");
    connect(exit, SIGNAL(triggered()), this, SLOT(exit_s()));
    file_menu->addAction(exit);

    operation_menu = new QMenu("Работа");
    calculate = new QAction("Вычислить");
    connect(calculate, SIGNAL(triggered()), this, SLOT(calculateY()));
    operation_menu->addAction(calculate);

    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(operation_menu);


    x_list_label = new QLabel("Массив X", this);
    x_list_label->setGeometry(15, 40, 120, 25);
    x_list = new QListWidget(this);
    x_list->setGeometry(15, 65, 120, 200);
    connect(x_list, SIGNAL(itemSelectionChanged()), this, SLOT(saveXPosition()));
    selected_x_lineedit = new QLineEdit(this);
    selected_x_lineedit->setGeometry(15, 270, 120, 25);
    replace_x_button = new QPushButton("Заменить X", this);
    replace_x_button->setGeometry(15, 300, 120, 25);
    connect(replace_x_button, SIGNAL(clicked(bool)), this, SLOT(changeXfromPosition()));

    y_combobox_label = new QLabel("Массив Y", this);
    y_combobox_label->setGeometry(140, 40, 120, 25);
    y_combobox = new QComboBox(this);
    y_combobox->setGeometry(140, 65, 120, 25);
    connect(y_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveYPosition()));
    selected_y_lineedit = new QLineEdit(this);
    selected_y_lineedit->setGeometry(140, 270, 120, 25);
    replace_y_button = new QPushButton("Заменить Y", this);
    replace_y_button->setGeometry(140, 300, 120, 25);
    connect(replace_y_button, SIGNAL(clicked(bool)), this, SLOT(changeYfromPosition()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::calculateX(){
    arrays->makeX();
    printX();
}

void MainWindow::calculateY(){
    arrays->makeYfromX();
    printY();
}

void MainWindow::load_file(){
    auto file_name = QFileDialog::getOpenFileName
            (this, "Загрузить", QDir::currentPath(),
             tr("Document files (*.txt *.dat);;All files (*.*)"));
    QFile fin(file_name);
    if (fin.isOpen()) fin.close();
    fin.open(QFile::ReadOnly);
    if (!fin.isOpen()) return;
    QTextStream in(&fin);
    std::vector<double> vec;
    while (!in.atEnd()){
        QString temp = in.readLine();
        qDebug() << temp;
        if (isParseble(temp))
            vec.push_back(parseToDouble(temp));
    }
    arrays->setX(vec);
    printX();
    fin.close();
}

void MainWindow::save_file(){
    auto file_name = QFileDialog::getExistingDirectory
            (this, "Сохранить", QDir::currentPath());
    QFile fout(file_name + "\\file.txt");
    if (fout.isOpen()) fout.close();
    fout.open(QFile::WriteOnly | QFile::Truncate);
    if (!fout.isOpen()) return;
    QTextStream out(&fout);
    std::vector<double> vec = arrays->getY();
    int i = 1;
    for (auto el : vec){
        out << "Число " << QString::number(i++) << '\n';
        out << '[' << QString::number(el, 'f', 3) << ']' << '\n';
    }
    fout.close();
}

void MainWindow::saveXPosition(){
    this->selected_x = x_list->currentItem();
    this->selected_x_lineedit->setText(this->selected_x->text());
}

void MainWindow::changeXfromPosition(){
    if (selected_x == nullptr) return;
    QString value = selected_x_lineedit->text();
    for (auto c : value){
        if (c.isLetter()) return;
    }
    selected_x->setText(value);
    std::vector<double> temp = readX();
    arrays->setX(temp);
}

void MainWindow::saveYPosition(){
    this->selected_y = y_combobox->currentIndex();
    this->selected_y_lineedit->setText(this->y_combobox->itemText(selected_y));
}

void MainWindow::changeYfromPosition(){
    if (selected_y == -1) return;
    QString value = selected_y_lineedit->text();
    for (auto c : value){
        if (c.isLetter()) return;
    }
    this->y_combobox->setItemText(selected_y, value);
    std::vector<double> temp = readY();
}

void MainWindow::exit_s(){
    std::exit(0);
}

void MainWindow::printX(){
    if(x_list->count() != 0) x_list->clear();
    std::vector<double> vec = arrays->getX();
    for (auto el : vec){
        x_list->addItem(QString::number(el, 'f', 3));
    }
}

void MainWindow::printY(){
    if(y_combobox->count() != 0) y_combobox->clear();
    std::vector<double> vec = arrays->getY();
    for (auto el : vec){
        y_combobox->addItem(QString::number(el, 'f', 3));
    }
}

bool MainWindow::isParseble(QString str){
    if (str.front() != '[' || str.back() != ']') return false;
    for (auto c : str){
        if (c.isLetter()) return false;
    }
    return true;
}

double MainWindow::parseToDouble(QString str){
    str.erase(str.begin(), str.begin()+1);
    str.erase(str.end()-1, str.end());
    return str.toDouble();
}

std::vector<double> MainWindow::readX(){
    std::vector<double> result;
    for (int i = 0; i < x_list->count(); i++){
        result.push_back(x_list->item(i)->text().toDouble());
    }
    return result;
}

std::vector<double> MainWindow::readY(){
    std::vector<double> result;
    for (int i = 0; i < y_combobox->count(); i++){
        result.push_back(y_combobox->itemText(i).toDouble());
    }
    return result;
}
