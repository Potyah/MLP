#include "dialoggraphic.h"

#include "ui_dialoggraphic.h"

DialogGraphic::DialogGraphic(std::vector<double> rez, QWidget *parent)
    : QDialog(parent), g_ui(new Ui::DialogGraphic) {
    g_ui->setupUi(this);
    chart = new QChart();
    QValueAxis axisX;
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    QBarSeries *series = new QBarSeries();
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();

    for (auto it : rez) {
        QBarSet *set0 = new QBarSet("Epoch");
        *set0 << int(it * 100);
        series->append(set0);
    }

    chart->addSeries(series);
    series->attachAxis(axisY);
    g_ui->graphicsView->setChart(chart);
}

DialogGraphic::~DialogGraphic() {
    if (chart != nullptr) delete chart;
    delete g_ui;
}
