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

#ifndef PLACESMODEL_H
#define PLACESMODEL_H

#include <QStandardItemModel>
#include <Kommodity/GIO/VolumeMonitor>
#include <Kommodity/GIO/FileMonitor>
#include <QIcon>
#include <QList>
#include <QDir>
#include <QUrl>
#include <QStandardPaths>
#include <QObject>

#include "bookmark.h"

class PlacesModel : public QStandardItemModel
{
public:
    PlacesModel();

    ~PlacesModel();

    int addBookmark(const QString &icon,const QString &text,const QUrl &url);
    void removeBookmark(const QString &text, const QUrl &url);

private:
    Kommodity::GIO::VolumeMonitor *m_volumeMonitor;
    PlacesItem *m_trash;
    PlacesItem *m_desktop;
    PlacesItem *m_home;
    PlacesItem *m_computer;
    PlacesItem *m_network;
    QList<Bookmark *> m_bookmarks;
};

#endif // PLACESMODEL_H

