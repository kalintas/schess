#include "schess.h"


int main()
{
    if(!schess::Init()) return -1;

    schess::Run();

    return 0;
}