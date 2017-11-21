//
// Created by Charles on 2017/11/17.
//

#include "element.hpp"

using namespace player::container::matroska;

static element_identify ebml_header_identifies[] = {
        element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::version, ebml_header::VERSION),
        element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::read_version, ebml_header::READ_VERSION),
        element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::max_id_length, ebml_header::MAX_ID_LENGTH),
        element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::max_size_length, ebml_header::MAX_SIZE_LENGTH),
        element_identify(element_type::STRING, &ebml_header::doc_type, ebml_header::DOC_TYPE),
        element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::doc_type_version, ebml_header::DOC_TYPE_VERSION),
        element_identify(element_type::UNSIGNED_INTEGER, &ebml_header::doc_type_read_version, ebml_header::DOC_TYPE_READ_VERSION)
};

const element_identify *::player::container::matroska::get_ebml_header_identifies()
{
    return ebml_header_identifies;
}


static element_identify meta_seek_identifies[] = {
        element_identify(element_type::BINARY, &meta_seek::id, meta_seek::ID),
        element_identify(element_type::UNSIGNED_INTEGER, &meta_seek::position, meta_seek::POSITION)
};

const element_identify *::player::container::matroska::get_meta_seek_identifies()
{
    return meta_seek_identifies;
}


static element_identify meta_seek_head_identifies[] = {
        element_identify(element_type::GENERIC_TYPE, &meta_seek_head::seeks, meta_seek_head::SEEK)
};

const element_identify *::player::container::matroska::get_meta_seek_head_identifies()
{
    return meta_seek_head_identifies;
}


static element_identify segment_chapter_translate_identifies[] = {
        element_identify(element_type::GENERIC_TYPE, &segment_chapter_translate::edition_uids, segment_chapter_translate::EDITION_UID),
        element_identify(element_type::UNSIGNED_INTEGER, &segment_chapter_translate::codec, segment_chapter_translate::CODEC),
        element_identify(element_type::BINARY, &segment_chapter_translate::id, segment_chapter_translate::ID)
};

const element_identify *::player::container::matroska::get_segment_chapter_translate_identifies()
{
    return segment_chapter_translate_identifies;
}
