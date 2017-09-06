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

    connect(m_scope, &ColorScope::contextChanged, this, &Theme::textColorChanged);
}

Theme::~Theme()
{
}

QColor Theme::textColor() const
{
    if (m_scope) {
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
    return m_textColor;
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
