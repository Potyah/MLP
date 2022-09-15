#include "paintscene.h"

namespace s21 {
paintScene::paintScene(QObject *parent) : QGraphicsScene(parent) {
  m_brushSize = 60;
  m_lineColor = Qt::black;
  m_rightButtonPressed = false;
}

paintScene::~paintScene() {}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    clear();
    emit clear_released();
    m_rightButtonPressed = true;
  } else if (event->button() == Qt::LeftButton) {
    previousPoint = event->scenePos();
    addEllipse(previousPoint.x() - m_brushSize / 2,
               previousPoint.y() - m_brushSize / 2, m_brushSize, m_brushSize,
               QPen(m_lineColor, 0, Qt::SolidLine, Qt::RoundCap),
               QBrush(m_lineColor));
  }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    addLine(previousPoint.x(), previousPoint.y(), event->scenePos().x(),
            event->scenePos().y(),
            QPen(m_lineColor, m_brushSize, Qt::SolidLine, Qt::RoundCap));
    previousPoint = event->scenePos();
  }
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
      emit mouse_released();
  }
}

}  // namespace s21
