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
#include <QDebug>


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
qWarning()<<"JJJ"<<context;
    m_context = context;
    emit contextChanged();
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
    if (m_scope && m_scope->context() == ColorScope::Complementary) {
        return QColor(0, 255, 0, 255);
    }
    return m_textColor;
}

Theme *Theme::qmlAttachedProperties(QObject *object)
{
    return new Theme(object);
}

#include "moc_theme.cpp"
