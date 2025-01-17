/**
 * @file
 * OO DataDefinition Redesign
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "ddl/dd/ddenum.h"

namespace ddl {

DDEnum::DDEnum(const DDEnum& other) : _dd(other._dd)
{
    _enum_type = _dd.getEnumTypes().access(other.getEnumType().getName());
}

DDEnum& DDEnum::operator=(const DDEnum& other)
{
    _dd = other._dd;
    _enum_type = _dd.getEnumTypes().access(other.getEnumType().getName());
    return *this;
}

DDEnum::DDEnum(const std::string& name,
               const dd::DataType& data_type,
               const std::vector<dd::EnumType::Element>& elements)
{
    _dd.add(data_type, {});
    _dd.getEnumTypes().emplace(dd::EnumType(name, data_type.getName(), elements));
    _enum_type = _dd.getEnumTypes().access(name);
}

DDEnum::DDEnum(const std::string& name,
               const DDDataType& data_type,
               const std::vector<dd::EnumType::Element>& elements)
{
    _dd.add(data_type.getDD());
    _dd.getDataTypes().add(data_type.getDataType());
    _dd.getEnumTypes().emplace(dd::EnumType(name, data_type.getDataType().getName(), elements));
    _enum_type = _dd.getEnumTypes().access(name);
}

DDEnum& DDEnum::addElement(const dd::EnumType::Element& element)
{
    _enum_type->getElements().add(element);
    return *this;
}

DDEnum& DDEnum::addElement(const std::string& name, int64_t value)
{
    return addElement(dd::EnumType::Element(name, std::to_string(value)));
}

DDEnum& DDEnum::addElements(const std::vector<dd::EnumType::Element>& elements)
{
    for (const auto& elem: elements) {
        addElement(elem);
    }
    return *this;
}

const dd::EnumType& DDEnum::getEnumType() const
{
    return *_enum_type;
}

const dd::DataDefinition& DDEnum::getDD() const
{
    return _dd;
}

} // namespace ddl
