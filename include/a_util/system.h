/**
 * @file
 * Common include for component a_util::system
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef A_UTIL_SYSTEM_HEADER_INCLUDED
#define A_UTIL_SYSTEM_HEADER_INCLUDED

namespace a_util {
/// Serves as component for portable OS (Windows, Linux, ...) functionality
namespace system {
}
} // namespace a_util

#include "a_util/system/system.h"
#include "a_util/system/timer.h"
#include "a_util/system/uuid.h"

#endif // A_UTIL_SYSTEM_HEADER_INCLUDED
