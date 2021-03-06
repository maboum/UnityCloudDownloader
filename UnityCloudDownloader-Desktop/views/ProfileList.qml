import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import ucd 1.0

Page {
    id: profilePage
    width: 800
    height: 600

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 8

            Button {
                id: refreshButton
                Layout.leftMargin: 10
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    anchors.margins: 4
                    source: "white-sync.png"
                    scale: parent.pressed ? 0.95 : parent.hovered ? 1 : 0.9
                }

                onClicked: refreshSync()
            }

            Button {
                id: addButton
                text: qsTr("Add")
                Layout.fillWidth: true
                onClicked: {
                    mainStack.push("AddProfile.qml", {
                                       "profilesModel": profilesModel
                                   })
                }
            }

            Button {
                id: removeButton
                text: qsTr("Remove")
                Layout.fillWidth: true
                enabled: profileListView.currentIndex !== -1
                onClicked: {
                    profilesModel.remove(profileListView.currentIndex)
                }
            }

            Button {
                id: editButton
                text: qsTr("Edit")
                Layout.fillWidth: true
                Layout.rightMargin: 10
                enabled: profileListView.currentIndex !== -1
                onClicked: mainStack.push("EditProfile.qml", { "profilesModel": profilesModel, "profile": profilesModel.profile(profileListView.currentIndex) })
            }
        }
    }

    ListView {
        id: profileListView
        anchors.fill: parent
        model: ProfilesModel {
            id: profilesModel
        }

        delegate: ProfileDelegate {
            width: parent.width
        }
        currentIndex: -1
        highlight: ItemHighlight {}
        highlightMoveDuration: 200
    }
}
