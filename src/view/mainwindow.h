#ifndef SRC_VIEW_MAINWINDOW_H_
#define SRC_VIEW_MAINWINDOW_H_

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <string>
#include <vector>

#include "../controller/controller.h"
#include "common.h"
#include "dialoggraphic.h"
#include "paintscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_loadBMP_clicked();
    void on_button_train_clicked();
    void on_button_test_clicked();
    void on_button_load_weight_clicked();
    void on_button_save_weight_clicked();
    void imageConvert();
    void update();

    void setEmptyLetter();
    void on_checkBox_stateChanged(int arg1);
    void showGraph(std::vector<double> rez);

    void on_button_train_load_clicked();
    void on_button_test_load_clicked();
    void on_radio_graph_clicked();
    void on_radio_matrix_clicked();
    void on_layers_2_clicked();
    void on_layers_5_clicked();
    void on_layers_3_clicked();
    void on_layers_4_clicked();

private:
    Ui::MainWindow *ui;
    Controller *m_controller;
    paintScene *scene;
    unsigned int m_brushSize;
    QImage userImage;
    std::vector<double> userImageVector;

    int m_epochs;
    int m_layers;
    int m_crossvalidation;
    ModelType m_network;
    char m_result;
    QString m_bmp_file;
    QString m_weight_file;
    QString m_test_set;
    QString m_train_set;
    void imageToVector(QImage &img, std::vector<double> &vec);
    DialogGraphic *dg;
};
}  // namespace s21
#endif  // SRC_VIEW_MAINWINDOW_H_
