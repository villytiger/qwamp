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


const int kHandshakeMagic = 0x7F;

const int kJsonSerializer = 0x01;
const int kMsgPackSerializer = 0x02;


namespace qwamp {

SessionPrivate::SessionPrivate(QIODevice* io)
	: mIo(io) {
}

SessionPrivate::start() {
	connect(mIo, &QIODevice::bytesWritten, this
	quint8 buffer[] = {kHandshakeMagic, 0xF0 + kJsonSerializer, 0x00, 0x00};
	mIo.write(buffer, sizeof(buffer));
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

	connect(d->mIo, &QIODevice::bytesWritten, this

	quint8 buffer[] = {kHandshakeMagic, 0xF0 + kJsonSerializer, 0x00, 0x00};
	d->mIo.write(buffer, sizeof(buffer));
}

void Session::stop() {
}

}
