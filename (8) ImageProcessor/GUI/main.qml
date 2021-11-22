import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import ProcessingPlugin 1.0
import Qt.labs.platform 1.0 as ExtendedDialogs

ApplicationWindow {

    id: root

    SystemPalette {
        colorGroup: SystemPalette.Active
    }

    visible: true
    minimumHeight: 400
    minimumWidth: 700
    color: "gainsboro"


    MenuBar {
        id: menuBar
        width: root.width
        anchors.top: root.top

        Menu {
            title: "Файл"
            width: 200

            Action {
                id: actionOpenImage
                text: "Открыть изображение..."
                onTriggered: {
                    openFileDialog.open()
                }
            }

            MenuSeparator{}

            Action {
                text: "Сохранить результат как..."
                onTriggered: {
                    if (resultImage.source === "") return;
                    saveFileDialog.open()
                }
            }
        }

        MenuSeparator{}

        Menu {
            title: "Меню"
            width: 400

            Action {
                text: "Применить медианный фильтр"
                onTriggered: {
                    if (sourceImage.source != "") {
                        processor.applyMedianFilter(sourceImage.source);
                        progressDialog.runTimer()
                        progressDialog.open()
                    }
                }
            }

            MenuSeparator{}

            Action {
                text: "Настроить размер радиуса фильтра и количество потоков"
                onTriggered: appSettings.open()
            }
        }
    }

    Rectangle {
        id: sourseRect
        height: 20
        width: root.width / 2
        color: "gray"
        anchors.top: menuBar.bottom
        x: 0

        Text {
            text: "Исходное изображение"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: resRect
        height: 20
        width: root.width / 2
        color: "gray"
        anchors.top: menuBar.bottom
        anchors.left: sourseRect.right

        Text {
            text: "Результат обработки"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Row {
        id: workspace
        width: root.width
        height: root.height - menuBar.height - 25

        anchors.top: sourseRect.bottom

        Rectangle {
            id: leftLine
            height: parent.height
            width: 5
            color: "gray"
        }

        Image {
            id: sourceImage
            width: (parent.width / 2) - 7.5
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
        }

        Rectangle {
            id: borderLine
            height: parent.height
            width: 5
            color: "gray"
        }

        Image {
            id: resultImage
            width: (parent.width / 2) - 7.5
            height: parent.height
            fillMode: Image.PreserveAspectFit
        }

        Rectangle {
            id: rightLine
            height: parent.height
            width: 5
            color: "gray"
        }
    }

    Rectangle {
        id: downRect
        height: 5
        width: root.width
        color: "gray"
        anchors.top: workspace.bottom
    }

    FileDialog {
        id: openFileDialog

        title: "Выберете файл"
        folder: "./"
        onAccepted: {
            sourceImage.source = fileUrl
        }
        onRejected: {
            console.log("Canceled")
        }
        Component.onCompleted: visible = false
    }

    ExtendedDialogs.FileDialog {
        id: saveFileDialog
        fileMode: ExtendedDialogs.FileDialog.SaveFile
        nameFilters: ["JPG, JPEG (*.jpg)", "Все файлы (*)"]
        onAccepted: {
            if (resultImage.source !== "") {
                processor.copyImage(resultImage.source, file);
            }
        }
    }

    Dialog {
        id: progressDialog

        property alias value: _progressBar.value

        modality: Qt.WindowModal

        width: 400
        height: 100

        function runTimer() {
            _timer.start()
            _internal.secondsSpent = 0
        }

        function stopTimer() {
            _timer.stop()
        }

        Timer {
            id: _timer

            property double secondsPass: 0

            function run() {
                running = true
                secondsPass = 0;
            }

            function stop() {
                running = false
            }

            interval: 100
            running: false
            repeat: true
            onTriggered: {
                secondsPass += interval / 1000;
                _internal.secondsSpent = secondsPass;
            }
        }

        Rectangle {
            id: _progressBar

            property double maximum: _internal.maximum
            property alias value: _internal.value
            property alias minimum: _internal.minimum

            width: parent.width
            height: 40
            color: "gray"

            //border.width: 0.05 * height
            //radius: 0.5 * height

            Rectangle {
                id: _internal

                property double maximum: 100
                property double value: 0
                property double minimum: 0
                property double secondsSpent: 0

               visible: value > minimum
               width: Math.max(height, Math.min((value - minimum) / (maximum - minimum) * (parent.width), _progressBar.width))
               height: _progressBar.height
               color: "lightblue"
            }

            Text {
                 text: "Обработано " + _internal.value.toString() + "% (" + _internal.secondsSpent.toFixed(1) + " секунд)"
                 height: parent.height
                 anchors.horizontalCenter: parent.horizontalCenter
                 font.pointSize: parent.height / 2
            }
        }

        onRejected: {
            processor.cancelProcessing()
        }

        Component.onCompleted: visible = false
    }

    Processor {
        id: processor;

        onProcessingStarted: {
            progressDialog.standardButtons = StandardButton.Cancel
        }

        onProcessingFinished: {
            progressDialog.stopTimer()
            console.log("result temp image path: " + imagePath)
            resultImage.source = "file:" + imagePath
            progressDialog.standardButtons = StandardButton.Ok
        }

        onProgressChanged: {
            progressDialog.value = progress;
        }
    }

    SettingsWindow {

        id: appSettings

        processor: processor

        width: 300;
        height: 300;

        Component.onCompleted: visible = false
    }
}
