/**
 * @file
 * Implementation of the tester for the DataDefinition representation.
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
 *
 */

#include "./../../_common/test_oo_ddl.h"
#include "a_util/filesystem.h"
#include "a_util/result.h"
#include "ddl/dd/ddcompare.h"
#include "ddl/dd/ddfile.h"

#include <cstdio>
#include <gtest/gtest.h>

_MAKE_RESULT(-38, ERR_FAILED);

a_util::result::Result CompOutput(const std::string& strFile1, const std::string& strFile2)
{
    using namespace ddl;
    std::string strContent1;
    std::string strContent2;

    if (a_util::filesystem::readTextFile(strFile1, strContent1) != a_util::filesystem::OK ||
        a_util::filesystem::readTextFile(strFile2, strContent2) != a_util::filesystem::OK) {
        return ERR_FAILED;
    }

    a_util::result::Result res = DDCompare::isEqual(strContent1,
                                                    strContent2,
                                                    DDCompare::dcf_all | DDCompare::dcf_subset |
                                                        DDCompare::dcf_no_header_dates);
    return res;
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteHeader)
{
    using namespace ddl;
    DataDefinition dd_test_file_with_ext_declarations;

    ASSERT_NO_THROW(dd_test_file_with_ext_declarations = DDFile::fromXMLFile(
                        TEST_FILES_DIR "/test_insert_results_ref.description"););
    EXPECT_TRUE(dd_test_file_with_ext_declarations.isValid());

    dd::datamodel::Header created_header(dd::Version::ddl_version_12,
                                         "dev_essential team",
                                         "07.04.2010",
                                         "07.04.2010",
                                         "ADTF Common Description File");
    created_header.getExtDeclarations().add({"c3", "c3"});
    created_header.getExtDeclarations().add({"a", "a"});
    created_header.getExtDeclarations().add({"b", "b"});
    created_header.getExtDeclarations().add({"c", "c"});
    created_header.getExtDeclarations().add({"d", "d"});
    created_header.getExtDeclarations().add({"e", "e"});
    created_header.getExtDeclarations().add({"c2", "c2"});

    // test if the Header was read like expected!
    ASSERT_EQ(created_header, dd_test_file_with_ext_declarations.getHeader());

    // write the file with this header to the disk and check if it is written completly!
    std::remove(TEST_FILES_WRITE_DIR "/test_insert_results_ref_write_header_test.description");
    DDFile::toXMLFile(dd_test_file_with_ext_declarations,
                      TEST_FILES_WRITE_DIR
                      "/test_insert_results_ref_write_header_test.description");

    DataDefinition dd_test_file_with_ext_declarations_check_write;
    ASSERT_NO_THROW(dd_test_file_with_ext_declarations_check_write = DDFile::fromXMLFile(
                        TEST_FILES_WRITE_DIR
                        "/test_insert_results_ref_write_header_test.description"););
    // the header in the new file must be the same like epected
    ASSERT_EQ(created_header, dd_test_file_with_ext_declarations_check_write.getHeader());
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteDataTypes)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file = DDFile::fromXMLFile(TEST_FILES_DIR "/adtf.description"););

    EXPECT_TRUE(dd_test_file.isValid());

    EXPECT_EQ(dd_test_file.getDataTypes().getSize(), 12);
    // test if all expected datatypes are the
    test_ddl::containsDataType(dd_test_file, {"tBool", "8", "predefined ADTF tBool datatype"});
    test_ddl::containsDataType(dd_test_file, {"tChar", "8", "predefined ADTF tChar datatype"});
    test_ddl::containsDataType(dd_test_file, {"tUInt8", "8", "predefined ADTF tUInt8 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tInt8", "8", "predefined ADTF tInt8 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tUInt16", "16", "predefined ADTF tUInt16 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tInt16", "16", "predefined ADTF tInt16 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tUInt32", "32", "predefined ADTF tUInt32 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tInt32", "32", "predefined ADTF tInt32 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tUInt64", "64", "predefined ADTF tUInt64 datatype"});
    test_ddl::containsDataType(dd_test_file, {"tInt64", "64", "predefined ADTF tInt64 datatype"});
    test_ddl::containsDataType(dd_test_file,
                               {"tFloat32", "32", "predefined ADTF tFloat32 datatype"});
    test_ddl::containsDataType(dd_test_file,
                               {"tFloat64", "64", "predefined ADTF tFloat64 datatype"});

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    EXPECT_EQ(dd_test_file_written.getDataTypes().getSize(), 12);
    // test if all expected datatypes are the
    test_ddl::containsDataType(dd_test_file_written,
                               {"tBool", "8", "predefined ADTF tBool datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tChar", "8", "predefined ADTF tChar datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tUInt8", "8", "predefined ADTF tUInt8 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tInt8", "8", "predefined ADTF tInt8 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tUInt16", "16", "predefined ADTF tUInt16 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tInt16", "16", "predefined ADTF tInt16 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tUInt32", "32", "predefined ADTF tUInt32 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tInt32", "32", "predefined ADTF tInt32 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tUInt64", "64", "predefined ADTF tUInt64 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tInt64", "64", "predefined ADTF tInt64 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tFloat32", "32", "predefined ADTF tFloat32 datatype"});
    test_ddl::containsDataType(dd_test_file_written,
                               {"tFloat64", "64", "predefined ADTF tFloat64 datatype"});
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteEnumTypes)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file =
                        DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_changed_expected.xml"););

    EXPECT_TRUE(dd_test_file.isValid());

    dd::EnumType enum_pixelformat("tPixelFormat", "tInt16");
    enum_pixelformat.getElements().add({"PF_16BIT", "20"});
    enum_pixelformat.getElements().add({"PF_16BIT", "20"});
    enum_pixelformat.getElements().add({"PF_24BIT", "40"});
    enum_pixelformat.getElements().add({"PF_32BIT", "50"});
    enum_pixelformat.getElements().add({"PF_8BIT", "10"});
    enum_pixelformat.getElements().add({"PF_ABGR_4444", "29"});
    enum_pixelformat.getElements().add({"PF_ABGR_8888", "52"});
    enum_pixelformat.getElements().add({"PF_ARGB_8888", "51"});
    enum_pixelformat.getElements().add({"PF_BGRA_8888", "54"});
    enum_pixelformat.getElements().add({"PF_BGR_555", "34"});
    enum_pixelformat.getElements().add({"PF_BGR_565", "35"});
    enum_pixelformat.getElements().add({"PF_BGR_888", "46"});
    enum_pixelformat.getElements().add({"PF_CUSTOM", "1000"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_10", "21"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_12", "22"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_14", "23"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_16", "24"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_18", "41"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_20", "42"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_22", "43"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_24", "44"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_32", "55"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_8", "11"});
    enum_pixelformat.getElements().add({"PF_GREYSCALE_FLOAT32", "56"});
    enum_pixelformat.getElements().add({"PF_RGBA_4444", "28"});
    enum_pixelformat.getElements().add({"PF_RGBA_8888", "53"});
    enum_pixelformat.getElements().add({"PF_RGB_444", "25"});
    enum_pixelformat.getElements().add({"PF_RGB_555", "26"});
    enum_pixelformat.getElements().add({"PF_RGB_565", "27"});
    enum_pixelformat.getElements().add({"PF_RGB_8", "12"});
    enum_pixelformat.getElements().add({"PF_RGB_888", "45"});
    enum_pixelformat.getElements().add({"PF_RIII_10", "30"});
    enum_pixelformat.getElements().add({"PF_RIII_12", "31"});
    enum_pixelformat.getElements().add({"PF_RIII_14", "32"});
    enum_pixelformat.getElements().add({"PF_RIII_16", "33"});
    enum_pixelformat.getElements().add({"PF_UNKNOWN", "0"});
    enum_pixelformat.getElements().add({"PF_YUV420P_888", "60"});

    ASSERT_TRUE(dd_test_file.getEnumTypes().get("tPixelFormat"));
    EXPECT_EQ(*dd_test_file.getEnumTypes().get("tPixelFormat"), enum_pixelformat);

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_changed_expected_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    // expect the same enum in the written file
    ASSERT_TRUE(dd_test_file_written.getEnumTypes().get("tPixelFormat"));
    EXPECT_EQ(*dd_test_file_written.getEnumTypes().get("tPixelFormat"), enum_pixelformat);
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteStructTypes)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file = DDFile::fromXMLFile(TEST_FILES_DIR "/adtf.description"););

    EXPECT_TRUE(dd_test_file.isValid());

    {
        // find structs with a simple test
        std::vector<std::pair<std::string, std::string>> tMediaTypeInfo_elements = {
            {"ui32MajorType", "tUInt32"}, {"ui32SubType", "tUInt32"}, {"ui32Flags", "tUInt32"}};
        ASSERT_TRUE(test_ddl::containsStructType(
            *dd_test_file.getModel(), "tMediaTypeInfo", tMediaTypeInfo_elements));

        std::vector<std::pair<std::string, std::string>> adtfcoremedia_type_elements = {
            {"mediatype", "tMediaTypeInfo"}};
        ASSERT_TRUE(test_ddl::containsStructType(
            *dd_test_file.getModel(), "adtf.core.media_type", adtfcoremedia_type_elements));

        auto struct_for_test = dd_test_file.getStructTypes().get("adtf.core.media_type");
        EXPECT_EQ(struct_for_test->getAlignment(), dd::OptionalSize(dd::Alignment::e1));
        EXPECT_EQ(struct_for_test->getVersion(), "1");
        EXPECT_EQ(struct_for_test->getComment(), "");
        EXPECT_EQ(struct_for_test->getLanguageVersion(), dd::Version::ddl_version_notset);

        auto element_to_test = struct_for_test->getElements().get("mediatype");
        EXPECT_EQ(element_to_test->getAlignment(), dd::Alignment::e1);
        EXPECT_EQ(element_to_test->getArraySize(), 1);
        EXPECT_EQ(element_to_test->getByteOrder(), dd::ByteOrder::e_le);
        EXPECT_EQ(element_to_test->getBytePos(), dd::OptionalSize(0));
        EXPECT_EQ(element_to_test->getComment(), "");
        EXPECT_EQ(element_to_test->getDefault(), "");
        EXPECT_EQ(element_to_test->getDescription(), "");
        EXPECT_EQ(element_to_test->getMax(), "");
        EXPECT_EQ(element_to_test->getMin(), "");
        EXPECT_EQ(element_to_test->getBitPos(), dd::OptionalSize());
        EXPECT_EQ(element_to_test->getNumBits(), dd::OptionalSize());
        EXPECT_EQ(element_to_test->getOffset(), "");
        EXPECT_EQ(element_to_test->getScale(), "");
        EXPECT_EQ(element_to_test->getTypeName(), "tMediaTypeInfo");
        EXPECT_EQ(element_to_test->getUnitName(), "");
        EXPECT_EQ(element_to_test->getValue(), "");
    }

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    // do the same test in the written file
    {
        // find structs with a simple test
        std::vector<std::pair<std::string, std::string>> tMediaTypeInfo_elements = {
            {"ui32MajorType", "tUInt32"}, {"ui32SubType", "tUInt32"}, {"ui32Flags", "tUInt32"}};
        ASSERT_TRUE(test_ddl::containsStructType(
            *dd_test_file_written.getModel(), "tMediaTypeInfo", tMediaTypeInfo_elements));

        std::vector<std::pair<std::string, std::string>> adtfcoremedia_type_elements = {
            {"mediatype", "tMediaTypeInfo"}};
        ASSERT_TRUE(test_ddl::containsStructType(
            *dd_test_file_written.getModel(), "adtf.core.media_type", adtfcoremedia_type_elements));

        auto struct_for_test = dd_test_file_written.getStructTypes().get("adtf.core.media_type");
        EXPECT_EQ(struct_for_test->getAlignment(), dd::OptionalSize(dd::Alignment::e1));
        EXPECT_EQ(struct_for_test->getVersion(), "1");
        EXPECT_EQ(struct_for_test->getComment(), "");
        EXPECT_EQ(struct_for_test->getLanguageVersion(), dd::Version::ddl_version_notset);

        auto element_to_test = struct_for_test->getElements().get("mediatype");
        EXPECT_EQ(element_to_test->getAlignment(), dd::Alignment::e1);
        EXPECT_EQ(element_to_test->getArraySize(), 1);
        EXPECT_EQ(element_to_test->getByteOrder(), dd::ByteOrder::e_le);
        EXPECT_EQ(element_to_test->getBytePos(), dd::OptionalSize(0));
        EXPECT_EQ(element_to_test->getComment(), "");
        EXPECT_EQ(element_to_test->getDefault(), "");
        EXPECT_EQ(element_to_test->getDescription(), "");
        EXPECT_EQ(element_to_test->getMax(), "");
        EXPECT_EQ(element_to_test->getMin(), "");
        EXPECT_EQ(element_to_test->getBitPos(), dd::OptionalSize());
        EXPECT_EQ(element_to_test->getNumBits(), dd::OptionalSize());
        EXPECT_EQ(element_to_test->getOffset(), "");
        EXPECT_EQ(element_to_test->getScale(), "");
        EXPECT_EQ(element_to_test->getTypeName(), "tMediaTypeInfo");
        EXPECT_EQ(element_to_test->getUnitName(), "");
        EXPECT_EQ(element_to_test->getValue(), "");
    }
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteStructTypesV4)
{
    using namespace ddl;

    DataDefinition dd_test_file;

    ASSERT_NO_THROW(dd_test_file = DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_v40.description"););
    EXPECT_TRUE(dd_test_file.isValid());

    // find structs with a simple test
    std::vector<std::pair<std::string, std::string>> tTest_elements = {
        {"bBool", "tBool"}, {"nInt8", "tInt8"}, {"nUInt32", "tUInt32"}, {"fFloat32", "tFloat32"}};
    ASSERT_TRUE(test_ddl::containsStructType(*dd_test_file.getModel(), "tTest", tTest_elements));

    {
        auto tTest_struct = dd_test_file.getStructTypes().get("tTest");
        EXPECT_EQ(tTest_struct->getAlignment(), dd::OptionalSize(dd::Alignment::e4));
        EXPECT_EQ(tTest_struct->getVersion(), "1");
        EXPECT_EQ(tTest_struct->getComment(), "");
        EXPECT_EQ(tTest_struct->getLanguageVersion(), dd::Version::ddl_version_20);

        auto element_to_test = tTest_struct->getElements().get("fFloat32");
        EXPECT_EQ(element_to_test->getAlignment(), dd::Alignment::e4);
        EXPECT_EQ(element_to_test->getArraySize(), 1);
        EXPECT_EQ(element_to_test->getByteOrder(), dd::ByteOrder::e_le);
        EXPECT_EQ(element_to_test->getBytePos(), dd::OptionalSize(6));
        EXPECT_EQ(element_to_test->getComment(), "");
        EXPECT_EQ(element_to_test->getDefault(), "");
        EXPECT_EQ(element_to_test->getDescription(), "");
        EXPECT_EQ(element_to_test->getMax(), "");
        EXPECT_EQ(element_to_test->getMin(), "");
        EXPECT_EQ(element_to_test->getBitPos(), dd::OptionalSize(0));
        EXPECT_EQ(element_to_test->getNumBits(), dd::OptionalSize(32));
        EXPECT_EQ(element_to_test->getOffset(), "");
        EXPECT_EQ(element_to_test->getScale(), "");
        EXPECT_EQ(element_to_test->getTypeName(), "tFloat32");
        EXPECT_EQ(element_to_test->getUnitName(), "");
        EXPECT_EQ(element_to_test->getValue(), "");
    }

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_v40_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    // do the same test in the written file
    EXPECT_TRUE(dd_test_file_written.isValid());

    ASSERT_TRUE(
        test_ddl::containsStructType(*dd_test_file_written.getModel(), "tTest", tTest_elements));
    {
        auto tTest_struct = dd_test_file_written.getStructTypes().get("tTest");
        EXPECT_EQ(tTest_struct->getAlignment(), dd::OptionalSize(dd::Alignment::e4));
        EXPECT_EQ(tTest_struct->getVersion(), "1");
        EXPECT_EQ(tTest_struct->getComment(), "");
        EXPECT_EQ(tTest_struct->getLanguageVersion(), dd::Version::ddl_version_20);

        auto element_to_test = tTest_struct->getElements().get("fFloat32");
        EXPECT_EQ(element_to_test->getAlignment(), dd::Alignment::e4);
        EXPECT_EQ(element_to_test->getArraySize(), 1);
        EXPECT_EQ(element_to_test->getByteOrder(), dd::ByteOrder::e_le);
        EXPECT_EQ(element_to_test->getBytePos(), dd::OptionalSize(6));
        EXPECT_EQ(element_to_test->getComment(), "");
        EXPECT_EQ(element_to_test->getDefault(), "");
        EXPECT_EQ(element_to_test->getDescription(), "");
        EXPECT_EQ(element_to_test->getMax(), "");
        EXPECT_EQ(element_to_test->getMin(), "");
        EXPECT_EQ(element_to_test->getBitPos(), dd::OptionalSize(0));
        EXPECT_EQ(element_to_test->getNumBits(), dd::OptionalSize(32));
        EXPECT_EQ(element_to_test->getOffset(), "");
        EXPECT_EQ(element_to_test->getScale(), "");
        EXPECT_EQ(element_to_test->getTypeName(), "tFloat32");
        EXPECT_EQ(element_to_test->getUnitName(), "");
        EXPECT_EQ(element_to_test->getValue(), "");
    }
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteStreamMetaTypes)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file = DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_v40.description"););

    EXPECT_EQ(dd_test_file.getStreamMetaTypes().getSize(), 3);
    // test if all expected datatypes are the
    dd::StreamMetaType stream_type_expected_test_parent_not_yet = {
        "test_parent_not_yet_defined", "1", "adtf/default", {}};
    dd::StreamMetaType stream_type_expected_adtf_default = {
        "adtf/default", "1", {}, {{"md_struct", "string"}, {"md_definitions", "string"}}};
    dd::StreamMetaType stream_type_expected_adtf_test = {
        "test", "1", "adtf/default", {{"test_prop", "tInt32"}}};

    EXPECT_EQ(*(dd_test_file.getStreamMetaTypes().get("test_parent_not_yet_defined")),
              stream_type_expected_test_parent_not_yet);
    EXPECT_EQ(*(dd_test_file.getStreamMetaTypes().get("adtf/default")),
              stream_type_expected_adtf_default);
    EXPECT_EQ(*(dd_test_file.getStreamMetaTypes().get("test")), stream_type_expected_adtf_test);

    EXPECT_TRUE(dd_test_file.isValid());

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_v40_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    EXPECT_EQ(dd_test_file_written.getStreamMetaTypes().getSize(), 3);
    // test if all expected stream meta types are
    EXPECT_EQ(*(dd_test_file_written.getStreamMetaTypes().get("test_parent_not_yet_defined")),
              stream_type_expected_test_parent_not_yet);
    EXPECT_EQ(*(dd_test_file_written.getStreamMetaTypes().get("adtf/default")),
              stream_type_expected_adtf_default);
    EXPECT_EQ(*(dd_test_file_written.getStreamMetaTypes().get("test")),
              stream_type_expected_adtf_test);
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteBaseUnits)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file =
                        DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_changed_expected.xml"););

    EXPECT_EQ(dd_test_file.getBaseUnits().getSize(), 11);
    // test if some expected datatypes are there
    dd::datamodel::BaseUnit unit_metre("Metre", "m", "Fundamental unit for length");
    dd::datamodel::BaseUnit unit_kilogram("Kilogram", "kg", "Fundamental unit for mass");
    dd::datamodel::BaseUnit unit_nou("nou", "", "No SI, but needed for no unit definitions");

    EXPECT_EQ(*(dd_test_file.getBaseUnits().get("Metre")), unit_metre);
    EXPECT_EQ(*(dd_test_file.getBaseUnits().get("Kilogram")), unit_kilogram);
    EXPECT_EQ(*(dd_test_file.getBaseUnits().get("nou")), unit_nou);

    EXPECT_TRUE(dd_test_file.isValid());

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_changed_expected_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    // do the same test like above
    EXPECT_EQ(dd_test_file_written.getBaseUnits().getSize(), 11);

    EXPECT_EQ(*(dd_test_file_written.getBaseUnits().get("Metre")), unit_metre);
    EXPECT_EQ(*(dd_test_file_written.getBaseUnits().get("Kilogram")), unit_kilogram);
    EXPECT_EQ(*(dd_test_file_written.getBaseUnits().get("nou")), unit_nou);
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteUnitPrefixes)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file =
                        DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_changed_expected.xml"););

    EXPECT_EQ(dd_test_file.getUnitPrefixes().getSize(), 20);
    // test if some expected unit prefixes are there
    dd::datamodel::UnitPrefix unit_prefix_yotta("yotta", "Y", 24);
    dd::datamodel::UnitPrefix unit_prefix_zepto("zepto", "z", -21);
    dd::datamodel::UnitPrefix unit_prefix_yocto("yocto", "y", -24);

    EXPECT_EQ(*(dd_test_file.getUnitPrefixes().get("yotta")), unit_prefix_yotta);
    EXPECT_EQ(*(dd_test_file.getUnitPrefixes().get("zepto")), unit_prefix_zepto);
    EXPECT_EQ(*(dd_test_file.getUnitPrefixes().get("yocto")), unit_prefix_yocto);

    EXPECT_TRUE(dd_test_file.isValid());

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write =
        TEST_FILES_WRITE_DIR "/adtf_changed_expected_test_unit.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    // do the same test like above
    EXPECT_EQ(dd_test_file_written.getUnitPrefixes().getSize(), 20);

    EXPECT_EQ(*(dd_test_file_written.getUnitPrefixes().get("yotta")), unit_prefix_yotta);
    EXPECT_EQ(*(dd_test_file_written.getUnitPrefixes().get("zepto")), unit_prefix_zepto);
    EXPECT_EQ(*(dd_test_file_written.getUnitPrefixes().get("yocto")), unit_prefix_yocto);
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteUnits)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file =
                        DDFile::fromXMLFile(TEST_FILES_DIR "/test_insert_valid.description"););

    EXPECT_EQ(dd_test_file.getUnits().getSize(), 4);
    // test if some expected unit prefixes are there
    dd::datamodel::Unit unit_a_unit("a_unit",
                                    "1",
                                    "1",
                                    "1.000000",
                                    {{"a", 1, "a"}, {"b", 1, "b"}, {"d", 1, "d"}, {"e", 1, "e"}});
    dd::datamodel::Unit unit_e_unit("e_unit", "5", "5", "5.000000");

    EXPECT_EQ(*(dd_test_file.getUnits().get("a_unit")), unit_a_unit);
    EXPECT_EQ(*(dd_test_file.getUnits().get("e_unit")), unit_e_unit);

    EXPECT_TRUE(dd_test_file.isValid());

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write =
        TEST_FILES_WRITE_DIR "/adtf_changed_expected_test_unit_again.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    // do the same test like above
    EXPECT_EQ(dd_test_file_written.getUnits().getSize(), 4);

    EXPECT_EQ(*(dd_test_file_written.getUnits().get("a_unit")), unit_a_unit);
    EXPECT_EQ(*(dd_test_file_written.getUnits().get("e_unit")), unit_e_unit);
}

/**
 * @detail The building up of a DataDefinition object representation.
 */
TEST(TesterDDFile, readAndWriteStreams)
{
    using namespace ddl;

    DataDefinition dd_test_file;
    ASSERT_NO_THROW(dd_test_file = DDFile::fromXMLFile(TEST_FILES_DIR "/adtf_1_0p.description"););

    ASSERT_EQ(dd_test_file.getStreams().getSize(), 4);

    dd::Stream can_stream = {"can_stream", "adtf.core.media_type", "can", {{"", "tCanMessage", 0}}};
    dd::Stream extended_can_stream = {
        "extended_can_stream", "adtf.core.media_type", "extended can", {{"", "tCanMessageExt", 0}}};
    dd::Stream video_stream = {"video_stream", "adtf.type.video", "video stream", {}};

    EXPECT_TRUE(dd_test_file.isValid());

    // check if the stream are read like expected
    ASSERT_EQ(*dd_test_file.getStreams().get("can_stream"), can_stream);
    ASSERT_EQ(*dd_test_file.getStreams().get("extended_can_stream"), extended_can_stream);
    ASSERT_EQ(*dd_test_file.getStreams().get("video_stream"), video_stream);

    // write the file with this header to the disk and check if it is written completly!
    std::string test_file_write = TEST_FILES_WRITE_DIR "/adtf_1_0p_test.description";
    std::remove(test_file_write.c_str());
    DDFile::toXMLFile(dd_test_file, test_file_write);

    DataDefinition dd_test_file_written;
    ASSERT_NO_THROW(dd_test_file_written = DDFile::fromXMLFile(test_file_write););

    EXPECT_TRUE(dd_test_file_written.isValid());

    // check if the stream are read again like expected
    ASSERT_EQ(*dd_test_file_written.getStreams().get("can_stream"), can_stream);
    ASSERT_EQ(*dd_test_file_written.getStreams().get("extended_can_stream"), extended_can_stream);
    ASSERT_EQ(*dd_test_file_written.getStreams().get("video_stream"), video_stream);
}

/**
 * @detail The compatibility to 'old' version 1.0+ gets tested.
 * Read a DDL1.0+ from a file, test DDLPrinter and DDLResolver
 * @req_id ACORE-3904
 */
TEST(TesterDDFile, testCompatibility)
{
    using namespace ddl;

    dd::DataDefinition dd_read;

    std::string read_file = TEST_FILES_DIR "/adtf_1_0p.description";
    std::string write_file = TEST_FILES_WRITE_DIR "/adtf_1_0p_out.xml";
    // test import
    EXPECT_NO_THROW(dd_read = DDFile::fromXMLFile(read_file);) << "Import of DDL1.0+ failed.";

    std::remove(write_file.c_str());
    EXPECT_NO_THROW(DDFile::toXMLFile(dd_read, write_file);) << "Printing of DDL1.0+ failed.";

    ASSERT_EQ(a_util::result::SUCCESS,
              CompOutput(write_file, TEST_FILES_DIR "/adtf_1_0p_out_expected.xml"));
}

/**
 * @detail The old parser accepted tag defintions with invalid set file versions
 * This will read some files where versions are set to old file version and used new tags (like
 * serialized/deserialized)
 */
TEST(TesterDDFile, testCompatibilityForInvalidFiles)
{
    using namespace ddl;

    dd::DataDefinition dd_read;

    std::string read_invalid_file_1 = TEST_FILES_DIR "/invalid_but_legacy_test.description";
    std::string read_invalid_file_2 = TEST_FILES_DIR "/invalid_but_legacy_test_2.description";

    // test import
    EXPECT_NO_THROW(dd_read = DDFile::fromXMLFile(read_invalid_file_1);)
        << "Import of DDL 1.02 failed.";

    EXPECT_NO_THROW(dd_read = DDFile::fromXMLFile(read_invalid_file_2);)
        << "Import of DDL 1.02 failed.";

    // test import
    EXPECT_ANY_THROW(dd_read = DDFile::fromXMLFile(read_invalid_file_1, true);)
        << "Import of DDL 1.02 should fail, because it is strict, but did not!";

    EXPECT_ANY_THROW(dd_read = DDFile::fromXMLFile(read_invalid_file_2, true);)
        << "Import of DDL 1.02 should fail, because it is strict, but did not!";
}
