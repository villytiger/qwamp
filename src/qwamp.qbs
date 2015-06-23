import qbs

Product {
    name: "qwamp"
    type: "staticlibrary"

    files: ["*.cpp", "*.h", publicIncludePath + "/*.h"]

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }

    cpp.warningLevel: "all"
    cpp.treatWarningsAsErrors: true

    cpp.includePaths: [publicIncludePath]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt.core" }
        cpp.systemIncludePaths: [project.publicIncludePath]
    }
}
