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

#ifndef PlatformTheme_H
#define PlatformTheme_H

#include <QObject>
#include <QQuickItem>
#include <QColor>
#include <QPointer>

class PlatformTheme;

class PlatformTheme : public QObject
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

    Q_PROPERTY(QFont defaultFont READ defaultFont NOTIFY defaultFontChanged)


    Q_PROPERTY(PlatformTheme::Context colorContext READ colorContext WRITE setColorContext NOTIFY colorContextChanged)

    Q_ENUMS(Context)

    //TODO:
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

public:
    //kcolorscheme also has selection: this would replace highlightColor/highlightedTextColor which would become just
    //wouldn't bother about tooltip for now, can always be added
    enum Context {
        Inherit = 0,
        Window,
        Button,
        View,
        Complementary
    };

    explicit PlatformTheme(QObject *parent = 0);
    ~PlatformTheme();

    void setColorContext(PlatformTheme::Context);
    PlatformTheme::Context colorContext() const;

    QColor textColor() const;
    QColor disabledTextColor() const;
    QColor backgroundColor() const;
    QColor highlightColor() const;
    QColor highlightedTextColor() const;
    QColor backgroundColor() const;
    //TODO: add positive/neutral/negative text
    // hoverColor/focusColor would go there if the explicit properties are removed
    //any other stuff missing from kcolorscheme?

    //can we kill link and visited link?
    QColor linkColor() const;
    QColor visitedLinkColor() const;


    //TODO: kill all those methods:
    //can those be all killed and all of this just become dependent from colorContext?
    //porting can be painful if all of this gets dropeed
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
    //this will be used by desktopicon to fetch icons with KIconLoader
    QIcon iconFromTheme(const QString &name);
    //this may be used somehow by the desktop QQC2 to set the styleoption palettes
    QPalette currentPalette();

    static PlatformTheme *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void themeChanged();
    void defaultFontChanged();
    void colorContextChanged();

private:
    void findParentStyle();
    void setActualColorContext(PlatformTheme::Context);
    static QColor tint(const QColor &c1, const QColor &c2, qreal ratio);

    PlatformTheme::Context m_colorContext = PlatformTheme::Auto;
    PlatformTheme::Context m_actualColorContext = PlatformTheme::Window;
    QSet<PlatformTheme *> m_childPlatformThemes;
    QPointer<PlatformTheme> m_parentPlatformTheme;
};

QML_DECLARE_TYPEINFO(PlatformTheme, QML_HAS_ATTACHED_PROPERTIES)

#endif // PlatformTheme_H
