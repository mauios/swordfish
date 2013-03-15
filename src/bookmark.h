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

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QIcon>
#include <QDir>
#include <QUrl>
#include <QStandardPaths>

#include "placesitem.h"

class Bookmark : public PlacesItem
{
public:
    Bookmark(const QString &text, const QString &icon, const QUrl &url);

    void setText(const QString &text);
    void setIcon(const QString &icon);
    void setUrl(const QUrl &url);

    ~Bookmark();

    QString text() const
    {
        return m_text;
    }

    QUrl  url() const
    {
        return m_url;
    }

    QIcon icon() const
    {
        return m_icon;
    }

private:
    QString m_text;
    QUrl m_url;
    QIcon m_icon;
};

#endif // BOOKMARK_H
