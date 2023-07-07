#include "util/settings-manager.hpp"
#include "util/logger.hpp"
#include "app.hpp"

int main(int argc, char** argv)
{
    ar::util::settings_manager::configure();
    ar::util::logger::configure_based_on_settings();

    ar::app app(argc, argv);

    return app.run();
}
