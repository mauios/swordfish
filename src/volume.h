/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef VOLUME_H
#define VOLUME_H

#include <Kommodity/GIO/Volume>
#include <Kommodity/GIO/Mount>
#include <Kommodity/GIO/File>

#include "placesitem.h"

class Volume : public PlacesItem
{
public:
    bool isMounted();
    void update();
    Volume(const Kommodity::GIO::Volume &volume);

    bool canEject()const {
        return m_volume->canEject();
    }

    Kommodity::GIO::Volume *volume()const {
        return m_volume;
    }

private:
    Kommodity::GIO::Volume *m_volume;

};

#endif // VOLUME_H
