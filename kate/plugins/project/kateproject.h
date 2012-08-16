/*  This file is part of the Kate project.
 *
 *  Copyright (C) 2010 Christoph Cullmann <cullmann@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef KATE_PROJECT_H
#define KATE_PROJECT_H

#include <QThread>
#include <QStandardItemModel>
#include <QMap>

#include "kateprojectitem.h"

/**
 * Class representing a project.
 * Holds project properties like name, groups, contained files, ...
 */
class KateProject : public QObject
{
  Q_OBJECT

  public:
    /**
     * construct empty project
     */
    KateProject ();

    /**
     * deconstruct project
     */
    ~KateProject ();

    /**
     * Load a project.
     * Only works once, afterwards use reload().
     * @param fileName name of project file
     * @return success
     */
    bool load (const QString &fileName);

    /**
     * Try to reload a project.
     * If the reload fails, e.g. because the file is not readable or corrupt, nothing will happen!
     * @return success
     */
    bool reload ();

    /**
     * Accessor to file name.
     * @return file name
     */
    const QString &fileName () const
    {
      return m_fileName;
    }

    /**
     * Accessor to project name.
     * @return project name
     */
    const QString &name () const
    {
      return m_name;
    }

    /**
     * Accessor to project map containing the whole project info.
     * @return project info
     */
    const QVariantMap &projectMap () const
    {
      return m_projectMap;
    }

    /**
     * Accessor for the model.
     * @return model of this project
     */
    QStandardItemModel *model ()
    {
      return m_model;
    }

    /**
     * Flat list of all files in the project
     */
    QStringList files ()
    {
      return m_file2Item.keys ();
    }

    /**
     * get item for file
     * @param file file to get item for
     * @return item for given file or 0
     */
    QStandardItem *itemForFile (const QString &file)
    {
      return m_file2Item.value (file);
    }

  private:
    /**
     * Load one project inside the project tree.
     * Fill data from JSON storage to model and recurse to sub-projects.
     * @param parent parent standard item in the model
     * @param project variant map for this group
     */
    void loadProject (QStandardItem *parent, const QVariantMap &project);

    /**
     * Load one files entry in the current parent item.
     * @param parent parent standard item in the model
     * @param filesEntry one files entry specification to load
     */
    void loadFilesEntry (QStandardItem *parent, const QVariantMap &filesEntry);

  private:
    /**
     * our internal thread to load stuff and do things in background
     */
    QThread m_thread;
    
    /**
     * the worker inside the background thread
     */
    class KateProjectWorker *m_worker;
    
    /**
     * project file name
     */
    QString m_fileName;

    /**
     * project name
     */
    QString m_name;
    
    /**
     * variant map representing the project
     */
    QVariantMap m_projectMap;

    /**
     * standard item model with content of this project
     */
    QStandardItemModel *m_model;

    /**
     * mapping files => items
     */
    QMap<QString, QStandardItem *> m_file2Item;
};

#endif

// kate: space-indent on; indent-width 2; replace-tabs on;