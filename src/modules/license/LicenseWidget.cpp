/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2015, Anke Boersma <demm@kaosx.us>
 *   Copyright 2015, Alexandre Arnt <qtgzmanager@gmail.com>
 *   Copyright 2015, Teo Mrnjavac <teo@kde.org>
 *   Copyright 2018, Adriaan de Groot <groot@kde.org>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#include "LicenseWidget.h"

#include "utils/Logger.h"

#include <QDesktopServices>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

static QString
loadLocalFile( const QUrl& u )
{
    if ( !u.isLocalFile() )
    {
        return QString();
    }

    QFile file( u.path() );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        cWarning() << "Could not load license file" << u.path();
        return QString();
    }

    return QString( "\n" ) + file.readAll();
}

LicenseWidget::LicenseWidget( LicenseEntry entry, QWidget* parent )
    : QWidget( parent )
    , m_entry( std::move( entry ) )
    , m_label( new QLabel( this ) )
    , m_viewLicenseButton( new QPushButton( this ) )
    , m_fullText( nullptr )
    , m_isExpanded( m_entry.expandByDefault() )
{
    QPalette pal( palette() );
    pal.setColor( QPalette::Background, palette().window().color().lighter( 108 ) );

    setObjectName( "licenseItem" );

    setAutoFillBackground( true );
    setPalette( pal );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    setContentsMargins( 4, 4, 4, 4 );

    QHBoxLayout* wiLayout = new QHBoxLayout;

    m_label->setWordWrap( true );
    m_label->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
    wiLayout->addWidget( m_label );

    m_viewLicenseButton->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
    wiLayout->addWidget( m_viewLicenseButton );

    if ( m_entry.isLocal() )
    {
        QVBoxLayout* vLayout = new QVBoxLayout;
        vLayout->addLayout( wiLayout );
        m_fullText = new QLabel( this );
        m_fullText->setText( loadLocalFile( m_entry.m_url ) );
        m_fullText->setStyleSheet( "border-top: 1px solid black; margin-top: 1em; padding-top: 1em;" );
        m_fullText->setObjectName( "licenseItemFullText" );

        m_fullText->setHidden( !m_isExpanded );

        vLayout->addWidget( m_fullText );
        setLayout( vLayout );
        connect( m_viewLicenseButton, &QAbstractButton::clicked, this, &LicenseWidget::expandClicked );
    }
    else
    {
        setLayout( wiLayout );  // Only the horizontal layout needed
        connect( m_viewLicenseButton, &QAbstractButton::clicked, this, &LicenseWidget::viewClicked );
    }

    retranslateUi();
}

LicenseWidget::~LicenseWidget() {}

void
LicenseWidget::retranslateUi()
{
    QString productDescription;
    switch ( m_entry.m_type )
    {
    case LicenseEntry::Type::Driver:
        //: %1 is an untranslatable product name, example: Creative Audigy driver
        productDescription = tr( "<strong>%1 driver</strong><br/>"
                                 "by %2" )
                                 .arg( m_entry.m_prettyName )
                                 .arg( m_entry.m_prettyVendor );
        break;
    case LicenseEntry::Type::GpuDriver:
        //: %1 is usually a vendor name, example: Nvidia graphics driver
        productDescription = tr( "<strong>%1 graphics driver</strong><br/>"
                                 "<font color=\"Grey\">by %2</font>" )
                                 .arg( m_entry.m_prettyName )
                                 .arg( m_entry.m_prettyVendor );
        break;
    case LicenseEntry::Type::BrowserPlugin:
        productDescription = tr( "<strong>%1 browser plugin</strong><br/>"
                                 "<font color=\"Grey\">by %2</font>" )
                                 .arg( m_entry.m_prettyName )
                                 .arg( m_entry.m_prettyVendor );
        break;
    case LicenseEntry::Type::Codec:
        productDescription = tr( "<strong>%1 codec</strong><br/>"
                                 "<font color=\"Grey\">by %2</font>" )
                                 .arg( m_entry.m_prettyName )
                                 .arg( m_entry.m_prettyVendor );
        break;
    case LicenseEntry::Type::Package:
        productDescription = tr( "<strong>%1 package</strong><br/>"
                                 "<font color=\"Grey\">by %2</font>" )
                                 .arg( m_entry.m_prettyName )
                                 .arg( m_entry.m_prettyVendor );
        break;
    case LicenseEntry::Type::Software:
        productDescription = tr( "<strong>%1</strong><br/>"
                                 "<font color=\"Grey\">by %2</font>" )
                                 .arg( m_entry.m_prettyName )
                                 .arg( m_entry.m_prettyVendor );
    }
    m_label->setText( productDescription );
    updateExpandToolTip();
}

void
LicenseWidget::expandClicked()
{
    m_isExpanded = !m_isExpanded;
    // Show/hide based on the new arrow direction.
    if ( m_fullText )
    {
        m_fullText->setHidden( !m_isExpanded );
    }

    updateExpandToolTip();
}

/** @brief Called on retranslate and when button state changes. */
void
LicenseWidget::updateExpandToolTip()
{
    if ( m_entry.isLocal() )
    {
        m_viewLicenseButton->setText( m_isExpanded ? tr( "Hide license text" ) : tr( "Show the license text" ) );
    }
    else
    {
        m_viewLicenseButton->setText( tr( "Open license agreement in browser." ) );
    }
}

void
LicenseWidget::viewClicked()
{
    QDesktopServices::openUrl( m_entry.m_url );
}
