#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "algo.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <deque>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    deque_model_.random_generator = std::mt19937{std::random_device{}()};
    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::ApplyModel() {

    auto preserve_iter = deque_model_.iterator;

    ui->list_widget->clear();

    for (size_t i = 0; i < deque_model_.items.size(); ++i) {
        QString item = QString::number(i) + ": " + QString::fromStdString(deque_model_.items[i]);
        ui->list_widget->addItem(item);
    }
    ui->list_widget->addItem("end");

    if (preserve_iter != deque_model_.items.end()) {
        size_t idx = std::distance(deque_model_.items.begin(), preserve_iter);
        if (idx < deque_model_.items.size()) {
            deque_model_.iterator = preserve_iter;
        } else {
            deque_model_.iterator = deque_model_.items.begin();
        }
    } else {
        deque_model_.iterator = deque_model_.items.end();
    }

    ApplyIterator();

    ui->txt_size->setText(QString::number(deque_model_.items.size()));
    ui->btn_pop_front->setDisabled(deque_model_.items.empty());
    ui->btn_pop_back->setDisabled(deque_model_.items.empty());
    ui->lbl_count->setText(QString::number(deque_model_.count));
}

void MainWindow::ApplyIterator() {
    size_t pos = std::distance(deque_model_.items.begin(), deque_model_.iterator);
    size_t size = deque_model_.items.size();

    if (pos < size) {
        ui->list_widget->setCurrentRow(static_cast<int>(pos));
    } else {
        ui->list_widget->setCurrentRow(static_cast<int>(size));
    }

    bool at_end = (deque_model_.iterator == deque_model_.items.end());
    ui->btn_edits->setDisabled(at_end);
    ui->btn_erase->setDisabled(at_end);
    ui->btn_inc_iterator->setDisabled(at_end);
    ui->btn_dec_iterator->setDisabled(deque_model_.iterator == deque_model_.items.begin());

    if (at_end) {
        ui->txt_elem_content->clear();
    } else {
        ui->txt_elem_content->setText(QString::fromStdString(*deque_model_.iterator));
    }
}

void MainWindow::on_list_widget_currentRowChanged(int currentRow) {

    currentRow = std::clamp(currentRow, 0, static_cast<int>(deque_model_.items.size()));
    if (static_cast<size_t>(currentRow) < deque_model_.items.size()) {
        deque_model_.iterator = deque_model_.items.begin() + currentRow;
    } else {
        deque_model_.iterator = deque_model_.items.end();
    }
    ApplyIterator();
}

void MainWindow::on_btn_pop_back_clicked() {
    if (deque_model_.items.empty()) {
        return;
    }
    deque_model_.items.pop_back();
    BeginCountZero();
    ApplyModel();
}

void MainWindow::on_btn_push_back_clicked() {
    QString new_item = ui->txt_elem_content->text();
    if (new_item.isEmpty()) {
        return;
    }
    deque_model_.items.push_back(new_item.toStdString());
    BeginCountZero();
    ApplyModel();
}

void MainWindow::on_btn_clear_clicked() {
    deque_model_.items.clear();
    BeginCountZero();
    ApplyModel();
}

void MainWindow::on_btn_erase_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }
    deque_model_.items.erase(deque_model_.iterator);
    BeginCountZero();
    ApplyModel();
}

void MainWindow::on_btn_insert_clicked() {
    QString new_item = ui->txt_elem_content->text();
    if (new_item.isEmpty()) {
        return;
    }
    if (deque_model_.iterator == deque_model_.items.end()) {
        deque_model_.items.push_back(new_item.toStdString());
    } else {
        deque_model_.items.insert(deque_model_.iterator, new_item.toStdString());
    }
    BeginCountZero();
    ApplyModel();
}

void MainWindow::on_btn_edits_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }
    QString new_value = ui->txt_elem_content->text();
    if (new_value.isEmpty()) {
        return;
    }
    *deque_model_.iterator = new_value.toStdString();
    deque_model_.count = 0;
    ApplyModel();
}

void MainWindow::on_btn_resize_clicked()
{
    int num = (ui->txt_size->text()).toInt();
    if (num < 0 || ui->txt_size->text().isEmpty()) {
        return;
    }
    if (num > 1000) {
        num = 1000;
    }
    deque_model_.iterator = deque_model_.items.begin();
    deque_model_.items.resize(num);
    deque_model_.count = 0;
    ApplyModel();
}


void MainWindow::on_btn_find_clicked()
{
    QString new_value = ui->txt_elem_content->text();
    if (new_value.isEmpty()) {
        return;
    }
    std::string target = new_value.toStdString();
    auto it = std::find(deque_model_.items.begin(), deque_model_.items.end(), target);
    deque_model_.iterator = it;
    deque_model_.count = 0;
    ApplyModel();
}


void MainWindow::on_btn_count_clicked()
{
    QString new_value = ui->le_count->text();
    if (new_value.isEmpty()) {
        return;
    }
    std::string target = new_value.toStdString();
    auto count = std::count(deque_model_.items.begin(), deque_model_.items.end(), target);
    deque_model_.count = static_cast<size_t>(count);
    ApplyModel();
}


void MainWindow::on_btn_min_e_clicked()
{
    auto it = std::min_element(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.iterator = it;
    deque_model_.count = 0;
    ApplyModel();
}


void MainWindow::on_btn_max_e_clicked()
{
    auto it = std::max_element(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.iterator = it;
    deque_model_.count = 0;
    ApplyModel();
}


void MainWindow::on_btn_sort1_clicked()
{
    deque_model_.items = MergeSort(deque_model_.items, std::less<>{});
    BeginCountZero();
    ApplyModel();
}


void MainWindow::on_btn_sort2_clicked()
{
    deque_model_.items = MergeSort(deque_model_.items,
                                   [](const std::string& a, const std::string& b) {
                                       return QString::compare(QString::fromStdString(a),
                                                               QString::fromStdString(b),
                                                               Qt::CaseInsensitive) < 0;
                                   });
    BeginCountZero();
    ApplyModel();
}


void MainWindow::on_btn_unique_clicked()
{

    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }
    const auto new_end = std::unique(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.items.erase(new_end, deque_model_.items.end());
    BeginCountZero();
    ApplyModel();
}


void MainWindow::on_btn_reverse_clicked()
{
    auto preserve_iter = deque_model_.iterator;
    std::reverse(deque_model_.items.begin(), deque_model_.items.end());
    if (preserve_iter != deque_model_.items.end()) {
        size_t idx = std::distance(deque_model_.items.begin(), preserve_iter);
        if (idx < deque_model_.items.size()) {
            deque_model_.iterator = preserve_iter;
        } else {
            deque_model_.iterator = deque_model_.items.begin();
        }
    }
    deque_model_.count = 0;
    ApplyModel();
}


void MainWindow::on_btn_shuffle_clicked()
{
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(), deque_model_.random_generator);
    deque_model_.count = 0;
    ApplyModel();
}

void MainWindow::SetRandomGen(const std::mt19937& random_gen) {

    deque_model_.random_generator = random_gen;
}


void MainWindow::on_btn_pop_front_clicked()
{
    if (deque_model_.items.empty()) {
        return;
    }
    deque_model_.items.pop_front();
    BeginCountZero();
    ApplyModel();
}


void MainWindow::on_btn_push_front_clicked()
{
    QString new_item = ui->txt_elem_content->text();
    if (new_item.isEmpty()) {
        return;
    }
    deque_model_.items.push_front(new_item.toStdString());
    BeginCountZero();
    ApplyModel();
}


void MainWindow::on_btn_lower_bound_clicked()
{
    QString item = ui->txt_elem_content->text();
    if (item.isEmpty()) {
        return;
    }
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }

    std::string key = item.toStdString();
    auto it = std::lower_bound(deque_model_.items.begin(), deque_model_.items.end(), key);
    deque_model_.iterator = it;
    deque_model_.count = 0;
    ApplyModel();

}


void MainWindow::on_btn_upper_bound_clicked()
{
    QString item = ui->txt_elem_content->text();
    if (item.isEmpty()) {
        return;
    }
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }

    std::string key = item.toStdString();
    auto it = std::upper_bound(deque_model_.items.begin(), deque_model_.items.end(), key);
    deque_model_.iterator = it;
    deque_model_.count = 0;
    ApplyModel();
}


void MainWindow::on_btn_dec_iterator_clicked()
{
    if (deque_model_.iterator != deque_model_.items.begin()) {
        --deque_model_.iterator;
        deque_model_.count = 0;
        ApplyIterator();
    }
}


void MainWindow::on_btn_inc_iterator_clicked()
{
    if (deque_model_.iterator != deque_model_.items.end()) {
        ++deque_model_.iterator;
        deque_model_.count = 0;
        ApplyIterator();
    }
}


void MainWindow::on_btn_begin_clicked()
{
    BeginCountZero();
    ApplyIterator();
}


void MainWindow::on_btn_end_clicked()
{
    deque_model_.iterator = deque_model_.items.end();
    deque_model_.count = 0;
    ApplyIterator();
}


void MainWindow::on_btn_tea_clicked()
{
    static std::deque<std::string> tea {
        "Чай Лунцзин",
        "Эрл Грей",
        "Сенча",
        "Пуэр",
        "Дарджилинг",
        "Ассам",
        "Матча",
        "Ганпаудер",
        "Оолонг",
        "Лапсанг Сушонг"
    };

    deque_model_.items = tea;
    BeginCountZero();
    ApplyModel();

}


void MainWindow::on_btn_cakes_clicked()
{
    static std::deque<std::string> cakes {
        "Красный бархат",
        "Наполеон",
        "Медовик",
        "Тирамису",
        "Прага",
        "Чизкейк",
        "Захер",
        "Эстерхази",
        "Морковный торт",
        "Чёрный лес",
    };

    deque_model_.items = cakes;
    BeginCountZero();
    ApplyModel();

}

void MainWindow::BeginCountZero() {
    deque_model_.iterator = deque_model_.items.begin();
    deque_model_.count = 0;
}
