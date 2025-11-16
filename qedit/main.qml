import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 420
    height: 180
    title: "Contact List"

    property int visibleRows: 3
    property int rowHeight: 50

    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"

        Rectangle {
            width: listView.width
            height: rowHeight
            color: "#87cefa"
            opacity: 1.0
            radius: 4
            anchors.horizontalCenter: parent.horizontalCenter
            y: rowHeight  // always second row
            z: 0
        }

        FocusScope {
            id: scope
            anchors.fill: parent
            focus: true

            ListView {
                id: listView
                width: parent.width
                height: visibleRows * rowHeight
                model: contactModel
                currentIndex: 0
                spacing: 2
                clip: true
                interactive: false
                highlightFollowsCurrentItem: false

                delegate: Rectangle {
                    width: listView.width
                    height: rowHeight
                    color: "transparent"

                    Row {
                        anchors.centerIn: parent
                        spacing: 20
                        Text { text: name; font.pixelSize: 16 }
                        Text { text: number; font.pixelSize: 16 }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // Send selected index only on click
                            queueWorker.sendSelectedIndex(index)
                        }
                    }
                }

                // Fixed highlight rectangle: stays in 2nd visible row

                function adjustScrollForKeyboard(up) {
                    if (up) {
                        if (currentIndex > 0) {
                            currentIndex--
                        }
                    } else {
                        if (currentIndex < count - 1) {
                            currentIndex++
                        }
                    }

                    // Scroll the list so currentIndex is under highlight
                    contentY = Math.max(0, currentIndex - 1) * rowHeight

                    // Request next batch if last element visible
                    if (currentIndex === count - 1) {
                        queueWorker.requestNextBatch(currentIndex)
                    }
                }

                Component.onCompleted: {
                    // Initialize contentY to show first elements correctly
                    contentY = 0
                }
            }

            Keys.onUpPressed: {
                listView.adjustScrollForKeyboard(true)
            }

            Keys.onDownPressed: {
                listView.adjustScrollForKeyboard(false)
            }
        }
    }
}
