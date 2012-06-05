#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_

#include <QtGui>
#include <mhash.h>
#include "main-dlg.h"

class QCheckSum : public QThread
{
	Q_OBJECT

public:
	QCheckSum( QMainDialog *parent, hashid id, const QString &name, const QString &file );

	void run( void );
	void stop( void );
	int  progress( void );

signals:
	void emitAdd( const QString &name, const QString &hash );
	void emitUpdate( void );

private:
	hashid			m_id;
	QString			m_name;
	QFile			m_file;

	QMutex			m_lock;
	bool			m_stop;
	int				m_progress;

	void setProgress( int prog );
};

#endif // _CHECKSUM_H_
