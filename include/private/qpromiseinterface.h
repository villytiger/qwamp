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

#ifndef QPROMISEINTERFACE_H
#define QPROMISEINTERFACE_H

#include <functional>
#include <tuple>

#include <QObject>


namespace qpromise {

template<typename... T>
class QPromise;

template<typename... T>
class QPromiseInterface {
};

template<typename... R, typename... E, typename... U>
class QPromiseInterface<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>> {
	Q_DISABLE_COPY(QPromiseInterface)

public:
	QPromiseInterface(QPromiseInterface&&);

	template<typename T>
	QPromise<T> then(const std::function<T (R...)>& success,
			 const std::function<T (E...)>& error = std::function<T (E...)>(),
		         const std::function<T (U...)>& update = std::function<T (U...)>());

	QPromise<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>> promise();

	void setResult(R...);
	void setError(E...);
	void setUpdate(U...);
};

}

#endif
