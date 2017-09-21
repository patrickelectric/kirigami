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

#ifndef PLATFORMTHEME_H
#define PLATFORMTHEME_H

#include <QObject>
#include <QQuickItem>
#include <QColor>
#include <QPalette>

#include <kirigami_export.h>

class PlatformThemePrivate;

class KIRIGAMI_EXPORT PlatformTheme : public QObject
{
    Q_OBJECT

    // colors
    Q_PROPERTY(QColor textColor READ textColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor disabledTextColor READ disabledTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor highlightColor READ highlightColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor highlightedTextColor READ highlightedTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor linkColor READ linkColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor visitedLinkColor READ visitedLinkColor NOTIFY colorsChanged)

    Q_PROPERTY(QFont defaultFont READ defaultFont NOTIFY defaultFontChanged)
    Q_PROPERTY(QPalette palette READ palette NOTIFY paletteChanged)

    Q_PROPERTY(bool inherit READ inherit WRITE setInherit NOTIFY inheritChanged)
    Q_PROPERTY(PlatformTheme::ColorSet colorSet READ colorSet WRITE setColorSet NOTIFY colorSetChanged)

    Q_ENUMS(ColorSet)

public:
    //kcolorscheme also has selection: this would replace highlightColor/highlightedTextColor which would become just
    //wouldn't bother about tooltip for now, can always be added
    enum ColorSet {
        Window = 0,
        Button,
        View,
        Complementary
    };

    explicit PlatformTheme(QObject *parent = 0);
    ~PlatformTheme();

    void setColorSet(PlatformTheme::ColorSet);
    PlatformTheme::ColorSet colorSet() const;

    bool inherit() const;
    void setInherit(bool inherit);


    QColor textColor() const;
    QColor disabledTextColor() const;
    QColor backgroundColor() const;
    QColor highlightColor() const;
    QColor highlightedTextColor() const;
    //TODO: add positive/neutral/negative text
    // hoverColor/focusColor would go there if the explicit properties are removed
    //any other stuff missing from kcolorscheme?

    //can we kill link and visited link?
    QColor linkColor() const;
    QColor visitedLinkColor() const;

    //Setters, not accessible from QML but from implementations
    void setTextColor(const QColor &color);
    void setDisabledTextColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setHighlightColor(const QColor &color);
    void setHighlightedTextColor(const QColor &color);
    void setLinkColor(const QColor &color);
    void setVisitedLinkColor(const QColor &color);


    QFont defaultFont() const;
    void setDefaultFont(const QFont &defaultFont);

    QPalette palette() const;
    void setPalette(const QPalette &palette);

    //this will be used by desktopicon to fetch icons with KIconLoader
    virtual Q_INVOKABLE QIcon iconFromTheme(const QString &name);
    //this may be used somehow by the desktop QQC2 to set the styleoption palettes

    //QML attached property
    static PlatformTheme *qmlAttachedProperties(QObject *object);

    /**
     * @internal
     */
    static void setFallbackThemeQmlPath(const QUrl &path);
    static QUrl fallbackThemeQmlPath();

Q_SIGNALS:
    void colorsChanged();
    void defaultFontChanged();
    void colorSetChanged();
    void paletteChanged();
    void inheritChanged();

private:
    PlatformThemePrivate *d;
    friend class PlatformThemePrivate;
};

QML_DECLARE_TYPEINFO(PlatformTheme, QML_HAS_ATTACHED_PROPERTIES)

QT_BEGIN_NAMESPACE
#define PlatformTheme_iid "org.kde.kirigami.PlatformTheme"
Q_DECLARE_INTERFACE(PlatformTheme, PlatformTheme_iid)
QT_END_NAMESPACE

#endif // PLATFORMTHEME_H
