// built-in system libraries
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
// custom libraries
#include <aria2/aria2.h>
#include <pybind11/embed.h>

namespace py = pybind11;

int rv;

int downloadEventCallback(aria2::Session* session, aria2::DownloadEvent event, aria2::A2Gid gid, void* userData)
{
    switch (event) {
        case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
            std::cerr << "COMPLETE";
            break;
        case aria2::EVENT_ON_DOWNLOAD_ERROR:
            std::cerr << "ERROR";
            break;
        default:
            return 0;
    }
    std::cerr << " [" << aria2::gidToHex(gid) << "]\n";

    return 0;
}

int callAria2(int uLen, char* uris[])
{
    aria2::libraryInit();
    aria2::Session* session;
    aria2::SessionConfig config;

    config.downloadEventCallback = downloadEventCallback;
    session = aria2::sessionNew(aria2::KeyVals(), config);

    for(int i = 1; i < uLen; ++i) {
        aria2::KeyVals options;
        rv = aria2::addUri(session, nullptr, uris, options);
        if(rv < 0) {
            std::cerr << "Failed to add download" << uris[0] << std::endl;
        }
    }

    for(;;) {
        rv = aria2::run(session, aria2::RUN_ONCE);
        if(rv != 1) {
            break;
        }
    }

    rv = aria2::sessionFinal(session);
    aria2::libraryDeinit();
    return rv;
}

PYBIND11_EMBEDDED_MODULE(aria, m) {
    m.doc() = "Internal C++ module accessible inside Python";
    m.def("call", &callAria2, "Calls libaria2", py::arg("a"), py::arg("b"));
}

int main(int argc, char* argv[])
{
    py::scoped_interpreter guard{};

    py::dict globals = py::globals();

    py::exec(R"(

        import aria

        def f(argc, argv):
            // aria.call()

    )", globals, globals);

    globals["f"](argc, argv);
}

