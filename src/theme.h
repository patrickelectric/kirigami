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

#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QQuickItem>
#include <QColor>

class ColorScope : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(ColorScope *_kirigami_ColorScope READ kirigami_ColorScope CONSTANT)
    Q_PROPERTY(Context context READ context WRITE setContext NOTIFY contextChanged)
    Q_ENUMS(Context)

public:
    enum Context {
        Window=0,
        Button,
        View,
        Complementary
    };

    explicit ColorScope(QQuickItem *parent = 0);
    ~ColorScope();

    Context context() const;
    void setContext(Context context);

    ColorScope *kirigami_ColorScope();

Q_SIGNALS:
    void contextChanged();

private:
    Context m_context = Window;
};

class Theme : public QObject
{
    Q_OBJECT

    // colors
    Q_PROPERTY(QColor textColor READ textColor NOTIFY textColorChanged)

public:
    explicit Theme(QObject *parent = 0);
    ~Theme();


    QColor textColor() const;

    static Theme *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void textColorChanged();

private:
    ColorScope *m_scope;
    QColor m_textColor = QColor(255,0,0,255);
};

QML_DECLARE_TYPEINFO(Theme, QML_HAS_ATTACHED_PROPERTIES)

#endif // THEME_H
