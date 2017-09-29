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

#ifndef KIRIGAMI_BUILD_TYPE_STATIC
#include <kirigami2_export.h>
#endif

namespace Kirigami {

class PlatformThemePrivate;

#ifdef KIRIGAMI_BUILD_TYPE_STATIC
class PlatformTheme : public QObject
#else
class KIRIGAMI2_EXPORT PlatformTheme : public QObject
#endif
{
    Q_OBJECT

    /**
     * This enumeration describes the color set for which a color is being selected.
     *
     * Color sets define a color "environment", suitable for drawing all parts of a
     * given region. Colors from different sets should not be combined.
     */
    Q_PROPERTY(ColorSet colorSet READ colorSet WRITE setColorSet NOTIFY colorSetChanged)

    /**
     * If true, the colorSet will be inherited from the colorset of a theme of one
     * of the ancestor items
     * default: true
     */
    Q_PROPERTY(bool inherit READ inherit WRITE setInherit NOTIFY inheritChanged)

    // foreground colors
    Q_PROPERTY(QColor textColor READ textColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor disabledTextColor READ disabledTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor highlightedTextColor READ highlightedTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor activeTextColor READ activeTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor linkColor READ linkColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor visitedLinkColor READ visitedLinkColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor negativeTextColor READ negativeTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor neutralTextColor READ neutralTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor positiveTextColor READ positiveTextColor NOTIFY colorsChanged)

    //background colors
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor highlightColor READ highlightColor NOTIFY colorsChanged)

    //decoration colors
    Q_PROPERTY(QColor focusColor READ focusColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor hoverColor READ hoverColor NOTIFY colorsChanged)

    // font and palette
    Q_PROPERTY(QFont defaultFont READ defaultFont NOTIFY defaultFontChanged)
    Q_PROPERTY(QPalette palette READ palette NOTIFY paletteChanged)

    Q_ENUMS(ColorSet)

public:
    //kcolorscheme also has selection: this would replace highlightColor/highlightedTextColor which would become just
    //wouldn't bother about tooltip for now, can always be added
    enum ColorSet {
        View = 0,
        Window,
        Button,
        Selection,
        Tooltip,
        Complementary
    };

    explicit PlatformTheme(QObject *parent = 0);
    ~PlatformTheme();

    void setColorSet(PlatformTheme::ColorSet);
    PlatformTheme::ColorSet colorSet() const;

    bool inherit() const;
    void setInherit(bool inherit);

    //foreground colors
    QColor textColor() const;
    QColor disabledTextColor() const;
    QColor highlightedTextColor() const;
    QColor activeTextColor() const;
    QColor linkColor() const;
    QColor visitedLinkColor() const;
    QColor negativeTextColor() const;
    QColor neutralTextColor() const;
    QColor positiveTextColor() const;

    //background colors
    QColor backgroundColor() const;
    QColor highlightColor() const;
    //TODO: add active/positive/neutral/negative backgrounds?

    //decoration colors
    QColor focusColor() const;
    QColor hoverColor() const;
    
    //Setters, not accessible from QML but from implementations
    
    //foreground colors
    void setTextColor(const QColor &color);
    void setDisabledTextColor(const QColor &color);
    void setHighlightedTextColor(const QColor &color);
    void setActiveTextColor(const QColor &color);
    void setLinkColor(const QColor &color);
    void setVisitedLinkColor(const QColor &color);
    void setNegativeTextColor(const QColor &color);
    void setNeutralTextColor(const QColor &color);
    void setPositiveTextColor(const QColor &color);

    //background colors
    void setBackgroundColor(const QColor &color);
    void setHighlightColor(const QColor &color);
    
    //decoration colors
    void setFocusColor(const QColor &color);
    void setHoverColor(const QColor &color);


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
    //TODO: parameters to signals as this is also a c++ api
    void colorsChanged();
    void defaultFontChanged();
    void colorSetChanged();
    void paletteChanged();
    void inheritChanged();

private:
    PlatformThemePrivate *d;
    friend class PlatformThemePrivate;
};

}

QML_DECLARE_TYPEINFO(Kirigami::PlatformTheme, QML_HAS_ATTACHED_PROPERTIES)

#endif // PLATFORMTHEME_H
