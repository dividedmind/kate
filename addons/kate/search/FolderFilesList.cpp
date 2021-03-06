/*   Kate search plugin
 * 
 * Copyright (C) 2013 by Kåre Särs <kare.sars@iki.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program in a file called COPYING; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "FolderFilesList.h"
#include "FolderFilesList.moc"
#include <kmimetype.h>
#include <kdebug.h>

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

FolderFilesList::FolderFilesList(QObject *parent) : QThread(parent) {}

FolderFilesList::~FolderFilesList()
{
    m_cancelSearch = true;
    wait();
}

void FolderFilesList::run()
{
    m_files.clear();

    QFileInfo folderInfo(m_folder);
    checkNextItem(folderInfo);

    if (m_cancelSearch) m_files.clear();
}

void FolderFilesList::generateList(const QString &folder,
                                   bool recursive,
                                   bool hidden,
                                   bool symlinks,
                                   bool binary,
                                   const QString &types,
                                   const QString &excludes)
{
    m_cancelSearch = false;
    m_folder       = folder;
    m_recursive    = recursive;
    m_hidden       = hidden;
    m_symlinks     = symlinks;
    m_binary       = binary;
    m_types        = types.split(',', QString::SkipEmptyParts);

    if (m_types.isEmpty()) {
        m_types << "*";
    }

    QStringList tmpExcludes = excludes.split(',');
    m_excludeList.clear();
    for (int i=0; i<tmpExcludes.size(); i++) {
        QRegExp rx(tmpExcludes[i]);
        rx.setPatternSyntax(QRegExp::Wildcard);
        m_excludeList << rx;
    }

    start();
}

QStringList FolderFilesList::fileList() { return m_files; }

void FolderFilesList::cancelSearch()
{
    m_cancelSearch = true;
}

void FolderFilesList::checkNextItem(const QFileInfo &item)
{
    if (m_cancelSearch) {
        return;
    }
    if (item.isFile()) {
        if (!m_binary && KMimeType::isBinaryData(item.absoluteFilePath())) {
            return;
        }
        m_files << item.absoluteFilePath();
    }
    else {
        QDir currentDir(item.absoluteFilePath());

        if (!currentDir.isReadable()) {
            kDebug() << currentDir.absolutePath() << "Not readable";
            return;
        }

        QDir::Filters    filter  = QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;
        if (m_hidden)    filter |= QDir::Hidden;
        if (m_recursive) filter |= QDir::AllDirs;
        if (!m_symlinks) filter |= QDir::NoSymLinks;

        QFileInfoList currentItems = currentDir.entryInfoList(m_types, filter);

        bool skip;
        for (int i = 0; i<currentItems.size(); ++i) {
            skip = false;
            for (int j=0; j<m_excludeList.size(); j++) {
                if (m_excludeList[j].exactMatch(currentItems[i].fileName())) {
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                checkNextItem(currentItems[i]);
            }
        }
    }
}

