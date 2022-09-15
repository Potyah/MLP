#ifndef SRC_VIEW_PAINTSCENE_H_
#define SRC_VIEW_PAINTSCENE_H_

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

namespace s21 {
class paintScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();

signals:
    void mouse_released();
    void clear_released();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QPointF previousPoint;
    unsigned int m_brushSize;
    QColor m_lineColor;
    bool m_rightButtonPressed;
};
}  // namespace s21

#endif  // SRC_VIEW_PAINTSCENE_H_
