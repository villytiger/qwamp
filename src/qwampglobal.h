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

#ifndef QWMAPGLOBAL_P_H
#define QWAMPGLOBAL_P_H

#ifndef QWAMP_NAMESPACE

#define QWAMP_BEGIN_NAMESPACE
#define QWAMP_END_NAMESPACE

#else

#define QWAMP_BEGIN_NAMESPACE namespace QWAMP_NAMESPACE {
#define QWAMP_END_NAMESPACE }

#endif

#endif
