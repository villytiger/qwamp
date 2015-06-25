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

#ifndef QPROMISEIMPL_H
#define QPROMISEIMPL_H

namespace qpromise {

template<typename... T>
QPromise<T...>::QPromise(QSharedPointer<QPromiseInterface<T...>> p)
	: d(p) {
}

template<typename... T>
QPromise<T...>::QPromise(QPromise&& rhs)
	: d(std::move(rhs.d)) {
}

template<typename... R, typename... E, typename... U>
template<typename T>
QPromise<T> QPromise<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>
	::then(const std::function<T (R...)>& success, const std::function<T (E...)>& error,
	       const std::function<T (U...)>& update) {
	return d->then(success, error, update);
}

template<typename... T>
QDeferred<T...>::QDeferred()
	: d(QSharedPointer<QPromiseInterface<T...>>::create()){
}

template<typename... T>
QDeferred<T...>::QDeferred(QDeferred&& rhs)
	: d(std::move(rhs.d)){
}

template<typename... R, typename... E, typename... U>
QPromise<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>
QDeferred<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>::promise() {
	return d->promise();
}

template<typename... R, typename... E, typename... U>
void QDeferred<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>::setResult(R... r) {
	return d->setResult(r...);
}

template<typename... R, typename... E, typename... U>
void QDeferred<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>::setError(E... e) {
	return d->setError(e...);
}

template<typename... R, typename... E, typename... U>
void QDeferred<std::tuple<R...>, std::tuple<E...>, std::tuple<U...>>::setUpdate(U... u) {
	return d->setUpdate(u...);
}

}

#endif
