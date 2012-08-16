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

#ifndef _PLUGIN_KATE_PROJECT_H_
#define _PLUGIN_KATE_PROJECT_H_

#include <kate/mainwindow.h>
#include <kate/plugin.h>
#include <kxmlguiclient.h>

#include "kateproject.h"

class KateProjectPlugin : public Kate::Plugin
{
  Q_OBJECT

  public:
    explicit KateProjectPlugin( QObject* parent = 0, const QList<QVariant>& = QList<QVariant>() );
    virtual ~KateProjectPlugin();

    Kate::PluginView *createView( Kate::MainWindow *mainWindow );
    
    /**
     * Get project for given project filename.
     * Will open a new one if not already open, else return the already open one.
     * Null pointer if no project can be opened.
     * File name will be canonicalized!
     * @param fileName file name for the project
     * @return project or null if not openable
     */
    KateProject *projectForFileName (const QString &fileName);
    
    /**
     * Search and open project that contains given url, if possible.
     * Will search upwards for .kateproject file, if the url is a local file.
     * Will use internally projectForFileName if a .kateproject file is found.
     * @param url url to search matching project for
     * @return project or null if not openable
     */
    KateProject *projectForUrl (const KUrl &url);
    
    /**
     * get list of all current open projects
     * @return list of all open projects
     */
    QList<KateProject *> projects () const
    {
      return m_fileName2Project.values();
    }
    
  signals:
    /**
     * Signal that a new project got created.
     * @param project new created project
     */
    void projectCreated (KateProject *project);
    
  private slots:
    /**
     * New document got created, we need to update our connections
     * @param document new created document
     */
    void slotDocumentCreated (KTextEditor::Document *document);
    
    /**
     * Url changed, to auto-load projects
     */
    void slotDocumentUrlChanged (KTextEditor::Document *document);
    
  private:
    /**
     * open plugins, map fileName => project
     */
    QMap<QString, KateProject *> m_fileName2Project;
};

#endif

// kate: space-indent on; indent-width 2; replace-tabs on;
