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

#ifndef QWMAP_P_H
#define QWAMP_P_H

#include <qwamp.h>

#include <QJsonArray>


namespace qwamp {

class SessionPrivate: public QObject {
	Q_OBJECT
	Q_DECLARE_PUBLIC(Session)

	Session* q_ptr;
	QIODevice* mIo;
	int mState;
	int mFrameType;
	int mFrameLength;
	quint64 mSessionId;

	SessionPrivate(QIODevice* io);
	void start();
	void join(const QString& realm);
	void leave(const QString& reason);

	void sendMessage(const QJsonArray& data);
	void sendFrame(int frameType, const QByteArray& data);

	void onMessage(const QJsonArray& data);
	void onHelloReply(const QJsonArray& data);
	void onLeaveReply(const QJsonArray& data);

	void onMessageReceived(const QByteArray& data);
	void onPingReceived(const QByteArray& data);
	void onPongReceived(const QByteArray& data);

private slots:
	void onHandshakeReply();
	void onDataReceived();
};

}

#endif
