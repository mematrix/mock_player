//
// Created by Charles on 2017/11/20.
//

#include "parser.hpp"

#include <istream>

using namespace player::container::matroska;


class ebml_parser_walker
{
public:
    ebml_parser_walker(ebml_parser &p, int64_t pos) : parser(p)
    {
        parser.down_to_sub_element(pos);
    }

    ~ebml_parser_walker()
    {
        parser.up_to_parent();
    }

private:
    ebml_parser &parser;
};


template<typename T>
static void read_value_to_element(T &result, const element_identify &identify, ebml_parser &parser, const ebml_node &node)
{
    typedef uint64_t T::* UnsignedMemPtr;
    typedef int64_t T::* SignedMemPtr;
    typedef double T::* FloatMemPtr;
    typedef std::string T::* StringMemPtr;
    typedef binary T::* BinaryMemPtr;

    switch (identify.type) {
        case element_type::UNSIGNED_INTEGER: {
            uint64_t unsigned_value;
            if (parser.read_unsigned_integer(node, unsigned_value) == 0) {
                result.*(identify.class_member_ptr.access<UnsignedMemPtr>()) = unsigned_value;
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::SIGNED_INTEGER: {
            int64_t int_value;
            if (parser.read_integer(node, int_value) == 0) {
                result.*(identify.class_member_ptr.access<SignedMemPtr>()) = int_value;
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::FLOAT: {
            double float_value;
            if (parser.read_float(node, float_value) == 0) {
                result.*(identify.class_member_ptr.access<FloatMemPtr>()) = float_value;
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::STRING: {
            std::string str_value;
            if (parser.read_string(node, str_value) == 0) {
                result.*(identify.class_member_ptr.access<StringMemPtr>()) = std::move(str_value);
                result.mask |= identify.name_mask;
            }
            break;
        }
        case element_type::BINARY: {
            binary bin_value;
            if (parser.read_binary(node, bin_value) == 0) {
                result.*(identify.class_member_ptr.access<BinaryMemPtr>()) = std::move(bin_value);
                result.mask |= identify.name_mask;
            }
            break;
        }
    }
}


parser::parser(std::unique_ptr<std::istream> &&stream, parser_callback &cb) : in_stream(std::move(stream)), callback(cb), ep(*in_stream)
{
}

int32_t parser::parse_ebml_header(ebml_header &result)
{
    ebml_node node;
    if (ep.parse_next(node) != 0) {
        return -1;
    }

    unsigned char *id = 0;//node.id;
    if (id[0] != 0x1A || id[1] != 0x45 || id[2] != 0xDF || id[3] != 0xA3) {
        return -1;
    }

    ebml_parser_walker walker(ep, node.position);
    auto size = node.size;
    while (size > 0) {
        ebml_node n;
        if (ep.parse_next(n) != 0) {
            return -1;
        }

        size -= n.size;

        if (n.id != 0x42) {
            continue;   // ebml header sub element'id all starts with 0x42
        }

        uint64_t value;
        switch (n.id) {
            case 0x86: // EBMLVersion
            {
                if (ep.read_unsigned_integer(n, value) == 0) {
                    result.set_version(value);
                }
                break;
            }
            case 0xF7: // EBMLReadVersion
            {
                if (ep.read_unsigned_integer(n, value) == 0) {
                    result.set_read_version(value);
                }
                break;
            }
            case 0xF2: // EBMLMaxIDLength
            {
                if (ep.read_unsigned_integer(n, value) == 0) {
                    result.set_max_id_length(value);
                }
                break;
            }
            case 0xF3: // EBMLMaxSizeLength
            {
                if (ep.read_unsigned_integer(n, value) == 0) {
                    result.set_max_size_length(value);
                }
                break;
            }
            case 0x82: // DocType
            {
                std::string str;
                if (ep.read_string(n, str) == 0) {
                    result.set_doc_type(std::move(str));
                }
                break;
            }
            case 0x87: // DocTypeVersion
            {
                if (ep.read_unsigned_integer(n, value) == 0) {
                    result.set_doc_type_version(value);
                }
                break;
            }
            case 0x85: // DocTypeReadVersion
            {
                if (ep.read_unsigned_integer(n, value) == 0) {
                    result.set_doc_type_read_version(value);
                }
                break;
            }
            default:
                break;
        }
    }

    return 0;
}

int32_t parser::parse_segment()
{
    ebml_node segment_node;
    if (ep.parse_next(segment_node) != 0) {
        return -1;
    }

    unsigned char *id = 0;//segment_node.id;
    if (id[0] != 0x18 || id[1] != 0x53 || id[2] != 0x80 || id[3] != 0x67) {
        return -1;
    }

    ebml_parser_walker walker(ep, segment_node.position);
    auto size = segment_node.size;
    while (size > 0) {
        ebml_node n;
        if (ep.parse_next(n) != 0) {
            return -1;
        }

        size -= n.size;

        /**
         * Level 1 elements:
         *
         * Meta Seek Information    [11][4D][9B][74]
         * Segment Information      [15][49][A9][66]
         * Cluster                  [1F][43][B6][75]
         * Track                    [16][54][AE][6B]
         * Cueing Data              [1C][53][BB][6B]
         * Attachment               [19][41][A4][69]
         * Chapters                 [10][43][A7][70]
         * Tagging                  [12][54][C3][67]
         */
        /*switch (n.id[0]) {
            case 0x11: {
                if (n.id[1] == 0x4D && n.id[2] == 0x9B && n.id[3] == 0x74) {
                    ebml_parser_walker meta_seek_info_walker(ep, n.position);
                    do_meta_seek_info_parse();
                }
            }
            case 0x15:
            case 0x1F:
            case 0x16:
            case 0x1C:
            case 0x19:
            case 0x10:
            case 0x12:
            default:
                break;
        }*/
    }

    return 0;
}

void parser::do_meta_seek_info_parse()
{

}

void parser::do_segment_info_parse()
{

}

void parser::do_cluster_parse()
{

}
