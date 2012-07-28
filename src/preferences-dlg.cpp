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

#include "preferences-dlg.h"


QPreferencesDialog::QPreferencesDialog( QList <QHashItem *> &hashs, QConfigApp &config ):
	QConfigApp( config )
{
	setupUi( this );
	
	connect( buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( onButtonBox( QAbstractButton * ) ) );
	connect( checkBoxAutoCalc, SIGNAL( toggled( bool ) ), this, SLOT( onAutoCalc() ) );

	loadConfig();
	loadHashs( &hashs );
	getHashsState( false );
}


QPreferencesDialog::~QPreferencesDialog( void )
{
	unloadHashs();
}


void QPreferencesDialog::onButtonBox( QAbstractButton *button )
{
	switch( buttonBox->standardButton( button ) )
	{
	case QDialogButtonBox::Ok:
		done
			(
				QPrefCode::ChangeConfig
				|
				setHashsState()
			);
		break;
		
	case QDialogButtonBox::Cancel:
		done( QPrefCode::None );
		break;
		
	case QDialogButtonBox::RestoreDefaults:
		getHashsState( true );
		break;
		
	default:
		return;
	}
}


void QPreferencesDialog::onAutoCalc( void )
{
	setAutoCalc( checkBoxAutoCalc->checkState() == Qt::Checked );
}


void QPreferencesDialog::loadConfig( void )
{
	checkBoxAutoCalc->setCheckState( autoCalc() ? Qt::Checked : Qt::Unchecked );
}


void QPreferencesDialog::loadHashs( QList <QHashItem *> *hashs )
{
	QVBoxLayout *currBox = NULL;

	foreach( QHashItem *item, *hashs )
	{
		QHashButton *button = new QHashButton( *item );
		
		if( item->newBox() || currBox == NULL )
		{
			currBox = new QVBoxLayout();
			currBox->setAlignment( Qt::AlignTop );

			m_box.append( currBox );
			horizontalLayout->addLayout( currBox );
		}

		m_buttons.append( button );
		currBox->addWidget( button );
 	}
}


void QPreferencesDialog::unloadHashs( void )
{
	foreach( QHashButton *button, m_buttons )
        delete button;

	foreach( QVBoxLayout *box, m_box )
        delete box;

	m_buttons.clear();
	m_box.clear();
}


void QPreferencesDialog::getHashsState( bool def )
{
	foreach( QHashButton *button, m_buttons )
	{
		QHashItem *hash = button->hash();
		bool state = def ? hash->def() : hash->active();

		button->setCheckState( state ? Qt::Checked : Qt::Unchecked );
	}
}


int QPreferencesDialog::setHashsState( void )
{
	int ret = QPrefCode::None;

	foreach( QHashButton *button, m_buttons )
	{
		QHashItem *hash = button->hash();
		bool state = button->checkState() == Qt::Checked;
		
		if( state != hash->active() )
		{
			ret = QPrefCode::ChangeHash;
			hash->setActive( state );
		}
	}
	
	return ret;
}
