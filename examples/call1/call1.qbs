import qbs

CppApplication {
    name: "call1"
    files: ["*.cpp"]
    Depends { name: "qwamp" }
    Depends { name: "Qt.network" }
}
