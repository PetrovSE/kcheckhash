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

#ifndef _PREFERENCES_DLG_H_
#define _PREDERENCES_DLG_H_

#include <QtGui>
#include <QCheckBox>
#include "hashitem.h"
#include "ui_preferences-dlg.h"


class QHashButton : public QCheckBox
{
	Q_OBJECT
	
public:
	QHashButton( QHashItem &hash ) : QCheckBox( hash.name() )
	{
		m_hash = &hash;
	}
	
	QHashItem *hash( void ) const
	{
		return m_hash;
	}

private:
	QHashItem	*m_hash;
};


class QPreferencesDialog : public QDialog, private Ui::dialogPreferences
{
	Q_OBJECT
 
public:
	QPreferencesDialog( QList <QHashItem *> *hashs );
	~QPreferencesDialog( void );

public slots:
	void onButtonBox( QAbstractButton *button );

protected:
	void loadHashs( QList <QHashItem *> *hashs );
	void unloadHashs( void );
	
	void getHashsState( bool def );
	bool setHashsState( void );

	QList <QVBoxLayout *>	m_box;
	QList <QHashButton *>	m_buttons;
};

#endif // _PREFERENCES_DLG_H_
