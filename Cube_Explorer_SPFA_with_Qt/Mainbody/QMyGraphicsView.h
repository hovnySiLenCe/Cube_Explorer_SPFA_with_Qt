#pragma once

#include <QGraphicsView>
#include <qgraphicsitem.h>
#include <qmenu.h>
#include <QMouseEvent>

class QMyGraphicsView : public QGraphicsView
{
	Q_OBJECT

private:
	QRect rec_select;

public:
	QMyGraphicsView(QWidget *parent);
	~QMyGraphicsView();

protected:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:
	void mouseReleased(QRect rec);
};
