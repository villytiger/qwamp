import qbs

import qbs.ModUtils

Project {
    id: qwamp
    property path publicIncludePath: qwamp.sourceDirectory + "/include"
    references: [
        "examples/examples.qbs",
        "src/qwamp.qbs"
    ]
}
