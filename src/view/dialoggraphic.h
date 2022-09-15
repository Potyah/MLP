#ifndef SRC_VIEW_DIALOGGRAPHIC_H_
#define SRC_VIEW_DIALOGGRAPHIC_H_

#include <QDialog>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QBarSet>

namespace Ui {
class DialogGraphic;
}

class DialogGraphic : public QDialog {
  Q_OBJECT

 public:
  explicit DialogGraphic(std::vector<double> rez, QWidget *parent = nullptr);
  ~DialogGraphic();

 private:
  Ui::DialogGraphic *g_ui;
  QChart *chart;
};

#endif  //  SRC_VIEW_DIALOGGRAPHIC_H_
