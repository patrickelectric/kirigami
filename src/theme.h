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

class Theme;

class ThemeDeclarative
{
public:
    ThemeDeclarative();
    virtual ~ThemeDeclarative();

    void setQmlPath(const QUrl &path);
    QObject *instance(const Theme *theme);

private:
    QUrl m_qmlPath;
    QObject *m_declarativeTheme = nullptr;
};

class ColorScope : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(ColorScope *_kirigami_ColorScope READ kirigami_ColorScope CONSTANT)
    Q_PROPERTY(Context context READ context WRITE setContext NOTIFY contextChanged)
    Q_PROPERTY(QPalette palette READ palette NOTIFY paletteChanged)
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

    QPalette palette() const;

    ColorScope *kirigami_ColorScope();

Q_SIGNALS:
    void contextChanged();
    void paletteChanged();

private:
    Context m_context = Window;
};

class Theme : public QObject
{
    Q_OBJECT

    // colors
    Q_PROPERTY(QColor textColor READ textColor NOTIFY themeChanged)
    Q_PROPERTY(QColor disabledTextColor READ disabledTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor highlightColor READ highlightColor NOTIFY themeChanged)
    Q_PROPERTY(QColor highlightedTextColor READ highlightedTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor linkColor READ linkColor NOTIFY themeChanged)
    Q_PROPERTY(QColor visitedLinkColor READ visitedLinkColor NOTIFY themeChanged)

    Q_PROPERTY(QColor buttonTextColor READ buttonTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor buttonBackgroundColor READ buttonBackgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor buttonHoverColor READ buttonHoverColor NOTIFY themeChanged)
    Q_PROPERTY(QColor buttonFocusColor READ buttonFocusColor NOTIFY themeChanged)

    Q_PROPERTY(QColor viewTextColor READ viewTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor viewBackgroundColor READ viewBackgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor viewHoverColor READ viewHoverColor NOTIFY themeChanged)
    Q_PROPERTY(QColor viewFocusColor READ viewFocusColor NOTIFY themeChanged)

    Q_PROPERTY(QColor complementaryTextColor READ complementaryTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor complementaryBackgroundColor READ complementaryBackgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor complementaryHoverColor READ complementaryHoverColor NOTIFY themeChanged)
    Q_PROPERTY(QColor complementaryFocusColor READ complementaryFocusColor NOTIFY themeChanged)

    Q_PROPERTY(QFont defaultFont READ defaultFont NOTIFY defaultFontChanged)
    //FIXME: this is due https://bugreports.qt.io/browse/QTBUG-63089
    Q_PROPERTY(QStringList keys READ keys CONSTANT)

public:
    explicit Theme(QObject *parent = 0);
    ~Theme();


    QColor textColor() const;
    QColor disabledTextColor() const;
    QColor highlightColor() const;
    QColor highlightedTextColor() const;
    QColor backgroundColor() const;
    QColor linkColor() const;
    QColor visitedLinkColor() const;

    QColor buttonTextColor() const;
    QColor buttonBackgroundColor() const;
    QColor buttonHoverColor() const;
    QColor buttonFocusColor() const;

    QColor viewTextColor() const;
    QColor viewBackgroundColor() const;
    QColor viewHoverColor() const;
    QColor viewFocusColor() const;

    QColor complementaryTextColor() const;
    QColor complementaryBackgroundColor() const;
    QColor complementaryHoverColor() const;
    QColor complementaryFocusColor() const;

    QFont defaultFont() const;

    QStringList keys() const;

    static Theme *qmlAttachedProperties(QObject *object);

    static ThemeDeclarative *themeDeclarative();

Q_SIGNALS:
    void themeChanged();
    void defaultFontChanged();

private:
    ColorScope *m_scope;
    QColor m_textColor = QColor(255,0,0,255);
};

QML_DECLARE_TYPEINFO(Theme, QML_HAS_ATTACHED_PROPERTIES)

#endif // THEME_H
