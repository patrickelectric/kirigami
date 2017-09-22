/*
*   Copyright (C) 2017 by Marco Martin <mart@kde.org>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU Library General Public License as
*   published by the Free Software Foundation; either version 2, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Library General Public License for more details
*
*   You should have received a copy of the GNU Library General Public
*   License along with this program; if not, write to the
*   Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "basictheme_p.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QPalette>
#include <QDebug>
#include <QQuickWindow>
#include <QTimer>

namespace Kirigami {

class BasicThemeDeclarativeSingleton
{
public:
    BasicThemeDeclarativeSingleton()
    {}

    BasicThemeDeclarative self;
};

Q_GLOBAL_STATIC(BasicThemeDeclarativeSingleton, privateBasicThemeDeclarativeSelf)

BasicThemeDeclarative::BasicThemeDeclarative()
{
    m_colorSyncTimer = new QTimer;
    m_colorSyncTimer->setInterval(0);
    m_colorSyncTimer->setSingleShot(true);
}

BasicThemeDeclarative::~BasicThemeDeclarative()
{
    delete m_colorSyncTimer;
}

void BasicThemeDeclarative::setQmlPath(const QUrl &path)
{
    m_qmlPath = path;
}

QUrl BasicThemeDeclarative::qmlPath() const
{
    return m_qmlPath;
}

QObject *BasicThemeDeclarative::instance(const BasicTheme *theme)
{
    if (m_declarativeBasicTheme) {
        return m_declarativeBasicTheme;
    }

    QQmlEngine *engine = qmlEngine(theme->parent());
    Q_ASSERT(engine);

    QQmlComponent c(engine);
    c.loadUrl(m_qmlPath);

    m_declarativeBasicTheme = c.create();

    return m_declarativeBasicTheme;
}



BasicTheme::BasicTheme(QObject *parent)
    : PlatformTheme(parent)
{
    //TODO: correct?
    connect(qApp, &QGuiApplication::fontDatabaseChanged, this, [this]() {setDefaultFont(qApp->font());});

    //connect all the declarative object signals to the timer start to compress, use the old syntax as they are all signals defined in QML
    connect(basicThemeDeclarative()->instance(this), SIGNAL(textColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(disabledTextColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(highlightColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(highlightedTextColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(backgroundColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(linkColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(visitedLinkColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));

    connect(basicThemeDeclarative()->instance(this), SIGNAL(buttonTextColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(buttonBackgroundColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(buttonHoverColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(buttonFocusColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));

    connect(basicThemeDeclarative()->instance(this), SIGNAL(viewTextColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(viewBackgroundColorChanged()), 
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(viewHoverColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(viewFocusColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));

    connect(basicThemeDeclarative()->instance(this), SIGNAL(complementaryTextColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(complementaryBackgroundColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(complementaryHoverColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));
    connect(basicThemeDeclarative()->instance(this), SIGNAL(complementaryFocusColorChanged()),
            basicThemeDeclarative()->m_colorSyncTimer, SLOT(start()));

    //finally connect the timer to the sync
    connect(basicThemeDeclarative()->m_colorSyncTimer, &QTimer::timeout,
            this, &BasicTheme::syncColors);
    connect(this, &BasicTheme::colorSetChanged,
            this, &BasicTheme::syncColors);
    syncColors();
}

BasicTheme::~BasicTheme()
{
}

QStringList BasicTheme::keys() const
{
    QStringList props;
    for (int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i) {
        const QString prop = metaObject()->property(i).name();
        if (prop != "keys") {
            props << prop;
        }
    }
    return props;
}

//TODO: tint for which we need to chain to m_parentBasicTheme's color
#define RESOLVECOLOR(colorName, upperCaseColor) \
    QColor color;\
    switch (colorSet()) {\
    case BasicTheme::Button:\
        color = basicThemeDeclarative()->instance(this)->property("button"#upperCaseColor).value<QColor>();\
        break;\
    case BasicTheme::View:\
        color = basicThemeDeclarative()->instance(this)->property("view"#upperCaseColor).value<QColor>();\
        break;\
    case BasicTheme::Complementary:\
        color = basicThemeDeclarative()->instance(this)->property("complementary"#upperCaseColor).value<QColor>();\
        break;\
    case BasicTheme::Window:\
    default:\
        color = basicThemeDeclarative()->instance(this)->property(#colorName).value<QColor>();\
    }\


#define PROXYCOLOR(colorName, upperCaseColor) \
    basicThemeDeclarative()->instance(this)->property(#colorName).value<QColor>()


void BasicTheme::syncColors()
{
    {
        RESOLVECOLOR(textColor, TextColor);
        setTextColor(color);
    }{
        setDisabledTextColor(PROXYCOLOR(disabledTextColor, DisabledTextColor));
    }{
        RESOLVECOLOR(backgroundColor, BackgroundColor)
        setBackgroundColor(color);
    }{
        setHighlightColor(PROXYCOLOR(highlightColor, HighlightColor));
    }{
        setHighlightedTextColor(PROXYCOLOR(highlightedTextColor, HighlightedTextColor));
    }{
        setLinkColor(PROXYCOLOR(linkColor, LinkColor));
    }{
        setVisitedLinkColor(PROXYCOLOR(visitedLinkColor, VisitedLinkColor));
    }

    //legacy
    {
        m_buttonTextColor = PROXYCOLOR(buttonTextColor, ButtonTextColor);
        m_buttonBackgroundColor = PROXYCOLOR(buttonBackgroundColor, ButtonBackgroundColor);
        m_buttonHoverColor = PROXYCOLOR(buttonHoverColor, ButtonHoverColor);
        m_buttonFocusColor = PROXYCOLOR(buttonFocusColor, ButtonFocusColor);
        m_viewTextColor = PROXYCOLOR(viewTextColor, ViewTextColor);
        m_viewBackgroundColor = PROXYCOLOR(viewBackgroundColor, ViewBackgroundColor);
        m_viewHoverColor = PROXYCOLOR(viewHoverColor, ViewHoverColor);
        m_viewFocusColor = PROXYCOLOR(viewFocusColor, ViewFocusColor);
    }
    //TODO: build the qpalette
    emit colorsChanged();
}

QColor BasicTheme::buttonTextColor() const
{
    return m_buttonTextColor;
}

QColor BasicTheme::buttonBackgroundColor() const
{
    return m_buttonBackgroundColor;
}

QColor BasicTheme::buttonHoverColor() const
{
    return m_buttonHoverColor;
}

QColor BasicTheme::buttonFocusColor() const
{
    return m_buttonFocusColor;
}


QColor BasicTheme::viewTextColor() const
{
    return m_viewTextColor;
}

QColor BasicTheme::viewBackgroundColor() const
{
    return m_viewBackgroundColor;
}

QColor BasicTheme::viewHoverColor() const
{
    return m_viewHoverColor;
}

QColor BasicTheme::viewFocusColor() const
{
    return m_viewFocusColor;
}

BasicThemeDeclarative *BasicTheme::basicThemeDeclarative()
{
    return &privateBasicThemeDeclarativeSelf->self;
}

}

#include "moc_basictheme_p.cpp"
