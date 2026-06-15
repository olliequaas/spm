#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>

#include <aria2/aria2.h>

int rv;

int downloadEventCallback(aria2::Session* session, aria2::DownloadEvent event,
                          aria2::A2Gid gid, void* userData)
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

int main(int argc, char* argv[])
{
    aria2::libraryInit();
    aria2::Session* session;
    aria2::SessionConfig config;

    config.downloadEventCallback = downloadEventCallback;
    session = aria2::sessionNew(aria2::KeyVals(), config);

    for(int i = 1; i < argc; ++i) {
        std::vector<std::string> uris = {argv[i]};
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
