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

#include <QDateTime>
#include <QDir>
#include <QFileSystemModel>
#include <QMimeDatabase>
#include <QMimeType>
#include <QIcon>

#include <VibeCore/VFileString>

#include "infodialog.h"
#include "ui_infodialog.h"
#include "fileviewcontroller.h"
#include "directorysizejob.h"

InfoDialog::InfoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InfoDialog)
    , m_model(0)
{
    ui->setupUi(this);

    connect(ui->ownerReadButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->ownerWriteButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->ownerExecuteButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->groupReadButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->groupWriteButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->groupExecuteButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->otherReadButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->otherWriteButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
    connect(ui->otherExecuteButton, SIGNAL(toggled(bool)),
            this, SLOT(permissionChanged()));
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::setModel(QFileSystemModel *model)
{
    m_model = model;
}

void InfoDialog::setModelIndexList(const QModelIndexList &list)
{
    Q_ASSERT(m_model);

    if (list.size() == 1) {
        QFileInfo info = m_model->fileInfo(list.at(0));
        setWindowTitle(tr("%1 Properties").arg(info.fileName()));
        setupForOneItem(list.at(0));
    } else {
        setupForMultipleItems(list);
    }
}

void InfoDialog::setupForOneItem(const QModelIndex &index)
{
    QFileInfo info = m_model->fileInfo(index);

    m_infos.clear();
    m_infos.append(info);

    QMimeDatabase mimeDb;
    QMimeType mimeType = mimeDb.mimeTypeForFile(info.absoluteFilePath());

    ui->fileIcon->setPixmap(m_model->fileIcon(index).pixmap(32));
    ui->fileName->setText(info.fileName());

    ui->fileCreationTime->setText(info.created().toString());
    ui->fileAccessTime->setText(info.lastRead().toString());
    ui->fileModificationTime->setText(info.lastModified().toString());
    ui->fileMimeType->setText(mimeType.name());
    ui->fileLocation->setText(info.absolutePath());
    ui->fileVolume->setText(info.absoluteDir().rootPath());

    ui->fileOwner->setText(info.owner());
    //ui->fileGroupCombo->
    ui->ownerReadButton->setChecked(info.permission(QFile::ReadOwner));
    ui->ownerWriteButton->setChecked(info.permission(QFile::WriteOwner));
    ui->ownerExecuteButton->setChecked(info.permission(QFile::ExeOwner));
    ui->groupReadButton->setChecked(info.permission(QFile::ReadGroup));
    ui->groupWriteButton->setChecked(info.permission(QFile::WriteGroup));
    ui->groupExecuteButton->setChecked(info.permission(QFile::ExeGroup));
    ui->otherReadButton->setChecked(info.permission(QFile::ReadOther));
    ui->otherWriteButton->setChecked(info.permission(QFile::WriteOther));
    ui->otherExecuteButton->setChecked(info.permission(QFile::ExeOther));

    if (info.isDir()) {
        // Set the type label for starters
        ui->fileTypeAndSize->setText(mimeType.comment());

        // Hide contents not needed for folders
        ui->openWithGroupBox->setVisible(false);

        // Calculate the source directory size
        VJob *job = new DirectorySizeJob(info.absolutePath());
        connect(job, SIGNAL(result(VJob *)), this, SLOT(directorySizeCalculated(VJob *)));
        //job->start();
    } else {
        ui->fileTypeAndSize->setText(tr("%1 - %2")
                                     .arg(VFileString::forSize(info.size()))
                                     .arg(mimeType.comment()));
    }
}

void InfoDialog::setupForMultipleItems(const QModelIndexList &list)
{
    m_infos.clear();
    for (int i = 0; i < list.size(); i++) {
        QFileInfo info = m_model->fileInfo(list.at(i));
        m_infos.append(info);
    }

    ui->fileIcon->setPixmap(QIcon::fromTheme("document-multiple").pixmap(32));
    ui->fileName->setText(tr("%1 files").arg(list.size()));

    // Hide parts of the UI
    ui->fileCreationTime->setVisible(false);
    ui->fileAccessTime->setVisible(false);
    ui->fileModificationTime->setVisible(false);
    ui->fileMimeType->setVisible(false);
    ui->openWithGroupBox->setVisible(false);
}

void InfoDialog::directorySizeCalculated(VJob *job)
{
    DirectorySizeJob *dirSizeJob = qobject_cast<DirectorySizeJob *>(job);
    if (dirSizeJob) {
        ui->fileTypeAndSize->setText(tr("Folder with %1 items, %2")
                                     .arg(dirSizeJob->totalFiles())
                                     .arg(VFileString::forSize(dirSizeJob->totalSize())));
    }

    job->deleteLater();
}

void InfoDialog::permissionChanged()
{
    for (int i = 0; i < m_infos.size(); i++) {
        QFileInfo info = m_infos.at(i);

        QFile::Permissions permissions = 0;
        if (ui->ownerReadButton->isChecked())
            permissions |= QFile::ReadOwner;
        if (ui->ownerWriteButton->isChecked())
            permissions |= QFile::WriteOwner;
        if (ui->ownerExecuteButton->isChecked())
            permissions |= QFile::ExeOwner;
        if (ui->otherReadButton->isChecked())
            permissions |= QFile::ReadOther;
        if (ui->otherWriteButton->isChecked())
            permissions |= QFile::WriteOther;
        if (ui->otherExecuteButton->isChecked())
            permissions |= QFile::ExeOther;
        if (ui->groupReadButton->isChecked())
            permissions |= QFile::ReadGroup;
        if (ui->groupWriteButton->isChecked())
            permissions |= QFile::WriteGroup;
        if (ui->groupExecuteButton->isChecked())
            permissions |= QFile::ExeGroup;

        QFile::setPermissions(info.absoluteFilePath(), permissions);
    }
}

#include "moc_infodialog.cpp"
