/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
    ---
    Copyright (C) 2007,2009 Joseph Wenninger <jowenn@kde.org>
*/

#ifndef KATE_QUICK_OPEN_H
#define KATE_QUICK_OPEN_H

#include <kate/plugin.h>
#include <kate/application.h>
#include <kate/documentmanager.h>
#include <kate/mainwindow.h>

#include <kdialog.h>

#include <QPointer>
#include <qsortfilterproxymodel.h>

class KateMainWindow;
class KLineEdit;

class QListView;
class QModelIndex;
class QStandardItemModel;
class QTreeView;

namespace KTextEditor {
    class Document;
}

class KateQuickOpen : public QWidget {
    Q_OBJECT
    public:
        KateQuickOpen(QWidget *parent, KateMainWindow *mainWindow);
        /**
         * update state
         * will fill model with current open documents, project documents, ...
         */
        void update ();

        /**
         * super fast quick switch
         */
        void justSwitchToPreviousDocument ();

    protected:
        bool eventFilter(QObject *obj, QEvent *event);

    private Q_SLOTS:
        void reselectFirst();

        /**
         * Return pressed, activate the selected document
         * and go back to background
         */
        void slotReturnPressed ();

        /**
         * Switch to a document with given index
         */
        void switchTo (const QModelIndex&);

    private:
        KateMainWindow *m_mainWindow;
        QTreeView *m_listView;
        KLineEdit *m_inputLine;

        /**
         * our model we search in
         */
        QStandardItemModel *m_base_model;

        /**
         * filtered model we search in
         */
        QSortFilterProxyModel *m_model;
};

#endif
