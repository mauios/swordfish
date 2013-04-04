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

#include "placesmodel.h"

PlacesModel::PlacesModel(QObject* parent)
    : QStandardItemModel(parent),
      m_showApplications(true),
      m_showDesktop(true),
      m_showTrash(true)
{   
    // Set role names
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Qt::DecorationRole] = "decoration";
    roles[PlacesItem::CategoryRole] = "category";
    roles[PlacesItem::UrlRole] = "url";
    setItemRoleNames(roles);

    m_volumeMonitor = new Kommodity::GIO::VolumeMonitor();

    PlacesItem *item;

    m_home = new PlacesItem("user-home",
                            QStandardPaths::displayName(QStandardPaths::HomeLocation),
                            QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_home->setEditable(false);
    appendRow(m_home);

    m_desktop = new PlacesItem("user-desktop", tr("Desktop"),
                               QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_desktop->setEditable(false);
    appendRow(m_desktop);

    m_filesystemUrl.setUrl("file:///");
    m_filesystem = new PlacesItem("volume", tr("File system"), m_filesystemUrl);
    m_filesystem->setEditable(false);
    appendRow(m_filesystem);

    m_trashUrl.setUrl("trash:///");
    m_trash = new PlacesItem("user-trash", tr("Trash"), m_trashUrl);
    m_trash->setEditable(false);
    appendRow(m_trash);

    m_computerUrl.setUrl("computer:///");
    m_computer = new PlacesItem("computer", tr("Computer"), m_computerUrl);
    m_computer->setEditable(false);
    appendRow(m_computer);

    m_networkRoot = new QStandardItem(tr("Network"));
    m_networkRoot->setEditable(false);
    m_networkRoot->setSelectable(false);
    appendRow(m_networkRoot);
    m_networkUrl.setUrl("network:///");
    m_network = new PlacesItem("network", tr("Network"), m_networkUrl);
    m_network->setEditable(false);
    appendRow(m_network);

    // Find all volumes and append them to m_devicesRoot
    QList<Kommodity::GIO::Volume *> volumes = m_volumeMonitor->getVolumes();

    for (int i = 0; i < volumes.size(); ++i) {
        Kommodity::GIO::Volume *volume = volumes.at(i);
        item = new Volume(*volume);
        appendRow(item);
    }

    // Find all  mounts and append them to m_devicesRoot
    QList<Kommodity::GIO::Mount *> mounts = m_volumeMonitor->getMounts();

    for (int i = 0; i < mounts.size(); ++i) {
        Kommodity::GIO::Mount *mount = mounts.at(i);
        Kommodity::GIO::Volume *volume = mount->getVolume();

        if (!volume) {
            item = new Mount(*mount);
            appendRow(item);
        }
    }

    // Bookmarks
    addBookmark("folder-documents-symbolic",tr("Documents"),
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    addBookmark("folder-music-symbolic",tr("Music"),
                QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
    addBookmark("folder-pictures-symbolic",tr("Pictures"),
                QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    addBookmark("folder-videos-symbolic",tr("Videos"),
                QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    addBookmark("folder-download-symbolic",tr("Downloads"),
                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

#if 0
    connect(m_volumeMonitor, SIGNAL(mountAdded(const VolumeMonitor *,const Mount *)),
            this, SLOT(mountAdded(Kommodity::GIO::VolumeMonitor *, Kommodity::GIO::Mount *, PlacesModel *)));
#endif
}

void PlacesModel::mountAdded(Kommodity::GIO::VolumeMonitor *volumeMonitor,
                             Kommodity::GIO::Mount *mount, PlacesModel *placesModel)
{
    Kommodity::GIO::Volume *volume = mount->getVolume();
    if (!volume)
        return;

    Volume *volumeItem = placesModel->itemFromVolume(volume);

    if (volumeItem && volumeItem->url().isEmpty()) {
        Kommodity::GIO::File file = mount->getRoot();
        QUrl murl = file.getUri();
        volumeItem->setUrl(murl);
    } else {
        Mount *mountItem = placesModel->itemFromMount(mount);
        if (!mountItem) {
            mountItem = new Mount(*mount);
            appendRow(mountItem);
        }
    }
}

void PlacesModel::addBookmark(const QString &icon, const QString &text, const QUrl &url)
{
    Bookmark *item = new Bookmark(text, icon, url);
    m_items.append(item);
    appendRow(item);
}

void PlacesModel::removeBookmark(const QString &text, const QUrl &url)
{
    for(int i = 0; i < m_items.length(); i++) {
        Bookmark *item = static_cast<Bookmark *>(m_items.at(i));
        if (!item)
            continue;

        if (item->text() == text && item->url() == url)
            m_items.removeAt(i);
    }
}

Volume *PlacesModel::itemFromVolume(Kommodity::GIO::Volume *volume)
{
    for (int i = 0; i < m_items.length(); ++i) {
        PlacesItem *item = static_cast<PlacesItem *>(m_items.at(i));
        Volume *volumeItem = static_cast<Volume *>(item);
        if (volumeItem)
            return volumeItem;
    }

    return 0;
}

Mount *PlacesModel::itemFromMount(Kommodity::GIO::Mount *mount)
{
    for (int i = 0; i < m_items.length(); ++i) {
        PlacesItem *item = static_cast<PlacesItem *>(m_items.at(i));
        Mount *mountItem = static_cast<Mount *>(item);
        if (mountItem)
            return mountItem;
    }

    return 0;
}

Bookmark *PlacesModel::itemFromBookmark(Bookmark *item)
{
    return 0;
}

#include "moc_placesmodel.cpp"