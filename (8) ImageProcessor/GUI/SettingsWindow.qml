import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import ProcessingPlugin 1.0


Dialog {
    id: _dialog

    property var processor

    title: "Настройки"
    modality: Qt.WindowModal
    standardButtons: StandardButton.Ok
    width: grid.width



    Row {
        anchors.fill: parent

        ColumnLayout {
            id: _colRadius

            height: 100
            width: (parent.width / 2) - 10

            Text {
                Layout.fillWidth: true
                text: "Радиус \nфильтра: " + processor.medianFilterRadius;
                font.pointSize: 12
            }

            Button {
                Layout.fillWidth: true
                text: "Добавить"
                onClicked: {
                    processor.setMedianFilterRadius(processor.medianFilterRadius + 1);
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Убавить"
                onClicked: {
                    processor.setMedianFilterRadius(processor.medianFilterRadius - 1);
                }
            }
        }

        Rectangle {
            width: 20
            height: parent.height
            color: "transparent"
        }

        ColumnLayout {
            id: _colThreads

            height: 100
            width: (parent.width / 2) - 10

            Text {
                Layout.fillWidth: true
                text: "Количество \nпотоков: " + processor.threadsCount;
                font.pointSize: 12
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                Layout.fillWidth: true
                text: "Добавить"
                onClicked: {
                    processor.setThreadsCount(processor.threadsCount + 1)
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Убавить"
                onClicked: {
                    processor.setThreadsCount(processor.threadsCount - 1)
                }
            }
        }
    }
}
