import qbs

CppApplication {
    consoleApplication: true
    files: [
        "Entity.cpp",
        "Entity.h",
        "GeometryOperations.h",
        "main.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
