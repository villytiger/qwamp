import qbs

CppApplication {
    name: "call1"
    files: ["*.cpp", "*.h"]
    Depends { name: "qwamp" }
    Depends { name: "Qt.network" }
}
