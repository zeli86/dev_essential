/**
 * @file
 * Package header for DataDefinition
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

#ifndef DDL_HEADER_
#define DDL_HEADER_

/**
 * @brief definition of the ddl namespace
 */
namespace ddl {
/**
 * @brief definition of the dd namespace
 */
namespace dd {
/**
 * @brief definition of the datamodel namespace
 */
namespace datamodel {
}
/**
 * @brief definition of the utility namespace
 */
namespace utility {
}
} // namespace dd
/**
 * @brief definition of the access_element namespace (codecs)
 */
namespace access_element {
}
/**
 * @brief definition of the serialization namespace
 */
namespace serialization {
}
/**
 * @brief definition of the mapping namespace
 */
namespace mapping {
/**
 * @brief definition of the dd namespace for mapping
 */
namespace oo {
}
/**
 * @brief definition of the rt namespace for mapping
 */
namespace rt {
}
} // namespace mapping

} // namespace ddl

#include "datamodel/datamodel_datadefinition.h"
// access and creating API
#include "dd/dd.h"
#include "dd/ddcompare.h"
#include "dd/ddfile.h"
#include "dd/ddstring.h"
#include "dd/ddstructure.h"
// codec API
#include "codec/pkg_codec.h"
#include "mapping/pkg_mapping.h"
#include "serialization/pkg_serialization.h"

#endif // DDL_HEADER_
