// Copyright (C) 2015 Ilya Lyubimov
//
// This file is part of qwamp.
//
//  Qwamp is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Qwamp is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with qwamp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef QWMAP_H
#define QWAMP_H

#include <QIODevice>

namespace qwamp {

class SessionPrivate;
class Session: QObject {
	Q_OBJECT
	Q_DISABLE_COPY(Session)
	Q_DECLARE_PRIVATE(Session)

	QScopedPointer<SessionPrivate> d_ptr;

public:
	Session(QIODevice* io, QObject* parent = 0);
	virtual ~Session();

	void start();
	void stop();

signals:
	void started();
	void stopped();
};

}

#endif
