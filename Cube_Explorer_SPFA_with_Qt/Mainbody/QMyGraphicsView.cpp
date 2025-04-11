#include "QMyGraphicsView.h"

QMyGraphicsView::QMyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
}

QMyGraphicsView::~QMyGraphicsView()
{
}

void QMyGraphicsView::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {	//鼠标左键触发按下事件
		QPoint point = event->pos();			//获取鼠标事件触发点的View坐标系坐标
		rec_select.setX(point.x());				//设置选取框左上角x轴坐标
		rec_select.setY(point.y());				//设置选取框左上角y轴坐标
	}
	QGraphicsView::mousePressEvent(event);
}

void QMyGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {	//鼠标左键触发松开事件
		QPoint point = event->pos();			//获取鼠标事件触发点的View坐标系坐标
		int w = abs(point.x() - rec_select.x());		
		int h = abs(point.y() - rec_select.y());		
		if (point.x() < rec_select.x()) rec_select.setX(point.x());	//确认选取框左上角x轴坐标
		if (point.y() < rec_select.y()) rec_select.setY(point.y());	//确认选取框左上角y轴坐标
		rec_select.setWidth(w);					
		rec_select.setHeight(h);				

		emit mouseReleased(rec_select);
	}
	QGraphicsView::mouseReleaseEvent(event);
}

