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

#include "test.h"


Test::Test()
	: mSocket(new QTcpSocket(this))
	, mSession(new Session(mSocket, this)) {
	connect(mSocket, &QTcpSocket::connected, this, &Test::socketConnected);

	mSocket->connectToHost("127.0.0.1", 8090);
}

void Test::socketConnected() {
	disconnect(mSocket, &QTcpSocket::connected, this, &Test::socketConnected);
	connect(mSession, &Session::started, this, &Test::sessionStarted);

	mSession->start();
}

void Test::sessionStarted() {
	disconnect(mSession, &Session::started, this, &Test::sessionStarted);

	emit finished();
}
