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

#include "qwamp_p.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>


const int kHandshakeMagic = 0x7F;
const int kMaxInFrameSizeExponent = 0x0F;
const int kMaxInFrameSize = (2 << (9 + kMaxInFrameSizeExponent));

const int kJsonSerializer = 0x01;
const int kMsgPackSerializer = 0x02;

enum SessionState {
	kNewSessionState,
	kStartedSessionState,
	kHelloSentSessionState,
	kEstablishedSessionState,
	kGoodbyeSentSessionState,
	kStoppedSessionState
};

enum FrameType {
	kRegularFrameType = 1,
	kPingFrameType = 2,
	kPongFrameType = 3,
	kUnknownFrameType = 4
};

enum MessageCode {
	kHelloMessageCode = 1,
	kWelcomeMessageCode = 2,
	kAbortMessageCode = 3,
	kGoodbyeMessageCode = 6,
	kErrorMessageCode = 8,
	kCallMessageCode = 48,
	kResultMessageCode = 50
};


namespace qwamp {

SessionPrivate::SessionPrivate(QIODevice* io)
	: mIo(io)
	, mState(kNewSessionState)
	, mFrameLength(0)
	, mRequestId(0) {
	if (mIo->openMode() & QIODevice::Unbuffered) throw "todo";
}

void SessionPrivate::start() {
	const quint8 buffer[] = {
		kHandshakeMagic, (kMaxInFrameSizeExponent << 4) + kJsonSerializer, 0x00, 0x00
	};
	const qint64 r = mIo->write(reinterpret_cast<const char*>(buffer), sizeof(buffer));
	if (r == -1) throw "todo";

	connect(mIo, &QIODevice::readyRead, this, &SessionPrivate::onHandshakeReply);
}

void SessionPrivate::join(const QString& realm) {
	QJsonObject roles;
	roles.insert("caller", QJsonObject());
	roles.insert("callee", QJsonObject());
	roles.insert("publisher", QJsonObject());
	roles.insert("subscriber", QJsonObject());
	QJsonObject details;
	details.insert("roles", roles);

	QJsonArray data;
	data.append(kHelloMessageCode);
	data.append(realm);
	data.append(details);

	sendMessage(data);
	mState = kHelloSentSessionState;
}

void SessionPrivate::leave(const QString& reason, const QVariantMap& details) {
 	QJsonArray data;
	data.append(kGoodbyeMessageCode);
	data.append(QJsonObject::fromVariantMap(details));
	data.append(reason);

	sendMessage(data);
	mState = kGoodbyeSentSessionState;
}

QPromise<std::tuple<QVariantList, QVariantMap>, std::tuple<QString, QVariantList, QVariantMap>>
SessionPrivate::call(const QString& procedure, const QVariantList& arguments, const QVariantMap& argumentsKw) {
	mRequestId += 1;

	QJsonArray data;
	data.append(kCallMessageCode);
	data.append(mRequestId);
	data.append(QJsonObject());
	data.append(procedure);
	data.append(QJsonArray::fromVariantList(arguments));
	data.append(QJsonObject::fromVariantMap(argumentsKw));

	auto it = mCallIdMap.emplace(mRequestId, QDeferred<std::tuple<QVariantList, QVariantMap>,
				     std::tuple<QString, QVariantList, QVariantMap>>());
	//return std::move(it->promise());
}

void SessionPrivate::sendMessage(const QJsonArray& data) {
	sendFrame(kRegularFrameType, QJsonDocument(data).toJson());
}

void SessionPrivate::sendFrame(int frameType, const QByteArray& data) {
	Q_ASSERT(frameType < 8);

	if (data.size() > mMaxOutFrameSize) throw "todo";

	const int size = data.size();
	const quint8 buffer[] = {
		static_cast<quint8>(frameType), static_cast<quint8>(size >> 16),
		static_cast<quint8>(size >> 8), static_cast<quint8>(size)
	};
	qint64 r = mIo->write(reinterpret_cast<const char*>(buffer), sizeof(buffer));
	if (r == -1) throw "todo";

	r = mIo->write(data);
	if (r == -1) throw "todo";
}

void SessionPrivate::processGoodbye(const QJsonArray& data) {
	if (data.size() != 3) throw "todo";
	QVariantMap details = data[1].toObject().toVariantMap();
	QString reason = data[2].toString();

	QJsonArray reply;
	reply.append(kGoodbyeMessageCode);
	reply.append(QJsonObject());
	reply.append("wamp.error.goodbye_and_out");
	sendMessage(reply);

	mState = kStartedSessionState;

	Q_Q(Session);
	emit q->left(reason, details);
}

void SessionPrivate::processResult(const QJsonArray& data) {
	if (data.size() < 3) throw "todo";

/*	qint64 requestId = data[1].toVariant().toLongLong();

	QVariantList arguments = (data.size() < 4) ? QVariantList()
		: data[3].toArray().toVariantList();

	QVariantMap argumentsKw = (data.size() < 5) ? QVariantMap()
		: data[4].toObject().toVariantMap();

	Q_Q(Session);
	emit q->result(requestId, arguments, argumentsKw);*/
}

void SessionPrivate::processError(const QJsonArray& data) {
	if (data.size() < 5) throw "todo";

/*	int requestType = data[1].toInt();
	qint64 requestId = data[2].toVariant().toLongLong();
	QString error = data[4].toString();

	QVariantList arguments = (data.size() < 6) ? QVariantList()
		: data[5].toArray().toVariantList();

	QVariantMap argumentsKw = (data.size() < 7) ? QVariantMap()
		: data[6].toObject().toVariantMap();

	Q_Q(Session);
	switch (requestType) {
	case kCallMessageCode: emit q->callError(requestId, error, arguments, argumentsKw); break;
	default: throw "todo";
	}*/
}

void SessionPrivate::onMessage(const QJsonArray& data) {
	switch (data[0].toInt()) {
	case kResultMessageCode: return processResult(data);
	case kErrorMessageCode: return processError(data);
	case kGoodbyeMessageCode: return processGoodbye(data);
	default: throw "todo";
	}
}

void SessionPrivate::onHelloReply(const QJsonArray& data) {
	if (data[0].toInt() == kAbortMessageCode) {
		if (data.size() != 3) throw "todo";

		QVariantMap details = data[1].toObject().toVariantMap();
		QString reason = data[2].toString();

		mState = kStartedSessionState;

		Q_Q(Session);
		emit q->aborted(reason, details);
	}

	if (data[0].toInt() != kWelcomeMessageCode) throw "todo";
	if (data.size() != 3) throw "todo";

	mSessionId = data[1].toVariant().toLongLong();
	mState = kEstablishedSessionState;

	Q_Q(Session);
	emit q->joined();
}

void SessionPrivate::onGoodbyeReply(const QJsonArray& data) {
	if (data[0].toInt() != kGoodbyeMessageCode) throw "todo";
	if (data.size() != 3) throw "todo";

	QVariantMap details = data[1].toObject().toVariantMap();
	QString reason = data[2].toString();

	mState = kStartedSessionState;

	Q_Q(Session);
	emit q->left(reason, details);
}

void SessionPrivate::onMessageReceived(const QByteArray& data) {
	QJsonDocument doc = QJsonDocument::fromJson(data);
	if (!doc.isArray()) throw "todo";

	QJsonArray arr = doc.array();
	if (arr.size() < 1) throw "todo";

	switch (mState) {
	case kEstablishedSessionState: return onMessage(arr);
	case kHelloSentSessionState: return onHelloReply(arr);
	case kGoodbyeSentSessionState: return onGoodbyeReply(arr);
	default: throw "todo";
	}
}

void SessionPrivate::onPingReceived(const QByteArray& data) {
	sendFrame(kPongFrameType, data);
}

void SessionPrivate::onPongReceived(const QByteArray& data) {
	(void)data;
}

void SessionPrivate::onHandshakeReply() {
	if (mIo->bytesAvailable() < 4) return;

	quint8 buffer[4];
	const qint64 r = mIo->read(reinterpret_cast<char*>(buffer), sizeof(buffer));
	if (r < 0) throw "todo";
	if (r != sizeof(buffer)) throw "todo";
	if (buffer[0] != kHandshakeMagic) throw "todo";
	if ((buffer[1] & 0x0F) != kJsonSerializer) throw "todo";

	mMaxOutFrameSize = (2 << ((buffer[1] >> 4) + 9));

	disconnect(mIo, &QIODevice::readyRead, this, &SessionPrivate::onHandshakeReply);
	connect(mIo, &QIODevice::readyRead, this, &SessionPrivate::onDataReceived);

	mState = kStartedSessionState;

	Q_Q(Session);
	emit q->started();
}

void SessionPrivate::onDataReceived() {
	if (mFrameLength == 0) {
		if (mIo->bytesAvailable() < 4) return;
		quint8 buffer[4];
		const qint64 r = mIo->read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		if (r < 0) throw "todo";
		if (r != sizeof(buffer)) throw "todo";
		if ((buffer[0] & 0xF8) != 0) throw "todo";

		mFrameType = buffer[0] & 0xF7;
		if (mFrameType >= kUnknownFrameType) throw "todo";

		mFrameLength = (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
	}

	if (mFrameLength > kMaxInFrameSize) throw "todo";

	if (mIo->bytesAvailable() < mFrameLength) return;

	const QByteArray buffer = mIo->read(mFrameLength);
	if (buffer.size() != mFrameLength) throw "todo";
	mFrameLength = 0;

	switch (mFrameType) {
	case kRegularFrameType: return onMessageReceived(buffer);
	case kPingFrameType: return onPingReceived(buffer);
	case kPongFrameType: return onPongReceived(buffer);
	default: Q_ASSERT(!"Unknown message type");
	}
}

Session::Session(QIODevice* io, QObject* parent)
	: QObject(parent)
	, d_ptr(new SessionPrivate(io)) {
	d_ptr->q_ptr = this;
}

Session::~Session() {
}

void Session::start() {
	Q_D(Session);
	d->start();
}

void Session::stop() {
	Q_D(Session);
	d->mState = kStoppedSessionState;
	emit stopped();
}

void Session::join(const QString& realm) {
	Q_D(Session);
	d->join(realm);
}

void Session::leave(const QString& reason, const QVariantMap& details) {
	Q_D(Session);
	d->leave(reason, details);
}

/*quint64 Session::call(const QString& procedure, const QVariantList& arguments,
		      const QVariantMap& argumentsKw) {
	Q_D(Session);
	return d->call(procedure, arguments, argumentsKw);
	}*/

}
