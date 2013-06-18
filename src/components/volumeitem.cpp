/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <Kommodity/GIO/Icon>
#include <Kommodity/GIO/ThemedIcon>

#include "volumeitem.h"

using namespace Kommodity::GIO;

VolumeItem::VolumeItem(Kommodity::GIO::Volume *volume)
    : PlacesItem()
    , m_volume(volume)
{
    setCategory(QObject::tr("Devices"));
    setEditable(false);
    update();
}

void VolumeItem::update()
{
    setText(m_volume->getName());
    setIconFromVolume();

    if (m_volume->getMount())
        setUrl(m_volume->getMount()->getRoot().getUri());
}

bool VolumeItem::isMounted()
{
    if (m_volume->getMount())
        return true;
    else
        return false;
}

void VolumeItem::setVolume(Kommodity::GIO::Volume *volume)
{
    m_volume = volume;
    update();
}

void VolumeItem::setIconFromVolume()
{
    Q_ASSERT(m_volume);

    Icon *icon = m_volume->getIcon();
    ThemedIcon *themedIcon = qobject_cast<ThemedIcon *>(icon);
    if (themedIcon) {
        foreach (QString iconName, themedIcon->getNames()) {
            if (QIcon::hasThemeIcon(iconName)) {
                setIcon(QIcon::fromTheme(iconName));
                return;
            }
        }
    }

    setIcon(QIcon(icon->fileName()));
}
