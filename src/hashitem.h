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

#ifndef _HASH_ITEM_H_
#define _HASH_ITEM_H_

#include <QString>
#include <mhash.h>

class QHashItem
{
public:
	QHashItem( const QString &name, hashid id, bool active, bool newBox = false )
	{
		m_name	= name;
		m_id	= id;
		m_def	= active;

		m_newBox = newBox;
		setActive( active );
	}

	QString name( void ) const
	{
		return m_name;
	}
	
	hashid id( void ) const
	{
		return m_id;
	}

	bool newBox( void ) const
	{
		return m_newBox;
	}

	bool def( void ) const
	{
		return m_def;
	}

	bool active( void ) const
	{
		return m_active;
	}
	
	void setActive( bool active )
	{
		m_active = active;
	}
	
private:
	QString		m_name;
	hashid		m_id;

	bool		m_def;
	bool		m_active;
	bool		m_newBox;
};

#endif // _HASH_ITEM_H_
