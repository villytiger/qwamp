import qbs
import qbs.File
import qbs.FileInfo
import qbs.TextFile

Product {
    name: "qwamp"
    type: "staticlibrary"

    files: ["*.cpp", "*.h"]

    property var headersMap: {
        return {
            "qwamprawsession.h": "QWampRawsession",
        };
    }

    FileTagger {
        fileTags: ["export_hpp"]
        patterns: Object.keys(headersMap).concat("qwampglobal.h")
    }

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.network" }

    cpp.includePaths: [FileInfo.joinPaths(buildDirectory, "include")]

    cpp.warningLevel: "all"
    cpp.treatWarningsAsErrors: true

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt.core" }
    }

    Rule {
        inputs: ["export_hpp"]

        outputFileTags: ["hpp"]
        outputArtifacts: {
            var outputs = [];
            for (k in inputs) {
                outputs.push({
                    filePath: FileInfo.joinPaths(product.buildDirectory, "include/QWamp", k),
                    fileTags: ["hpp"]
                });
                if (k in product.headersMap) {
                    outputs.push({
                        filePath: FileInfo.joinPaths(product.buildDirectory, "include/QWamp", product.headersMap[k]),
                        fileTags: ["hpp"]
                    });
                }
            }
            return outputs;
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "creating export header " + input.fileName;
            cmd.highlight = "filegen";
            cmd.sourceCode = function () {
                var dst = FileInfo.joinPaths(product.buildDirectory, "include/QWamp", input.fileName);
                File.copy(input.filePath, dst);

                if (!(input.fileName in product.headersMap)) {
                    return;
                }

                var fileName = product.headersMap[input.fileName];
                var f = new TextFile(FileInfo.joinPaths(product.buildDirectory, "include/QWamp", fileName), TextFile.WriteOnly);
                f.writeLine("#include \"" + input.fileName + "\"");
                f.close();
            }

            return cmd;
        }
    }
}
