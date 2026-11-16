import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 980
    height: 720
    visible: true
    title: qsTr("Kalanso")
    color: "#ffffff"
    property color bg: "#f1f5f9"
    property color card: "#ffffff"
    property color border: "#dbe3ef"
    property color primary: "#2563eb"
    property color primaryHover: "#1d4ed8"
    property color textMain: "#0f172a"
    property color textMuted: "#475569"
    property color accentSoft: "#eff6ff"

    function softButtonStyle(btn) {
        btn.background = btnBackground.createObject(btn, { "radius": 8, "implicitHeight": 36 })
    }

    property var notesModel: []
    property var usersModel: []
    property var absencesModel: []
    property var classesModel: []
    property var edtModel: []
    property var actualitesModel: []

    function reloadModels() {
        notesModel = schoolFacade.student ? schoolFacade.getMyNotes() : schoolFacade.getNotesLines()
        usersModel = schoolFacade.getUsersLines()
        classesModel = schoolFacade.admin ? schoolFacade.getAllClasses() : schoolFacade.getMyClasses()
        edtModel = schoolFacade.admin ? schoolFacade.getEDTGlobal() : schoolFacade.getMyEDT()
        actualitesModel = schoolFacade.admin ? schoolFacade.getActualites() : []
    }

    Component {
        id: btnBackground
        Rectangle {
            border.width: 0
            color: parent && parent.down ? primaryHover : (parent && parent.hovered ? primaryHover : primary)
            opacity: parent && parent.enabled ? 1.0 : 0.45
        }
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 12
        radius: 14
        color: bg
        border.color: border

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 8

            Label {
                text: "Kalanso"
                font.pixelSize: 28
                font.bold: true
                color: textMain
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                visible: !schoolFacade.logged
                color: card
                border.color: border
                radius: 12
                Layout.fillWidth: true
                implicitHeight: 220

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 10

                    TextField { id: loginInput; placeholderText: "Identifiant"; Layout.fillWidth: true }
                    TextField { id: pwdInput; placeholderText: "Mot de passe"; echoMode: TextInput.Password; Layout.fillWidth: true }
                    Button {
                        text: "Se connecter"
                        Layout.fillWidth: true
                        font.bold: true
                        Component.onCompleted: softButtonStyle(this)
                        onClicked: {
                            if (schoolFacade.login(loginInput.text, pwdInput.text)) {
                                reloadModels()
                            }
                        }
                    }
                    Label {
                        text: schoolFacade.statusMessage
                        visible: text.length > 0
                        color: "#b91c1c"
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }
                }
            }

            ColumnLayout {
                visible: schoolFacade.logged
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 8

                Rectangle {
                    Layout.fillWidth: true
                    color: accentSoft
                    border.color: border
                    radius: 10
                    implicitHeight: 44
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 16
                        Label { text: "Utilisateur: " + schoolFacade.fullName; color: textMain }
                        Label { text: "Role: " + schoolFacade.role; color: textMuted }
                        Label { text: "Identifiant: " + schoolFacade.identifiant; color: textMuted }
                    }
                }

                TabBar {
                    id: topBar
                    Layout.fillWidth: true
                    TabButton { text: "Notes" }
                    TabButton { text: "Utilisateurs"; visible: schoolFacade.admin }
                    TabButton { text: "Absences"; visible: schoolFacade.admin || schoolFacade.professeur }
                    TabButton { text: "Classes" }
                    TabButton { text: "EDT" }
                    TabButton { text: "Actualites"; visible: schoolFacade.admin }
                }

                StackLayout {
                    currentIndex: topBar.currentIndex
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ScrollView {
                        clip: true
                        ColumnLayout {
                            width: 920
                            spacing: 6
                            RowLayout {
                                Layout.fillWidth: true
                                visible: schoolFacade.admin || schoolFacade.professeur
                                TextField { id: noteIdField; placeholderText: "ID note"; Layout.preferredWidth: 110 }
                                TextField { id: noteValueField; placeholderText: "Nouvelle valeur"; Layout.preferredWidth: 140 }
                                Button {
                                    text: "Modifier note"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.modifyNote(parseInt(noteIdField.text), parseFloat(noteValueField.text))
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                visible: schoolFacade.professeur
                                TextField { id: addNoteClasseField; placeholderText: "ID classe"; Layout.preferredWidth: 100 }
                                TextField { id: addNoteStudentField; placeholderText: "ID etudiant"; Layout.preferredWidth: 110 }
                                TextField { id: addNoteMatiereField; placeholderText: "ID matiere"; Layout.preferredWidth: 100 }
                                TextField { id: addNoteValeurField; placeholderText: "Valeur"; Layout.preferredWidth: 90 }
                                TextField { id: addNoteCoefField; placeholderText: "Coef"; text: "1"; Layout.preferredWidth: 80 }
                                TextField { id: addNoteAppField; placeholderText: "Appreciation"; Layout.fillWidth: true }
                                Button {
                                    text: "Ajouter note (addNoteEtu)"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.addNoteEtu(
                                            parseInt(addNoteClasseField.text),
                                            parseInt(addNoteStudentField.text),
                                            parseInt(addNoteMatiereField.text),
                                            parseFloat(addNoteValeurField.text),
                                            parseFloat(addNoteCoefField.text),
                                            addNoteAppField.text)
                                        reloadModels()
                                    }
                                }
                            }
                            Repeater {
                                model: notesModel
                                delegate: Text { text: modelData; color: "#0f172a" }
                            }
                        }                        
                    }

                    ScrollView {
                        clip: true
                        ColumnLayout {
                            width: 920
                            spacing: 8
                            Label { visible: !schoolFacade.admin; text: "Acces reserve a l'administrateur."; color: "#0f172a" }
                            RowLayout {
                                visible: schoolFacade.admin
                                TextField { id: studentNomField; placeholderText: "Nom etudiant"; Layout.preferredWidth: 180 }
                                TextField { id: studentPrenomField; placeholderText: "Prenom etudiant"; Layout.preferredWidth: 180 }
                                TextField { id: studentMdpField; placeholderText: "Mot de passe"; Layout.preferredWidth: 180 }
                                Button {
                                    text: "Creer Student"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.createUser(studentNomField.text, studentPrenomField.text, studentMdpField.text, 1)
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                TextField { id: profNomField; placeholderText: "Nom professeur"; Layout.preferredWidth: 180 }
                                TextField { id: profPrenomField; placeholderText: "Prenom professeur"; Layout.preferredWidth: 180 }
                                TextField { id: profMdpField; placeholderText: "Mot de passe"; Layout.preferredWidth: 180 }
                                Button {
                                    text: "Creer Professeur"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.createUser(profNomField.text, profPrenomField.text, profMdpField.text, 2)
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                TextField { id: userIdField; placeholderText: "ID utilisateur"; Layout.preferredWidth: 120 }
                                Button {
                                    text: "Supprimer"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: { schoolFacade.deleteUser(parseInt(userIdField.text)); reloadModels() }
                                }
                            }
                            Repeater {
                                model: usersModel
                                delegate: Text { text: modelData; color: "#0f172a" }
                            }
                        }
                    }

                    ScrollView {
                        clip: true
                        ColumnLayout {
                            width: 920
                            spacing: 8
                            Label { visible: !(schoolFacade.admin || schoolFacade.professeur); text: "Acces reserve au professeur/admin."; color: "#0f172a" }
                            RowLayout {
                                visible: schoolFacade.admin || schoolFacade.professeur
                                TextField { id: absStudentIdField; placeholderText: "ID etudiant"; Layout.preferredWidth: 110 }
                                TextField { id: absDateField; placeholderText: "Date YYYY-MM-DD"; Layout.preferredWidth: 140 }
                                TextField { id: absHeureField; placeholderText: "Heure HH:MM"; Layout.preferredWidth: 110 }
                                TextField { id: absMotifField; placeholderText: "Motif"; Layout.fillWidth: true }
                                TextField { id: absJustifField; placeholderText: "0/1"; text: "0"; Layout.preferredWidth: 70 }
                            }
                            RowLayout {
                                visible: schoolFacade.admin || schoolFacade.professeur
                                Button {
                                    text: "Ajouter"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.addAbsence(parseInt(absStudentIdField.text), absDateField.text, absHeureField.text, absMotifField.text, absJustifField.text === "1")
                                        absencesModel = schoolFacade.getAbsencesByStudent(parseInt(absStudentIdField.text))
                                    }
                                }
                                Button {
                                    text: "Voir"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: absencesModel = schoolFacade.getAbsencesByStudent(parseInt(absStudentIdField.text))
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                TextField { id: absIdField; placeholderText: "ID absence"; Layout.preferredWidth: 110 }
                                Button {
                                    text: "Modifier absence"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.updateAbsence(parseInt(absIdField.text), absJustifField.text === "1", absMotifField.text)
                                        absencesModel = schoolFacade.getAbsencesByStudent(parseInt(absStudentIdField.text))
                                    }
                                }
                            }
                            Repeater {
                                model: absencesModel
                                delegate: Text { text: modelData; color: "#0f172a" }
                            }
                        }
                    }

                    ScrollView {
                        clip: true
                        ColumnLayout {
                            width: 920
                            spacing: 6
                            Label { visible: !schoolFacade.admin; text: "Vue lecture seule (admin gere les affectations)."; color: "#0f172a" }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: createClasseNomField; placeholderText: "Nom classe"; Layout.preferredWidth: 160 }
                                TextField { id: createClasseNiveauField; placeholderText: "Niveau"; Layout.preferredWidth: 120 }
                                TextField { id: createClasseVoieField; placeholderText: "Voie"; Layout.preferredWidth: 120 }
                                Button {
                                    text: "Creer classe"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.addClasse(createClasseNomField.text, createClasseNiveauField.text, createClasseVoieField.text)
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: assignEtuIdField; placeholderText: "ID etudiant"; Layout.preferredWidth: 110 }
                                TextField { id: assignClasseIdField; placeholderText: "ID classe"; Layout.preferredWidth: 110 }
                                Button {
                                    text: "Assigner classe"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.assignStudentToClass(parseInt(assignEtuIdField.text), parseInt(assignClasseIdField.text))
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: assignClassProfIdField; placeholderText: "ID professeur"; Layout.preferredWidth: 120 }
                                TextField { id: assignClassToProfClasseIdField; placeholderText: "ID classe"; Layout.preferredWidth: 110 }
                                Button {
                                    text: "Assigner classe a prof"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.assignProfToClass(parseInt(assignClassProfIdField.text), parseInt(assignClassToProfClasseIdField.text))
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: assignProfIdField; placeholderText: "ID professeur"; Layout.preferredWidth: 120 }
                                TextField { id: assignMatProfIdField; placeholderText: "ID matiere"; Layout.preferredWidth: 110 }
                                Button {
                                    text: "Assigner matiere prof"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.assignProfMatiere(parseInt(assignProfIdField.text), parseInt(assignMatProfIdField.text))
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: affectClasseIdField; placeholderText: "ID classe"; Layout.preferredWidth: 110 }
                                TextField { id: affectMatiereIdField; placeholderText: "ID matiere"; Layout.preferredWidth: 110 }
                                TextField { id: affectProfIdField; placeholderText: "ID prof"; Layout.preferredWidth: 90 }
                                TextField { id: affectObligField; placeholderText: "Obligatoire 0/1"; text: "1"; Layout.preferredWidth: 110 }
                                Button {
                                    text: "Affecter matiere a classe"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.affectMatiereClasse(
                                            parseInt(affectClasseIdField.text),
                                            parseInt(affectMatiereIdField.text),
                                            parseInt(affectProfIdField.text),
                                            affectObligField.text === "1")
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: edtAssignProfIdField; placeholderText: "ID prof"; Layout.preferredWidth: 90 }
                                TextField { id: edtAssignClasseIdField; placeholderText: "ID classe"; Layout.preferredWidth: 95 }
                                TextField { id: edtAssignMatiereIdField; placeholderText: "ID matiere"; Layout.preferredWidth: 95 }
                                TextField { id: edtAssignJourField; placeholderText: "Jour"; Layout.preferredWidth: 90 }
                                TextField { id: edtAssignDebutField; placeholderText: "Debut"; Layout.preferredWidth: 90 }
                                TextField { id: edtAssignFinField; placeholderText: "Fin"; Layout.preferredWidth: 90 }
                                TextField { id: edtAssignSalleField; placeholderText: "Salle"; Layout.preferredWidth: 80 }
                                Button {
                                    text: "Attribuer EDT prof"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.assignEDTToProf(
                                            parseInt(edtAssignProfIdField.text),
                                            parseInt(edtAssignClasseIdField.text),
                                            parseInt(edtAssignMatiereIdField.text),
                                            edtAssignJourField.text,
                                            edtAssignDebutField.text,
                                            edtAssignFinField.text,
                                            edtAssignSalleField.text)
                                        reloadModels()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: edtAssignEtuIdField; placeholderText: "ID etudiant"; Layout.preferredWidth: 110 }
                                TextField { id: edtAssignEtuClasseIdField; placeholderText: "ID classe"; Layout.preferredWidth: 110 }
                                Button {
                                    text: "Attribuer EDT etudiant"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.assignEDTToStudent(parseInt(edtAssignEtuIdField.text), parseInt(edtAssignEtuClasseIdField.text))
                                        reloadModels()
                                    }
                                }
                            }
                            Label { visible: schoolFacade.admin; text: "Etudiants disponibles (ID):"; color: "#0f172a" }
                            Repeater {
                                model: schoolFacade.admin ? schoolFacade.getUsersByRole("etudiant") : []
                                delegate: Text { text: modelData; color: "#334155" }
                            }
                            Label { visible: schoolFacade.admin; text: "Professeurs disponibles (ID):"; color: "#0f172a" }
                            Repeater {
                                model: schoolFacade.admin ? schoolFacade.getUsersByRole("professeur") : []
                                delegate: Text { text: modelData; color: "#334155" }
                            }
                            Label { visible: schoolFacade.admin; text: "Classes disponibles (ID):"; color: "#0f172a" }
                            Repeater {
                                model: classesModel
                                delegate: Text { text: modelData; color: "#0f172a" }
                            }
                            Label { visible: schoolFacade.admin; text: "Matieres disponibles (ID):"; color: "#0f172a" }
                            Repeater {
                                model: schoolFacade.admin ? schoolFacade.getAllMatieres() : []
                                delegate: Text { text: modelData; color: "#334155" }
                            }
                        }
                    }

                    ScrollView {
                        clip: true
                        ColumnLayout {
                            width: 920
                            spacing: 6
                            Label { visible: !schoolFacade.admin; text: "Vue lecture seule (admin peut creer/modifier cours)."; color: "#0f172a" }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: edtJourField; placeholderText: "Jour"; Layout.preferredWidth: 90 }
                                TextField { id: edtDebutField; placeholderText: "Debut HH:MM"; Layout.preferredWidth: 110 }
                                TextField { id: edtFinField; placeholderText: "Fin HH:MM"; Layout.preferredWidth: 110 }
                                TextField { id: edtSalleField; placeholderText: "Salle"; Layout.preferredWidth: 90 }
                                TextField { id: edtClasseIdField; placeholderText: "ID classe"; Layout.preferredWidth: 90 }
                                TextField { id: edtMatiereIdField; placeholderText: "ID matiere"; Layout.preferredWidth: 95 }
                                TextField { id: edtProfIdField; placeholderText: "ID prof"; Layout.preferredWidth: 80 }
                                Button {
                                    text: "Creer cours"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.addCours(
                                            edtJourField.text,
                                            edtDebutField.text,
                                            edtFinField.text,
                                            edtSalleField.text,
                                            parseInt(edtClasseIdField.text),
                                            parseInt(edtMatiereIdField.text),
                                            parseInt(edtProfIdField.text))
                                        edtModel = schoolFacade.getEDTGlobal()
                                    }
                                }
                            }
                            RowLayout {
                                visible: schoolFacade.admin
                                Layout.fillWidth: true
                                TextField { id: edtCoursIdField; placeholderText: "ID cours"; Layout.preferredWidth: 90 }
                                Button {
                                    text: "Modifier cours"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.updateCours(
                                            parseInt(edtCoursIdField.text),
                                            edtJourField.text,
                                            edtDebutField.text,
                                            edtFinField.text,
                                            edtSalleField.text)
                                        edtModel = schoolFacade.getEDTGlobal()
                                    }
                                }
                                Button {
                                    text: "Supprimer cours"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: {
                                        schoolFacade.deleteCours(parseInt(edtCoursIdField.text))
                                        edtModel = schoolFacade.getEDTGlobal()
                                    }
                                }
                            }
                            Repeater {
                                model: edtModel
                                delegate: Text { text: modelData; color: "#0f172a" }
                            }
                        }                        
                    }

                    ScrollView {
                        clip: true
                        ColumnLayout {
                            width: 920
                            spacing: 8
                            Label { visible: !schoolFacade.admin; text: "Acces reserve a l'administrateur."; color: "#0f172a" }
                            RowLayout {
                                visible: schoolFacade.admin
                                TextField { id: actTitreField; placeholderText: "Titre"; Layout.preferredWidth: 140 }
                                TextField { id: actDateField; placeholderText: "Date"; Layout.preferredWidth: 110 }
                                TextField { id: actContenuField; placeholderText: "Contenu"; Layout.fillWidth: true }
                                Button {
                                    text: "Publier"
                                    Component.onCompleted: softButtonStyle(this)
                                    onClicked: { schoolFacade.publishActualite(actTitreField.text, actContenuField.text, actDateField.text); reloadModels() }
                                }
                            }
                            Repeater {
                                model: actualitesModel
                                delegate: Text { text: modelData; color: "#0f172a" }
                            }
                        }
                    }
                }

                Label {
                    text: schoolFacade.statusMessage
                    color: "#334155"
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }
                Button {
                    text: "Se deconnecter"
                    Layout.fillWidth: true
                    Component.onCompleted: softButtonStyle(this)
                    onClicked: {
                        schoolFacade.logout()
                        notesModel = []
                        usersModel = []
                    }
                }
            }
        }
    }
}
