/*
 *   Copyright 2017 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
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

import QtQuick 2.0
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.2 as Kirigami

Kirigami.ScrollablePage {
    id: page
    title: "Color Scopes"

    ColumnLayout {
        Rectangle {
            color: Kirigami.Theme.backgroundColor
            Layout.minimumHeight: childrenRect.height
            Layout.fillWidth: true
            ColumnLayout {
                width: parent.width
                Kirigami.Label {
                    text: "Window scope"
                }
                Kirigami.BasicListItem {
                    width: parent.width
                    icon: "go-next"
                    label: "Delegate1"
                }
                Kirigami.BasicListItem {
                    width: parent.width
                    label: "Delegate2"
                }
            }
        }

        Rectangle {
            Kirigami.Theme.colorContext: Kirigami.Theme.View
            Layout.minimumHeight: childrenRect.height
            Layout.fillWidth: true
            color: Kirigami.Theme.backgroundColor
            height: childrenRect.height
            width: parent.width
            ColumnLayout {
                width: parent.width
                Kirigami.Label {
                    text: "View scope"
                }
                Kirigami.BasicListItem {
                    width: parent.width
                    icon: "go-next"
                    label: "Delegate1"
                }
                Kirigami.BasicListItem {
                    width: parent.width
                    label: "Delegate2"
                }
            }
        }

        Rectangle {
            Kirigami.Theme.colorContext: Kirigami.Theme.Complementary
            Layout.minimumHeight: childrenRect.height
            Layout.fillWidth: true
            color: Kirigami.Theme.backgroundColor
            height: childrenRect.height
            width: parent.width
            ColumnLayout {
                width: parent.width
                Kirigami.Label {
                    text: "Complementary scope"
                }
                Kirigami.BasicListItem {
                    width: parent.width
                    icon: "go-next"
                    label: "Delegate1"
                }
                Kirigami.BasicListItem {
                    width: parent.width
                    label: "Delegate2"
                }
            }
        }

        Rectangle {
            Kirigami.Theme.colorContext: Kirigami.Theme.Complementary
            color: Kirigami.Theme.backgroundColor
            Layout.minimumHeight: 30
            Layout.fillWidth: true
            Rectangle {
                color: Kirigami.Theme.textColor
                width: parent.width
                height: 9
            }
        }
    }
}
