#ifndef _MAIN_DLG_H_
#define _MAIN_DLG_H_

#include <QtGui>
#include "ui_main-dlg.h"

#define PROG_SIZE			256

class QCheckSum;
class QMainDialog : public QMainWindow, private Ui::mainDialog
{
	Q_OBJECT

public:
	QMainDialog( const QString &file = "" );

public slots:
	void onAdd( const QString &name, const QString &hash );
	void onCheck( const QString &hash );
	void onUpdate( void );
	void onFinished( void );
	void onButtonClick( QAbstractButton *button );
	void onShowContextMenu( const QPoint &point );

signals:
	void emitCheck( const QString &hash );

private:
	QStandardItemModel	m_model;
	QList <QCheckSum *>	m_calcs;

	QIcon				m_ok;
	QIcon				m_cross;
	QString				m_path;

	void closeEvent( QCloseEvent *event );

	void open( void );
	void start( const QString &file );
	void stop( void );
};

#endif // _MAIN_DLG_H_
