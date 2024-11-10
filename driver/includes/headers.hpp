#pragma once

#include <ntifs.h>
#include <ntddstor.h>
#include <mountdev.h>
#include <ntddvol.h>
#include <ntstrsafe.h>
#include <ntimage.h>

#include "utilities.hpp"

//sdelat s etim chto to
extern "C" {
    #include "../libraries/ksocket/src/ksocket/ksocket.h"
    #include "../libraries/ksocket/src/ksocket/berkeley.h"
}

#include "interface.hpp"