/*   Copyright (C) 2011-2012 PetrovSE
 *   <http://forum.ubuntu.ru/index.php?action=profile;u=12963>
 * 
 *   This file is part of kcheckhash.
 *
 *   kcheckhash is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   kcheckhash is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with kcheckhash.  If not, see <http://www.gnu.org/licenses/>.
 */

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
