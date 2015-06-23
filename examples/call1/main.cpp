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

#include <QCoreApplication>
#include <QTcpSocket>

#include <qwamp.h>


using qwamp::Session;


class Test: public QObject {
	Q_OBJECT

	QTcpSocket* mSocket;
	Session* mSession;

public:
	Test()
		: mSocket(new QTcpSocket(this))
		, mSession(new Session(mSocket, this)) {
		connect(mSocket, &QTcpSocket::connected, this, &Test::socketConnected);

		mSocket->connectToHost("127.0.0.1", 8090);
	}

	virtual ~Test() {}

private slots:
	void socketConnected() {
		disconnect(mSocket, &QTcpSocket::connected, this, &Test::socketConnected);
		connect(mSession, &Session::started, this, &Test::sessionStarted);

		mSession->start();
	}

	void sessionStarted() {
		disconnect(mSession, &Session::started, this, &Test::sessionStarted);

		emit finished();
	}

signals:
	void finished();
};


int main(int argc, char* argv[]) {
	QCoreApplication app(argc, argv);

	Test test;
	QObject::connect(&test, &Test::finished, &app, &QCoreApplication::quit);

	return app.exec();
}
