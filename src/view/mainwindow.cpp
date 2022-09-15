#include "mainwindow.h"

#include <QDebug>
#include <QThread>

#include "ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  if (m_weight_file.isEmpty()) ui->button_test->setEnabled(false);
  if (m_test_set.isEmpty()) ui->button_test->setEnabled(false);
  if (m_train_set.isEmpty()) ui->button_train->setEnabled(false);
  scene = new paintScene();
  scene->setSceneRect(0, 0, 560, 560);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_brushSize = 20;
  ui->label_recognized_letter->setText(QChar::fromLatin1(m_result));
  connect(scene, SIGNAL(mouse_released()), this, SLOT(imageConvert()));
  connect(scene, SIGNAL(clear_released()), this, SLOT(setEmptyLetter()));
  update();
  m_controller = new Controller();
  m_controller->updateNetwork(m_network, m_layers);
}

MainWindow::~MainWindow() {
  if (dg != nullptr) delete dg;
  if (m_controller != nullptr) delete m_controller;
  if (scene != nullptr) delete scene;
  delete ui;
}

void MainWindow::on_button_loadBMP_clicked() {
  update();
  m_bmp_file = QFileDialog::getOpenFileName(this, "Select a file to open...",
                                            QDir::currentPath(), "*.bmp");
  userImageVector.clear();
  if (!m_bmp_file.isEmpty()) {
    userImage.load(m_bmp_file);
    userImage.invertPixels();
    userImage.toPixelFormat(QImage::Format_Grayscale8);
    if (userImage.width() <= 512 && userImage.height() <= 512) {
      imageToVector(userImage, userImageVector);
    } else {
      QMessageBox msg;
      msg.setText("File too large");
      msg.exec();
    }
  }
  QString result;
  result = m_controller->singleTest(userImageVector);
  ui->label_recognized_letter->setText(result);
}

void MainWindow::on_button_train_clicked() {
  update();
  m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                              m_train_set.toStdString());
  std::vector<double> rez =
      m_controller->trainingNetwork(m_crossvalidation, m_epochs);
  showGraph(rez);
}

void MainWindow::on_button_test_clicked() {
  m_controller->setPathTestFile(m_test_set.toStdString());
  std::array<double, 6> rez =
      m_controller->testFile(ui->doubleSpinBox_sample_size->value());
  ui->label_accuracy_value->setText(QString::number(rez[1]));
  ui->label_precision_value->setText(QString::number(rez[2]));
  ui->label_recall_value->setText(QString::number(rez[3]));
  ui->label_f_measure_value->setText(QString::number(rez[4]));
  ui->label_time_value->setText(QString::number(rez[5]));
}

void MainWindow::on_button_load_weight_clicked() {
  update();
  m_weight_file = QFileDialog::getOpenFileName(
      this, "Select a file to open...", QDir::currentPath(), "*.weights");
  QChar layer = m_weight_file.split('/').last().at(0);
  if ((layer.toLatin1() - 46) == m_layers) {
    // 48 - ASCII код 0, минус 2 слоя нейронов (вводный и выходной) итого 46
    try {
      m_controller->loadWeights(m_weight_file.toStdString());
    } catch (std::exception e) {
      QMessageBox msg;
      msg.setText(e.what());
      msg.exec();
    }
  }
}

void MainWindow::on_button_save_weight_clicked() {
  m_weight_file = QFileDialog::getSaveFileName(
      this, "Select a file to save...", QDir::currentPath(), "*.weights");
  if (!m_weight_file.isEmpty())
    m_controller->saveWeights(m_weight_file.toStdString());
}

void MainWindow::imageConvert() {
  userImage = ui->graphicsView->grab().toImage();
  userImage.invertPixels();
  imageToVector(userImage, userImageVector);
  QString result;
  result = m_controller->singleTest(userImageVector);
  ui->label_recognized_letter->setText(result);
}

void MainWindow::update() {
  m_epochs = ui->spinBox_epochs->value();
  if (ui->layers_2->isChecked())
    m_layers = 4;
  else if (ui->layers_3->isChecked())
    m_layers = 5;
  else if (ui->layers_4->isChecked())
    m_layers = 6;
  else if (ui->layers_5->isChecked())
    m_layers = 7;
  if (ui->checkBox->isChecked()) {
    m_crossvalidation = ui->spinBox_crossvalidation->value();
  } else {
    m_crossvalidation = 0;
  }
  if (ui->radio_graph->isChecked())
    m_network = Graph;
  else
    m_network = Matrix;
}

void MainWindow::setEmptyLetter() {
  ui->label_recognized_letter->setText(QChar::fromLatin1(' '));
}

void MainWindow::imageToVector(QImage &img, std::vector<double> &vec) {
  vec.clear();
  img = img.scaled(28, 28, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  vec.push_back(0.0);
  for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 28; j++) {
      int val = img.pixelColor(i, j).value();
      if (val < 10) val = 0.0;
      if (val > 234) val = 255.0;
      vec.push_back(val);
    }
  }
}

void MainWindow::on_checkBox_stateChanged(int arg1) {
  if (arg1)
    ui->spinBox_epochs->setEnabled(false);
  else
    ui->spinBox_epochs->setEnabled(true);
}

void MainWindow::showGraph(std::vector<double> rez) {
  dg = new DialogGraphic(rez);
  dg->show();
}

void MainWindow::on_button_train_load_clicked() {
  m_train_set = QFileDialog::getOpenFileName(this, "Select train dataset...",
                                             QDir::currentPath(), "*.csv");
  if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
    ui->button_train->setEnabled(true);
    ui->button_test->setEnabled(true);
  }
}

void MainWindow::on_button_test_load_clicked() {
  m_test_set = QFileDialog::getOpenFileName(this, "Select train dataset...",
                                            QDir::currentPath(), "*.csv");
  if (!m_test_set.isEmpty()) {
    ui->button_test->setEnabled(true);
    if (!m_train_set.isEmpty()) {
      ui->button_train->setEnabled(true);
    }
  }
}

void MainWindow::on_radio_graph_clicked() {
  if (m_network == Matrix) {
    update();
    if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
      m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                                  m_train_set.toStdString());
    } else {
      m_controller->updateNetwork(m_network, m_layers);
    }
  }
}

void MainWindow::on_radio_matrix_clicked() {
  if (m_network == Graph) {
    update();
    if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
      m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                                  m_train_set.toStdString());
    } else {
      m_controller->updateNetwork(m_network, m_layers);
    }
  }
}

void MainWindow::on_layers_2_clicked() {
  if (m_layers != 4) {
    update();
    if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
      m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                                  m_train_set.toStdString());
    } else {
      m_controller->updateNetwork(m_network, m_layers);
    }
  }
}

void MainWindow::on_layers_5_clicked() {
  if (m_layers != 7) {
    update();
    if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
      m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                                  m_train_set.toStdString());
    } else {
      m_controller->updateNetwork(m_network, m_layers);
    }
  }
}

void MainWindow::on_layers_3_clicked() {
  if (m_layers != 5) {
    update();
    if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
      m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                                  m_train_set.toStdString());
    } else {
      m_controller->updateNetwork(m_network, m_layers);
    }
  }
}

void MainWindow::on_layers_4_clicked() {
  if (m_layers != 6) {
    update();
    if (!m_test_set.isEmpty() && !m_train_set.isEmpty()) {
      m_controller->updateNetwork(m_network, m_layers, m_test_set.toStdString(),
                                  m_train_set.toStdString());
    } else {
      m_controller->updateNetwork(m_network, m_layers);
    }
  }
}

}  // namespace s21
