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

int callAria2(const std::vector<std::string>& uris)
{
    int rv = 0;

    aria2::libraryInit();
    aria2::Session* session;
    aria2::SessionConfig config;

    config.downloadEventCallback = downloadEventCallback;
    session = aria2::sessionNew(aria2::KeyVals(), config);

    for(size_t i = 0; i < uris.size(); ++i) {
        std::vector<std::string> uri = {uris[i]};
        aria2::KeyVals options;
        rv = aria2::addUri(session, nullptr, uri, options);
        if(rv < 0) {
            std::cerr << "Failed to add download" << uris[i] << std::endl;
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
    m.def("call", &callAria2);
}

int main(int argc, char* argv[])
{
    py::scoped_interpreter guard{};

    py::dict globals = py::globals();

    py::exec(R"(

        import aria

        def f(argc, argv):
            pass
            # aria.call()

    )", globals, globals);

    py::list py_argv;

    for (int i = 0; i < argc; ++i) {
        py_argv.append(argv[i]);
    }

    globals["f"](argc, py_argv);
}

