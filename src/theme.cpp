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

#include "theme.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QPalette>
#include <QDebug>



class ThemeDeclarativeSingleton
{
public:
    ThemeDeclarativeSingleton()
    {}

    ThemeDeclarative self;
};

Q_GLOBAL_STATIC(ThemeDeclarativeSingleton, privateThemeDeclarativeSelf)

ThemeDeclarative::ThemeDeclarative()
{}

ThemeDeclarative::~ThemeDeclarative()
{}

void ThemeDeclarative::setQmlPath(const QUrl &path)
{
    m_qmlPath = path;
}

QObject *ThemeDeclarative::instance(const Theme *theme)
{
    if (m_declarativeTheme) {
        return m_declarativeTheme;
    }

    QQmlEngine *engine = qmlEngine(theme->parent());
    Q_ASSERT(engine);

    QQmlComponent c(engine);
    c.loadUrl(m_qmlPath);

    m_declarativeTheme = c.create();
    qWarning()<<"Declarative Theme"<<m_declarativeTheme;
    return m_declarativeTheme;
}


ColorScope::ColorScope(QQuickItem *parent)
    : QQuickItem(parent)
{
}

ColorScope::~ColorScope()
{}

ColorScope::Context ColorScope::context() const
{
    return m_context;
}

void ColorScope::setContext(ColorScope::Context context)
{
    if (m_context == context) {
        return;
    }

    m_context = context;
    emit contextChanged();
}

QPalette ColorScope::palette() const
{
    if (m_context == Complementary) {
        QPalette pal = qApp->palette();
        //TODO: build the palette from the Theme declarative thing
        pal.setBrush(QPalette::Button, Theme::themeDeclarative()->instance(nullptr)->property("complementaryBackgroundColor").value<QColor>());
        pal.setBrush(QPalette::ButtonText, Theme::themeDeclarative()->instance(nullptr)->property("complementaryTextColor").value<QColor>());
        return pal;
    }
        
    return static_cast<QGuiApplication *>(QGuiApplication::instance())->palette();
}


ColorScope *ColorScope::kirigami_ColorScope()
{
    return this;
}



Theme::Theme(QObject *parent)
    : QObject(parent)
{
    m_scope = qobject_cast<ColorScope *>(parent);

    if (!m_scope) {
        m_scope = QQmlEngine::contextForObject(parent)->contextProperty("_kirigami_ColorScope").value<ColorScope *>();
    }
    if (!m_scope) {
        QQuickItem *candidate = qobject_cast<QQuickItem *>(parent);
        while (candidate) {
            if ((m_scope = qobject_cast<ColorScope *>(candidate))) {
                break;
            }
            candidate = candidate->parentItem();
        }
    }

    if (m_scope) {
        connect(m_scope, &ColorScope::contextChanged, this, &Theme::themeChanged);
    }
    //TODO: correct?
    connect(qApp, &QGuiApplication::fontDatabaseChanged, this, &Theme::defaultFontChanged);

    //now, connect all the declarative object signals to themeChanged, use the old syntax as they are all signals defined in QML
    connect(themeDeclarative()->instance(this), SIGNAL(textColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(disabledTextColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(highlightColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(highlightedTextColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(backgroundColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(linkColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(visitedLinkColorChanged()), this, SIGNAL(themeChanged()));

    connect(themeDeclarative()->instance(this), SIGNAL(buttonTextColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(buttonBackgroundColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(buttonHoverColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(buttonFocusColorChanged()), this, SIGNAL(themeChanged()));

    connect(themeDeclarative()->instance(this), SIGNAL(viewTextColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(viewBackgroundColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(viewHoverColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(viewFocusColorChanged()), this, SIGNAL(themeChanged()));

    connect(themeDeclarative()->instance(this), SIGNAL(complementaryTextColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(complementaryBackgroundColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(complementaryHoverColorChanged()), this, SIGNAL(themeChanged()));
    connect(themeDeclarative()->instance(this), SIGNAL(complementaryFocusColorChanged()), this, SIGNAL(themeChanged()));
}

Theme::~Theme()
{
}

QStringList Theme::keys() const
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

#define RESOLVECOLOR(colorName, upperCaseColor) \
    if (m_scope) {\
        switch (m_scope->context()) {\
        case ColorScope::Button:\
            return themeDeclarative()->instance(this)->property("button"#upperCaseColor).value<QColor>();\
        case ColorScope::View:\
            return themeDeclarative()->instance(this)->property("view"#upperCaseColor).value<QColor>();\
        case ColorScope::Complementary:\
            return themeDeclarative()->instance(this)->property("complementary"#upperCaseColor).value<QColor>();\
        case ColorScope::Window:\
        default:\
            return themeDeclarative()->instance(this)->property(#colorName).value<QColor>();\
        }\
    }\
    return themeDeclarative()->instance(this)->property(#colorName).value<QColor>();\

#define PROXYCOLOR(colorName, upperCaseColor) \
    return themeDeclarative()->instance(this)->property(#colorName).value<QColor>();

QColor Theme::textColor() const
{
    RESOLVECOLOR(textColor, TextColor)
}

QColor Theme::disabledTextColor() const
{
    PROXYCOLOR(disabledTextColor, DisabledTextColor)
}

QColor Theme::highlightColor() const
{
    PROXYCOLOR(highlightColor, HighlightColor)
}

QColor Theme::highlightedTextColor() const
{
    PROXYCOLOR(highlightedTextColor, HighlightedTextColor)
}

QColor Theme::backgroundColor() const
{
    RESOLVECOLOR(backgroundColor, BackgroundColor)
}

QColor Theme::linkColor() const
{
    PROXYCOLOR(linkColor, LinkColor)
}

QColor Theme::visitedLinkColor() const
{
    PROXYCOLOR(visitedLinkColor, VisitedLinkColor)
}

QColor Theme::buttonTextColor() const
{
    PROXYCOLOR(buttonTextColor, ButtonTextColor)
}

QColor Theme::Theme::buttonBackgroundColor() const
{
    PROXYCOLOR(buttonBackgroundColor, ButtonBackgroundColor)
}

QColor Theme::buttonHoverColor() const
{
    PROXYCOLOR(buttonHoverColor, ButtonHoverColor)
}

QColor Theme::Theme::buttonFocusColor() const
{
    PROXYCOLOR(buttonFocusColor, ButtonFocusColor)
}


QColor Theme::viewTextColor() const
{
    PROXYCOLOR(viewTextColor, ViewTextColor)
}

QColor Theme::viewBackgroundColor() const
{
    PROXYCOLOR(viewBackgroundColor, ViewBackgroundColor)
}

QColor Theme::viewHoverColor() const
{
    PROXYCOLOR(viewHoverColor, ViewHoverColor)
}

QColor Theme::viewFocusColor() const
{
    PROXYCOLOR(viewFocusColor, ViewFocusColor)
}


QColor Theme::complementaryTextColor() const
{
    PROXYCOLOR(complementaryTextColor, ComplementaryTextColor)
}

QColor Theme::complementaryBackgroundColor() const
{
    PROXYCOLOR(complementaryBackgroundColor, ComplementaryBackgroundColor)
}

QColor Theme::complementaryHoverColor() const
{
    PROXYCOLOR(complementaryHoverColor, ComplementaryHoverColor)
}

QColor Theme::complementaryFocusColor() const
{
    PROXYCOLOR(complementaryFocusColor, ComplementaryFocusColor)
}

QFont Theme::defaultFont() const
{
    return qApp->font();
}



ThemeDeclarative *Theme::themeDeclarative()
{
    return &privateThemeDeclarativeSelf->self;
}

Theme *Theme::qmlAttachedProperties(QObject *object)
{
    return new Theme(object);
}

#include "moc_theme.cpp"
