/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2013-2015  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "ui_html5permissionsnotification.h"
#include "html5permissionsnotification.h"
#include "html5permissionsmanager.h"
#include "mainapplication.h"
#include "iconprovider.h"

#include <QWebEnginePage>

HTML5PermissionsNotification::HTML5PermissionsNotification(const QUrl &origin, QWebEnginePage* page, const QWebEnginePage::Feature &feature)
    : AnimatedWidget(AnimatedWidget::Down, 300, 0)
    , ui(new Ui::HTML5PermissionsNotification)
    , m_origin(origin)
    , m_page(page)
    , m_feature(feature)
{
    setAutoFillBackground(true);
    ui->setupUi(widget());

    ui->close->setIcon(IconProvider::standardIcon(QStyle::SP_DialogCloseButton));

    const QString site = m_origin.host().isEmpty() ? tr("this site") : QString("<b>%1</b>").arg(m_origin.host());

    switch (feature) {
    case QWebEnginePage::Notifications:
        ui->iconLabel->setPixmap(QPixmap(":icons/other/notification.png"));
        ui->textLabel->setText(tr("Allow %1 to show desktop notifications?").arg(site));
        break;

    case QWebEnginePage::Geolocation:
        ui->iconLabel->setPixmap(QPixmap(":icons/other/geolocation.png"));
        ui->textLabel->setText(tr("Allow %1 to locate your position?").arg(site));
        break;

    case QWebEnginePage::MediaAudioCapture:
        ui->iconLabel->setPixmap(QPixmap(":icons/other/audiocapture.png"));
        ui->textLabel->setText(tr("Allow %1 to use your microphone?").arg(site));
        break;

    case QWebEnginePage::MediaVideoCapture:
        ui->iconLabel->setPixmap(QPixmap(":icons/other/webcam.png"));
        ui->textLabel->setText(tr("Allow %1 to use your web camera?").arg(site));
        break;

    case QWebEnginePage::MediaAudioVideoCapture:
        ui->iconLabel->setPixmap(QPixmap(":icons/other/microphone-webcam.png"));
        ui->textLabel->setText(tr("Allow %1 to use your microphone and web camera?").arg(site));
        break;

    case QWebEnginePage::MouseLock:
        ui->iconLabel->setPixmap(QPixmap(":icons/other/mouselock.png"));
        ui->textLabel->setText(tr("Allow %1 to hide your pointer?").arg(site));
        break;

    default:
        qWarning() << "Unknown feature" << feature;
        break;
    }

    connect(ui->allow, SIGNAL(clicked()), this, SLOT(grantPermissions()));
    connect(ui->deny, SIGNAL(clicked()), this, SLOT(denyPermissions()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(denyPermissions()));

    startAnimation();
}

void HTML5PermissionsNotification::grantPermissions()
{
    if (!m_page) {
        return;
    }

    m_page->setFeaturePermission(m_origin, m_feature, QWebEnginePage::PermissionGrantedByUser);

    if (ui->remember->isChecked()) {
        mApp->html5PermissionsManager()->rememberPermissions(m_origin, m_feature, QWebEnginePage::PermissionGrantedByUser);
    }

    hide();
}

void HTML5PermissionsNotification::denyPermissions()
{
    if (!m_page) {
        return;
    }

    m_page->setFeaturePermission(m_origin, m_feature, QWebEnginePage::PermissionDeniedByUser);

    if (ui->remember->isChecked()) {
        mApp->html5PermissionsManager()->rememberPermissions(m_origin, m_feature, QWebEnginePage::PermissionDeniedByUser);
    }

    hide();
}

HTML5PermissionsNotification::~HTML5PermissionsNotification()
{
    delete ui;
}
