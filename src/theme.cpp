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
    m_scope = QQmlEngine::contextForObject(parent)->contextProperty("_kirigami_ColorScope").value<ColorScope *>();

    connect(m_scope, &ColorScope::contextChanged, this, &Theme::themeChanged);
    //TODO: correct?
    connect(qApp, &QGuiApplication::fontDatabaseChanged, this, &Theme::defaultFontChanged);

    //now, connect all the declarative object signals to themeChanged, use the old syntax as they are all signals defined in QML
    connect(themeDeclarative()->instance(this), SIGNAL(textColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(disabledTextColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(highlightColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(highlightedTextColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(backgroundColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(linkColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(visitedLinkColorChanged), this, SLOT(themeChanged));

    connect(themeDeclarative()->instance(this), SIGNAL(buttonTextColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(buttonBackgroundColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(buttonHoverColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(buttonFocusColorChanged), this, SLOT(themeChanged));

    connect(themeDeclarative()->instance(this), SIGNAL(viewTextColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(viewBackgroundColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(viewHoverColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(viewFocusColorChanged), this, SLOT(themeChanged));

    connect(themeDeclarative()->instance(this), SIGNAL(complementaryTextColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(complementaryBackgroundColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(complementaryHoverColorChanged), this, SLOT(themeChanged));
    connect(themeDeclarative()->instance(this), SIGNAL(complementaryFocusColorChanged), this, SLOT(themeChanged));
}

Theme::~Theme()
{
}

#define RESOLVECOLOR(colorName, upperCaseColor) \
    if (m_scope) {\
        switch (m_scope->context()) {\
        case ColorScope::Button:\
            return themeDeclarative()->instance(this)->property("button##upperCaseColor").value<QColor>();\
        case ColorScope::View:\
            return themeDeclarative()->instance(this)->property("view##upperCaseColor").value<QColor>();\
        case ColorScope::Complementary:\
            return themeDeclarative()->instance(this)->property("complementary##upperCaseColor").value<QColor>();\
        case ColorScope::Window:\
        default:\
            return themeDeclarative()->instance(this)->property("colorName").value<QColor>();\
        }\
    }\
    \
    return themeDeclarative()->instance(this)->property("colorName").value<QColor>();\

#define PROXYCOLOR(colorName, upperCaseColor) \
    return themeDeclarative()->instance(this)->property("colorName").value<QColor>();

QColor Theme::textColor() const
{
    RESOLVECOLOR(textColor, TextColor)
 /*   if (m_scope) {
        switch (m_scope->context()) {
        case ColorScope::Button:
            return themeDeclarative()->instance(this)->property("buttonTextColor").value<QColor>();
        case ColorScope::View:
            return themeDeclarative()->instance(this)->property("viewTextColor").value<QColor>();
        case ColorScope::Complementary:
            return themeDeclarative()->instance(this)->property("complementaryTextColor").value<QColor>();
        case ColorScope::Window:
        default:
            return themeDeclarative()->instance(this)->property("textColor").value<QColor>();
        }
    }

    return themeDeclarative()->instance(this)->property("textColor").value<QColor>();
    return m_textColor;*/
}

QColor Theme::disabledTextColor() const
{
    RESOLVECOLOR(disabledTextColor, DisabledTextColor)
}

QColor Theme::highlightColor() const
{
    RESOLVECOLOR(highlightColor, HighlightColor)
}

QColor Theme::highlightedTextColor() const
{
    RESOLVECOLOR(highlightedTextColor, HighlightedTextColor)
}

QColor Theme::backgroundColor() const
{
    RESOLVECOLOR(backgroundColor, BackgroundColor)
}

QColor Theme::linkColor() const
{
    RESOLVECOLOR(linkColor, LinkColor)
}

QColor Theme::visitedLinkColor() const
{
    RESOLVECOLOR(visitedLinkColor, VisitedLinkColor)
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
