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

#include "platformtheme.h"
#include "basictheme.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QDebug>
#include <QPointer>
#include <QQuickWindow>


class PlatformThemePrivate {
public:
    PlatformThemePrivate(PlatformTheme *q);
    ~PlatformThemePrivate();

    void findParentStyle();
    static QColor tint(const QColor &c1, const QColor &c2, qreal ratio);


    PlatformTheme *q;
    PlatformTheme::ColorSet m_colorSet = PlatformTheme::Window;
    QSet<PlatformTheme *> m_childThemes;
    QPointer<PlatformTheme> m_parentTheme;

    QColor textColor;
    QColor disabledTextColor;
    QColor backgroundColor;
    QColor highlightColor;
    QColor highlightedTextColor;
    QColor linkColor;
    QColor visitedLinkColor;
    QFont font;
    QPalette palette;
    bool m_inherit = true;
};

PlatformThemePrivate::PlatformThemePrivate(PlatformTheme *q)
    : q(q)
{}

PlatformThemePrivate::~PlatformThemePrivate()
{}

void PlatformThemePrivate::findParentStyle()
{
    if (m_parentTheme) {
        m_parentTheme->d->m_childThemes.remove(q);
    }
    QQuickItem *candidate = qobject_cast<QQuickItem *>(q->parent());
    while (candidate) {
        candidate = candidate->parentItem();
        PlatformTheme *t = static_cast<PlatformTheme *>(qmlAttachedPropertiesObject<PlatformTheme>(candidate, false));
        if (t) {
            t->d->m_childThemes.insert(q);
            m_parentTheme = t;
            if (m_inherit) {
                q->setColorSet(t->colorSet());
            }
            break;
        }
        
    }
}

QColor PlatformThemePrivate::tint(const QColor &c1, const QColor &c2, qreal ratio)
{
    qreal r = c1.redF() + (c2.redF() - c1.redF()) * ratio;
    qreal g = c1.greenF() + (c2.greenF() - c1.greenF()) * ratio;
    qreal b = c1.blueF() + (c2.blueF() - c1.blueF()) * ratio;
 
    return QColor::fromRgbF(r, g, b, 1);
}




PlatformTheme::PlatformTheme(QObject *parent)
    : QObject(parent),
      d(new PlatformThemePrivate(this))
{
    d->findParentStyle();

    if (QQuickItem *item = qobject_cast<QQuickItem *>(parent)) {
        connect(item, &QQuickItem::windowChanged, this, [this]() {
            d->findParentStyle();
        });
        connect(item, &QQuickItem::parentChanged, this, [this]() {
            d->findParentStyle();
        });
    }

    //TODO: correct?
    connect(qApp, &QGuiApplication::fontDatabaseChanged, this, &PlatformTheme::defaultFontChanged);
}

PlatformTheme::~PlatformTheme()
{
    if (d->m_parentTheme) {
        d->m_parentTheme->d->m_childThemes.remove(this);
    }
}

void PlatformTheme::setColorSet(PlatformTheme::ColorSet colorSet)
{
    if (d->m_colorSet == colorSet) {
        return;
    }

    d->m_colorSet = colorSet;

    for (PlatformTheme *t : d->m_childThemes) {
        if (t->inherit()) {
            t->setColorSet(colorSet);
        }
    }

    emit colorSetChanged();
    emit colorsChanged();
}

PlatformTheme::ColorSet PlatformTheme::colorSet() const
{
    return d->m_colorSet;
}

bool PlatformTheme::inherit() const
{
    return d->m_inherit;
}

void PlatformTheme::setInherit(bool inherit)
{
    if (d->m_inherit == inherit) {
        return;
    }

    d->m_inherit = inherit;
    if (inherit && d->m_parentTheme) {
        setColorSet(d->m_parentTheme->colorSet());
    }
    emit inheritChanged();
}


QColor PlatformTheme::textColor() const
{
    return d->textColor;
}

QColor PlatformTheme::disabledTextColor() const
{
    return d->disabledTextColor;
}

QColor PlatformTheme::highlightColor() const
{
    return d->highlightColor;
}

QColor PlatformTheme::highlightedTextColor() const
{
    return d->highlightedTextColor;
}

QColor PlatformTheme::backgroundColor() const
{
    return d->backgroundColor;
}

QColor PlatformTheme::linkColor() const
{
    return d->linkColor;
}

QColor PlatformTheme::visitedLinkColor() const
{
    return d->visitedLinkColor;
}

void PlatformTheme::setTextColor(const QColor &color)
{
    if (d->textColor == color) {
        return;
    }

    d->textColor = color;
    emit colorsChanged();
}

void PlatformTheme::setDisabledTextColor(const QColor &color)
{
    if (d->disabledTextColor == color) {
        return;
    }

    d->disabledTextColor = color;
    emit colorsChanged();
}

void PlatformTheme::setBackgroundColor(const QColor &color)
{
    if (d->backgroundColor == color) {
        return;
    }

    d->backgroundColor = color;
    emit colorsChanged();
}

void PlatformTheme::setHighlightColor(const QColor &color)
{
    if (d->highlightColor == color) {
        return;
    }

    d->highlightColor = color;
    emit colorsChanged();
}

void PlatformTheme::setHighlightedTextColor(const QColor &color)
{
    if (d->highlightedTextColor == color) {
        return;
    }

    d->highlightedTextColor = color;
    emit colorsChanged();
}

void PlatformTheme::setLinkColor(const QColor &color)
{
    if (d->linkColor == color) {
        return;
    }

    d->linkColor = color;
    emit colorsChanged();
}

void PlatformTheme::setVisitedLinkColor(const QColor &color)
{
    if (d->visitedLinkColor == color) {
        return;
    }

    d->visitedLinkColor = color;
    emit colorsChanged();
}



QFont PlatformTheme::defaultFont() const
{
    return d->font;
}

void PlatformTheme::setDefaultFont(const QFont &font)
{
    if (d->font == font) {
        return;
    }

    d->font = font;
    emit defaultFontChanged();
}

QPalette PlatformTheme::palette() const
{
    return d->palette;
}

void PlatformTheme::setPalette(const QPalette &palette)
{
    if (d->palette == palette) {
        return;
    }

    d->palette = palette;
    emit paletteChanged();
}

QIcon PlatformTheme::iconFromTheme(const QString &name)
{
    return QIcon::fromTheme(name);
}



PlatformTheme *PlatformTheme::qmlAttachedProperties(QObject *object)
{
    return new BasicTheme(object);
}

void PlatformTheme::setFallbackThemeQmlPath(const QUrl &path)
{
    BasicTheme::basicThemeDeclarative()->setQmlPath(path);
}

QUrl PlatformTheme::fallbackThemeQmlPath()
{
    return BasicTheme::basicThemeDeclarative()->qmlPath();
}


#include "moc_platformtheme.cpp"
