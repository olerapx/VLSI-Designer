#include "loader/loader.h"

int main(int argc, char* argv[])
{
    Loader loader(argc, argv);

    return loader.load();
}
