/*****************************************************************************
*   Copyright (C) 2011, 2012 by Shaun Reich <shaun.reich@kdemail.net>        *
*   Copyright (C) 2008 by Montel Laurent <montel@kde.org>                    *
*                                                                            *
*   This program is free software; you can redistribute it and/or            *
*   modify it under the terms of the GNU General Public License as           *
*   published by the Free Software Foundation; either version 2 of           *
*   the License, or (at your option) any later version.                      *
*                                                                            *
*   This program is distributed in the hope that it will be useful,          *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*   GNU General Public License for more details.                             *
*                                                                            *
*   You should have received a copy of the GNU General Public License        *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
*****************************************************************************/

#include "kateprofilesengine.h"
#include "kateprofilesservice.h"

#include <KStandardDirs>
#include <KDirWatch>
#include <QFileInfo>
#include <kio/global.h>
#include <KGlobalSettings>
#include <KDebug>

KateProfilesEngine::KateProfilesEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args),
      m_dirWatch(0)
{

}

KateProfilesEngine::~KateProfilesEngine()
{
}

void KateProfilesEngine::init()
{
    m_dirWatch = new KDirWatch( this );
    loadProfiles();
    connect(m_dirWatch, SIGNAL(dirty(QString)), this, SLOT(profilesChanged()));
}

Plasma::Service *KateProfilesEngine::serviceForSource(const QString &source)
{
    //create a new service for this profile's name, so it can be operated on.
    return new KateProfilesService(this, source);
}

void KateProfilesEngine::profilesChanged()
{
    //wipe the data clean, load it again. (there's not a better way of doing this but no big deal)
    removeAllSources();
    loadProfiles();
}

void KateProfilesEngine::loadProfiles()
{
    QStringList sessions = QStringList();
    const QStringList list = KGlobal::dirs()->findAllResources( "data", QLatin1String("kate/sessions/*.katesession"), KStandardDirs::NoDuplicates );
    kDebug() << "FOUND LIST: " << list;
    KUrl url;
    for (QStringList::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it)
    {
        /*        KConfig _config( *it, KConfig::SimpleConfig );
         *   KConfigGroup config(&_config, "General" );
         *   QString name =  config.readEntry( "Name" );*/
        url.setPath(*it);
        QString name=url.fileName();
        kDebug() << "FOUND NAME: " << name;
        name = QUrl::fromPercentEncoding(QFile::encodeName(url.fileName()));
        kDebug() << "translated: " << name;
        name.chop(12);///.katesession==12
        setData("name:" + name, QVariant());
    }
}

K_EXPORT_PLASMA_DATAENGINE(katesessionsengine, KateProfilesEngine)

#include "kateprofilesengine.moc"
