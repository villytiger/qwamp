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

#ifndef QPROMISE_H
#define QPROMISE_H

#include "private/qpromiseinterface.h"


namespace qpromise {

template<typename... T>
class QPromise {
	Q_DISABLE_COPY(QPromise)

	QSharedPointer<QPromiseInterface<T...>> d;

public:
	explicit QPromise(QSharedPointer<QPromiseInterface<T...>> p);
	QPromise(QPromise&& rhs);
};

template<typename... R, typename... E, typename... U>
class QPromise<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>> {
	QSharedPointer<QPromiseInterface<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>> d;

public:
	template<typename T>
	QPromise<T> then(const std::function<T (R...)>& success,
			 const std::function<T (E...)>& error = std::function<T (E...)>(),
		         const std::function<T (U...)>& update = std::function<T (U...)>());
};

template<typename... T>
class QDeferred {
	Q_DISABLE_COPY(QDeferred)

	QSharedPointer<QPromiseInterface<T...>> d;

public:
	QDeferred();
	QDeferred(QDeferred&& rhs);
};

template<typename... R, typename... E, typename... U>
class QDeferred<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>> {
	QSharedPointer<QPromiseInterface<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>> d;

public:
	QPromise<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>> promise();

	void setResult(R...);
	void setError(E...);
	void setUpdate(U...);
};

}


#include "private/qpromiseimpl.h"

#endif
