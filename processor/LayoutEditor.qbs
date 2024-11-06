import qbs

CppApplication {
    consoleApplication: true
    files: [
        "Entity.cpp",
        "Entity.h",
        "GeometryOperations.cpp",
        "GeometryOperations.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
